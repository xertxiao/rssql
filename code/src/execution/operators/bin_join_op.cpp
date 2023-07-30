#include "execution/operators/bin_join_op.h"
#include <algorithm>


namespace Execution {
    BinaryJoinOp::BinaryJoinOp (unsigned int id, std::ostream& LOG) : Operator(), mId(id) , LOG(LOG){

        mJoinOut = new AEval();
        mPredEval = new BEval();
        mEvalContext = new EvalContext();

        mJoinOut->setEvalContext(mEvalContext);
        mPredEval->setEvalContext(mEvalContext);

        thisWindowIdb = new std::vector<Element>();
    }
    BinaryJoinOp::~BinaryJoinOp (){
        delete mJoinOut;
        delete mPredEval;
        delete mEvalContext;
        delete thisWindowIdb;
    };


    int BinaryJoinOp::run () {
        if ((*mCurrentId) == this->mId) {
            //std::cerr << "=====JOIN-"<<this->mId<<"-Start===== " << std::endl;
            if(this->DEBUG) std::cerr << "=====JOIN-"<<this->mId<<"-Start===== " << std::endl;
            auto start = std::chrono::high_resolution_clock::now();
            int rc;
            if (mClearOut) this->mOutWorkSpace->clear();
            if (mUsingQbi) {
                rc = runWithQbi();
            } else {
                rc = runWithOutQbi();
            }
            // *(this->MicroSecDuration) += std::chrono::duration_cast<MicroSec>(std::chrono::high_resolution_clock::now() - start).count();
            // std::cerr<<"Generated "<<this->mOutWorkSpace->size()<<std::endl;
            return rc;
        }

        return 0;
    }

    int BinaryJoinOp::runWithQbi() {

        std::vector<Element> allEdb;
        std::vector<Element> oldIdb;
        std::vector<Element> newIdb;
        this->mEdb->getAllAtoms(allEdb);
        this->mIdb->getAllAtoms(oldIdb);
        /// First if it is the first time inside a window, then compute newEdb x oldIdb.
        if (this->mWindowStart) {
            std::vector<Element> newJoinAll;
            this->join(this->mNewEdbBuf, &oldIdb, newJoinAll);
            this->mWindowStart = false;
            newIdb = newJoinAll;
        }
        for (auto e : *(this->mInWorkSpace)) {
            newIdb.push_back(e);
            this->thisWindowIdb->push_back(e);
        }
        /// Now compute allEdb x newIdb

        this->join(&allEdb, &newIdb, *(this->mOutWorkSpace));

        this->_joinCt += allEdb.size();
        this->_joinCt += newIdb.size();


        // TODO: add for test july5:10:55
        std::vector<Element> thisWindowIdbTemp = *(this->thisWindowIdb);
        //std::cerr<<"join ct: "<<_joinCt<<std::endl;

//        this->mOutWorkSpace->erase(
//                std::remove_if(
//                        this->mOutWorkSpace->begin(),
//                        this->mOutWorkSpace->end(),
//                        [&oldIdb](const Element& e)
//                        {
//                            auto it = std::find(oldIdb.begin(), oldIdb.end(), e);
//                            return (it != oldIdb.end());
//                        }
//                ),
//                this->mOutWorkSpace->end()
//        );

//        this->mOutWorkSpace->erase(
//                std::remove_if(
//                        this->mOutWorkSpace->begin(),
//                        this->mOutWorkSpace->end(),
//                        [&newIdb](const Element& e)
//                        {
//                            auto it = std::find(newIdb.begin(), newIdb.end(), e);
//                            return (it != newIdb.end());
//                        }
//                ),
//                this->mOutWorkSpace->end()
//        );

        if(this->DEBUG) std::cerr << "=====JOI2N-"<<this->mId<<"-Start===== " << std::endl;
        if (this->reachFixPoint() && this->thisWindowIdb) {
            // std::cerr << "=====JOI22222"<<std::endl; 
            this->thisWindowIdb->clear();
            // std::cerr << "=====JOI333"<<std::endl; 
            this->resetForNewWindow();
        } else {
            (*mCurrentId)++;
        }

        if(this->DEBUG) std::cerr << "=====JOI3N-"<<this->mId<<"-Start===== " << std::endl;
        // This will store only if mStoreOut is true;
        this->condStoreComputedResult();
        if(this->DEBUG) std::cerr << "=====JOIN-"<<this->mId<<"-End===== " << std::endl;

        return 0;
    }

