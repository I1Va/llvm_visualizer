#include "llvm/IR/Module.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/ModuleSlotTracker.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/FormatVariadic.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"

#include "dot_builder.hpp"
#include "graph_builder.hpp"
#include <iostream>
#include <fstream>
#include <google/protobuf/stubs/common.h>
#include "llvm/Transforms/Utils/ModuleUtils.h"
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
    Type *voidTy;
    Type *int8PtrTy;
    Type *int32Ty;
    Type *int64Ty;

public:
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
        gb::GraphBuilder graph_builder;
        ModuleSlotTracker MST(&M);
        LLVMContext &Ctx = M.getContext();
        IRBuilder<> builder(Ctx);

        voidTy = Type::getVoidTy(Ctx);
        int8PtrTy = PointerType::get(Ctx, 0);
        int32Ty = Type::getInt32Ty(Ctx);
        int64Ty = Type::getInt64Ty(Ctx);

        for (auto &F : M) {
            if (F.isDeclaration()) continue;
            MST.incorporateFunction(F);
            insert_call_log(M, F, builder); 
           

            auto* f_cl = graph_builder.create_cluster<FCluster>((gb::IdT)&F);
            f_cl->label() = F.getName().str();

            for (auto &BB : F) {
                insert_basic_block_start_log(M, BB, builder);
                auto* bb_cl = graph_builder.create_cluster<BBCluster>((gb::IdT)&BB);
                bb_cl->label() = BB.getName().str();
                bb_cl->set_parent(f_cl); 

                for (auto &I : BB) {
                    if (auto err = build_instruction_nodes(I, *bb_cl, *f_cl, graph_builder, MST)) {
                        errs() << "Error: " << toString(std::move(err)) << "\n";
                    }
                }
            }

            outs() << '\n';
            bool verif = verifyFunction(F, &outs());
            outs() << "[VERIFICATION] " << (verif ? "FAIL\n\n" : "OK\n\n");
        }
    
        std::string dynamic_info_path = "dynamic_info.bin";
        insert_dynamic_info_dump(M, dynamic_info_path);
        
        return PreservedAnalyses::all();
    }

private:
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

    void insert_dynamic_info_dump(Module &M, std::string &dump_file_path) {
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
    
    Error build_instruction_nodes(Instruction &I, BBCluster &bb_cl, FCluster &f_cl, 
                                    gb::GraphBuilder &graph_builder, ModuleSlotTracker &MST) {
            
        auto* i_node = graph_builder.create_node<InstrNode>((gb::IdT)&I);
        i_node->label() = I.getOpcodeName();
        i_node->set_parent(&bb_cl);

        if (auto* prev = I.getPrevNode()) {
            if (auto* prev_node = graph_builder.get_node((gb::IdT)prev)) {
                graph_builder.create_edge<FlowEdge>(*prev_node, *i_node);
            }
        }

        for (auto &U : I.operands()) {
            if (auto* target_bb = dyn_cast<BasicBlock>(U.get())) {
                auto* target_cl = graph_builder.create_cluster<BBCluster>((gb::IdT)target_bb);
                graph_builder.create_edge<FlowEdge>(*i_node, *target_cl);
            }
        }

        for (auto &U : I.operands()) {
            Value* val = U.get();
            if (val && !isa<BasicBlock>(val)) {
                auto* v_node = graph_builder.create_node<ValueNode>((gb::IdT)val);
                if (v_node->label().empty()) {
                    v_node->label() = get_value_str(*val, MST);
                    v_node->set_parent(&bb_cl);
                }
                graph_builder.create_edge<DataEdge>(*v_node, *i_node);
            }
        }

        if (auto* cb = dyn_cast<CallBase>(&I)) {
            if (auto* callee = cb->getCalledFunction()) {
                auto* callee_cl = graph_builder.create_cluster<FCluster>((gb::IdT)callee);
                callee_cl->label() = callee->getName().str();
                graph_builder.create_edge<CallEdge>(*i_node, *callee_cl);
            }
        }

        GOOGLE_PROTOBUF_VERIFY_VERSION;
        
        if (gb::GraphBuilderSerializer::Serialize(graph_builder, "static_info.bin") != 0) {
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