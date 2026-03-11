#include "llvm/IR/Module.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/ModuleSlotTracker.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/FormatVariadic.h"

#include "dot_builder.hpp"

using namespace llvm;

#include <iostream>
#include <string>

class MyModPass : public PassInfoMixin<MyModPass> {
    const char DOT_DUMP_FILE_PATH[10] = "graph.dot";
public:
    PreservedAnalyses run(Module& M, ModuleAnalysisManager& AM) {
        dot::DotBuilder dot_builder;
        ModuleSlotTracker MST(&M);

        if (auto Err = build_dot_control_flow(M, MST, dot_builder)) {
            errs() << "Error building control flow graph: " << toString(std::move(Err)) << "\n";
            return PreservedAnalyses::all();
        }
        
        if (auto Err = build_dot_data_flow(M, MST, dot_builder)) {
            errs() << "Error building data flow graph: " << toString(std::move(Err)) << "\n";
            return PreservedAnalyses::all();
        }
        
        if (auto Err = dump_dot(DOT_DUMP_FILE_PATH, dot_builder)) {
            errs() << "Error dumping dot file: " << toString(std::move(Err)) << "\n";
            return PreservedAnalyses::all();
        }

        return PreservedAnalyses::all();
    }

private:
    Error build_dot_control_flow
    (
        Module& M,
        ModuleSlotTracker& MST,
        dot::DotBuilder& dot_builder
    ) {
        for (auto& F : M) {
            MST.incorporateFunction(F);
            dot::FCluster* Fcluster = dot_builder.create_cluster<dot::FCluster>((uint64_t) &F, F.getName().str());
            if (!Fcluster) {
                return createStringError(std::errc::invalid_argument,
                    "failed to create Function Cluster for function: %s", F.getName().str().c_str());
            }
    
            for (auto& B : F) {
                dot::BBCluster* BBcluster = dot_builder.create_cluster_with_parent<dot::BBCluster>(
                    (uint64_t) &B, *Fcluster, B.getName().str());
                if (!BBcluster) {
                    return createStringError(std::errc::invalid_argument,
                        "failed to create BasicBlock Cluster for block: %s in function: %s", 
                        B.getName().str().c_str(), F.getName().str().c_str());
                }

                dot::InstrNode* prev_node = nullptr;
                for (auto& I : B) {
                    dot::InstrNode* node = dot_builder.create_node_in_cluster<dot::InstrNode>(
                        (uint64_t) &I, *BBcluster, I.getOpcodeName());
                    if (!node) {
                        return createStringError(std::errc::invalid_argument,
                            "failed to create InstrNode for instruction in block: %s", B.getName().str().c_str());
                    }
        
                    if (prev_node) {
                        if (!dot_builder.create_edge<dot::FlowEdge>(*prev_node, *node)) {
                            return createStringError(std::errc::invalid_argument,
                                "failed to create FlowEdge between instructions");
                        }
                    }
                    
                    if (CallBase* callInst = dyn_cast<CallBase>(&I)) {
                        if (Function* calledFunc = callInst->getCalledFunction()) {
                            dot::ICluster* func_cluster = 
                                dot_builder.create_cluster<dot::FCluster>(
                                    (dot::DotId) calledFunc, calledFunc->getName().str());
                            if (!func_cluster) {
                                return createStringError(std::errc::invalid_argument,
                                    "failed to create Function Cluster for called function: %s", 
                                    calledFunc->getName().str().c_str());
                            }

                            if (!dot_builder.create_edge<dot::CallEdge>(*node, *func_cluster, "")) {
                                return createStringError(std::errc::invalid_argument,
                                    "failed to create CallEdge to function: %s", 
                                    calledFunc->getName().str().c_str());
                            }
                        }
                    }
                    
                    for (auto &U : I.operands()) {
                        BasicBlock* bb_operand = dyn_cast<BasicBlock>(U.get());
                        if (bb_operand) {
                            dot::BBCluster *bb_cluster = 
                                dot_builder.create_cluster_with_parent<dot::BBCluster>(
                                    (uint64_t) bb_operand, *Fcluster, bb_operand->getName().str());
                            if (!bb_cluster) {
                                return createStringError(std::errc::invalid_argument,
                                    "failed to create operand basic block cluster for block: %s", 
                                    bb_operand->getName().str().c_str());
                            }
                            
                            if (!dot_builder.create_edge<dot::FlowEdge>(*node, *bb_cluster, "")) {
                                return createStringError(std::errc::invalid_argument,
                                    "failed to create FlowEdge to basic block: %s", 
                                    bb_operand->getName().str().c_str());
                            }
                        }
                    }
                    
                    prev_node = node;
                }
            }
        }
        
        return Error::success();
    }

    // Error build_instruction_data_flow
    // (
    //     Module& M, Instruction &I,
    //     ModuleSlotTracker& MST,
    //     dot::DotBuilder& dot_builder
    // ) {

