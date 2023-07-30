#include "experiments/pr_experiment.h"
#include "execution/operators/stream_source_op.h"
#include "execution/operators/recursion_op.h"
#include "execution/operators/proj_op.h"
#include "execution/operators/bin_join_op.h"
#include "execution/operators/select_op.h"
#include "execution/operators/min_aggr.h"
#include "execution/operators/count_sum_aggr.h"

#include<iostream>

namespace Experiment {
    PRExperiment::PRExperiment(unsigned int w, unsigned int s, bool q) {
//        this->strmMgr = new Metadata::StreamManager();
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

    PRExperiment::~PRExperiment() {
//        delete this->strmMgr;
        delete this->instOps;
        delete this->emptyWS;
        delete this->emptyQbi;
        delete this->fileSource;
        delete this->totalDur;
        delete this->latencies;
        delete this->memConsumption;

//        delete this->workspace.second;
//        delete this->mNewEdbBuf;
    }

    int PRExperiment::registerSchema() {
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


    int PRExperiment::registerPhysical() {
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


        // Then a Project to get every tuple count=1
        Execution::ProjectOp *_projOp = new Execution::ProjectOp(1,
                                                                      reinterpret_cast<std::ostream &>(stderr));

        _projOp->mProjEval->addInstr(Execution::AInstr{Execution::FLT_CPY,
                                                       0, 0,
                                                       0, 0,
                                                       1, 0});

        _projOp->mProjEval->addInstr(Execution::AInstr{Execution::FLT_ONE,
                                                       0, 0,
                                                       0, 0,
                                                       1, 1});

        unsigned int idProjOutWs;
        _projOp->setAggrOffset(1);
        _projOp->setCurrentId(&(this->mGU->CurOpId));
        _projOp->setInWorkSpace(this->mGU->connectWorkSpace(idSSOutWs));
        _projOp->setOutWorkSpace(this->mGU->registerWorkSpace(idProjOutWs));
        _projOp->setNewEdbBuf(this->emptyWS);
        _projOp->setEdb(this->emptyQbi);
        _projOp->setIdb(this->emptyQbi);
        _projOp->setClearOut(true);
        _projOp->setStoreOut(false);
        _projOp->setStoreOutQbi(this->emptyQbi);

        ops.push_back(_projOp);


        // Then a recur
        Execution::RecursionOp *_recurOp = new Execution::RecursionOp(2,
                                                                      reinterpret_cast<std::ostream &>(stderr));
        unsigned int idRucOutWs;
        _recurOp->setAggrOffset(1);
        _recurOp->setOtherOp(7);
        _recurOp->mFlowBackTo = 4;
        _recurOp->setCurrentId(&(this->mGU->CurOpId));
        _recurOp->setInWorkSpace(this->mGU->connectWorkSpace(idProjOutWs));
        _recurOp->setOutWorkSpace(this->mGU->registerWorkSpace(idRucOutWs));
        _recurOp->setEdb(this->emptyQbi);
        _recurOp->setIdb(this->mGU->connectRQbi());
        _recurOp->setAggrOffset(1);
        _recurOp->setClearOut(true);
        _recurOp->setStoreOut(true);
        _recurOp->setStoreOutQbi(this->mGU->connectRQbi());
        ops.push_back(_recurOp);



        // Then a COUNT operator
        Execution::CountSumOp *_countOp = new Execution::CountSumOp(3,
                                                                    reinterpret_cast<std::ostream &>(stderr));

        _countOp->mCompareEval->addInstr(Execution::BInstr{Execution::A_TRUE,
                                                        0, 1,
                                                        1, 1,
                                                           nullptr, nullptr});


        _countOp->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY,
                                                       0, 0,
                                                       0, 0,
                                                       2, 0});


        _countOp->mInitEval->addInstr(Execution::AInstr{Execution::FLT_ONE,
                                                       0, 1,
                                                       0, 0,
                                                       2, 1});


        _countOp->mUpdateEval->addInstr(Execution::AInstr{Execution::FLT_ADO,
                                                         0, 0,
                                                         0, 0,
                                                         1, 1});

        unsigned int idCountOutWs, idCountNRCQbi;
        _countOp->setAggrOffset(1);
        _countOp->setCurrentId(&(this->mGU->CurOpId));
        _countOp->setInWorkSpace(this->mGU->connectWorkSpace(idSSOutWs));
        _countOp->setOutWorkSpace(this->mGU->registerWorkSpace(idCountOutWs));
        _countOp->setNewEdbBuf(this->emptyWS);
        _countOp->setEdb(this->emptyQbi);
        _countOp->setIdb(this->mGU->registerNRQbi(idCountNRCQbi));
        _countOp->setClearOut(true);
        _countOp->setStoreOut(true);
        _countOp->setStoreOutQbi(this->mGU->connectNRQbi(idCountNRCQbi));


        ops.push_back(_countOp);


        // Join Op Ref join PR
        Execution::BinaryJoinOp *_joinOp = new Execution::BinaryJoinOp(4,
                                                                       reinterpret_cast<std::ostream &>(stderr));

        _joinOp->mPredEval->addInstr(Execution::BInstr{Execution::FLT_EQ,
                                                       0, 0,
                                                       1, 0,
                                                       nullptr, nullptr});
        _joinOp->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_CPY,
                                                      0, 0,
                                                      0, 0,
                                                      2, 0});
        _joinOp->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_DIV,
                                                      1, 1,
                                                      0, 1,
                                                      2, 1});


        unsigned int idJoinOutWs;
        _joinOp->setCurrentId(&(this->mGU->CurOpId));
        _joinOp->setInWorkSpace(this->mGU->connectWorkSpace(idRucOutWs));
        _joinOp->setOutWorkSpace(this->mGU->registerWorkSpace(idJoinOutWs));
        _joinOp->setNewEdbBuf(this->mGU->connectWorkSpace(idCountOutWs));
        _joinOp->setEdb(this->mGU->connectNRQbi(idCountNRCQbi));
