#ifndef _STREAMLOG_PROTOTYPE_PROJ_OP_
#define _STREAMLOG_PROTOTYPE_PROJ_OP_

#ifndef _OPERATOR_
#include "execution/operators/operator.h"
#endif

#ifndef _QUEUE_
#include "execution/queues/queue.h"
#include "execution/internals/aeval.h"

#endif


namespace Execution {
	/**
	 * A generic projection operator.
	 *
	 * Each attribute in the output schema is an arbitrary arithmetic
	 * function over the input attributes and constants 
	 *
	 */
	class ProjectOp : public Operator {
	private:
		/// System-wide unique mId
		unsigned int mId;
		
		/// System log
		std::ostream &LOG;

        //TODO: for now public for the ease of debugging.
    public:
        /// Eval
        AEval *mProjEval;
        EvalContext *mEvalContext;
        static const unsigned int PROJ_INPUT_ROLE = 0;
        static const unsigned int PROJ_OUTPUT_ROLE = 1;

    public:
		ProjectOp(unsigned int id, std::ostream &LOG);
		virtual ~ProjectOp();
		
        // Initializing routing
		int setInputQueue (Queue *inputQueue);
		int setOutputQueue (Queue *outputQueue);
		int setProjEvaluator (AEval *projEval);
		
		int run();
	};
}

#endif