#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
using namespace llvm;

namespace {
  struct 	GlobVarPass : public ModulePass 
	{
    	static char ID;
    		GlobVarPass() : ModulePass(ID) {}

	    virtual bool runOnModule(Module &M) 
	    {
      		for (GlobalVariable &GV : M.globals())
      		{
      			errs() << GV.getName() << "\n";
      		}
      		return false;
    	}
  	};
}

char GlobVarPass::ID = 0;

static RegisterPass<GlobVarPass> X("gvar", "Shows global variables");
