//
// Created by _____ on 6/24/22.
//
#include "execution/operators/stream_source_op.h"

namespace Execution {
    StreamSourceOp::StreamSourceOp (unsigned int id, std::ostream& LOG) : Operator(), mId(id) , LOG(LOG){
        dataRead = 0;
    }
    StreamSourceOp::~StreamSourceOp (){};

    int StreamSourceOp::addAttr (Type type, unsigned int len, Column outCol) {

        return 0;
    }

    int StreamSourceOp::initialize () {

        return 0;
    }

    int StreamSourceOp::run () {

        if (*mCurrentId == this->mId) {
            auto start = std::chrono::high_resolution_clock::now();
            if(this->DEBUG) std::cerr << "=====StreamSrc-"<<this->mId<<"-Start===== " << std::endl;

            std::vector<Element> newEs;
            int rc = this->source->getNextBatch(newEs);
            if (mClearOut) this->mOutWorkSpace->clear();
            if(this->DEBUG) std::cerr<<"reading: "<<newEs.size()<<std::endl;


//            if (dataRead > 5) {
//                *mCurrentId = 99;
//                return 0;
//            }
            // if (rc != 0 || dataRead > 100000) {
            //     *mCurrentId = 99;
            //     return 0;
            // }

            for (const auto& e : newEs) {
                this->dataRead++;
                this->mOutWorkSpace->push_back(e);
                this->mNewEdbBuf->push_back(e);
            }


            if (!this->mUsingQbi) {
                if (newEs.size() != 0) {
                    std::vector<Element> temp;
                    this->mStoringQbi->getAllAtoms(temp);
                    this->mGU->clearAllIdb();
                    for (auto e : temp) {
                        this->mOutWorkSpace->push_back(e);
                    }
                }
            }

            (*mCurrentId)++;
            this->condStoreComputedResult();

            for (auto e : *(this->mOutWorkSpace)) if(this->DEBUG) std::cerr<<"SS: "<<e<<std::endl;
            if(this->DEBUG) std::cerr << "=====StreamSrc-"<<this->mId<<"-End===== " << std::endl;
            // *(this->MicroSecDuration) += std::chrono::duration_cast<MicroSec>(std::chrono::high_resolution_clock::now() - start).count();

            // if (this->dataRead - this->prevDatRead > 0) {
            //     std::cerr<<"Already read: "<<this->dataRead<<std::endl;
            //     // std::cerr<<"Current time taken: "<<*(this->MicroSecDuration)<<std::endl;
            //     this->prevDatRead = this->dataRead;
            // }


        }
        return 0;
    }

    void StreamSourceOp::setData(std::vector<Element> *data) {
        StreamSourceOp::data = data;
    }

    void StreamSourceOp::setSource(Interface::FileSource *newSrc) {
        this->source = newSrc;
    }
}