//        _joinOp->setIdb(this->mGU->connectRQbi());
        _joinOp->setIdb(this->emptyQbi);
        _joinOp->setAggrOffset(1);
        _joinOp->setClearOut(true);
        _joinOp->setStoreOut(false);
        _joinOp->setStoreOutQbi(this->emptyQbi);
        ops.push_back(_joinOp);


        // Join Op (Ref join PR) join pub
        Execution::BinaryJoinOp *_joinOp2 = new Execution::BinaryJoinOp(5,
                                                                       reinterpret_cast<std::ostream &>(stderr));

        _joinOp2->mPredEval->addInstr(Execution::BInstr{Execution::FLT_EQ,
                                                       0, 0,
                                                       1, 0,
                                                       nullptr, nullptr});
        _joinOp2->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_CPY,
                                                      0, 1,
                                                      0, 0,
                                                      2, 0});
        _joinOp2->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_CPY,
                                                      1, 1,
                                                      0, 0,
                                                      2, 1});


        unsigned int idJoin2OutWs;
        _joinOp2->setCurrentId(&(this->mGU->CurOpId));
        _joinOp2->setInWorkSpace(this->mGU->connectWorkSpace(idJoinOutWs));
        _joinOp2->setOutWorkSpace(this->mGU->registerWorkSpace(idJoin2OutWs));
        _joinOp2->setNewEdbBuf(this->mGU->connectWorkSpace(idSSOutWs));
        _joinOp2->setEdb(this->mGU->connectNRQbi(idSSEdbQbi));
        _joinOp2->setIdb(this->mGU->connectRQbi());
        _joinOp2->setAggrOffset(1);
        _joinOp2->setClearOut(true);
        _joinOp2->setStoreOut(false);
        _joinOp2->setStoreOutQbi(this->emptyQbi);

        ops.push_back(_joinOp2);

        // Then a SUM operator
        Execution::CountSumOp *_sumOp = new Execution::CountSumOp(6,
                                                                    reinterpret_cast<std::ostream &>(stderr));

        _sumOp->mCompareEval->addInstr(Execution::BInstr{Execution::A_TRUE,
                                                           0, 1,
                                                           1, 1,
                                                           nullptr, nullptr});

        _sumOp->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY,
                                                        0, 0,
                                                        0, 0,
                                                        2, 0});


        _sumOp->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY,
                                                        0, 1,
                                                        0, 0,
                                                        2, 1});

        _sumOp->mUpdateEval->addInstr(Execution::AInstr{Execution::FLT_ADD,
                                                          0, 1,
                                                          1, 1,
                                                          1, 1});

        _sumOp->setAggrOffset(1);
        _sumOp->setCurrentId(&(this->mGU->CurOpId));
        _sumOp->setInWorkSpace(this->mGU->connectWorkSpace(idJoin2OutWs));
        _sumOp->setOutWorkSpace(this->mGU->connectWorkSpace(idProjOutWs));
        _sumOp->setNewEdbBuf(this->emptyWS);
        _sumOp->setEdb(this->emptyQbi);
        _sumOp->setIdb(this->mGU->connectRQbi());
        _sumOp->setClearOut(true);
        _sumOp->setStoreOut(true);
        _sumOp->setStoreOutQbi(this->mGU->connectRQbi());

        ops.push_back(_sumOp);


        this->mGU->setWindowsAll(0, WINDOW_LEN, WINDOW_SLIDE);

        for (auto o : ops) {
            o->setMgu(this->mGU);
            o->setUsingQbi(this->useQBI);
            o->setDebug(false);
            o->setDuration(this->totalDur);
            o->setLatencyVec(this->latencies);
            o->setOverheadVec(this->memConsumption);

        }



        return 0;
    }

    int PRExperiment::runExperiment() {
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