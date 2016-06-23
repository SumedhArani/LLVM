#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/ADT/DenseMap.h"
#include <cmath>
using namespace llvm;

namespace
{
   struct HotField : public FunctionPass 
   {
      static char ID;
      HotField() : FunctionPass(ID) {}
      typedef DenseMap<Type*, DenseMap<const Value *, int>> InstMap;

      void getAnalysisUsage(AnalysisUsage &AU) const override 
      {
         AU.setPreservesCFG();
         AU.addRequired<LoopInfoWrapperPass>();
      }

      virtual bool runOnFunction(Function &F) override
      {
         static unsigned numOfInst = 0;
         InstMap imap;
         LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();

         for (Function::iterator B = F.begin(), E = F.end(); B != E; ++B)
         {
            for(BasicBlock:: iterator I = B->begin(), e = B->end(); I!=e; ++I)
            {
               for (unsigned num = 0; num < I->getNumOperands(); ++num)
               {
                  if (Instruction *gep = dyn_cast<Instruction>(I->getOperand(num)))
                  {
                     if (gep->getOpcode() == Instruction::GetElementPtr)
                     {
                        if (GetElementPtrInst *G = dyn_cast<GetElementPtrInst>(gep))
                        {
                           Type* ty = G->getSourceElementType();
                           Type* pty = G->getPointerOperandType();
                           if(isa<StructType>(*ty))
                           {
                              if(gep->getNumOperands()>2)
                              {
                                 unsigned wt = pow(10, LI.getLoopDepth(&*B));
                                 imap[&*pty][&*(G->getOperand(2))]+=wt;
                              }
                           }
                        }                           
                     }
                  }
               }
            }
         }

         /*
         //normalize the data
         double max=0;
         double min=0;
         //find max and min to normalize the data
         for(DenseMapIterator<const Value *, int> B = imap.begin(), E = imap.end(); B != E; ++B)
         {
            if(B->second<min)
               min =B->second;
            else if(B->second>max)
               max =B->second;
         }
         double diff =max -min;
         for(DenseMapIterator<const Value *, int> B = imap.begin(), E = imap.end(); B != E; ++B)
         {
            B->second = (B->second - min)/diff;
         }

         for(DenseMapIterator<const Value *, int> b = imap.begin(), E = imap.end(); b != E; ++b)
         {
            if(b->second > 0.6)
               errs() << *(b->first) << " : " << b->second << " (Hot)\n";
            else
               errs() << *(b->first) << " : " << b->second << " (Cold)\n";
         }
         */
         errs() << F.getName() << "\n";
         for(DenseMapIterator<Type*, DenseMap<const Value *, int>> b = imap.begin(), E = imap.end(); b != E; ++b)
         {
            errs() << *(b->first) << " :\n";
            for(DenseMapIterator<const Value *, int> be = b->second.begin(), en = b->second.end(); be != en; ++be)
            {
               errs() << *(be->first) << " : " << be->second << "\n";
            }
         }
         errs() << "\n";
         return false;
      }
   };
}

char HotField::ID = 0;
static RegisterPass<HotField> X("hf", "Find hot fields");