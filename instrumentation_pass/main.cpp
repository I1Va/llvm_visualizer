#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/ModuleSlotTracker.h"
#include "llvm/IR/Verifier.h"

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

#include "llvm/Support/Casting.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/FormatVariadic.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/Transforms/Utils/ModuleUtils.h"

#include "graph_builder.hpp"
#include "graph_serializer.hpp"
#include "gb_llvm_types.hpp"

using namespace llvm;

class MyModPass : public PassInfoMixin<MyModPass> {
    const std::string STATIC_INFO_PATH = "info/static_info.bin"; 
    const std::string DYNAMIC_INFO_PATH = "info/dynamic_info.bin";
 
    Type *voidTy;
    Type *int8PtrTy;
    Type *int32Ty;
    Type *int64Ty;

public:
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
        ModuleSlotTracker MST(&M);
        LLVMContext &Ctx = M.getContext();
        IRBuilder<> builder(Ctx);
        gb::GraphBuilder graph_builder;

        voidTy = Type::getVoidTy(Ctx);
        int8PtrTy = PointerType::get(Ctx, 0);
        int32Ty = Type::getInt32Ty(Ctx);
        int64Ty = Type::getInt64Ty(Ctx);
    
        gather_static_info(M, MST, graph_builder);
        perform_instrumentation(M, builder);
        
        if (proto::GraphSerializer::Serialize(graph_builder, STATIC_INFO_PATH) != 0) {
            errs() << "serialization failed\n";
        }
        
        return PreservedAnalyses::all();
    }

