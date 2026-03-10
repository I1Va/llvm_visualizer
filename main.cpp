#include "llvm/IR/Module.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/ModuleSlotTracker.h"

#include "dot_builder.hpp"

using namespace llvm;

#include <iostream>

struct MyModPass : public PassInfoMixin<MyModPass> {
    PreservedAnalyses run(Module& M, ModuleAnalysisManager& AM) {
        dot::DotBuilder dot_builder;
        llvm::ModuleSlotTracker MST(&M);
    
        for (auto& F : M) {
            MST.incorporateFunction(F);
            dot::FCluster* Fcluster = dot_builder.create_cluster<dot::FCluster>((uint64_t) &F, F.getName().str());
            for (auto& B : F) {
                dot::BBCluster* BBcluster = 
                    dot_builder.create_cluster_with_parent<dot::BBCluster>((uint64_t) &B, *Fcluster, B.getName().str());

                dot::InstrNode* prev_node = nullptr;
                for (auto& I : B) {
                    dot::InstrNode* node = dot_builder.create_node_in_cluster<dot::InstrNode>((uint64_t) &I, *BBcluster, I.getOpcodeName());
                    if (prev_node) dot_builder.create_edge<dot::FlowEdge>(*prev_node, *node);
                    
                    int slot = MST.getLocalSlot(&I);
                    std::string slot_str = (slot > 0 ? "%" + std::to_string(slot) : "");
    
                    for (auto &U : I.uses()) {
                        Instruction* instr_user = dyn_cast<Instruction> (U.getUser());
                        if (instr_user) {
                            llvm::BasicBlock *user_basic_block = instr_user->getParent();
                            dot::ICluster* user_cluster = 
                                dot_builder.create_cluster_with_parent<dot::BBCluster>((dot::DotId) user_basic_block, *Fcluster, user_basic_block->getName().str());
                            

                            dot::InstrNode* user_node = 
                            dot_builder.create_node_in_cluster<dot::InstrNode>((uint64_t) instr_user, *user_cluster, instr_user->getOpcodeName());
                            dot_builder.create_edge<dot::DataEdge>(*node, *user_node, slot_str);
                            continue;
                        }
                    }
                    for (auto &U : I.operands()) {
                        BasicBlock* bb_operand = dyn_cast<BasicBlock> (U.get());
                        if (bb_operand) {
                            dot::BBCluster *bb_cluster = 
                            dot_builder.create_cluster_with_parent<dot::BBCluster>((uint64_t) bb_operand, *Fcluster, bb_operand->getName().str());
                            dot_builder.create_edge<dot::FlowEdge>(*node, *bb_cluster, "");
                            continue;
                        }

                        llvm::Value *value_op = U.get();

                        std::string str;
                        llvm::raw_string_ostream rso(str);
                        value_op->printAsOperand(rso, false, MST);

                        std::string s;
                        llvm::raw_string_ostream os(s);

                        value_op->getType()->print(os);

                        std::string type_str = os.str();

                        dot::DotId node_id =  (dot::DotId) value_op;
                        if (!dot_builder.node_id_exist(node_id)) {
                            dot::ValueNode* value_node = 
                                dot_builder.create_node_in_cluster<dot::ValueNode>((uint64_t) value_op, *BBcluster, type_str + " " + str);
                            dot_builder.create_edge<dot::DataEdge>(*value_node, *node, "");
                        }
                       
                    }

                    prev_node = node;
                }
            }
        }

        std::string result;
        {
            std::ostringstream sstream;

            dot_builder.serialize_dot(sstream);
            result = sstream.str();
        }
    
        std::error_code EC;
        raw_fd_ostream dot_file("graph.dot", EC, sys::fs::OF_Text);
        if (EC) {
            errs() << "Error opening file: " << EC.message() << "\n";
            return PreservedAnalyses::all();
        }
        dot_file << result;

        return PreservedAnalyses::all();
    }
};

PassPluginLibraryInfo getPassPluginInfo() {
  const auto callback = [](PassBuilder &PB) {
    // PB.registerPipelineStartEPCallback([](ModulePassManager &MPM, auto) {
    //   MPM.addPass(MyModPass{});
    //   return true;
    // });
    PB.registerOptimizerLastEPCallback([](ModulePassManager &MPM, auto, auto) {
      MPM.addPass(MyModPass{});
      return true;
    });
  };

  return {LLVM_PLUGIN_API_VERSION, "MyPlugin", "0.0.1", callback};
};

/* When a plugin is loaded by the driver, it will call this entry point to
obtain information about this plugin and about how to register its passes.
*/
extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return getPassPluginInfo();
}
