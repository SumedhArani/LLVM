//===- BlockProfiling.cpp - Insert counters for block profiling -----------===//
//
//                      The LLVM Compiler Infrastructure
//
// This file was developed by the LLVM research group and is distributed under
// the University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This pass instruments the specified program with counters for basic block or
// function profiling.  This is the most basic form of profiling, which can tell
// which blocks are hot, but cannot reliably detect hot paths through the CFG.
// Block profiling counts the number of times each basic block executes, and
// function profiling counts the number of times each function is called.
//
// Note that this implementation is very naive.  Control equivalent regions of
// the CFG should not require duplicate counters, but we do put duplicate
// counters in.
//
//===----------------------------------------------------------------------===//

#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Instrumentation.h"
//#include "llvm/Transforms/ProfilingUtils.h"
#include <iostream>

using namespace llvm;

namespace {
  class FunctionProfiler : public ModulePass
  {
    bool runOnModule(Module &M);
  };

  RegisterOpt<FunctionProfiler> X("insert-function-profiling",
                               "Insert instrumentation for function profiling");
}

ModulePass *llvm::createFunctionProfilerPass() {
  return new FunctionProfiler();
}

bool FunctionProfiler::runOnModule(Module &M) {
  Function *Main = M.getMainFunction();
  if (Main == 0) {
    std::cerr << "WARNING: cannot insert function profiling into a module"
              << " with no main function!\n";
    return false;  // No main, no instrumentation!
  }

  unsigned NumFunctions = 0;
  for (Module::iterator I = M.begin(), E = M.end(); I != E; ++I)
    if (!I->isExternal())
      ++NumFunctions;

  const Type *ATy = ArrayType::get(Type::UIntTy, NumFunctions);
  GlobalVariable *Counters =
    new GlobalVariable(ATy, false, GlobalValue::InternalLinkage,
                       Constant::getNullValue(ATy), "FuncProfCounters", &M);

  // Instrument all of the functions...
  unsigned i = 0;
  for (Module::iterator I = M.begin(), E = M.end(); I != E; ++I)
    if (!I->isExternal())
      // Insert counter at the start of the function
      IncrementCounterInBlock(I->begin(), i++, Counters);

  // Add the initialization call to main.
  InsertProfilingInitCall(Main, "llvm_start_func_profiling", Counters);
  return true;
}