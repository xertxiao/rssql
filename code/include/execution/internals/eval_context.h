#ifndef _STREAMLOG_PROTOTYPE_EVAL_CONTEXT_
#define _STREAMLOG_PROTOTYPE_EVAL_CONTEXT_

#ifndef _TUPLE_

#include <cassert>
#include "execution/internals/tuple.h"
#endif

//#define ILOC(c,o) (*(((int *)mRoles[(c)]) + (o)))
//#define FLOC(c,o) (*(((float *)mRoles[(c)]) + (o)))
//#define BLOC(c,o) (*((mRoles[(c)]) + (o)))
//#define CLOC(c,o) ((mRoles[(c)]) + (o))

/// For now assuming tuples all have float type.
//#define FLOC(c,o) (*(*(mRoles)[c])[o])

#define FLOC(r,c) ((roles->at(r))->at(c))
#define ROUND2(f) floorf(f * 100) / 100

//float* FLOC(unsigned int r, unsigned int c) {
//    return &(mRoles->at(r))->at(c);
//}


//TODO: what is this?
namespace Execution {
    class EvalContext {
    private:
        static const unsigned int MAX_ROLES = 10;
    public:
        std::vector<Tuple *> mRoles;

        EvalContext() {
            for (int i = 0; i < MAX_ROLES; i++) mRoles.push_back(nullptr);
        }
        ~EvalContext() {
            for (auto p : mRoles) delete p;
        }

        void bind(Tuple *tuple, unsigned int roleId) {
            assert(roleId < MAX_ROLES);
            mRoles[roleId] = tuple;
        }


    };
//	struct EvalContext {
//		static const unsigned int MAX_ROLES = 10;
//		char *mRoles [MAX_ROLES];
//		void bind (Tuple tuple, unsigned int roleId);
//	};
}

#endif
