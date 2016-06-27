#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/ADT/DenseMap.h"
#include <cmath>
#include <vector>
using namespace llvm;

namespace
{
  struct DynProfilePass : public FunctionPass 
  {
    static char ID;
    DynProfilePass() : FunctionPass(ID) {}
    static std::vector<GlobalVariable*> g_vars;

    void getAnalysisUsage(AnalysisUsage &AU) const override
    {
      AU.setPreservesCFG();
      AU.addRequired<LoopInfoWrapperPass>();
    }

    virtual bool runOnFunction(Function &F) override
    { 
      LLVMContext& Ctx = F.getContext();
      
      Constant* logFunc = F.getParent()->getOrInsertFunction("count", Type::getVoidTy(Ctx), NULL);
      Constant* cntFunc = F.getParent()->getOrInsertFunction("newloop", Type::getVoidTy(Ctx), NULL);

      Constant* cntNFunc = F.getParent()->getOrInsertFunction("newNestedLoop", Type::getVoidTy(Ctx), NULL);
      Constant* logNFunc = F.getParent()->getOrInsertFunction("countNested", Type::getVoidTy(Ctx), NULL);

      LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
      Function::iterator BB_ptr = F.begin();
      Function::iterator BB_end = F.end();
      Module *M = BB_ptr->getModule();

      while(BB_ptr!=BB_end)
      {
        //Takes care of a loop at a time
        unsigned depth = 0;
        while(LI.getLoopDepth(&*BB_ptr)!=1 and BB_ptr!=BB_end)
        {
          Instruction* op = &*(BB_ptr->begin());
          IRBuilder<> builder(op);
          builder.SetInsertPoint(&*BB_ptr, --(BB_ptr->end()));
          // Insert a call to our function.
          builder.CreateCall(cntFunc);
          depth =1;
          ++BB_ptr;
        }

        if(LI.getLoopDepth(&*BB_ptr)==1)
        {
          //Insert counter
          ++BB_ptr;
          Instruction* op = &*(BB_ptr->begin());
          IRBuilder<> builder(op);
          builder.SetInsertPoint(&*BB_ptr, BB_ptr->begin());
          // Insert a call to our function.
          builder.CreateCall(logFunc);
          depth =1;
        }

        while(LI.getLoopDepth(&*BB_ptr)!=0)
        {
          while(LI.getLoopDepth(&*BB_ptr)==depth and BB_ptr!=BB_end)
          {
            ++BB_ptr;
          }

          if(LI.getLoopDepth(&*BB_ptr)>depth)
          {
            //Insert counter
            ++BB_ptr;
            Instruction* op = &*(BB_ptr->begin());
            IRBuilder<> builder(op);
            builder.SetInsertPoint(&*BB_ptr, BB_ptr->begin());
            // Insert a call to our function.
            builder.CreateCall(logNFunc);

            depth = LI.getLoopDepth(&*BB_ptr);
            ++BB_ptr;
          }

          else
            ++BB_ptr;
        }
      }

      return false;
    }
  };
}

char DynProfilePass::ID = 0;

static void registerDynProfilePass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new DynProfilePass());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerDynProfilePass);