    int BinaryJoinOp::runWithOutQbi() {

        std::vector<Element> allEdb;
        std::vector<Element> allIdb;

        this->mEdb->getAllAtoms(allEdb);

        // TODO: add for test july5:10:55
        std::vector<Element> oldIdb;
        this->mIdb->getAllAtoms(oldIdb);

        /// Now compute allEdb x allIdb
        for (auto e : *(this->mInWorkSpace)) {
            allIdb.push_back(e);
            this->thisWindowIdb->push_back(e);
        }

        for (auto e : allIdb) if(this->DEBUG) std::cerr<<"newIDB: "<<e<<std::endl;
//            for (auto e : allEdb) if(this->DEBUG) std::cerr<<"allEdb: "<<e<<std::endl;
//        if(this->DEBUG) std::cerr << "join1" << std::endl;

//        std::cerr<<"id: "<<*(this->mCurrentId)<<" without qbi ws size before join: "<<this->mOutWorkSpace->size()<<std::endl;

        this->_joinCt += allEdb.size();
        this->_joinCt += allIdb.size();

        this->join(&allEdb, &allIdb, *(this->mOutWorkSpace));
//        if(this->DEBUG) std::cerr << "join2" << std::endl;

        for (auto e :  *(this->mOutWorkSpace)) if(this->DEBUG) std::cerr<<"outWS: "<<e<<" with size: "<<e.tuple.size()<<std::endl;

        std::vector<Element> tempS;
//        std::cerr<<"id: "<<*(this->mCurrentId)<<" Before getAll, without qbi allIdb size: "<<allIdb.size()<<std::endl;

        this->mIdb->getAllAtoms(allIdb);
//        std::cerr<<"id: "<<*(this->mCurrentId)<<" without qbi allIdb size: "<<allIdb.size()<<std::endl;
        for (auto e : allIdb) if(this->DEBUG) std::cerr<<"idb: "<<e<<" with size: "<<e.tuple.size()<<std::endl;
//        std::sort(allIdb.begin(), allIdb.end());
//        std::sort(mOutWorkSpace->begin(), mOutWorkSpace->end());
//
//        std::vector<Element> diff;
//
//        std::set_difference(mOutWorkSpace->begin(), mOutWorkSpace->end(), allIdb.begin(), allIdb.end(),
//                            std::inserter(diff, diff.begin()));

//        for (auto e : diff) if(this->DEBUG) std::cerr<<"diff: "<<e<<"with size: "<<e.tuple.size()<<std::endl;

//        //TODO: changed
//        for ( auto it = mOutWorkSpace->begin(); it != mOutWorkSpace->end(); )
//        {
//            if ( std::find( allIdb.begin(), allIdb.end(), *it ) != allIdb.end() )
//            {
//                it = mOutWorkSpace->erase( it );
//            }
//            else
//            {
//                ++it;
//            }
//        }

//        this->mOutWorkSpace->erase(
//                std::remove_if(
//                        this->mOutWorkSpace->begin(),
//                        this->mOutWorkSpace->end(),
//                        [&allIdb](const Element& e)
//                        {
//                            auto it = std::find(allIdb.begin(), allIdb.end(), e);
//                            return (it != allIdb.end());
//                        }
//                ),
//                this->mOutWorkSpace->end()
//        );

        // TODO: add for test july5:10:55
        std::vector<Element> thisWindowIdbTemp = *(this->thisWindowIdb);
        //std::cerr<<"this windowidbemp withouot qbi sie: "<<thisWindowIdbTemp.size()<<std::endl;
        //std::cerr<<"join ct: "<<_joinCt<<std::endl;


//        this->mOutWorkSpace->erase(
//                std::remove_if(
//                        this->mOutWorkSpace->begin(),
//                        this->mOutWorkSpace->end(),
//                        [&oldIdb](const Element& e)
//                        {
//                            auto it = std::find(oldIdb.begin(), oldIdb.end(), e);
//                            return (it != oldIdb.end());
//                        }
//                ),
//                this->mOutWorkSpace->end()
//        );



//        std::cerr<<"id: "<<*(this->mCurrentId)<<" without qbi ws size: "<<this->mOutWorkSpace->size()<<std::endl;


//        this->mOutWorkSpace->erase(
//                std::remove_if(
//                        this->mOutWorkSpace->begin(),
//                        this->mOutWorkSpace->end(),
//                        [&allIdb](const Element& e)
//                        {
//                            auto it = std::find(allIdb.begin(), allIdb.end(), e);
//                            return (it != allIdb.end());
//                        }
//                ),
//                this->mOutWorkSpace->end()
//        );

        for (auto e :  *(this->mOutWorkSpace)) if(this->DEBUG) std::cerr<<"outWS after dedup: "<<e<<" with size: "<<e.tuple.size()<<std::endl;


//        if(this->DEBUG) std::cerr << "join3" << std::endl;

        if (this->reachFixPoint()) {
            this->thisWindowIdb->clear();
            this->resetForNewWindow();
//            if(this->DEBUG) std::cerr << "join4" << std::endl;
        } else {
            (*mCurrentId)++;
        }

        this->condStoreComputedResult();

        //TODO: Delete
//        allIdb.clear();
//        this->mIdb->getAllAtoms(allIdb);
//        for (auto e : allIdb) if(this->DEBUG) std::cerr<<"idb new: "<<e<<" with size: "<<e.tuple.size()<<std::endl;

        if(this->DEBUG) for (auto e : *(this->mOutWorkSpace)) std::cerr<<"JOIN: "<<e<<std::endl;

//        std::cerr<<"BJ, noQBI: "<<this->mOutWorkSpace->size()<<std::endl;
        if(this->DEBUG) std::cerr << "=====JOIN-"<<this->mId<<"-End===== " << std::endl;
        return 0;
    }