private:
    void perform_instrumentation(Module &M, IRBuilder<> &builder) {
         for (auto &F : M) {
            if (F.isDeclaration()) continue;
            insert_call_log(M, F, builder); 
            for (auto &BB : F) {
                insert_basic_block_start_log(M, BB, builder);
            }
        }
        insert_dynamic_info_dump(M, DYNAMIC_INFO_PATH);
    }

    void gather_static_info(Module &M, ModuleSlotTracker &MST, gb::GraphBuilder &graph_builder) {
         for (auto &F : M) {
            if (F.isDeclaration()) continue;
            MST.incorporateFunction(F);
        
            auto* f_cl = graph_builder.create_cluster<gb::ClusterTypes::F>((gb::IdT)&F);
            f_cl->label() = F.getName().str();

            for (auto &BB : F) {
                auto* bb_cl = graph_builder.create_cluster<gb::ClusterTypes::BB>((gb::IdT)&BB);
                bb_cl->label() = BB.getName().str();
                bb_cl->set_parent(f_cl); 

                for (auto &I : BB) {
                    if (auto err = build_instruction_nodes(I, *bb_cl, *f_cl, graph_builder, MST)) {
                        errs() << "Error: " << toString(std::move(err)) << "\n";
                    }
                }
            }
        }
    }

    void insert_basic_block_start_log(Module &M, BasicBlock &B, IRBuilder<> &builder) {
        FunctionType *bb_start_func_type =
            FunctionType::get(voidTy, {int64Ty}, false);
        FunctionCallee bb_start_func =
            M.getOrInsertFunction("basic_block_start_logger", bb_start_func_type);
       
        builder.SetInsertPoint(&*B.getFirstInsertionPt());

        uintptr_t bb_id = reinterpret_cast<uintptr_t>(&B);
        Value *idArg = ConstantInt::get(int64Ty, bb_id);
        builder.CreateCall(bb_start_func, {idArg});
    }

    void insert_call_log(Module &M, Function &F, IRBuilder<> &builder) {
        std::vector<Type *> call_log_param_types = {int64Ty, int64Ty};
        FunctionType *call_log_func_type =
            FunctionType::get(voidTy, call_log_param_types, false);
        FunctionCallee call_log_func =
            M.getOrInsertFunction("call_logger", call_log_func_type);

        std::vector<Type *> res_int_log_param_types = {int64Ty, int64Ty};
        FunctionType *res_int_log_func_type =
            FunctionType::get(voidTy, res_int_log_param_types, false);
        FunctionCallee res_int_log_func =
            M.getOrInsertFunction("res_int_logger", res_int_log_func_type);

        for (auto &B : F) {
            for (auto &I : B) {
                Value *call_node_id = ConstantInt::get(int64Ty, (int64_t)(&I));
                if (auto *call = dyn_cast<CallInst>(&I)) {
                    builder.SetInsertPoint(call);

                    Function *callee = call->getCalledFunction();
                    if (callee) {
                        Value *callerr_cluster_id = ConstantInt::get(int64Ty, (int64_t)(callee));
                        Value *args[] = {call_node_id, callerr_cluster_id};
                        builder.CreateCall(call_log_func, args);

                        if (!call->getType()->isVoidTy()) {
                            builder.SetInsertPoint(call->getNextNode());
                                

                            Value *CastVal = nullptr;
                            if (call->getType()->isIntegerTy()) {
                                CastVal = builder.CreateIntCast(call, int64Ty, true); 
                            } else if (call->getType()->isPointerTy()) {
                                CastVal = builder.CreatePtrToInt(call, int64Ty);
                            }
                            if (CastVal) {
                                Value *res_args[] = {CastVal, call_node_id};
                                builder.CreateCall(res_int_log_func, res_args);
                            }
                        }
                    }
                }
            }
        }
    }

    void insert_dynamic_info_dump(Module &M, const std::string &dump_file_path) {
        LLVMContext &Ctx = M.getContext();
    
        Value *pathConst = IRBuilder<>(Ctx).CreateGlobalString(dump_file_path, "dump_dynamic_logger_info_path", 0, &M);

        FunctionType *wrapperTy = FunctionType::get(voidTy, false);
        Function *wrapperFn = Function::Create(wrapperTy, Function::InternalLinkage, "__dtor_wrapper", &M);
        BasicBlock *BB = BasicBlock::Create(Ctx, "entry", wrapperFn);
        IRBuilder<> builder(BB);

        FunctionType *loggerTy = FunctionType::get(voidTy, {PointerType::getUnqual(Ctx)}, false);
        FunctionCallee loggerFunc = M.getOrInsertFunction("dump_dynamic_logger_info", loggerTy);

        builder.CreateCall(loggerFunc, {pathConst});
        builder.CreateRetVoid();

        appendToGlobalDtors(M, wrapperFn, 0);
    }
    
    Error build_instruction_nodes(Instruction &I, gb::ICluster &bb_cl, gb::ICluster &f_cl, 
                                    gb::GraphBuilder &graph_builder, ModuleSlotTracker &MST) {
            
        auto* i_node = graph_builder.create_node<gb::NodeTypes::Instr>((gb::IdT)&I);
        i_node->label() = I.getOpcodeName();
        i_node->set_parent(&bb_cl);

        if (auto* prev = I.getPrevNode()) {
            if (auto* prev_node = graph_builder.get_node((gb::IdT)prev)) {
                graph_builder.create_edge<gb::EdgeTypes::Flow>(*prev_node, *i_node);
            }
        }

        for (auto &U : I.operands()) {
            if (auto* target_bb = dyn_cast<BasicBlock>(U.get())) {
                auto* target_cl = graph_builder.create_cluster<gb::ClusterTypes::BB>((gb::IdT)target_bb);
                graph_builder.create_edge<gb::EdgeTypes::Flow>(*i_node, *target_cl);
            }
        }

        for (auto &U : I.operands()) {
            Value* val = U.get();
            if (val && !isa<BasicBlock>(val)) {
                auto* v_node = graph_builder.create_node<gb::NodeTypes::Value>((gb::IdT)val);
                if (v_node->label().empty()) {
                    v_node->label() = get_value_str(*val, MST);
                    v_node->set_parent(&bb_cl);
                }
                graph_builder.create_edge<gb::EdgeTypes::Data>(*v_node, *i_node);
            }
        }

        if (auto* cb = dyn_cast<CallBase>(&I)) {
            if (auto* callee = cb->getCalledFunction()) {
                auto* callee_cl = graph_builder.create_cluster<gb::ClusterTypes::F>((gb::IdT)callee);
                callee_cl->label() = callee->getName().str();
                graph_builder.create_edge<gb::EdgeTypes::Call>(*i_node, *callee_cl);
            }
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