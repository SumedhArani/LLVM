#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/SCCIterator.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/SourceMgr.h"
#include <string>
#include <vector>
#include <stack>
#define V 100
using namespace llvm;

namespace
{
  struct CountSCCPass : public FunctionPass 
  {
    static char ID;
    CountSCCPass() : FunctionPass(ID) {}
    typedef DenseMap<const BasicBlock *, int> BBgraph;


    bool runOnFunction(Function &F) 
    {
      unsigned i =0;
      errs() << "SCCs for Function " << F.getName() << ": ";
      //an iterator that iterates through the func returning basic blocks
      //dense map cause my key is small
      BBgraph graph;
      
      for(Function::iterator bb=F.begin(), end=F.end(); bb!= end; ++bb)
      {
        //assign numbers to the basic blocks within a function
        graph[&*bb] = i++;
      }
      unsigned sccNum = 0;
      sccNum = findSCC(graph, F);
      errs() << sccNum << "\n";
      return true;
    }
    //wrapper funciton
    int findSCC(BBgraph graph, Function &F);
    //helper funtion
    int SCC(const BasicBlock* u, int disc[], int low[], std::stack<int> *st, bool stackMember[], BBgraph graph);

  };
}

char CountSCCPass::ID = 0;
static RegisterPass<CountSCCPass> Y("conncomp", "Print SCCs of each function CFG");

int CountSCCPass::findSCC(BBgraph graph, Function &F)
{
  //wrapper funtion
  unsigned total = 0;
  int *disc = new int[V];
  int *low = new int[V];
  bool *stackMember = new bool[V];
  std::stack<int> *st = new std::stack<int>();
 
  // Initialize disc and low, and stackMember arrays
  for (int i = 0; i < V; i++)
  {
    disc[i] = -1;
    low[i] = -1;
    stackMember[i] = false;
  }
 
  // Call the recursive helper function to find strongly
  // connected components in DFS tree with vertex 'i'
  for(Function::iterator bb=F.begin(), end=F.end(); bb!= end; ++bb)
  {
    //assign numbers to the basic blocks within a function
    unsigned temp=0;
    if (disc[graph[&*bb]] == -1)
       temp = SCC(&*bb, disc, low, st, stackMember, graph);
    total += temp;
  }
  return total;
}

// u --> The vertex to be visited next
// disc[] --> Stores discovery times of visited vertices
// low[] -- >> earliest visited vertex (the vertex with minimum
//             discovery time) that can be reached from subtree
//             rooted with current vertex
// *st -- >> To store all the connected ancestors
// stackMember[] --> bool array for faster check whethera node is in stack

int CountSCCPass::SCC(const BasicBlock* u, int disc[], int low[], std::stack<int> *st, bool stackMember[], BBgraph graph)
{
    // A static variable is used for keeping track
    static int time = 0;
 
    // Initialize discovery time and low value
    disc[graph[u]] = low[graph[u]] = ++time;
    st->push(graph[u]);
    stackMember[graph[u]] = true;
    
    // Go through all basic block which are successor to the current one
    const TerminatorInst *TInst = u->getTerminator();
    for (unsigned c = 0, NSucc = TInst->getNumSuccessors(); c < NSucc; ++c)
    {
        BasicBlock* v = TInst->getSuccessor(c);
        // If v is not visited yet, then recur for it
        if (disc[graph[v]] == -1)
        {
            SCC(v, disc, low, st, stackMember, graph);
             // Case 1 (per above discussion on Disc and Low value)
            low[graph[u]]=low[graph[u]]<low[graph[v]]?low[graph[u]]:low[graph[v]];
        }
        // Update low value of 'u' only of 'v' is still in stack
        // To check if it's a back edge
        // Case 2
        else if (stackMember[graph[u]] == true)
            low[graph[u]]  = low[graph[u]]<disc[graph[v]]?low[graph[u]]:disc[graph[v]];
    }
    
    // head node found, pop the stack
    unsigned count=0;
    int w = 0;  // To store stack extracted vertices
    //if low and disc time is same then it is a head
    if (low[graph[u]] == disc[graph[u]])
    {
        while (st->top() != graph[u])
        {
            w = (int) st->top();
            stackMember[w] = false;
            st->pop();
            count++;
        }
        w = (int) st->top();
        stackMember[w] = false;
        st->pop();
        count++;
    }    
    return count;
}