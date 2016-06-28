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
#include "llvm/ADT/Twine.h"
#include "llvm/ADT/StringRef.h"
#include <cstdlib>
#include <cmath>
#include <vector>
using namespace llvm;

namespace
{
  struct DynProfilePass : public ModulePass 
  {
    static char ID;
    DynProfilePass() : ModulePass(ID) {}
    static std::vector<GlobalVariable*> g_vars;

    void getAnalysisUsage(AnalysisUsage &AU) const override
    {
      AU.setPreservesCFG();
      AU.addRequired<LoopInfoWrapperPass>();
    }

    bool doInitialization(Module &M) override
    {




      return true;
    }

    virtual bool runOnModule(Module &M) override
    { 

      LLVMContext &c = M.getContext();
      Type *intTy = TypeBuilder<int,false>::get(c);

      GlobalVariable* gvar_ptr_a = new GlobalVariable(/*Module=*/M, 
        /*Type=*/intTy,
        /*isConstant=*/false,
        /*Linkage=*/GlobalValue::InternalLinkage,
        /*Initializer=*/0,
        /*Name=*/"a");
      gvar_ptr_a->setAlignment(4);
      Constant* const_int_a = ConstantInt::get(intTy, 1);
      gvar_ptr_a->setInitializer(const_int_a);

      GlobalVariable* gvar_ptr_b = new GlobalVariable(/*Module=*/M, 
        /*Type=*/intTy,
        /*isConstant=*/false,
        /*Linkage=*/GlobalValue::InternalLinkage,
        /*Initializer=*/0, // has initializer, specified below
        /*Name=*/"b");
      gvar_ptr_b->setAlignment(4);
      Constant* const_int_b = ConstantInt::get(intTy, 1);
      gvar_ptr_b->setInitializer(const_int_b);

      for(Module::iterator F = M.begin(), m_end = M.end(); F!=m_end; ++F)
      {
        if (!F->isDeclaration())
        {
          LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>(*F).getLoopInfo();

          Function::iterator BB_ptr = F->begin();
          Function::iterator BB_end = F->end();
          Module *M = BB_ptr->getModule();

          while(BB_ptr!=BB_end)
          {

            Value *p = M->getNamedGlobal("a");
            Value *q = M->getNamedGlobal("b");

            //Takes care of a loop at a time
            unsigned depth = 0;
            while(LI.getLoopDepth(&*BB_ptr)!=1 and BB_ptr!=BB_end)
            {
              //Resets the count for inner and outer loops
              Instruction* op_reset = &*(BB_ptr->begin());
              IRBuilder<> builder(op_reset);
              builder.SetInsertPoint(&*BB_ptr, BB_ptr->begin());
              if (GlobalVariable* g_reset_a = dyn_cast<GlobalVariable>(p))
              {
                LoadInst *Load_reset = builder.CreateLoad(g_reset_a);
                Value *Inc_reset = builder.CreateAdd(builder.getInt32(1), builder.getInt32(0));
                StoreInst *Store = builder.CreateStore(Inc_reset, g_reset_a);
              }
              if (GlobalVariable* g_reset_b = dyn_cast<GlobalVariable>(q))
              {
                LoadInst *Load_reset = builder.CreateLoad(g_reset_b);
                Value *Inc_reset = builder.CreateAdd(builder.getInt32(1), builder.getInt32(0));
                StoreInst *Store = builder.CreateStore(Inc_reset, g_reset_b);
              }

              //Inserts a new line
              std::vector<llvm::Type *> args;
              args.push_back(llvm::Type::getInt8PtrTy(c));
              llvm::FunctionType *printfType =
                  llvm::FunctionType::get(builder.getInt32Ty(), args, true);
              llvm::Constant *printfFunc =
                  M->getOrInsertFunction("printf", printfType);
              std::vector<llvm::Value *> values;
              llvm::Value *formatStr = builder.CreateGlobalStringPtr("\n");
              values.clear();
              values.push_back(formatStr);
              builder.CreateCall(printfFunc, values);
              ++BB_ptr;
            }

            if(LI.getLoopDepth(&*BB_ptr)==1)
            {
              //Inserts counter
              ++BB_ptr; //Body of the for loop
              Instruction* op = &*(BB_ptr->begin());
              IRBuilder<> builder(op);
              builder.SetInsertPoint(&*BB_ptr, BB_ptr->begin());
              if (GlobalVariable* g = dyn_cast<GlobalVariable>(p))
              {
                //Increment the global variable
                LoadInst *Load = builder.CreateLoad(g);
                Value *Inc = builder.CreateAdd(builder.getInt32(1), Load);
                StoreInst *Store = builder.CreateStore(Inc, p);

                //Print the value
                std::vector<llvm::Type *> args;
                args.push_back(llvm::Type::getInt8PtrTy(c));
                llvm::FunctionType *printfType =
                    llvm::FunctionType::get(builder.getInt32Ty(), args, true);
                llvm::Constant *printfFunc =
                    M->getOrInsertFunction("printf", printfType);
                std::vector<llvm::Value *> values;
                llvm::Value *formatStr = builder.CreateGlobalStringPtr("Value outer = %d\n");
                values.clear();
                values.push_back(formatStr);
                values.push_back(Load);
                builder.CreateCall(printfFunc, values);
              }
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
                //Inserts counter
                ++BB_ptr;
                Instruction* op = &*(BB_ptr->begin());
                IRBuilder<> builder(op);
                builder.SetInsertPoint(&*BB_ptr, BB_ptr->begin());
                // Insert a call to our function.
                if (GlobalVariable* gb = dyn_cast<GlobalVariable>(q))
                {
                  LoadInst *Load_b = builder.CreateLoad(gb);
                  Value *Inc_b = builder.CreateAdd(builder.getInt32(1), Load_b);
                  StoreInst *Store = builder.CreateStore(Inc_b, q);

                  std::vector<llvm::Type *> args;
                  args.push_back(llvm::Type::getInt8PtrTy(c));
                  llvm::FunctionType *printfType =
                      llvm::FunctionType::get(builder.getInt32Ty(), args, true);
                  llvm::Constant *printfFunc =
                      M->getOrInsertFunction("printf", printfType);
                  std::vector<llvm::Value *> values;
                  llvm::Value *formatStr = builder.CreateGlobalStringPtr("value nested = %d\n");
                  values.clear();
                  values.push_back(formatStr);
                  values.push_back(Load_b);
                  builder.CreateCall(printfFunc, values);
                }
                depth = LI.getLoopDepth(&*BB_ptr);
                ++BB_ptr;
              }
              else
              {
                ++BB_ptr;
              }
            }
          }
        }
      }
      //M.print(dbgs(),0);
      return true;
    }
  };
}

char DynProfilePass::ID = 0;

static RegisterPass<DynProfilePass> X("dp", "Profiling loops");
