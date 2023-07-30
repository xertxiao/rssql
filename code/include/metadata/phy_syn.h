#ifndef _STREAMLOG_PROTOTYPE_PHY_SYN_
#define _STREAMLOG_PROTOTYPE_PHY_SYN_


#include "execution/synopses/win_syn.h"
#include <vector>

namespace Physical {

	// forward decl.
	struct Operator;
	
	struct Store;
	
	struct Synopsis {
		
		/// indexes the array PlanManagerImpl.syns
		unsigned int id;


        std::vector<Execution::Tuple> store;
		
		/// Operator that owns the synopsis
		Operator *ownOp;

        Execution::WindowSynopsis     *winSyn;

	};
}

#endif
