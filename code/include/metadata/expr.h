#ifndef STREAMLOG_PROTOTYPE_EXPR_H
#define STREAMLOG_PROTOTYPE_EXPR_H

#include <variant>
#include "common/types.h"
#include "metadata/attribute.h"
#include "common/op.h"

namespace Physical {

    enum ExprKind {
        CONST_VAL, ATTR_REF, COMP_EXPR
    };

    struct Expr {
        ExprKind kind;
        Type outType;

        union {
            int iVal;
            float fVal;

            struct {
                ArithOp  op;
                Expr    *left;
                Expr    *right;
            } COMP_EXPR;

            Metadata::Attribute   attr;

        } u;
    };

    struct BExpr {
        CompOp      op;
        Expr       *left;
        Expr       *right;
        BExpr      *next;  // next predicate in the conjunction
    };
}

#endif //STREAMLOG_PROTOTYPE_EXPR_H
