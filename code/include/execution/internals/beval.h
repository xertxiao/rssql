#ifndef _STREAMLOG_PROTOTYPE_BEVAL_
#define _STREAMLOG_PROTOTYPE_BEVAL_

#include <string.h>

#ifndef _EVAL_CONTEXT_
#include "execution/internals/eval_context.h"
#endif

#ifndef _AEVAL_
#include "execution/internals/aeval.h"
#endif

#define CHECK(b) { if(!(b)) return false;}
#define EVAL(e) { if ((e)) (e)-> eval();}

namespace Execution {

	// The set of boolean operators
	enum BOp {
        A_TRUE,
        A_FALSE,
		FLT_LT,
		FLT_LE,
		FLT_GT,
		FLT_GE,
		FLT_EQ,
		FLT_NE,
		C_FLT_LT,
		C_FLT_LE,
		C_FLT_GT,
		C_FLT_GE,
		C_FLT_EQ,
		C_FLT_NE
	};
	
	struct BInstr {
		BOp            op;
		Row  r1;
		Column         c1;
		Row   r2;
		Column         c2;
		AEval         *e1;
		AEval         *e2;
	};
	
	class BEval {
	private:
		static const unsigned int MAX_INSTRS = 20;
		
        std::vector<BInstr> instrs;
        std::vector<Tuple *> *roles;
		
	public:
		BEval () = default;
		~BEval () = default;

        int addInstr (BInstr instr) {
            instrs.push_back(instr);
            return instrs.size() - 1;
        }

        int setEvalContext (EvalContext *evalContext) {
            this->roles = &(evalContext->mRoles);
            return 0;
        }
		
		inline bool eval () const {
			for (unsigned int i = 0 ; i < instrs.size(); i++) {
				switch (instrs [i].op) {
                    case A_FALSE:
                        CHECK(false);
                        break;
                    case A_TRUE:
                        CHECK(true);
                        break;

				case FLT_LT: 
					CHECK(FLOC(instrs[i].r1, instrs[i].c1) <
						  FLOC(instrs[i].r2, instrs[i].c2));
					break;

				case FLT_LE:
					CHECK(FLOC(instrs[i].r1, instrs[i].c1) <=
						  FLOC(instrs[i].r2, instrs[i].c2));
					break;

				case FLT_GT:
                        CHECK(FLOC(instrs[i].r1, instrs[i].c1) >
						  FLOC(instrs[i].r2, instrs[i].c2));
					break;

				case FLT_GE:
					CHECK(FLOC(instrs[i].r1, instrs[i].c1) >=
						  FLOC(instrs[i].r2, instrs[i].c2));
					break;

				case FLT_EQ:
					CHECK(FLOC(instrs[i].r1, instrs[i].c1) ==
						  FLOC(instrs[i].r2, instrs[i].c2));
					break;

				case FLT_NE:
					CHECK(FLOC(instrs[i].r1, instrs[i].c1) !=
						  FLOC(instrs[i].r2, instrs[i].c2));
					break;
					
				case C_FLT_LT:
					EVAL (instrs[i].e1);
					EVAL (instrs[i].e2);
					
					CHECK(FLOC(instrs[i].r1, instrs[i].c1) <
						  FLOC(instrs[i].r2, instrs[i].c2));
					break;

				case C_FLT_LE:
					EVAL (instrs[i].e1);
					EVAL (instrs[i].e2);
					
					CHECK(FLOC(instrs[i].r1, instrs[i].c1) <=
						  FLOC(instrs[i].r2, instrs[i].c2));
					break;

				case C_FLT_GT:
					EVAL (instrs[i].e1);
					EVAL (instrs[i].e2);
					
					CHECK(FLOC(instrs[i].r1, instrs[i].c1) >
						  FLOC(instrs[i].r2, instrs[i].c2));
					break;

				case C_FLT_GE:
					EVAL (instrs[i].e1);
					EVAL (instrs[i].e2);
					
					CHECK(FLOC(instrs[i].r1, instrs[i].c1) >=
						  FLOC(instrs[i].r2, instrs[i].c2));
					break;

				case C_FLT_EQ:
					EVAL (instrs[i].e1);
					EVAL (instrs[i].e2);
					
					CHECK(FLOC(instrs[i].r1, instrs[i].c1) ==
						  FLOC(instrs[i].r2, instrs[i].c2));
					break;

				case C_FLT_NE:
					EVAL (instrs[i].e1);
					EVAL (instrs[i].e2);
					
					CHECK(FLOC(instrs[i].r1, instrs[i].c1) !=
						  FLOC(instrs[i].r2, instrs[i].c2));
					break;
					
				default:
					return false;
				}
			}
			
			return true;
		}
	};
}
#endif
