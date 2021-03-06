#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
using namespace llvm;

namespace {
  struct SkeletonPass : public FunctionPass {
    static char ID;
    SkeletonPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) 
    {
      int num_bb;
      for(Function::iterator bb = F.begin(), e = F.end(); bb != e; ++bb)
        num_bb++;
      errs() << "Number of basic blocks: " << num_bb << "!\n";
      return false;
    }
  };
}

char SkeletonPass::ID = 0;

static RegisterPass<basicblockPass> X("bbcounter", "Counts Basic Blocks functions");