    int BinaryJoinOp::join(std::vector<Element> *left, std::vector<Element> *right, std::vector<Element> &out) {
        for (auto l : *(left)) {
            for (auto r : *(right)) {
                if(this->DEBUG) std::cerr<<"l: "<<l<<", r:"<<r<<std::endl;
                /// Ts shall be assigned by the larger value to fulfill the sequential
                /// semantic
                Timestamp newTs = l.timestamp > r.timestamp ? l.timestamp : r.timestamp;

                /// Dpt shall be assigned by the smaller value to fulfill the
                /// Qbi
                Dpt newDpt = l.dpt < r.dpt ?  l.dpt : r.dpt;

                Element newOut {static_cast<unsigned int>(this->mJoinOut->getInstrSize()),
                                newTs, newDpt};
                mEvalContext->bind(&(l.tuple), JOIN_OUTER_ROLE);
                mEvalContext->bind(&(r.tuple), JOIN_INNER_ROLE);
                mEvalContext->bind(&(newOut.tuple), JOIN_OUTPUT_ROLE);
                if (mPredEval->eval()) {
                    mJoinOut->eval();
                    out.push_back(newOut);
                    if(this->DEBUG) std::cerr<<"newOut "<<newOut<<" with size: "<<newOut.tuple.size()<<std::endl;

                }
//                else {
//                    mEvalContext->bind(&(r.tuple), JOIN_OUTER_ROLE);
//                    mEvalContext->bind(&(l.tuple), JOIN_INNER_ROLE);
//                    mEvalContext->bind(&(newOut.tuple), JOIN_OUTPUT_ROLE);
//                    if (mPredEval->eval()) {
//                        mJoinOut->eval();
//                        out.push_back(newOut);
////                        if(this->DEBUG) std::cerr<<"newOut "<<newOut<<std::endl;
//                    }
//                }
            }
        }
        return 0;
    }

    void BinaryJoinOp::resetForNewWindow(){
        //if(_joinCt != 0) std::cerr<<"This window computes: "<<_joinCt<<" atoms"<<std::endl;
        // if (this->latencies->empty()) this->latencies->push_back(*(this->MicroSecDuration));
        // else this->latencies->push_back(*(this->MicroSecDuration) - this->latencies->back());

        // this->memOverhead->push_back(this->mGU->getAllSize());




        this->mGU->advanceAll();
        this->mWindowStart = true;
        (*mCurrentId) = 0;
        this->mNewEdbBuf->clear();
    }


}
