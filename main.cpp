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
#include "graph_builder.hpp"
#include <iostream>
#include <fstream>
#include <google/protobuf/stubs/common.h>
#include "serializer.hpp"

using namespace llvm;

#include <iostream>
#include <string>
#include <sstream>

using InstrNode = gb::Node;
using ValueNode = gb::Node;
using FCluster  = gb::Cluster;
using BBCluster = gb::Cluster;
using FlowEdge  = gb::Edge;
using DataEdge  = gb::Edge;
using CallEdge  = gb::Edge;

class MyModPass : public PassInfoMixin<MyModPass> {
    const char DOT_DUMP_FILE_PATH[10] = "graph.dot";
public:
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
        gb::GraphBuilder builder;
        ModuleSlotTracker MST(&M);

        for (auto &F : M) {
            if (F.isDeclaration()) continue;
            MST.incorporateFunction(F);

            auto* f_cl = builder.create_cluster<FCluster>((gb::IdT)&F);
            f_cl->label() = F.getName().str();

            for (auto &BB : F) {
                auto* bb_cl = builder.create_cluster<BBCluster>((gb::IdT)&BB);
                bb_cl->label() = BB.getName().str();
                bb_cl->set_parent(f_cl); 

                for (auto &I : BB) {
                    if (auto err = build_instruction_nodes(I, *bb_cl, *f_cl, builder, MST)) {
                        errs() << "Error: " << toString(std::move(err)) << "\n";
                    }
                }
            }
        }

        
        return PreservedAnalyses::all();
    }

private:
    Error build_instruction_nodes(Instruction &I, BBCluster &bb_cl, FCluster &f_cl, 
                                    gb::GraphBuilder &builder, ModuleSlotTracker &MST) {
            
        auto* i_node = builder.create_node<InstrNode>((gb::IdT)&I);
        i_node->label() = I.getOpcodeName();
        i_node->set_parent(&bb_cl);

        if (auto* prev = I.getPrevNode()) {
            if (auto* prev_node = builder.get_node((gb::IdT)prev)) {
                builder.create_edge<FlowEdge>(*prev_node, *i_node);
            }
        }

        for (auto &U : I.operands()) {
            if (auto* target_bb = dyn_cast<BasicBlock>(U.get())) {
                auto* target_cl = builder.create_cluster<BBCluster>((gb::IdT)target_bb);
                builder.create_edge<FlowEdge>(*i_node, *target_cl);
            }
        }

        for (auto &U : I.operands()) {
            Value* val = U.get();
            if (val && !isa<BasicBlock>(val)) {
                auto* v_node = builder.create_node<ValueNode>((gb::IdT)val);
                if (v_node->label().empty()) {
                    v_node->label() = get_value_str(*val, MST);
                    v_node->set_parent(&bb_cl);
                }
                builder.create_edge<DataEdge>(*v_node, *i_node);
            }
        }

        if (auto* cb = dyn_cast<CallBase>(&I)) {
            if (auto* callee = cb->getCalledFunction()) {
                auto* callee_cl = builder.create_cluster<FCluster>((gb::IdT)callee);
                callee_cl->label() = callee->getName().str();
                builder.create_edge<CallEdge>(*i_node, *callee_cl);
            }
        }

        GOOGLE_PROTOBUF_VERIFY_VERSION;
        if (gb::ProtobufSerializer::Serialize(builder, "graph.bin") != 0) {
            return llvm::make_error<llvm::StringError>(
                "serialization failed", 
                llvm::inconvertibleErrorCode()
            );
        }

        return Error::success();
    }

    std::string get_value_str(Value& v, ModuleSlotTracker& MST) {
        std::string s;
        raw_string_ostream rso(s);
        v.printAsOperand(rso, false, MST);
        return s;
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