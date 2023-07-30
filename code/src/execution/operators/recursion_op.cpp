#include "execution/operators/recursion_op.h"

namespace Execution {
    RecursionOp::RecursionOp (unsigned int id, std::ostream& LOG) : Operator(), mId(id) , LOG(LOG){
        mOutBuffer = new std::vector<Execution::Element>();
    }
    RecursionOp::~RecursionOp (){
        delete mOutBuffer;
    };

    int RecursionOp::run() {


        /// This is for exit rule
        if (*mCurrentId == this->mId) {
            auto start = std::chrono::high_resolution_clock::now();
            if (mClearOut) this->mOutWorkSpace->clear();
            if (this->DEBUG) std::cerr << "=====Recur-I-"<<this->mId<<"-Start===== " << std::endl;

            *(this->mOutWorkSpace) = *(this->mInWorkSpace);

            // Changred 12:05 July-7
            std::vector<Element> tempDedup;
            this->mIdb->getAllAtoms(tempDedup);
            this->condDeDupeFromIdb(tempDedup);

//            for (auto e : *(this->mInWorkSpace)) {
//                this->mOutBuffer->push_back(e);
//                //TODO CHANGE the insert
////                this->mStoringQbi->insertBySlideNoAggr(this->getSlideFromDpt(e.timestamp, this->mStoringQbi->getSlide()), e);
////                this->mStoringQbi->insertBySlide(this->getSlideFromDpt(e.dpt, this->mStoringQbi->getSlide()),
////                                          e, this->AGGR_OFFSET);
//            }

//            for (auto eOB : *(mOutBuffer)) std::cerr << "outbuf: " << eOB << std::endl;
            this->condStoreComputedResult();
            (*mCurrentId)++;
            if (this->DEBUG) std::cerr << "=====Recur-I-"<<this->mId<<"-End===== " << std::endl;
            // *(this->MicroSecDuration) += std::chrono::duration_cast<MicroSec>(std::chrono::high_resolution_clock::now() - start).count();

        } else if ((*mCurrentId) == this->mIdOther) { /// This now re-stream back to join-op
            auto start = std::chrono::high_resolution_clock::now();
            if (this->DEBUG) std::cerr << "=====Recur-II-"<<this->mId<<"-Start===== " << std::endl;

            if (this->DEBUG) for(auto e : *(this->mInWorkSpace)) std::cerr<<"inws: "<<e<<std::endl;
            if (mClearOut) this->mOutWorkSpace->clear();
            *(this->mOutWorkSpace) = *(this->mInWorkSpace);

            // Changred 12:05 July-7
            std::vector<Element> tempDedup;
            this->mIdb->getAllAtoms(tempDedup);
            this->condDeDupeFromIdb(tempDedup);

//            for (auto e : *(this->mInWorkSpace)) {
//                this->mStoringQbi->insertBySlide(this->getSlideFromDpt(e.dpt, this->mStoringQbi->getSlide()),
//                                          e, this->AGGR_OFFSET);
//            }
//            for (auto eOB : *(this->mInWorkSpace)) std::cerr << "outbuf-other: " << eOB << std::endl;
//            for (auto e : *(this->mInWorkSpace)) {
//                std::cerr<<"not inserted??"<<e<<std::endl;
//                this->mStoringQbi->insertBySlide(this->getSlideFromDpt(e.dpt, this->mStoringQbi->getSlide()),
//                                          e, this->AGGR_OFFSET);
//                mOutWorkSpace->push_back(e);
//            }

            //TODO: we do not need to store the recursive computation here?
            this->condStoreComputedResult();
            *mCurrentId = mFlowBackTo;
            if (this->DEBUG) std::cerr << "=====Recur-II-"<<this->mId<<"-End===== " << std::endl;
            // if (this->DEBUG) std::cerr << "=====Recur-sdfae" << std::endl;
            // *(this->MicroSecDuration) += std::chrono::duration_cast<MicroSec>(std::chrono::high_resolution_clock::now() - start).count();

        }



        return 0;

    }
    }