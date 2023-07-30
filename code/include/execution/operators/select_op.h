#ifndef _STREAMLOG_PROTOTYPE_SELECT_OP_
#define _STREAMLOG_PROTOTYPE_SELECT_OP_

#ifndef _OPERATOR_
#include "execution/operators/operator.h"
#endif

#ifndef _QUEUE_
#include "execution/queues/queue.h"
//#include "beval.h"

#endif

namespace Execution {
	class SelectOp : public Operator {
	private:
		/// System-wide mId
		unsigned int mId;

		/// System log
		std::ostream &LOG;

		/// The selection predicate
		BEval *mPredicate;

		
	public:
		SelectOp(unsigned int id, std::ostream &LOG);
		virtual ~SelectOp();

        // Execution
		int run (); 
	};
}

#endif // _SELECT_OP_