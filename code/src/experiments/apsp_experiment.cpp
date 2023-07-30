#include "experiments/apsp_experiment.h"
#include "execution/operators/stream_source_op.h"
#include "execution/operators/recursion_op.h"
#include "execution/operators/proj_op.h"
#include "execution/operators/bin_join_op.h"
#include "execution/operators/select_op.h"
#include "execution/operators/min_aggr.h"
#include "execution/operators/count_sum_aggr.h"

#include<iostream>

namespace Experiment {
    ApspExperiment::ApspExperiment(unsigned int w, unsigned int s, bool q) {
        this->WINDOW_SLIDE = s;
        this->WINDOW_LEN = w;
        this->useQBI = q;

        this->instOps = new std::vector<Execution::Operator>();
        this->emptyWS = new Execution::WorkSpace(0);
        this->emptyQbi = new Execution::Qbi();
        this->fileSource = new Interface::FileSource(this->fileSourceName, WINDOW_LEN, WINDOW_SLIDE);


        this->mGU->CurOpId = 0;
        this->totalDur = new unsigned long long();
        this->latencies = new std::vector<unsigned long long>();
        this->memConsumption = new std::vector<unsigned long long>();


    }

    ApspExperiment::~ApspExperiment() {
        delete this->instOps;
        delete this->emptyWS;
        delete this->emptyQbi;
        delete this->fileSource;
        delete this->totalDur;
        delete this->latencies;
        delete this->memConsumption;


    }

    int ApspExperiment::registerSchema() {
//        Metadata::Schema thisSchema;
//        Metadata::Attribute from("from", FLOAT);
//        Metadata::Attribute to("to", FLOAT);
//        thisSchema.addAttr(from);
//        thisSchema.addAttr(to);
//        this->strmMgr->registerStream(thisSchema,arcId);
//
//        for (auto p : ops) delete p;


        return 0;
    }

    int ApspExperiment::registerPhysical() {
        // Stream Source
//        rootOps->kind = Physical::PO_STREAM_SOURCE;
//        rootOps->attrIds.push_back(arcId);
//        rootOps->u.STREAM_SOURCE.srcId = 1;
//        rootOps->u.STREAM_SOURCE.strId = 1;

        /// Initialize some execution operators


        // First we need a source OP
        Execution::StreamSourceOp  *_srcStreamOp = new Execution::StreamSourceOp(0,
                                                                                 reinterpret_cast<std::ostream &>(stderr));
        unsigned int idSSOutWs, idSSEdbQbi, idSSNewEdb;
        _srcStreamOp->setAggrOffset(0);
        _srcStreamOp->setCurrentId(&(this->mGU->CurOpId));
        // _srcStreamOp->setData(&sampleData);
        _srcStreamOp->setSource(this->fileSource);
        _srcStreamOp->setInWorkSpace(this->emptyWS);
        _srcStreamOp->setOutWorkSpace(this->mGU->registerWorkSpace(idSSOutWs));
        _srcStreamOp->setNewEdbBuf(this->mGU->registerWorkSpace(idSSNewEdb));
        _srcStreamOp->setEdb(this->mGU->registerNRQbi(idSSEdbQbi));
        _srcStreamOp->setIdb(this->emptyQbi);
        _srcStreamOp->setAggrOffset(2);
        _srcStreamOp->setClearOut(true);
        _srcStreamOp->setStoreOut(true);
        _srcStreamOp->setStoreOutQbi(this->mGU->connectNRQbi(idSSEdbQbi));
        ops.push_back(_srcStreamOp);




        // Min Aggr Op
        //Execution::MinAggOp
        Execution::CountSumOp *_minOp = new Execution::CountSumOp(1,
                                                              reinterpret_cast<std::ostream &>(stderr));

        _minOp->mCompareEval->addInstr(Execution::BInstr{Execution::FLT_LT,
                                                         0, 2,
                                                         1, 2,
                                                         nullptr, nullptr});

        _minOp->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY,
                                                      0, 0,
                                                      0, 0,
                                                      2, 0});

        _minOp->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY,
                                                      0, 1,
                                                      0, 0,
                                                      2, 1});

        _minOp->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY,
                                                      0, 2,
                                                      0, 0,
                                                      2, 2});


