#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
using namespace llvm;

namespace {
  struct CountBlk : public ModulePass {
    static char ID;
    CountBlk() : ModulePass(ID) {}

    virtual bool runOnModule(Module &M) 
    {
        int num_bb=0;
        for(Module::iterator f = M.begin(), e = M.end() ; f != e; ++f)
        {
            for(Function::iterator b= f->begin(), end = f->end(); b != end; ++b)
            {
                num_bb++;
            }
        }
        
        errs() << "Number of basic blocks: " << num_bb << "!\n";
        return false;
    }
  };
}

char CountBlk::ID = 0;

static RegisterPass<CountBlk> X("bbcounter", "Counts Basic Blocks");
