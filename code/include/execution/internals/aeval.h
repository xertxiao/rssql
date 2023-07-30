#ifndef _STREAMLOG_PROTOTYPE_AEVAL_
#define _STREAMLOG_PROTOTYPE_AEVAL_

#include <string>
#include <vector>

#ifndef _EVAL_CONTEXT_
#include "execution/internals/eval_context.h"
#endif

namespace Execution {
	
	enum AOp {
		FLT_ADD,
		FLT_SUB,
		FLT_MUL,
		FLT_DIV,
		FLT_CPY,
		FLT_UMX,
		FLT_UMN,
        FLT_ONE,
        FLT_ADO
//		FLT_AVG
	};
	
	struct AInstr {
		AOp op;
		Row r1;
        Column c1;
		Row r2;
        Column c2;
		Row dr;
        Column dc;
	};

	struct AEval {

    private:
		static const unsigned int MAX_INSTRS = 20;
        std::vector<AInstr> instrs;
        std::vector<Tuple *> *roles;



	public:
		AEval () = default;
		~AEval () = default;
		
		int addInstr (AInstr instr) {
            instrs.push_back(instr);
            return instrs.size() - 1;
        }
		int setEvalContext (EvalContext *evalContext) {
            this->roles = &(evalContext->mRoles);
            return 0;
        }
        unsigned int getInstrSize() {return instrs.size();}

		inline void eval () {
			for (unsigned int i = 0 ; i < instrs.size() ; i++) {
				
				switch (instrs[i].op) {
				case FLT_ADD: 
					FLOC(instrs[i].dr, instrs[i].dc) = ROUND2(
						FLOC (instrs[i].r1, instrs[i].c1)) +
                                ROUND2(FLOC (instrs[i].r2, instrs[i].c2));
					break;
				
				case FLT_SUB:
					FLOC(instrs[i].dr, instrs[i].dc) = ROUND2(
					
						FLOC (instrs[i].r1, instrs[i].c1)) -
                                ROUND2(FLOC (instrs[i].r2, instrs[i].c2));
					break;
				
				case FLT_MUL:

					FLOC(instrs[i].dr, instrs[i].dc) = ROUND2(
					
						FLOC (instrs[i].r1, instrs[i].c1) *
						FLOC (instrs[i].r2, instrs[i].c2));
					break;

				case FLT_DIV:

					FLOC(instrs[i].dr, instrs[i].dc) = ROUND2(
					
						FLOC (instrs[i].r1, instrs[i].c1) /
                            FLOC (instrs[i].r2, instrs[i].c2));
					break;

				case FLT_CPY:
                        FLOC (instrs[i].dr, instrs[i].dc) = ROUND2(
						FLOC (instrs[i].r1, instrs[i].c1));
					break;

				case FLT_UMX:
					if (FLOC (instrs [i].r1, instrs [i].c1) <
						FLOC (instrs [i].r2, instrs [i].c2)) {
						
						FLOC (instrs [i].dr, instrs [i].dc) = ROUND2(
							FLOC (instrs [i].r2, instrs [i].c2));
					}

					else {
						FLOC (instrs [i].dr, instrs [i].dc) = ROUND2(
							FLOC (instrs [i].r1, instrs [i].c1));
					}
					break;
					
				case FLT_UMN:
					if (FLOC (instrs [i].r1, instrs [i].c1) >
						FLOC (instrs [i].r2, instrs [i].c2)) {
						
						FLOC (instrs [i].dr, instrs [i].dc) = ROUND2(
							FLOC (instrs [i].r2, instrs [i].c2));
					}
					
					else {
						FLOC (instrs [i].dr, instrs [i].dc) = ROUND2(
							FLOC (instrs [i].r1, instrs [i].c1));
					}
					break;

                    case FLT_ONE:
                        FLOC (instrs[i].dr, instrs[i].dc) = 1.0;
                        break;

                    case FLT_ADO:
                        FLOC(instrs[i].dr, instrs[i].dc) =
                                FLOC(instrs[i].dr, instrs[i].dc) +
                                1.0;
                        break;

//				case FLT_AVG:
//					FLOC (instrs [i].dr, instrs[i].dc) =
//						FLOC (instrs [i].r1, instrs [i].c1) /
//						(1.0 * ILOC (instrs [i].r2, instrs [i].c2));
//					break;
				}
			}
		}
	};
}


#endif
