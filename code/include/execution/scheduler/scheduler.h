#ifndef _STREAMLOG_PROTOTYPE_SCHEDULER_
#define _STREAMLOG_PROTOTYPE_SCHEDULER_

#include "execution/operators/operator.h"

namespace Execution {
	class Scheduler {
	public:
		/**
		 * Add a new operator to schedule
		 */
		virtual int addOperator (Operator *op) = 0;
		
		/**
		 * Schedule the operators for a prescribed set of time units.
		 */		
		virtual int run(long long int numTimeUnits) = 0;
	};
}

#endif
