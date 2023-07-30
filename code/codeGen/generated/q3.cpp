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
     class rideExperiment : public Experiment {
     private:
          std::vector<Execution::Operator> *instOps;
          std::vector<Execution::Operator *> ops;
          std::string fileSourceName = "../../data/q3.dat";
          Interface::FileSource *fileSource;
          unsigned int WINDOW_LEN = 0;
          unsigned int WINDOW_SLIDE = 0;
          Execution::WorkSpace *emptyWS;
          Execution::Qbi *emptyQbi;
     public:
          rideExperiment(unsigned int w, unsigned int s, bool q) {
               this->WINDOW_SLIDE = s;
               this->WINDOW_LEN = w;
               this->useQBI = q;
               this->instOps = new std::vector<Execution::Operator>();
               this->emptyWS = new Execution::WorkSpace(0);
               this->emptyQbi = new Execution::Qbi();
               this->fileSource = new Interface::FileSource(this->fileSourceName, WINDOW_LEN, WINDOW_SLIDE);
               this->mGU->CurOpId = 0;
          }
          ~rideExperiment() {
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
               Execution::StreamSourceOp  *rideSSOp = new Execution::StreamSourceOp(0, reinterpret_cast<std::ostream &>(stderr));
               unsigned int SSInWs, SSOutWs, SSEdbBuf, SSEdbQbi, SSIdbQbi;
               rideSSOp->setSource(this->fileSource);
               rideSSOp->setCurrentId(&(this->mGU->CurOpId));
               rideSSOp->setInWorkSpace(this->emptyWS);
               rideSSOp->setOutWorkSpace(this->mGU->registerWorkSpace(SSOutWs));
               rideSSOp->setNewEdbBuf(this->emptyWS);
               rideSSOp->setEdb(this->emptyQbi);
               rideSSOp->setIdb(this->emptyQbi);
               rideSSOp->setAggrOffset(2);
               rideSSOp->setClearOut(true);
               rideSSOp->setStoreOut(false);
               rideSSOp->setUsingQbi(true);
               rideSSOp->setStoreOutQbi(this->emptyQbi);
               ops.push_back(rideSSOp);
               Execution::ProjectOp  *rideProj0Op = new Execution::ProjectOp(1, reinterpret_cast<std::ostream &>(stderr));
               unsigned int Proj0InWs, Proj0OutWs, Proj0EdbBuf, Proj0EdbQbi, Proj0IdbQbi;
               rideProj0Op->mProjEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 0, 0, 0, 1, 0});
               rideProj0Op->mProjEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 1, 0, 0, 1, 1});
               rideProj0Op->mProjEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 2, 0, 0, 1, 2});
               rideProj0Op->setCurrentId(&(this->mGU->CurOpId));
               rideProj0Op->setInWorkSpace(this->mGU->connectWorkSpace(SSOutWs));
               rideProj0Op->setOutWorkSpace(this->mGU->registerWorkSpace(Proj0OutWs));
               rideProj0Op->setNewEdbBuf(this->mGU->registerWorkSpace(Proj0EdbBuf));
               rideProj0Op->setEdb(this->mGU->registerNRQbi(Proj0EdbQbi));
               rideProj0Op->setIdb(this->emptyQbi);
               rideProj0Op->setAggrOffset(2);
               rideProj0Op->setClearOut(true);
               rideProj0Op->setStoreOut(true);
               rideProj0Op->setUsingQbi(true);
               rideProj0Op->setStoreOutQbi(this->mGU->connectNRQbi(Proj0EdbQbi));
               ops.push_back(rideProj0Op);
               // Note: though we init csop, it can also be used as min, so we used it for now, fix later
               Execution::CountSumOp  *rideMin0Op = new Execution::CountSumOp(2, reinterpret_cast<std::ostream &>(stderr));
               unsigned int Min0InWs, Min0OutWs, Min0EdbBuf, Min0EdbQbi, Min0IdbQbi;
               rideMin0Op->mCompareEval->addInstr(Execution::BInstr{Execution::FLT_LT, 0, 2, 1, 2, nullptr, nullptr});
               rideMin0Op->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 0, 0, 0, 2, 0});
               rideMin0Op->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 1, 0, 0, 2, 1});
               rideMin0Op->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 2, 0, 0, 2, 2});
               rideMin0Op->mUpdateEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 2, 0, 0, 1, 2});
               rideMin0Op->setCurrentId(&(this->mGU->CurOpId));
               rideMin0Op->setInWorkSpace(this->mGU->connectWorkSpace(Proj0OutWs));
               rideMin0Op->setOutWorkSpace(this->mGU->registerWorkSpace(Min0OutWs));
               rideMin0Op->setNewEdbBuf(this->emptyWS);
               rideMin0Op->setEdb(this->mGU->connectNRQbi(Proj0EdbQbi));
               rideMin0Op->setIdb(this->mGU->connectRQbi());
               rideMin0Op->setAggrOffset(2);
               rideMin0Op->setClearOut(true);
               rideMin0Op->setStoreOut(true);
               rideMin0Op->setUsingQbi(true);
               rideMin0Op->setStoreOutQbi(this->mGU->connectRQbi());
               ops.push_back(rideMin0Op);
               Execution::RecursionOp  *rideRecOp = new Execution::RecursionOp(3, reinterpret_cast<std::ostream &>(stderr));
               unsigned int RecInWs, RecOutWs, RecEdbBuf, RecEdbQbi, RecIdbQbi;
               rideRecOp->setCurrentId(&(this->mGU->CurOpId));
               rideRecOp->setInWorkSpace(this->mGU->connectWorkSpace(Min0OutWs));
               rideRecOp->setOutWorkSpace(this->mGU->registerWorkSpace(RecOutWs));
               rideRecOp->setNewEdbBuf(this->emptyWS);
               rideRecOp->setEdb(this->emptyQbi);
               rideRecOp->setIdb(this->mGU->connectRQbi());
               rideRecOp->setAggrOffset(2);
               rideRecOp->setClearOut(true);
               rideRecOp->setStoreOut(true);
               rideRecOp->setUsingQbi(true);
               rideRecOp->setStoreOutQbi(this->mGU->registerNRQbi(RecEdbQbi));
               ops.push_back(rideRecOp);
               Execution::BinaryJoinOp  *rideJoin0Op = new Execution::BinaryJoinOp(4, reinterpret_cast<std::ostream &>(stderr));
               unsigned int Join0InWs, Join0OutWs, Join0EdbBuf, Join0EdbQbi, Join0IdbQbi;
               rideJoin0Op->mPredEval->addInstr(Execution::BInstr{Execution::FLT_EQ, 0, 1, 1, 0, nullptr, nullptr});
               rideJoin0Op->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 0, 0, 0, 2, 0});
               rideJoin0Op->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_CPY, 1, 1, 0, 0, 2, 1});
               rideJoin0Op->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 2, 1, 2, 2, 2});
               rideJoin0Op->setCurrentId(&(this->mGU->CurOpId));
               rideJoin0Op->setInWorkSpace(this->mGU->connectWorkSpace(RecOutWs));
               rideJoin0Op->setOutWorkSpace(this->mGU->registerWorkSpace(Join0OutWs));
               rideJoin0Op->setNewEdbBuf(this->mGU->connectWorkSpace(Proj0OutWs));
               rideJoin0Op->setEdb(this->mGU->connectNRQbi(Proj0EdbQbi));
               rideJoin0Op->setIdb(this->emptyQbi);
               rideJoin0Op->setAggrOffset(2);
               rideJoin0Op->setClearOut(true);
               rideJoin0Op->setStoreOut(false);
               rideJoin0Op->setUsingQbi(true);
               rideJoin0Op->setStoreOutQbi(this->emptyQbi);
               ops.push_back(rideJoin0Op);
               // Note: though we init csop, it can also be used as min, so we used it for now, fix later
               Execution::CountSumOp  *rideMin1Op = new Execution::CountSumOp(5, reinterpret_cast<std::ostream &>(stderr));
               unsigned int Min1InWs, Min1OutWs, Min1EdbBuf, Min1EdbQbi, Min1IdbQbi;
               rideMin1Op->mCompareEval->addInstr(Execution::BInstr{Execution::FLT_LT, 0, 2, 1, 2, nullptr, nullptr});
               rideMin1Op->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 0, 0, 0, 2, 0});
               rideMin1Op->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 1, 0, 0, 2, 1});
               rideMin1Op->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 2, 0, 0, 2, 2});
               rideMin1Op->mUpdateEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 2, 0, 0, 1, 2});
               rideMin1Op->setCurrentId(&(this->mGU->CurOpId));
               rideMin1Op->setInWorkSpace(this->mGU->connectWorkSpace(Join0OutWs));
               rideMin1Op->setOutWorkSpace(this->mGU->connectWorkSpace(Min0OutWs));
               rideMin1Op->setNewEdbBuf(this->emptyWS);
               rideMin1Op->setEdb(this->mGU->connectNRQbi(Proj0EdbQbi));
               rideMin1Op->setIdb(this->mGU->connectRQbi());
               rideMin1Op->setAggrOffset(2);
               rideMin1Op->setClearOut(true);
               rideMin1Op->setStoreOut(true);
               rideMin1Op->setUsingQbi(true);
               rideMin1Op->setStoreOutQbi(this->mGU->connectRQbi());
               ops.push_back(rideMin1Op);
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
     Experiment::rideExperiment *exp = new Experiment::rideExperiment(60, 60, true);
     exp->registerSchema();
     exp->registerPhysical();
     exp->runExperiment();
     delete exp;
     return 0;}
