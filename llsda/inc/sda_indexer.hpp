#pragma once

#include <llvm/IR/Instruction.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/BasicBlock.h>

#include "graph_builder.hpp"

class Indexer {
    std::map<llvm::Value *, uint64_t> value_ids;
    std::map<std::pair<llvm::Instruction *, llvm::Constant *>, uint64_t, PairComparator> constant_ids;
public:
    gb::IdT id(llvm::Function &value) const {
        auto it = value_ids.find(&value);
        assert(it != value_ids.end());
        return it->second;
    }
    gb::IdT id(llvm::BasicBlock &value) const {
        auto it = value_ids.find(&value);
        assert(it != value_ids.end());
        return it->second;
    }
    gb::IdT id(llvm::Instruction &value) const {
        auto it = value_ids.find(&value);
        assert(it != value_ids.end());
        return it->second;
    }

    gb::IdT constant_id(llvm::Instruction &anchor, llvm::Constant &constant) const {
        auto it = constant_ids.find({&anchor, &constant});
        assert(it != constant_ids.end());
        return it->second;
    }

    void init_ids_table(llvm::Module &M) {
        value_ids.clear();
        constant_ids.clear();
        uint64_t value_id    = 0x0000000000000000;
        uint64_t constant_id = 0x1000000000000000;

        for (auto &F : M) {
            value_ids[&F] = ++value_id;
            for (auto &BB : F) {
                value_ids[&BB] = ++value_id;
                for (llvm::Instruction &I : BB) {
                    llvm::Value *V = &I;
                    if (value_ids.find(V) == value_ids.end())
                        value_ids[V] = ++value_id;
                    for (auto &Op : I.operands()) {
                        if (llvm::Constant *constant = dyn_cast<llvm::Constant>(Op)) {
                            std::pair<llvm::Instruction *, llvm::Constant *> key(&I, constant);
                            if (constant_ids.find(key) == constant_ids.end())
                                constant_ids[key] = ++constant_id;
                            continue;
                        } 
                        llvm::Value *operand = Op.get();
                        if (value_ids.find(operand) == value_ids.end())
                            value_ids[operand] = ++value_id;
                    }
                }
            }
        }
    }
};