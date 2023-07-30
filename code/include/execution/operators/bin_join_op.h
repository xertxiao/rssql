#ifndef _STREAMLOG_PROTOTYPE_BIN_JOIN_OP_
#define _STREAMLOG_PROTOTYPE_BIN_JOIN_OP_

#include "execution/operators/operator.h"
#include "execution/queues/queue.h"
#include "execution/synopses/win_syn.h"
#include "execution/internals/eval_context.h"
#include "execution/internals/beval.h"
#include "execution/internals/aeval.h"

namespace Execution {
	
	/**
	 * Binary join is a symmetric operator that joins two relations and
	 * produces another relation.
	 */
	class BinaryJoinOp : public Operator {		
	private:
		/// System-wide unique identifier
		unsigned int mId;
		
		/// System log
		std::ostream &LOG;

        /// Whether it is the start of the window
        bool mWindowStart = true;

        unsigned long _joinCt = 0;

        std::vector<Element> *thisWindowIdb;

        //TODO: for now public for the ease of debugging.
    public:
        /// Eval
        AEval *mJoinOut;
        BEval *mPredEval;
        EvalContext *mEvalContext;
        static const unsigned int JOIN_OUTER_ROLE = 0;
        static const unsigned int JOIN_INNER_ROLE = 1;
        static const unsigned int JOIN_OUTPUT_ROLE = 2;

    public:
		BinaryJoinOp (unsigned int id, std::ostream &LOG);
		virtual ~BinaryJoinOp ();

		// Executinon
		int run ();

	private:
        int join(std::vector<Element> *left, std::vector<Element> *right, std::vector<Element> &out);
        int runWithQbi ();
        int runWithOutQbi();

        void resetForNewWindow();

	};
}

#endif //_BIN_JOIN_OP_