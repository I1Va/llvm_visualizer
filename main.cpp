#include "llvm/IR/Module.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"

#include "dot_builder.hpp"

using namespace llvm;

struct MyModPass : public PassInfoMixin<MyModPass> {
    PreservedAnalyses run(Module& M, ModuleAnalysisManager& AM) {
        dot::DotBuilder dot_builder;

        for (auto& F : M) {
            dot::FCluster* Fcluster = dot_builder.create_cluster<dot::FCluster>((uint64_t) &F, F.getName());
            for (auto& B : F) {
                dot::BBCluster* BBcluster = 
                    dot_builder.create_cluster_with_parent<dot::BBCluster>((uint64_t) &B, *Fcluster, B.getName());

                dot::InstrNode* prev_node = nullptr;
                for (auto& I : B) {
                    dot::InstrNode* node = 
                        dot_builder.create_node_in_cluster<dot::InstrNode>((uint64_t) &I, *BBcluster, I.getOpcodeName());
                    
                    if (prev_node) {
                        dot_builder.create_edge<dot::FlowEdge>(*node, *prev_node);
                    }
                   
                    for (auto &U : I.uses()) {
                        Instruction *user = dyn_cast<Instruction> (U.getUser());
                        if (user) {
                            dot::InstrNode* user_node = 
                            dot_builder.create_node_in_cluster<dot::InstrNode>((uint64_t) user, *BBcluster, user->getOpcodeName());
                            dot_builder.create_edge<dot::FlowEdge>(*node, *user_node, "");
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
    PB.registerPipelineStartEPCallback([](ModulePassManager &MPM, auto) {
      MPM.addPass(MyModPass{});
      return true;
    });
    // PB.registerOptimizerLastEPCallback([](ModulePassManager &MPM, auto, auto) {
    //   MPM.addPass(MyModPass{});
    //   return true;
    // });
  };

  return {LLVM_PLUGIN_API_VERSION, "MyPlugin", "0.0.1", callback};
};

/* When a plugin is loaded by the driver, it will call this entry point to
obtain information about this plugin and about how to register its passes.
*/
extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return getPassPluginInfo();
}
