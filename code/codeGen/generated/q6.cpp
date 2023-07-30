#include "execution/operators/stream_source_op.h"
#include "execution/operators/recursion_op.h"
#include "execution/operators/proj_op.h"
#include "execution/operators/bin_join_op.h"
#include "execution/operators/select_op.h"
#include "execution/operators/min_aggr.h"
#include "execution/operators/count_sum_aggr.h"
#include "experiments/experiment.h"
#include "metadata/stream_mgr.h"
#include "metadata/phy_op.h"
#include "execution/qbi/qbi.h"
#include "interface/file_source.h"
#include<iostream>
namespace Experiment {
     class pubExperiment : public Experiment {
     private:
          std::vector<Execution::Operator> *instOps;
          std::vector<Execution::Operator *> ops;
          std::string fileSourceName = "../../data/q6.dat";
          Interface::FileSource *fileSource;
          unsigned int WINDOW_LEN = 0;
          unsigned int WINDOW_SLIDE = 0;
          Execution::WorkSpace *emptyWS;
          Execution::Qbi *emptyQbi;
     public:
          pubExperiment(unsigned int w, unsigned int s, bool q) {
               this->WINDOW_SLIDE = s;
               this->WINDOW_LEN = w;
               this->useQBI = q;
               this->instOps = new std::vector<Execution::Operator>();
               this->emptyWS = new Execution::WorkSpace(0);
               this->emptyQbi = new Execution::Qbi();
               this->fileSource = new Interface::FileSource(this->fileSourceName, WINDOW_LEN, WINDOW_SLIDE);
               this->mGU->CurOpId = 0;
          }
          ~pubExperiment() {
               delete this->instOps;
               delete this->emptyWS;
               delete this->emptyQbi;
               delete this->fileSource;
          }
          //Not needed here in this quries
          int registerSchema() {return 0;}
          int registerOthers(unsigned int window, unsigned int slide, bool qbi) {
               this->WINDOW_LEN = window;
               this->WINDOW_SLIDE = slide;
               this->useQBI = qbi;
               return 0;
          }
          int registerPhysical() {
               Execution::StreamSourceOp  *pubSSOp = new Execution::StreamSourceOp(0, reinterpret_cast<std::ostream &>(stderr));
               unsigned int SSInWs, SSOutWs, SSEdbBuf, SSEdbQbi, SSIdbQbi;
               pubSSOp->setSource(this->fileSource);
               pubSSOp->setCurrentId(&(this->mGU->CurOpId));
               pubSSOp->setInWorkSpace(this->emptyWS);
               pubSSOp->setOutWorkSpace(this->mGU->registerWorkSpace(SSOutWs));
               pubSSOp->setNewEdbBuf(this->emptyWS);
               pubSSOp->setEdb(this->emptyQbi);
               pubSSOp->setIdb(this->emptyQbi);
               pubSSOp->setAggrOffset(1);
               pubSSOp->setClearOut(true);
               pubSSOp->setStoreOut(false);
               pubSSOp->setUsingQbi(true);
               pubSSOp->setStoreOutQbi(this->emptyQbi);
               ops.push_back(pubSSOp);
               Execution::CountSumOp  *pubCount0Op = new Execution::CountSumOp(1, reinterpret_cast<std::ostream &>(stderr));
               unsigned int Count0InWs, Count0OutWs, Count0EdbBuf, Count0EdbQbi, Count0IdbQbi;
               pubCount0Op->mCompareEval->addInstr(Execution::BInstr{Execution::A_TRUE, 0, 1, 1, 1, nullptr, nullptr});
               pubCount0Op->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 0, 0, 0, 2, 0});
               pubCount0Op->mInitEval->addInstr(Execution::AInstr{Execution::FLT_ONE, 0, 1, 0, 0, 2, 1});
               pubCount0Op->mUpdateEval->addInstr(Execution::AInstr{Execution::FLT_ADO, 0, 0, 0, 0, 1, 1});
               pubCount0Op->setCurrentId(&(this->mGU->CurOpId));
               pubCount0Op->setInWorkSpace(this->mGU->connectWorkSpace(SSOutWs));
               pubCount0Op->setOutWorkSpace(this->mGU->registerWorkSpace(Count0OutWs));
               pubCount0Op->setNewEdbBuf(this->emptyWS);
               pubCount0Op->setEdb(this->emptyQbi);
               pubCount0Op->setIdb(this->emptyQbi);
               pubCount0Op->setAggrOffset(1);
               pubCount0Op->setClearOut(true);
               pubCount0Op->setStoreOut(true);
               pubCount0Op->setUsingQbi(true);
               pubCount0Op->setStoreOutQbi(this->mGU->registerNRQbi(Count0IdbQbi));
               ops.push_back(pubCount0Op);
               Execution::ProjectOp  *pubProj0Op = new Execution::ProjectOp(2, reinterpret_cast<std::ostream &>(stderr));
               unsigned int Proj0InWs, Proj0OutWs, Proj0EdbBuf, Proj0EdbQbi, Proj0IdbQbi;
               pubProj0Op->mProjEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 0, 0, 0, 1, 0});
               pubProj0Op->mProjEval->addInstr(Execution::AInstr{Execution::FLT_ONE, 0, 0, 0, 0, 1, 1});
               pubProj0Op->setCurrentId(&(this->mGU->CurOpId));
               pubProj0Op->setInWorkSpace(this->mGU->connectWorkSpace(Count0OutWs));
               pubProj0Op->setOutWorkSpace(this->mGU->registerWorkSpace(Proj0OutWs));
               pubProj0Op->setNewEdbBuf(this->mGU->registerWorkSpace(Proj0EdbBuf));
               pubProj0Op->setEdb(this->mGU->registerNRQbi(Proj0EdbQbi));
               pubProj0Op->setIdb(this->emptyQbi);
               pubProj0Op->setAggrOffset(1);
               pubProj0Op->setClearOut(true);
               pubProj0Op->setStoreOut(true);
               pubProj0Op->setUsingQbi(true);
               pubProj0Op->setStoreOutQbi(this->mGU->connectNRQbi(Proj0EdbQbi));
               ops.push_back(pubProj0Op);
               Execution::RecursionOp  *pubRecOp = new Execution::RecursionOp(3, reinterpret_cast<std::ostream &>(stderr));
               unsigned int RecInWs, RecOutWs, RecEdbBuf, RecEdbQbi, RecIdbQbi;
               pubRecOp->setCurrentId(&(this->mGU->CurOpId));
               pubRecOp->setInWorkSpace(this->mGU->connectWorkSpace(Proj0OutWs));
               pubRecOp->setOutWorkSpace(this->mGU->registerWorkSpace(RecOutWs));
               pubRecOp->setNewEdbBuf(this->emptyWS);
               pubRecOp->setEdb(this->emptyQbi);
               pubRecOp->setIdb(this->mGU->connectRQbi());
               pubRecOp->setAggrOffset(1);
               pubRecOp->setClearOut(true);
               pubRecOp->setStoreOut(true);
               pubRecOp->setUsingQbi(true);
               pubRecOp->setStoreOutQbi(this->mGU->registerNRQbi(RecEdbQbi));
               ops.push_back(pubRecOp);
               Execution::BinaryJoinOp  *pubJoin0Op = new Execution::BinaryJoinOp(4, reinterpret_cast<std::ostream &>(stderr));
               unsigned int Join0InWs, Join0OutWs, Join0EdbBuf, Join0EdbQbi, Join0IdbQbi;
               pubJoin0Op->mPredEval->addInstr(Execution::BInstr{Execution::FLT_EQ, 0, 0, 1, 0, nullptr, nullptr});
               pubJoin0Op->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 0, 0, 0, 2, 0});
               pubJoin0Op->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_CPY, 1, 1, 0, 1, 2, 1});
               pubJoin0Op->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_ADD, 0, 2, 1, 2, 2, 2});
               pubJoin0Op->setCurrentId(&(this->mGU->CurOpId));
               pubJoin0Op->setInWorkSpace(this->mGU->connectWorkSpace(RecOutWs));
               pubJoin0Op->setOutWorkSpace(this->mGU->registerWorkSpace(Join0OutWs));
               pubJoin0Op->setNewEdbBuf(this->mGU->connectWorkSpace(Proj0OutWs));
               pubJoin0Op->setEdb(this->mGU->connectNRQbi(Proj0EdbQbi));
               pubJoin0Op->setIdb(this->emptyQbi);
               pubJoin0Op->setAggrOffset(1);
               pubJoin0Op->setClearOut(true);
               pubJoin0Op->setStoreOut(false);
               pubJoin0Op->setUsingQbi(true);
               pubJoin0Op->setStoreOutQbi(this->emptyQbi);
               ops.push_back(pubJoin0Op);
               Execution::BinaryJoinOp  *pubJoin1Op = new Execution::BinaryJoinOp(5, reinterpret_cast<std::ostream &>(stderr));
               unsigned int Join1InWs, Join1OutWs, Join1EdbBuf, Join1EdbQbi, Join1IdbQbi;
               pubJoin1Op->mPredEval->addInstr(Execution::BInstr{Execution::FLT_EQ, 0, 0, 1, 0, nullptr, nullptr});
               pubJoin1Op->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 1, 0, 0, 2, 0});
               pubJoin1Op->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_CPY, 1, 1, 0, 0, 2, 1});
               pubJoin1Op->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_ADD, 0, 2, 1, 2, 2, 2});
               pubJoin1Op->setCurrentId(&(this->mGU->CurOpId));
               pubJoin1Op->setInWorkSpace(this->mGU->connectWorkSpace(Join0OutWs));
               pubJoin1Op->setOutWorkSpace(this->mGU->registerWorkSpace(Join1OutWs));
               pubJoin1Op->setNewEdbBuf(this->mGU->connectWorkSpace(Proj0OutWs));
               pubJoin1Op->setEdb(this->mGU->connectNRQbi(Proj0EdbQbi));
               pubJoin1Op->setIdb(this->emptyQbi);
               pubJoin1Op->setAggrOffset(1);
               pubJoin1Op->setClearOut(true);
               pubJoin1Op->setStoreOut(false);
               pubJoin1Op->setUsingQbi(true);
               pubJoin1Op->setStoreOutQbi(this->emptyQbi);
               ops.push_back(pubJoin1Op);
               Execution::CountSumOp  *pubSum0Op = new Execution::CountSumOp(6, reinterpret_cast<std::ostream &>(stderr));
               unsigned int Sum0InWs, Sum0OutWs, Sum0EdbBuf, Sum0EdbQbi, Sum0IdbQbi;
               pubSum0Op->mCompareEval->addInstr(Execution::BInstr{Execution::A_TRUE, 0, 1, 1, 1, nullptr, nullptr});
               pubSum0Op->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 0, 0, 0, 2, 0});
               pubSum0Op->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 1, 0, 0, 2, 1});
               pubSum0Op->mUpdateEval->addInstr(Execution::AInstr{Execution::FLT_ADO, 0, 1, 0, 0, 1, 1});
               pubSum0Op->setCurrentId(&(this->mGU->CurOpId));
               pubSum0Op->setInWorkSpace(this->mGU->connectWorkSpace(Join1OutWs));
               pubSum0Op->setOutWorkSpace(this->mGU->connectWorkSpace(Proj0OutWs));
               pubSum0Op->setNewEdbBuf(this->emptyWS);
               pubSum0Op->setEdb(this->emptyQbi);
               pubSum0Op->setIdb(this->emptyQbi);
               pubSum0Op->setAggrOffset(1);
               pubSum0Op->setClearOut(true);
               pubSum0Op->setStoreOut(false);
               pubSum0Op->setUsingQbi(true);
               pubSum0Op->setStoreOutQbi(this->mGU->connectRQbi());
               ops.push_back(pubSum0Op);
               this->mGU->setWindowsAll(0, WINDOW_LEN, WINDOW_SLIDE);
               for (auto o : ops) {
                    o->setMgu(this->mGU);
                    o->setUsingQbi(true);
                    o->setDebug(false);
               }
               return 0;
          }
          int runExperiment() {
               bool done = false;
               while (!done) {
                    for (auto op : ops) {
                         op->run();
                         if (this->mGU->CurOpId == 99) {
                              done = true;
                              break;
                         }
                    }
               }
               return 0;
          }
     };
}
int main (int argc, char *argv[]) {
     Experiment::pubExperiment *exp = new Experiment::pubExperiment(360,  30, true);
     exp->registerSchema();
     exp->registerPhysical();
     exp->runExperiment();
     delete exp;
     return 0;}
