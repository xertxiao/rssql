#ifndef _STREAMLOG_PROTOTYPE_PHY_QUEUE_
#define _STREAMLOG_PROTOTYPE_PHY_QUEUE_

#ifndef _QUEUE_
#include "execution/queues/queue.h"
#endif

namespace Physical {

	// forward decl
	struct Operator;
	struct Store;
	

	struct Queue {		
		/// indexes in PlanManagerImpl.queues
		unsigned int id;

		/// Instantiation of this queue
		Execution::Queue *instQueue;
		

        Operator *source;

        /// The destination operator
        Operator *dest;

        /// The input mIndex in the destination operator for this
        /// queue
        unsigned int index;

	};
}

#endif
