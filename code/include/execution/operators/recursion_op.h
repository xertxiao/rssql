#ifndef _STREAMLOG_PROTOTYPE_RECURSION_OP_
#define _STREAMLOG_PROTOTYPE_RECURSION_OP_

#ifndef _OPERATOR_
#include "execution/operators/operator.h"
#endif

#ifndef _QUEUE_
#include "execution/queues/queue.h"
//#include "beval.h"

#endif

namespace Execution {
	
	/**
	 * Binary join is a symmetric operator that joins two relations and
	 * produces another relation.
	 */
	class RecursionOp : public Operator {		
	private:
		/// System-wide unique identifier
		unsigned int mId;

        /// System-wide unique identifier for the other corresponding op
		unsigned int mIdOther;

		/// System log
		std::ostream &LOG;

        /// Eval
//        BEval *recurEval;
//        EvalContext *mEvalContext;
//        unsigned int INPUT_CONTEXT = 2;
    private:
        std::vector<Execution::Element> *mOutBuffer;
		
	public:
		RecursionOp  (unsigned int id, std::ostream &LOG);
		virtual ~RecursionOp  ();

        unsigned int mFlowBackTo;

		int setOtherOp (unsigned int idOther) {this->mIdOther = idOther; return 0;};
        int setOutBuffer (std::vector<Execution::Element> *outBuffer) {this->mOutBuffer = outBuffer; return 0;}


        // Execution
		int run (); 
	};	
}

#endif // _RECURSION_OP_
