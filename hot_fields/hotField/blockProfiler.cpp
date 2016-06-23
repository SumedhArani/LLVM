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

#include "llvm/Constants.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Module.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Instrumentation.h"
#include "ProfilingUtils.h"
#include <iostream>

using namespace llvm;

namespace {
  class BlockProfiler : public ModulePass {
    bool runOnModule(Module &M);
  };

  RegisterOpt<BlockProfiler> Y("insert-block-profiling",
                               "Insert instrumentation for block profiling");
}

ModulePass *llvm::createBlockProfilerPass() { return new BlockProfiler(); }

bool BlockProfiler::runOnModule(Module &M) {
  Function *Main = M.getMainFunction();
  if (Main == 0) {
    std::cerr << "WARNING: cannot insert block profiling into a module"
              << " with no main function!\n";
    return false;  // No main, no instrumentation!
  }

  unsigned NumBlocks = 0;
  for (Module::iterator I = M.begin(), E = M.end(); I != E; ++I)
    NumBlocks += I->size();

  const Type *ATy = ArrayType::get(Type::UIntTy, NumBlocks);
  GlobalVariable *Counters =
    new GlobalVariable(ATy, false, GlobalValue::InternalLinkage,
                       Constant::getNullValue(ATy), "BlockProfCounters", &M);

  // Instrument all of the blocks...
  unsigned i = 0;
  for (Module::iterator I = M.begin(), E = M.end(); I != E; ++I)
    for (Function::iterator BB = I->begin(), E = I->end(); BB != E; ++BB)
      // Insert counter at the start of the block
      IncrementCounterInBlock(BB, i++, Counters);

  // Add the initialization call to main.
  InsertProfilingInitCall(Main, "llvm_start_block_profiling", Counters);
  return true;
}