//        _minOp->mUpdateEval->addInstr(Execution::AInstr{Execution::FLT_CPY,
//                                                        0, 0,
//                                                        0, 0,
//                                                        1, 0});
//
//        _minOp->mUpdateEval->addInstr(Execution::AInstr{Execution::FLT_CPY,
//                                                        0, 1,
//                                                        0, 0,
//                                                        1, 1});

        _minOp->mUpdateEval->addInstr(Execution::AInstr{Execution::FLT_CPY,
                                                        0, 2,
                                                        0, 0,
                                                        1, 2});

        unsigned int idMinOutWs;
        _minOp->setAggrOffset(2);
        _minOp->setCurrentId(&(this->mGU->CurOpId));
        _minOp->setInWorkSpace(this->mGU->connectWorkSpace(idSSOutWs));
        _minOp->setOutWorkSpace(this->mGU->registerWorkSpace(idMinOutWs));
        _minOp->setNewEdbBuf(this->emptyWS);
        _minOp->setEdb(this->emptyQbi);
        _minOp->setIdb(this->mGU->connectRQbi());
        _minOp->setClearOut(true);
        _minOp->setStoreOut(true);
        _minOp->setStoreOutQbi(this->mGU->connectRQbi());

        ops.push_back(_minOp);



        // Recursion OP
        Execution::RecursionOp *_recurOp = new Execution::RecursionOp(2,
                                                                      reinterpret_cast<std::ostream &>(stderr));
        unsigned int idRucOutWs;
        _recurOp->setAggrOffset(2);
        _recurOp->setOtherOp(5);
        _recurOp->mFlowBackTo = 3;
        _recurOp->setCurrentId(&(this->mGU->CurOpId));
        _recurOp->setInWorkSpace(this->mGU->connectWorkSpace(idMinOutWs));
        _recurOp->setOutWorkSpace(this->mGU->registerWorkSpace(idRucOutWs));
        _recurOp->setEdb(this->emptyQbi);
        _recurOp->setIdb(this->emptyQbi);
        _recurOp->setAggrOffset(1);
        _recurOp->setClearOut(true);
        _recurOp->setStoreOut(false);
        _recurOp->setStoreOutQbi(this->emptyQbi);
        ops.push_back(_recurOp);


        // Join Op
        Execution::BinaryJoinOp *_joinOp = new Execution::BinaryJoinOp(3,
                                                                       reinterpret_cast<std::ostream &>(stderr));

        _joinOp->mPredEval->addInstr(Execution::BInstr{Execution::FLT_EQ,
                                                       0, 1,
                                                       1, 0,
                                                       nullptr, nullptr});
        _joinOp->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_CPY,
                                                      0, 0,
                                                      0, 0,
                                                      2, 0});
        _joinOp->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_CPY,
                                                      1, 1,
                                                      0, 0,
                                                      2, 1});
        _joinOp->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_ADD,
                                                      0, 2,
                                                      1, 2,
                                                      2, 2});
        unsigned int idJoinOutWs;
        _joinOp->setCurrentId(&(this->mGU->CurOpId));
        _joinOp->setInWorkSpace(this->mGU->connectWorkSpace(idRucOutWs));
        _joinOp->setOutWorkSpace(this->mGU->registerWorkSpace(idJoinOutWs));
        _joinOp->setNewEdbBuf(this->mGU->connectWorkSpace(idSSOutWs));
        _joinOp->setEdb(this->mGU->connectNRQbi(idSSEdbQbi));
        //_joinOp->setIdb(this->mGU->connectRQbi());
        _joinOp->setIdb(this->emptyQbi);
        _joinOp->setAggrOffset(2);
        _joinOp->setClearOut(true);
        _joinOp->setStoreOut(false);
        _joinOp->setStoreOutQbi(this->emptyQbi);
        ops.push_back(_joinOp);


        // Min Aggr Op
        //Execution::MinAggOp
        Execution::CountSumOp *_minOp2 = new Execution::CountSumOp(4,
                                                              reinterpret_cast<std::ostream &>(stderr));

        _minOp2->mCompareEval->addInstr(Execution::BInstr{Execution::FLT_LT,
                                                          0, 2,
                                                          1, 2,
                                                          nullptr, nullptr});

        _minOp2->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY,
                                                       0, 0,
                                                       0, 0,
                                                       2, 0});

        _minOp2->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY,
                                                       0, 1,
                                                       0, 0,
                                                       2, 1});

        _minOp2->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY,
                                                       0, 2,
                                                       0, 0,
                                                       2, 2});


//        _minOp2->mUpdateEval->addInstr(Execution::AInstr{Execution::FLT_CPY,
//                                                         0, 0,
//                                                         0, 0,
//                                                         2, 0});
//
//        _minOp2->mUpdateEval->addInstr(Execution::AInstr{Execution::FLT_CPY,
//                                                         0, 1,
//                                                         0, 0,
//                                                         2, 1});

        _minOp2->mUpdateEval->addInstr(Execution::AInstr{Execution::FLT_CPY,
                                                        0, 2,
                                                        0, 0,
                                                        1, 2});

        _minOp2->setAggrOffset(2);
        _minOp2->setCurrentId(&(this->mGU->CurOpId));
        _minOp2->setInWorkSpace(this->mGU->connectWorkSpace(idJoinOutWs));
        _minOp2->setOutWorkSpace(this->mGU->connectWorkSpace(idMinOutWs));
        _minOp2->setNewEdbBuf(this->emptyWS);
        _minOp2->setEdb(this->emptyQbi);
        _minOp2->setIdb(this->mGU->connectRQbi());
        _minOp2->setClearOut(true);
        _minOp2->setStoreOut(true);
        _minOp2->setStoreOutQbi(this->mGU->connectRQbi());
        ops.push_back(_minOp2);


        this->mGU->setWindowsAll(0, WINDOW_LEN, WINDOW_SLIDE);

        for (auto o : ops) {
            o->setMgu(this->mGU);
            o->setUsingQbi(this->useQBI );
            o->setDebug(false);
            o->setDuration(this->totalDur);
            o->setLatencyVec(this->latencies);
            o->setOverheadVec(this->memConsumption);

        }



        return 0;
    }

    int ApspExperiment::runExperiment() {
        bool done = false;
        std::cerr<<"Total Time before: "<<*(this->totalDur)<<std::endl;

        while (!done) {

            for (auto op : ops) {
                op->run();
                if (this->mGU->CurOpId == 99) {
                    done = true;
                    std::cerr<<"Total Time: "<<*totalDur<<std::endl;

                    break;
                }
            }
        }
        return 0;

    }
}
