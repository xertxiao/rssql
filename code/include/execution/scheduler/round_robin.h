#ifndef _STREAMLOG_PROTOTYPE_ROUND_ROBIN_
#define _STREAMLOG_PROTOTYPE_ROUND_ROBIN_

#include "execution/scheduler/scheduler.h"
#include <vector>

namespace Execution {
	class RoundRobinScheduler : public Scheduler {
	private:
		static const unsigned int MAX_OPS = 100;
		
		// Operators that we are scheduling
		std::vector<Operator> ops [MAX_OPS];
		
		// Number of operators that we have to schedule
		unsigned int numOps;
		
	public:
		RoundRobinScheduler ();		
		virtual ~RoundRobinScheduler ();
		
		// Inherited from Scheduler
		int addOperator (Operator op);				
		int run (long long int numTimeUnits);
		int stop ();
		int resume ();
	};
}

#endif
