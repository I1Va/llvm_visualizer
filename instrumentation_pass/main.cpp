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
        
        if (proto::GraphSerializer::serialize(graph_builder, STATIC_INFO_PATH) != 0) {
            errs() << "serialization failed\n";
        }
        
        return PreservedAnalyses::all();
    }

private:
    static gb::IdT get_constant_unique_id(const gb::IdT constant_id, const gb::IdT instr_id) {
        return constant_id ^ instr_id;
    }

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
            MST.incorporateFunction(F);
            gb::ICluster* f_cl = graph_builder.create_cluster<gb::ClusterTypes::F>((gb::IdT)&F); assert(f_cl);
            f_cl->label() = F.getName().str();
            for (auto &BB : F) {
                auto* bb_cl = graph_builder.create_cluster<gb::ClusterTypes::BB>((gb::IdT)&BB); assert(bb_cl);
                bb_cl->label() = BB.getName().str();
                bb_cl->set_parent(f_cl); 
                for (auto& I : BB) {
                    build_instruction_control_edges(M, MST, *f_cl, *bb_cl, I, graph_builder);
                    build_instruction_data_edges(M, MST, *f_cl, *bb_cl, I, graph_builder);
                }
            }
        }
    }

    void insert_basic_block_start_log(Module &M, BasicBlock &B, IRBuilder<> &builder) {
        FunctionType *bb_start_func_type = FunctionType::get(voidTy, {int64Ty}, false);
        FunctionCallee bb_start_func =
            M.getOrInsertFunction("basic_block_start_logger", bb_start_func_type);
       
        builder.SetInsertPoint(&*B.getFirstInsertionPt());

        gb::IdT bb_id = (gb::IdT)&B;
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
                Value *call_node_id = ConstantInt::get(int64Ty, (gb::IdT)&I);
                if (auto *call = dyn_cast<CallInst>(&I)) {
                    builder.SetInsertPoint(call);

                    Function *callee = call->getCalledFunction();
                    if (callee) {
                        Value *caller_cluster_id = ConstantInt::get(int64Ty, (int64_t)(callee));
                        Value *args[] = {call_node_id, caller_cluster_id};
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
    
    void build_instruction_control_edges
    (
        Module &M,
        ModuleSlotTracker &MST,
        gb::ICluster &Fcluster,
        gb::ICluster &BBcluster,
        Instruction &I,
        gb::GraphBuilder &dot_builder
    ) {
        gb::INode *node = dot_builder.create_node<gb::NodeTypes::Instr>((gb::IdT)&I); assert(node);
        node->set_parent(&BBcluster);
        node->label() = I.getOpcodeName(); 
    
        if (auto *prev_inst = I.getPrevNode()) {
            gb::INode *prev_node = dot_builder.get_node((gb::IdT) prev_inst); assert(prev_node);
            gb::IEdge *edge = dot_builder.create_edge<gb::EdgeTypes::Flow>(*prev_node, *node); assert(edge);
        }
        
        if (CallBase *callInst = dyn_cast<CallBase>(&I)) {
            if (Function *calledFunc = callInst->getCalledFunction()) {
                gb::ICluster *func_cluster = 
                    dot_builder.create_cluster<gb::ClusterTypes::F>((gb::IdT) calledFunc); assert(func_cluster);
                func_cluster->label() = calledFunc->getName().str();

                gb::IEdge *edge = dot_builder.create_edge<gb::EdgeTypes::Call>(*node, *func_cluster); assert(edge);
            }
        }
        
        for (auto &U : I.operands()) {
            BasicBlock* bb_operand = dyn_cast<BasicBlock>(U.get());
            if (bb_operand) {
                gb::ICluster *bb_cluster = 
                    dot_builder.create_cluster<gb::ClusterTypes::BB>((gb::IdT) bb_operand); assert(bb_cluster);
                
                bb_cluster->set_parent(&Fcluster);
                bb_cluster->label() = bb_operand->getName().str();
                
                gb::IEdge *edge = dot_builder.create_edge<gb::EdgeTypes::Flow>(*node, *bb_cluster); assert(edge);
            }
        }
    }
        
    void build_instruction_data_edges
    (
        Module &M,
        ModuleSlotTracker &MST,
        gb::ICluster &Fcluster,
        gb::ICluster &BBcluster,
        Instruction &I,
        gb::GraphBuilder &dot_builder
    ) {
        gb::INode *node = dot_builder.create_node<gb::NodeTypes::Instr>((gb::IdT) &I); assert(node);
        node->set_parent(&BBcluster);
        node->label() = I.getOpcodeName();

        for (auto &U : I.uses()) {
            if (Instruction* instr_user = dyn_cast<Instruction>(U.getUser())) {
                BasicBlock *user_basic_block = instr_user->getParent(); assert(user_basic_block);
                gb::ICluster* user_cluster = 
                    dot_builder.create_cluster<gb::ClusterTypes::BB>((gb::IdT) user_basic_block); assert(user_cluster);
                user_cluster->set_parent(&Fcluster);
                user_cluster->label() = user_basic_block->getName().str();
                    
                gb::INode *user_node = dot_builder.create_node<gb::NodeTypes::Instr>((gb::IdT) instr_user); assert(user_node);
                user_node->set_parent(user_cluster); 
                user_node->label() = instr_user->getOpcodeName();
                
                gb::IEdge *edge = dot_builder.create_edge<gb::EdgeTypes::Data>(*node, *user_node); assert(edge);
            }
        }

        for (auto &U : I.operands()) {
            Constant* value_op = dyn_cast<Constant>(U.get());
            if (!value_op) continue;
            
            if (!dot_builder.get_cluster((gb::IdT) value_op)) {
                gb::IdT node_id = get_constant_unique_id((gb::IdT)value_op, (gb::IdT)&I);
                gb::INode* value_node = dot_builder.create_node<gb::NodeTypes::Constant>(node_id); assert(value_node);  
                value_node->set_parent(&BBcluster); 
                value_node->label() = get_value_str(*value_op, MST);
                
                gb::IEdge *edge = dot_builder.create_edge<gb::EdgeTypes::Data>(*value_node, *node); assert(edge);
            }
        }
    }

    std::string get_value_str(Value& value, ModuleSlotTracker& MST) {
        std::string value_str;
        raw_string_ostream value_str_stream(value_str);
        value.printAsOperand(value_str_stream, false, MST);

        std::string value_type_str;
        raw_string_ostream value_type_str_stream(value_type_str);
        value.getType()->print(value_type_str_stream); 

        return value_type_str_stream.str() + " " + value_str;
    }

    std::string get_instr_str(Instruction& I) {
        std::string str;
        llvm::raw_string_ostream rso(str);
        I.print(rso);
        return rso.str();
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