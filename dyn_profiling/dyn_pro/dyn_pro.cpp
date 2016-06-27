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
#include "llvm/IR/TypeBuilder.h"
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

    bool doInitialization(Module &M) override
    {
      LLVMContext &c = M.getContext();
      Type *intTy = TypeBuilder<int,false>::get(c);
      //Value *p = M.getOrInsertGlobal("i",intTy);
      GlobalVariable* gvar_ptr_abc = new GlobalVariable(/*Module=*/M, 
        /*Type=*/intTy,
        /*isConstant=*/false,
        /*Linkage=*/GlobalValue::ExternalLinkage,
        /*Initializer=*/0, // has initializer, specified below
        /*Name=*/"abc");
      gvar_ptr_abc->setAlignment(4);
      return true;
    }

    virtual bool runOnFunction(Function &F) override
    { 
      LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();

      Function::iterator BB_ptr = F.begin();
      Function::iterator BB_end = F.end();
      Module *M = BB_ptr->getModule();

      LLVMContext &c = M->getContext();
      Constant* printFunc = F.getParent()->getOrInsertFunction("print", Type::getVoidTy(c), Type::getInt32Ty(c), NULL);

      Type *intTy = TypeBuilder<int,false>::get(c);
      Value *p = M->getOrInsertGlobal("abc",intTy);

      while(BB_ptr!=BB_end)
      {
        //Takes care of a loop at a time
        unsigned depth = 0;
        while(LI.getLoopDepth(&*BB_ptr)!=1 and BB_ptr!=BB_end)
        {
          ++BB_ptr;
        }

        if(LI.getLoopDepth(&*BB_ptr)==1)
        {
          //Insert counter
          ++BB_ptr;
          Instruction* op = &*(BB_ptr->begin());
          IRBuilder<> builder(op);
          builder.SetInsertPoint(&*BB_ptr, BB_ptr->begin());
          if (GlobalVariable* g = dyn_cast<GlobalVariable>(p))
          {
            LoadInst *Load = builder.CreateLoad(g);
            Value *Inc = builder.CreateAdd(builder.getInt32(1), Load);
            //Gives ERROR ----------
            //StoreInst *Store = builder.CreateStore(Inc, p);
            Value* args[] = {Inc};
            //builder.CreateCall(printFunc,args);
          }
          // Insert a call to our function.
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
