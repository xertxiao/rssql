#ifndef STREAMLOG_PROTOTYPE_AGGR_H
#define STREAMLOG_PROTOTYPE_AGGR_H

#include "types.h"

enum AggrFn {
    COUNT, SUM, AVG, MAX, MIN
};

Type getOutputType(AggrFn fn, Type inputType);


#endif //STREAMLOG_PROTOTYPE_AGGR_H