    // }


    Error build_dot_data_flow
    (
        Module& M,
        ModuleSlotTracker& MST, 
        dot::DotBuilder& dot_builder
    ) {
        for (auto& F : M) {
            MST.incorporateFunction(F);
            dot::FCluster* Fcluster = dot_builder.create_cluster<dot::FCluster>((uint64_t) &F, F.getName().str());
            if (!Fcluster) {
                return createStringError(std::errc::invalid_argument,
                    "failed to create Function Cluster for data flow in function: %s", 
                    F.getName().str().c_str());
            }
            
            for (auto& B : F) {
                dot::BBCluster* BBcluster = 
                    dot_builder.create_cluster_with_parent<dot::BBCluster>(
                        (uint64_t) &B, *Fcluster, B.getName().str());
                if (!BBcluster) {
                    return createStringError(std::errc::invalid_argument,
                        "failed to create BasicBlock Cluster for data flow in block: %s", 
                        B.getName().str().c_str());
                }
                
                for (auto& I : B) {
                    dot::InstrNode* node = dot_builder.create_node_in_cluster<dot::InstrNode>(
                        (uint64_t) &I, *BBcluster, I.getOpcodeName());
                    if (!node) {
                        return createStringError(std::errc::invalid_argument,
                            "failed to create InstrNode for data flow in instruction");
                    }

                    for (auto &U : I.uses()) {
                        if (Instruction* instr_user = dyn_cast<Instruction>(U.getUser())) {
                            BasicBlock *user_basic_block = instr_user->getParent();
                            if (!user_basic_block) {
                                return createStringError(std::errc::invalid_argument,
                                    "instruction user has no parent basic block");
                            }
                            
                            dot::ICluster* user_cluster = 
                                dot_builder.create_cluster_with_parent<dot::BBCluster>(
                                    (dot::DotId) user_basic_block, *Fcluster, user_basic_block->getName().str());
                            if (!user_cluster) {
                                return createStringError(std::errc::invalid_argument,
                                    "failed to create user cluster for basic block: %s", 
                                    user_basic_block->getName().str().c_str());
                            }
                                
                            dot::InstrNode* user_node = 
                                dot_builder.create_node_in_cluster<dot::InstrNode>(
                                    (uint64_t) instr_user, *user_cluster, instr_user->getOpcodeName());
                            if (!user_node) {
                                return createStringError(std::errc::invalid_argument,
                                    "failed to create user instruction node");
                            }
                            
                            if (!dot_builder.create_edge<dot::DataEdge>(*node, *user_node, "")) {
                                return createStringError(std::errc::invalid_argument,
                                    "failed to create DataEdge between instructions");
                            }
                        }
                    }

                    for (auto &U : I.operands()) {
                        Value* value_op = U.get();
                        if (!value_op) continue;

                        std::string str;
                        raw_string_ostream rso(str);
                        value_op->printAsOperand(rso, false, MST);

                        std::string s;
                        raw_string_ostream os(s);
                        value_op->getType()->print(os);
                        std::string type_str = os.str();

                        dot::DotId node_id = (dot::DotId) value_op;
                        if (!dot_builder.node_id_exist(node_id)) {
                            dot::ValueNode* value_node = 
                                dot_builder.create_node_in_cluster<dot::ValueNode>(
                                    (uint64_t) value_op, *BBcluster, type_str + " " + str);
                            if (!value_node) {
                                return createStringError(std::errc::invalid_argument,
                                    "failed to create ValueNode for operand");
                            }
                            
                            if (!dot_builder.create_edge<dot::DataEdge>(*value_node, *node, "")) {
                                return createStringError(std::errc::invalid_argument,
                                    "failed to create DataEdge from value to instruction");
                            }
                        }
                    }
                }
            }
        }
        
        return Error::success();
    }

    Error dump_dot(const std::string &path, dot::DotBuilder& dot_builder) {
        std::string result;
        {
            std::ostringstream sstream;
            dot_builder.serialize_dot(sstream);
            result = sstream.str();
        }
    
        std::error_code EC;
        raw_fd_ostream dot_file(path, EC, sys::fs::OF_Text);
        if (EC) {
            return createStringError(std::errc::io_error,
                "Error opening file '%s': %s", path.c_str(), EC.message().c_str());
        }
        
        dot_file << result;
        if (dot_file.has_error()) {
            return createStringError(std::errc::io_error,
                "Error writing to file '%s'", path.c_str());
        }
        
        return Error::success();
    }
};

PassPluginLibraryInfo getPassPluginInfo() {
    const auto callback = [](PassBuilder &PB) {
        PB.registerOptimizerLastEPCallback([](ModulePassManager &MPM, auto, auto) {
            MPM.addPass(MyModPass{});
            return true;
        });
    };

    return {LLVM_PLUGIN_API_VERSION, "MyPlugin", "0.0.1", callback};
}

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return getPassPluginInfo();
}