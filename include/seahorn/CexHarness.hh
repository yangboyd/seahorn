#ifndef _CEXHARNESS__HH_
#define _CEXHARNESS__HH_

#include "llvm/IR/Module.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "seahorn/Bmc.hh"
#include "seahorn/MemSimulator.hh"

namespace seahorn
{
  using namespace llvm;

  // Wrapper for BmcTrace and MemSimulator objects.
  // 
  // Both are defined wrt to the same BmcTrace but they can have
  // different models associated (accessed via the virtual method
  // eval).
  class BmcTraceWrapper {
    BmcTrace &m_trace;
  public:
    BmcTraceWrapper(BmcTrace &trace):
      m_trace(trace) {}
    
    /// The number of basic blocks in the trace 
    virtual unsigned size () const
    { return m_trace.size(); }
    
    /// The basic block at a given location 
    virtual const llvm::BasicBlock* bb (unsigned loc) const
    { return m_trace.bb(loc); }

    /// The value of the instruction at the given location     
    virtual Expr eval (unsigned loc, const llvm::Instruction &inst, bool complete);
  };

  class BmcTraceMemSim: public BmcTraceWrapper {
    MemSimulator &m_mem_sim;
  public:
    
    BmcTraceMemSim(MemSimulator &mem_sim):
      BmcTraceWrapper(mem_sim.trace()),
      m_mem_sim(mem_sim) {}

    virtual Expr eval (unsigned loc, const llvm::Instruction &inst, bool complete) override;
  };

  
  std::unique_ptr<llvm::Module> createCexHarness (BmcTraceWrapper &trace, 
						  const DataLayout &dl,
						  const TargetLibraryInfo &tli);
}

#endif
