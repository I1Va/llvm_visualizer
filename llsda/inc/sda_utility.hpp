#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <utility>

#include "llvm/IR/Instruction.h"
#include "llvm/IR/Constant.h"
#include "llvm/Support/raw_ostream.h"

struct PairComparator {
    bool operator()(const std::pair<llvm::Instruction *, llvm::Constant *> &lhs,
                    const std::pair<llvm::Instruction *, llvm::Constant *> &rhs) const {
        if (lhs.first != rhs.first)
            return lhs.first < rhs.first;
        return lhs.second < rhs.second;
    }
};

std::string get_value_str(llvm::Value& value, llvm::ModuleSlotTracker& MST) {
    std::string value_str;
    llvm::raw_string_ostream value_str_stream(value_str);
    value.printAsOperand(value_str_stream, false, MST);

    std::string value_type_str;
    llvm::raw_string_ostream value_type_str_stream(value_type_str);
    value.getType()->print(value_type_str_stream); 

    return value_type_str_stream.str() + " " + value_str;
}

std::string get_instr_str(llvm::Instruction& I) {
    std::string str;
    llvm::raw_string_ostream rso(str);
    I.print(rso);
    return rso.str();
}
