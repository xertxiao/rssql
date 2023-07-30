#ifndef STREAMLOG_PROTOTYPE_MIN_AGGR_H
#define STREAMLOG_PROTOTYPE_MIN_AGGR_H


#include <vector>
#include "execution/operators/operator.h"
#include "execution/queues/queue.h"
#include "execution/synopses/win_syn.h"
#include "execution/internals/eval_context.h"
#include "execution/internals/beval.h"
#include "execution/internals/aeval.h"
#include "interface/table_source.h"

namespace Execution {

    class MinAggOp : public Operator {
    private:
        /// System-wide unique mId
        unsigned int mId;

        /// System log
        std::ostream &LOG;

        // TODO: make it public for debugging
    public:
        /// Eval
        AEval *mUpdateEval;
        AEval *mInitEval;
        BEval *mCompareEval;
        EvalContext *mEvalContext;
        static const unsigned int MIN_INPUT_ROLE = 0;
        static const unsigned int MIN_OLD_ROLE = 1;
        static const unsigned int MIN_OUTPUT_ROLE = 2;


    public:
        MinAggOp(unsigned int id, std::ostream &LOG);
        virtual ~MinAggOp();

        // Initializing routing
        int setInputQueue (Queue *inputQueue);
        int setOutputQueue (Queue *outputQueue);
        int setProjEvaluator (AEval *projEval);

        int run();
    };
}


#endif //STREAMLOG_PROTOTYPE_MIN_AGGR_H
