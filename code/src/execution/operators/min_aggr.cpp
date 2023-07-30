#include "execution/operators/min_aggr.h"
#include <set>

namespace Execution {
    MinAggOp::MinAggOp (unsigned int id, std::ostream& LOG) : Operator(), mId(id) , LOG(LOG){
        this->mInitEval = new AEval();
        this->mUpdateEval = new AEval();
        this->mCompareEval = new BEval();
        this->mEvalContext = new EvalContext();

        this->mInitEval->setEvalContext(this->mEvalContext);
        this->mUpdateEval->setEvalContext(this->mEvalContext);
        this->mCompareEval->setEvalContext(this->mEvalContext);


    }
    MinAggOp::~MinAggOp (){
        delete this->mInitEval;
        delete this->mUpdateEval;
        delete this->mCompareEval;

        delete this->mEvalContext;
    };


    int MinAggOp::run () {
        // Debug



        if ((*mCurrentId) == this->mId) {
            auto start = std::chrono::high_resolution_clock::now();
            if(this->DEBUG) std::cerr << "=====MIN-"<<this->mId<<"-Start===== " << std::endl;
            std::vector<Element> tempOut;
            std::set<Tuple> updatedTupleKey;

            if (mClearOut) this->mOutWorkSpace->clear();

            for (auto e : *(this->mInWorkSpace)) {
                if(this->DEBUG) std::cerr<<"e in min: "<<e<<std::endl;
                std::vector<Element *> currentAggr;
                this->mStoringQbi->getAtomsByRByElement(e, this->AGGR_OFFSET, currentAggr);
                mEvalContext->bind(&(e.tuple), MIN_INPUT_ROLE);

                if (currentAggr.size() == 0) {
                    Element outE {e.tuple.size(), e.timestamp, e.dpt};
                    mEvalContext->bind(&(outE.tuple), MIN_OUTPUT_ROLE);
                    updatedTupleKey.insert(Tuple(e.tuple.begin(), e.tuple.begin() + this->AGGR_OFFSET));
                    std::cerr<<"in aggr min size=0 before "<<std::endl;
                    mInitEval->eval();
                    std::cerr<<"init? "<<std::endl;

                    this->mStoringQbi->insertBySlide(getSlideFromDpt(outE.dpt, this->mStoringQbi->getSlide()),
                                                     outE, this->AGGR_OFFSET);
                    if(this->DEBUG) std::cerr<<"In aggr min size=0 outE: "<<outE<<std::endl;
                } else {
                    for (auto oldE : currentAggr) {
                        if(this->DEBUG) std::cerr<<"oldE: "<<*oldE<<std::endl;
                        mEvalContext->bind(&(oldE->tuple), MIN_OLD_ROLE);
                        if (mCompareEval->eval()) {
                            if(this->DEBUG) std::cerr<<"after eval e: "<<e<<std::endl;
                            updatedTupleKey.insert(Tuple(e.tuple.begin(), e.tuple.begin() + this->AGGR_OFFSET));
                            mUpdateEval->eval();
//                            if(this->DEBUG) std::cerr<<"In aggr min size!=0: "<<outE<<std::endl;
                            if(this->DEBUG) std::cerr<<"oldE  after!: "<<*oldE<<std::endl;

                        }
                    }
                }
            }

            for (auto t : updatedTupleKey){
                std::vector<Element> tmp;
                std::cerr<<"in updateKey"<<std::endl;
                this->mStoringQbi->getAtomsByVByTuple(t, tmp);
                std::cerr<<"in updateKey after get"<<std::endl;

                this->mOutWorkSpace->insert(std::end(*this->mOutWorkSpace),
                                            std::begin(tmp), std::end(tmp));
                std::cerr<<"finish insert"<<std::endl;

            }

            (*mCurrentId)++;
            if(this->DEBUG) std::cerr << "=====MIN-"<<this->mId<<"-End===== " << std::endl;
            // *(this->MicroSecDuration) += std::chrono::duration_cast<MicroSec>(std::chrono::high_resolution_clock::now() - start).count();
        }

        return 0;
    }

}