#ifndef STREAMLOG_PROTOTYPE_PHY_OP_H
#define STREAMLOG_PROTOTYPE_PHY_OP_H

#include <vector>
#include "metadata/attribute.h"
#include "execution/operators/operator.h"
#include "metadata/phy_queue.h"
#include "metadata/phy_syn.h"
#include "expr.h"
#include "common/aggr.h"

namespace Physical {

    enum OperatorKind {
        // Selection
        PO_SELECT,

        // Projection
        PO_PROJECT,

        // Relation-Relation join
        PO_JOIN,

        // Recursion-Head
        PO_RECUR_HEAD,

        // Recursion-Tail
        PO_RECUR_TAIL,

        // Combinatin of a join followed by project
        PO_JOIN_PROJECT,

        // Group-by aggregation
        PO_GROUP_AGGR,

        // distinct
        PO_DISTINCT,

        // row window operator
        PO_ROW_WIN,

        // range window op
        PO_RANGE_WIN,

        // Istream
        PO_ISTREAM,

        // Union
        PO_UNION,

        // Source for a base stream
        PO_STREAM_SOURCE,

        // Base relation source
        PO_RELN_SOURCE,

        // "No-op" operator that sits on top of query views - other
        // queries that use the output of this query read from the query
        // source operator.  This is a dummy operator found at the
        // metadata level - when we generate the actual exec. operators we
        // will bypass it.
        PO_QUERY_SOURCE,

        // Output op that interfaces with the external world
        PO_OUTPUT,

        // Sink operator to consume tuples from unused ops
        PO_SINK,

        // System stream generator
        PO_SS_GEN
    };

    struct Operator {

        static const unsigned int MAX_IN_BRANCHING = 2;
        static const unsigned int MAX_GROUP_ATTRS = 10;
        static const unsigned int MAX_AGGR_ATTRS  = 10;

        /// indexes array PlanManagerImpl.ops
        unsigned int id;

        /// next & prev pointers to arrange the operators as a linked list
        /// for resource management purposes
        Operator *next;
        Operator *prev;

        /// Type of operator
        OperatorKind  kind;

        std::vector<size_t> attrIds;

        std::vector<Operator> inOPs;

        std::vector<Operator> outOPs;

        /// Input queues for each input
        std::vector<Queue> inQueues;
        std::vector<Queue> outQueues;

        /// Instantiated operator
        Execution::Operator *instOp;

        union {

            struct {
                // Id assigned by tableMgr
                unsigned int strId;

                // Id of the tableSource operator
                unsigned int srcId;
            } STREAM_SOURCE;

            struct {
                // Id assigned by tableMgr
                unsigned int relId;

                // Id of the tableSource operator
                unsigned int srcId;

                // Synopsis of the relation (used to generate MINUS tuples)
                Synopsis *outSyn;

            } RELN_SOURCE;

            struct {
                unsigned int outputId; // [[ Explanation ]]
                unsigned int queryId;
            } OUTPUT;

            struct {
                BExpr       *pred;
            } SELECT;

            struct {
                // Synopsis for out: required if project produces MINUS
                // tuples: (bStream == false)
                Synopsis *outSyn;

                // project expressions
                std::vector<Expr> projs;
            } PROJECT;

            struct {

                // My   output   schema    is   concatenation   of   first
                // numOuterAttrs  from left  input and  numInnerAttrs from
                // right  input. (Assert(numOuterAttrs +  numInnerAttrs ==
                // numAttrs).  We  store this information  at construction
                // time since the schemas of the input operators can later
                // "expand"

                unsigned int numOuterAttrs;
                unsigned int numInnerAttrs;

                // Join predicate
                BExpr *pred;

                // Synopsis for the inner relation
                Synopsis *innerSyn;

                // Synopsis for the outer relation
                Synopsis *outerSyn;

                // Synopsis for output (required to generate MINUS elements)
                Synopsis *joinSyn;
            } JOIN;

            struct {
                // My output schema is concatenation of first
                // numOuterAttrs from left input and numInnerAttrs from
                // right input. (Assert(numOuterAttrs + numInnerAttrs ==
                // numAttrs)

                unsigned int numOuterAttrs;
                unsigned int numInnerAttrs;

                // Join predicate
                BExpr *pred;

                // Synopsis for the inner relation
                Synopsis *innerSyn;
            } STR_JOIN;

            struct {
                // Project expressions
                std::vector<Expr> projs;


                // Join predicate
                BExpr *pred;

                // Synopsis for the inner relation
                Synopsis *innerSyn;

                // Synopsis for the outer relation
                Synopsis *outerSyn;

                // Synopsis for output (required to generate MINUS elements)
                Synopsis *joinSyn;
            } JOIN_PROJECT;

            struct {
                // Output projections
                std::vector<Expr> projs;


                // Join predicate
                BExpr *pred;

                // Synopsis for inner
                Synopsis *innerSyn;
            } STR_JOIN_PROJECT;

            struct {
                // grouping attributes
                std::vector<Metadata::Attribute> groupAttrs;


                // aggregated attributes
                std::vector<Metadata::Attribute> aggAttrs;


                // aggregation function
                std::vector<AggrFn> fn;

                // Synopsis for input
                Synopsis *inSyn;

                // Synopsis for output
                Synopsis *outSyn;
            } GROUP_AGGR;

            struct {

                Synopsis *outSyn;

            } DISTINCT;


            struct {
                // window mSlide
                unsigned int slideUnits;

                // window size
                unsigned int timeUnits;

                // Synopsis for the window
                Synopsis *winSyn;
            } RANGE_WIN;

            struct {
                // Synopsis for elements with current timestamp [[Explain]]
                Synopsis *nowSyn;

                // Store for the tuples in nowsyn
                Store *nowStore;
            } ISTREAM;

            struct {
                // Synopsis for elements with current timestamp [[Explain]]
                Synopsis *nowSyn;

                // Store for the tuples in nowsyn
                Store *nowStore;
            } DSTREAM;

            struct {
                // Synopsis of the input relation
                Synopsis *inSyn;
            } RSTREAM;

            struct {
                // Optional synopsis if one of the inputs is a relation.
                Synopsis *outSyn;
            } UNION;

            struct {
                // Internal Synopsis
                Synopsis *countSyn;

                // Output lineage synopsis
                Synopsis *outSyn;

                // Store for countSyn
                Store *countStore;
            } EXCEPT;

//            struct {
//                Store *outStores [MAX_OUT_BRANCHING];
//                Queue *outQueues [MAX_OUT_BRANCHING];
//                unsigned int numOutput;
//            } SS_GEN;
        } u;

    };
}

#endif //STREAMLOG_PROTOTYPE_PHY_OP_H
