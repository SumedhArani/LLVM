#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
using namespace llvm;

//Use of anonymous namespace works as a static in C
namespace {
	struct CountBlk : public BasicBlockPass {
    static char ID;
    int count=0;
    CountBlk() : BasicBlockPass(ID) {}

    virtual bool runOnBasicBlock(BasicBlock &B) 
    {
		count++;
		getCount();	
     	return false;
    }

    void getCount()
    {
    	errs()<<count<<"\n";
    }
  };
}

char CountBlk::ID = 0;

static RegisterPass<CountBlk> X("bbcounter", "Counts number of basic blocks!");
