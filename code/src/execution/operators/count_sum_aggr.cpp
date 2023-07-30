#include "execution/operators/count_sum_aggr.h"
#include <set>
namespace Execution {
    CountSumOp::CountSumOp (unsigned int id, std::ostream& LOG) : Operator(), mId(id) , LOG(LOG){
        this->mInitEval = new AEval();
        this->mUpdateEval = new AEval();
        this->mCompareEval = new BEval();
        this->mEvalContext = new EvalContext();

        this->mInitEval->setEvalContext(this->mEvalContext);
        this->mUpdateEval->setEvalContext(this->mEvalContext);
        this->mCompareEval->setEvalContext(this->mEvalContext);


    }
    CountSumOp::~CountSumOp (){
        delete this->mInitEval;
        delete this->mUpdateEval;
        delete this->mCompareEval;

        delete this->mEvalContext;
    };


    int CountSumOp::run () {

        if ((*mCurrentId) == this->mId) {
            auto start = std::chrono::high_resolution_clock::now();
            if (mClearOut) this->mOutWorkSpace->clear();
            if(this->DEBUG) std::cerr << "=====CT/SUM-"<<this->mId<<"-Start===== " << std::endl;
            std::vector<Element> tempOut;
            std::set<Tuple> updatedTupleKey;

            for (auto e : *(this->mInWorkSpace)) {

                std::vector<Element *> currentAggr;
                this->mStoringQbi->getAtomsByRByElement(e, this->AGGR_OFFSET, currentAggr);
                mEvalContext->bind(&(e.tuple), COUNT_SUM_INPUT_ROLE);


                if (currentAggr.size() == 0) {
                    updatedTupleKey.insert(Tuple(e.tuple.begin(), e.tuple.begin() + this->AGGR_OFFSET));
                    Element outE {e.tuple.size(), e.timestamp, e.dpt};
                    mEvalContext->bind(&(outE.tuple), COUNT_SUM_OUTPUT_ROLE);
                    mInitEval->eval();



                    //TODO: this is usually idb
                    this->mStoringQbi->insertBySlide(getSlideFromDpt(outE.dpt, this->mStoringQbi->getSlide()),
                                              outE, this->AGGR_OFFSET);
//                    tempOut.push_back(outE);
                    if(this->DEBUG) std::cerr<<"outE: "<<outE<<std::endl;


                } else {

                    if(this->DEBUG) std::cerr<<"here?"<<std::endl;

                    for (auto oldE : currentAggr) {
                        Slide oldESlide = getSlideFromDpt(oldE->dpt, this->mStoringQbi->getSlide());
                        Slide currenESlide = getSlideFromDpt(e.dpt, this->mStoringQbi->getSlide());
                        if(this->DEBUG) std::cerr<<"oldE: "<<*oldE<<" with slide: "<<oldESlide<<std::endl;
                        if(this->DEBUG) std::cerr<<"current e: "<<e<<" with size "<<e.tuple.size()<<" with slide: "<<
                        currenESlide<<std::endl;

                        mEvalContext->bind(&(oldE->tuple), COUNT_SUM_OLD_ROLE);
                        if(this->DEBUG) std::cerr<<"???: "<<*oldE<<std::endl;

                        if (mCompareEval->eval() && oldESlide == currenESlide) {
                            if(this->DEBUG) std::cerr<<"???2: "<<*oldE<<" with size "<<e.tuple.size()<<std::endl;

                            updatedTupleKey.insert(Tuple(e.tuple.begin(), e.tuple.begin() + this->AGGR_OFFSET));
                            mUpdateEval->eval();
//                        tempOut.push_back(*oldE);
                            if(this->DEBUG) std::cerr<<"oldE  after!: "<<*oldE<<std::endl;
                        }
//                        else if (oldESlide != currenESlide) {
//                            if(this->DEBUG) std::cerr<<"oldE second brac: "<<*oldE<<" with slide: "<<oldESlide<<std::endl;
//
//                            updatedTupleKey.insert(Tuple(e.tuple.begin(), e.tuple.begin() + this->AGGR_OFFSET));
//                            Element outE {e.tuple.size(), e.timestamp, e.dpt};
//                            mEvalContext->bind(&(e.tuple), COUNT_SUM_INPUT_ROLE);
//                            mEvalContext->bind(&(outE.tuple), COUNT_SUM_OUTPUT_ROLE);
//                            mInitEval->eval();
//
//                            if(this->DEBUG) std::cerr<<"outE Diff slide: "<<outE<<std::endl;
//
//
//                            //TODO: this is usually idb
//                            this->mStoringQbi->insertBySlide(getSlideFromDpt(outE.dpt, this->mStoringQbi->getSlide()),
//                                                             outE, this->AGGR_OFFSET);
//
//                        }
                        if(this->DEBUG) std::cerr<<"???3: "<<*oldE<<std::endl;



                    }
                }
            }


            for (auto t : updatedTupleKey){
                std::vector<Element> tmp;
                this->mStoringQbi->getAtomsByVByTuple(t, tmp);
                this->mOutWorkSpace->insert(std::end(*this->mOutWorkSpace),
                                            std::begin(tmp), std::end(tmp));
            }


            for (auto e : *(this->mOutWorkSpace)) if(this->DEBUG) std::cerr<<"COUNT/SUM: "<<e<<" with slide: "<<getSlideFromDpt(e.dpt, this->mStoringQbi->getSlide())<<std::endl;
            (*mCurrentId)++;

            if(this->DEBUG) std::cerr << "=====CT/SUM-"<<this->mId<<"-End===== " << std::endl;
            // *(this->MicroSecDuration) += std::chrono::duration_cast<MicroSec>(std::chrono::high_resolution_clock::now() - start).count();
//            std::cerr<<"AGGR, noQBI: "<<this->mOutWorkSpace->size()<<std::endl;
        }
        return 0;
    }

}