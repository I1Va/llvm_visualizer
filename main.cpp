#include "llvm/IR/Module.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/Casting.h"

#include "graph_builder.hpp"

using namespace llvm;

struct MyModPass : public PassInfoMixin<MyModPass> {
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
    outs() << "digraph G {\n";
    outs() << "rankdir=TD;";
    outs() << "splines=polyline\n";
    outs() << "nodesep=1.0;\n";
    outs() << "ranksep=1.5;\n";

    for (auto &F : M) {
      outs() << "subgraph cluster_" << (uint64_t) &F << "{ ";
      outs() << "label=\"" << F.getName() << "\"\n";
      outs() << "color=\"" << "#af09fcff" << "\"\n";
      outs() << "fillcolor=\"" << "#F5F5F5" << "\"\n"; 
      outs() << "style=\"" << "filled" << "\"\n"; 
      outs() << "style=\"" << "filled" << "\"\n"; 
      outs() << "penwidth=\"" << "3" << "\"\n"; 
      outs() << "fontcolor=\"" << "red" << "\"\n"; 
      outs() << "fontsize=\"" << "20" << "\"\n"; 

      for (auto &B : F) {
        
        outs() << "subgraph cluster_" << (uint64_t) &B << "{ ";
        outs() << "label=\"" << B.getName() << "\"\n";
        outs() << "color=\"" << "blue" << "\"\n";
        outs() << "fillcolor=\"" << "#F5F5F5" << "\"\n"; 
        outs() << "style=\"" << "filled" << "\"\n"; 
        outs() << "style=\"" << "filled" << "\"\n"; 
        outs() << "penwidth=\"" << "2" << "\"\n"; 
        outs() << "fontcolor=\"" << "red" << "\"\n"; 
        outs() << "fontsize=\"" << "20" << "\"\n"; 
  
        for (auto &I : B) {
          outs() << "n" << (uint64_t) &I;
          outs() << "[";
          outs() << "label=\""      << I.getOpcodeName() << "\" ";
          outs() << "shape=\""      << "rect" << "\" ";
          outs() << "style=\""      << "filled" << "\" ";
          outs() << "fillcolor=\""  << "#FFD0D0" << "\"";
          outs() << "];\n";
        }

        outs() << "}\n";
      }
      outs() << "}\n";
    }

    for (auto &F : M) {
      for (auto &B : F) {
        uint64_t prev_instr_id = 0;
        for (auto &I : B) {
          uint64_t cur_instr_id = (uint64_t) &I;
          if (prev_instr_id) {
            outs() << "n" << prev_instr_id << " -> " << "n" << cur_instr_id << " ";
            outs() << "[";
            outs() << "color=\"" << "red" << "\" ";
            outs() << "penwidth=\"" << "2" << "\" ";
            outs() << "style=\"" << "solid" << "\" ";
            outs() << "arrowhead=\"" << "normal" << "\"";
            outs() << "];\n";
          }
          prev_instr_id = cur_instr_id; 
            

          for (auto &U : I.uses()) {
            Instruction *user = dyn_cast<Instruction> (U.getUser());
            if (user) {
              uint64_t user_instr_id = (uint64_t) user;
              outs() << "n" << cur_instr_id << " -> " << "n" << user_instr_id << " ";
              outs() << "[";
              outs() << "color=\"" << "blue" << "\" ";
              outs() << "penwidth=\"" << "2" << "\" ";
              outs() << "style=\"" << "solid" << "\" ";
              outs() << "arrowhead=\"" << "normal" << "\"";
              outs() << "];\n";
            }
            
          }
        }
      }
    }
    outs() << "}\n";

    return PreservedAnalyses::all();
  };
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
