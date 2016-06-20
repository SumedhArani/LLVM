#include "llvm/ADT/SCCIterator.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

namespace {
  struct CountSCCPass : public FunctionPass 
  {
    static char ID;
    CountSCCPass() : FunctionPass(ID) {}
    bool runOnFunction(Function &F) 
    {
      unsigned sccNum = 0;
      errs() << "SCCs for Function " << F.getName() << ": ";
      for (scc_iterator<Function*> SCCiter = scc_begin(&F); !SCCiter.isAtEnd(); ++SCCiter) 
      {
        const std::vector<BasicBlock *> &SCCptr = *SCCiter;
        errs() << "\nSCC #" << ++sccNum << " : ";
        for (std::vector<BasicBlock*>::const_iterator I = SCCptr.begin(),
               E = SCCptr.end(); I != E; ++I)
          errs() << (*I)->getName() << ", ";
        if (SCCptr.size() == 1 && SCCiter.hasLoop())
          errs() << " (Has self-loop).";
      }
      errs() << "\n\n";

      return true;
    }
  };
}

char CountSCCPass::ID = 0;
static RegisterPass<CountSCCPass> Y("conncomp", "Print SCCs of each function CFG");