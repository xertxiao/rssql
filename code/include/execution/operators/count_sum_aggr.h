#ifndef STREAMLOG_PROTOTYPE_COUNT_SUM_AGGR_H
#define STREAMLOG_PROTOTYPE_COUNT_SUM_AGGR_H

#include <vector>
#include "execution/operators/operator.h"
#include "execution/queues/queue.h"
#include "execution/synopses/win_syn.h"
#include "execution/internals/eval_context.h"
#include "execution/internals/beval.h"
#include "execution/internals/aeval.h"
#include "interface/table_source.h"


namespace Execution {
    class CountSumOp : public Operator {
    private:
        /// System-wide mId
        unsigned int mId;

        /// System log
        std::ostream &LOG;

    public:
        /// Eval
        AEval *mUpdateEval;
        AEval *mInitEval;
        BEval *mCompareEval;
        EvalContext *mEvalContext;

        static const unsigned int COUNT_SUM_INPUT_ROLE = 0;
        static const unsigned int COUNT_SUM_OLD_ROLE = 1;
        static const unsigned int COUNT_SUM_OUTPUT_ROLE = 2;


    public:
        CountSumOp(unsigned int id, std::ostream &LOG);
        virtual ~CountSumOp();

        // Execution
        int run ();
    };
}

#endif //STREAMLOG_PROTOTYPE_COUNT_SUM_AGGR_H
