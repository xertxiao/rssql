#ifndef _STREAMLOG_PROTOTYPE_OPERATOR_
#define _STREAMLOG_PROTOTYPE_OPERATOR_


#include "execution/internals/element.h"
#include "execution/qbi/qbi.h"
#include "experiments/experiment.h"

//TODO: remove this
#include <iostream>
#include <chrono>
#include <algorithm>

namespace Execution {
    typedef std::chrono::duration<unsigned long long, std::ratio<1, 1000000>> MicroSec;

    class Operator{
    protected:
        /// Access to global unit
        GlobalUnit *mGU;

        /// First each operator will have access to Qbi's
        Qbi *mEdb;
        Qbi *mIdb;
        Qbi *mStoringQbi;

        /// Second each operator will have access to both current workspace
        /// and newly come edb buf
        Execution::WorkSpace *mInWorkSpace;
        Execution::WorkSpace *mOutWorkSpace;
        Execution::WorkSpace *mNewEdbBuf;

        /// Finally we want to know whether we are running with/without Qbi
        /// default to true;
        bool mUsingQbi = true;
        bool mClearOut = true;
        bool mStoreOut = false;
        bool mDedup = false;

        bool DEBUG = false;
//        bool TIMER = false;

        unsigned long long *MicroSecDuration;
        unsigned long long prevWindowFinishTs = 0;

        std::vector<unsigned long long> *latencies;
        std::vector<unsigned long long> *memOverhead;

        /// Also the offset of Aggregation Val, this will differ for each Op
        unsigned int AGGR_OFFSET = 0;
        unsigned int OUT_SCHEMA_SIZE = 0;

        unsigned int *mCurrentId;


    protected:

        Execution::Slide getSlideFromDpt(Dpt dpt, Execution::Slide slideLength) {
            return floor(dpt/slideLength);
        }

        void condDeDupeFromIdb(const std::vector<Element> &allIdb) {
            if (true) {
                this->mOutWorkSpace->erase(
                        std::remove_if(
                                this->mOutWorkSpace->begin(),
                                this->mOutWorkSpace->end(),
                                [&allIdb](const Element& e)
                                {
                                    auto it = std::find(allIdb.begin(), allIdb.end(), e);
                                    return (it != allIdb.end());
                                }
                        ),
                        this->mOutWorkSpace->end()
                );
            }
        }

        bool reachFixPoint() {return this->mOutWorkSpace->empty();}

        void condStoreComputedResult() {
            if (this->mStoreOut) {
                for (auto e : *(this->mOutWorkSpace)) {
                    this->mStoringQbi->insertBySlide(this->getSlideFromDpt(e.dpt,
                                                                           this->mStoringQbi->getSlide()),
                                                     e, this->AGGR_OFFSET);
                }
            }
        };

    public:
        Operator() = default;

        virtual ~Operator() = default;

        virtual int run() = 0;

        void setInWorkSpace(Execution::WorkSpace *newWorkSpace) {
            Operator::mInWorkSpace = newWorkSpace;
        }

        void setOutWorkSpace(Execution::WorkSpace *newWorkSpace) {
            Operator::mOutWorkSpace = newWorkSpace;
        }

        void setNewEdbBuf(Execution::WorkSpace *newWorkSpace) {
            Operator::mNewEdbBuf = newWorkSpace;
        }

        void setEdb(Qbi *edb) {
            Operator::mEdb = edb;
        }

        void setIdb(Qbi *idb) {
            Operator::mIdb = idb;
        }

        void setAggrOffset(unsigned int aggrOffset) {
            AGGR_OFFSET = aggrOffset;
        }

        void setMgu(GlobalUnit *mGu) {
            mGU = mGu;
        }
        void setCurrentId(unsigned int *currentId) {
            Operator::mCurrentId = currentId;
        }

        void setUsingQbi(bool mUsingQbi) {
            Operator::mUsingQbi = mUsingQbi;
        }

        void setClearOut(bool mClearOut) {
            Operator::mClearOut = mClearOut;
        }

        void setStoreOut(bool mStoreOut) {
            Operator::mStoreOut = mStoreOut;
        }

        void setStoreOutQbi(Qbi *toBeStored) {
            Operator::mStoringQbi = toBeStored;
        }

        void setOutSchemaSize(unsigned int size) {
            return;
        }

        void setDebug(bool flag){
            this->DEBUG = flag;
        }

        void setDuration(unsigned long long *dur) {
            this->MicroSecDuration = dur;
        }

        void setLatencyVec(std::vector<unsigned long long> *vec){
            this->latencies = vec;
        }

        void setOverheadVec(std::vector<unsigned long long> *vec){
            this->memOverhead = vec;
        }



    };
}
#endif // _OPERATOR_