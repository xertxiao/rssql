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
     class relateToExperiment : public Experiment {
     private:
          std::vector<Execution::Operator> *instOps;
          std::vector<Execution::Operator *> ops;
          std::string fileSourceName = "../../data/q4.dat";
          Interface::FileSource *fileSource;
          unsigned int WINDOW_LEN = 0;
          unsigned int WINDOW_SLIDE = 0;
          Execution::WorkSpace *emptyWS;
          Execution::Qbi *emptyQbi;
     public:
          relateToExperiment(unsigned int w, unsigned int s, bool q) {
               this->WINDOW_SLIDE = s;
               this->WINDOW_LEN = w;
               this->useQBI = q;
               this->instOps = new std::vector<Execution::Operator>();
               this->emptyWS = new Execution::WorkSpace(0);
               this->emptyQbi = new Execution::Qbi();
               this->fileSource = new Interface::FileSource(this->fileSourceName, WINDOW_LEN, WINDOW_SLIDE);
               this->mGU->CurOpId = 0;
          }
          ~relateToExperiment() {
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
               Execution::StreamSourceOp  *relateToSSOp = new Execution::StreamSourceOp(0, reinterpret_cast<std::ostream &>(stderr));
               unsigned int SSInWs, SSOutWs, SSEdbBuf, SSEdbQbi, SSIdbQbi;
               relateToSSOp->setSource(this->fileSource);
               relateToSSOp->setCurrentId(&(this->mGU->CurOpId));
               relateToSSOp->setInWorkSpace(this->emptyWS);
               relateToSSOp->setOutWorkSpace(this->mGU->registerWorkSpace(SSOutWs));
               relateToSSOp->setNewEdbBuf(this->emptyWS);
               relateToSSOp->setEdb(this->emptyQbi);
               relateToSSOp->setIdb(this->emptyQbi);
               relateToSSOp->setAggrOffset(1);
               relateToSSOp->setClearOut(true);
               relateToSSOp->setStoreOut(false);
               relateToSSOp->setUsingQbi(true);
               relateToSSOp->setStoreOutQbi(this->emptyQbi);
               ops.push_back(relateToSSOp);
               Execution::ProjectOp  *relateToProj0Op = new Execution::ProjectOp(1, reinterpret_cast<std::ostream &>(stderr));
               unsigned int Proj0InWs, Proj0OutWs, Proj0EdbBuf, Proj0EdbQbi, Proj0IdbQbi;
               relateToProj0Op->mProjEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 0, 0, 0, 1, 0});
               relateToProj0Op->mProjEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 1, 0, 0, 1, 1});
               relateToProj0Op->setCurrentId(&(this->mGU->CurOpId));
               relateToProj0Op->setInWorkSpace(this->mGU->connectWorkSpace(SSOutWs));
               relateToProj0Op->setOutWorkSpace(this->mGU->registerWorkSpace(Proj0OutWs));
               relateToProj0Op->setNewEdbBuf(this->mGU->registerWorkSpace(Proj0EdbBuf));
               relateToProj0Op->setEdb(this->mGU->registerNRQbi(Proj0EdbQbi));
               relateToProj0Op->setIdb(this->emptyQbi);
               relateToProj0Op->setAggrOffset(1);
               relateToProj0Op->setClearOut(true);
               relateToProj0Op->setStoreOut(true);
               relateToProj0Op->setUsingQbi(true);
               relateToProj0Op->setStoreOutQbi(this->mGU->connectNRQbi(Proj0EdbQbi));
               ops.push_back(relateToProj0Op);
               // Note: though we init csop, it can also be used as min, so we used it for now, fix later
               Execution::CountSumOp  *relateToMin0Op = new Execution::CountSumOp(2, reinterpret_cast<std::ostream &>(stderr));
               unsigned int Min0InWs, Min0OutWs, Min0EdbBuf, Min0EdbQbi, Min0IdbQbi;
               relateToMin0Op->mCompareEval->addInstr(Execution::BInstr{Execution::FLT_LT, 0, 1, 1, 1, nullptr, nullptr});
               relateToMin0Op->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 0, 0, 0, 2, 0});
               relateToMin0Op->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 1, 0, 0, 2, 1});
               relateToMin0Op->mUpdateEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 1, 0, 0, 1, 1});
               relateToMin0Op->setCurrentId(&(this->mGU->CurOpId));
               relateToMin0Op->setInWorkSpace(this->mGU->connectWorkSpace(Proj0OutWs));
               relateToMin0Op->setOutWorkSpace(this->mGU->registerWorkSpace(Min0OutWs));
               relateToMin0Op->setNewEdbBuf(this->emptyWS);
               relateToMin0Op->setEdb(this->mGU->connectNRQbi(Proj0EdbQbi));
               relateToMin0Op->setIdb(this->mGU->connectRQbi());
               relateToMin0Op->setAggrOffset(1);
               relateToMin0Op->setClearOut(true);
               relateToMin0Op->setStoreOut(true);
               relateToMin0Op->setUsingQbi(true);
               relateToMin0Op->setStoreOutQbi(this->mGU->connectRQbi());
               ops.push_back(relateToMin0Op);
               Execution::RecursionOp  *relateToRecOp = new Execution::RecursionOp(3, reinterpret_cast<std::ostream &>(stderr));
               unsigned int RecInWs, RecOutWs, RecEdbBuf, RecEdbQbi, RecIdbQbi;
               relateToRecOp->setCurrentId(&(this->mGU->CurOpId));
               relateToRecOp->setInWorkSpace(this->mGU->connectWorkSpace(Min0OutWs));
               relateToRecOp->setOutWorkSpace(this->mGU->registerWorkSpace(RecOutWs));
               relateToRecOp->setNewEdbBuf(this->emptyWS);
               relateToRecOp->setEdb(this->emptyQbi);
               relateToRecOp->setIdb(this->mGU->connectRQbi());
               relateToRecOp->setAggrOffset(1);
               relateToRecOp->setClearOut(true);
               relateToRecOp->setStoreOut(true);
               relateToRecOp->setUsingQbi(true);
               relateToRecOp->setStoreOutQbi(this->mGU->registerNRQbi(RecEdbQbi));
               ops.push_back(relateToRecOp);
               Execution::BinaryJoinOp  *relateToJoin0Op = new Execution::BinaryJoinOp(4, reinterpret_cast<std::ostream &>(stderr));
               unsigned int Join0InWs, Join0OutWs, Join0EdbBuf, Join0EdbQbi, Join0IdbQbi;
               relateToJoin0Op->mPredEval->addInstr(Execution::BInstr{Execution::FLT_EQ, 0, 1, 1, 0, nullptr, nullptr});
               relateToJoin0Op->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 0, 0, 0, 2, 0});
               relateToJoin0Op->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_CPY, 1, 1, 0, 0, 2, 1});
               relateToJoin0Op->setCurrentId(&(this->mGU->CurOpId));
               relateToJoin0Op->setInWorkSpace(this->mGU->connectWorkSpace(RecOutWs));
               relateToJoin0Op->setOutWorkSpace(this->mGU->registerWorkSpace(Join0OutWs));
               relateToJoin0Op->setNewEdbBuf(this->mGU->connectWorkSpace(Proj0OutWs));
               relateToJoin0Op->setEdb(this->mGU->connectNRQbi(Proj0EdbQbi));
               relateToJoin0Op->setIdb(this->emptyQbi);
               relateToJoin0Op->setAggrOffset(1);
               relateToJoin0Op->setClearOut(true);
               relateToJoin0Op->setStoreOut(false);
               relateToJoin0Op->setUsingQbi(true);
               relateToJoin0Op->setStoreOutQbi(this->emptyQbi);
               ops.push_back(relateToJoin0Op);
               // Note: though we init csop, it can also be used as min, so we used it for now, fix later
               Execution::CountSumOp  *relateToMin1Op = new Execution::CountSumOp(5, reinterpret_cast<std::ostream &>(stderr));
               unsigned int Min1InWs, Min1OutWs, Min1EdbBuf, Min1EdbQbi, Min1IdbQbi;
               relateToMin1Op->mCompareEval->addInstr(Execution::BInstr{Execution::FLT_LT, 0, 1, 1, 1, nullptr, nullptr});
               relateToMin1Op->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 0, 0, 0, 2, 0});
               relateToMin1Op->mInitEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 1, 0, 0, 2, 1});
               relateToMin1Op->mUpdateEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 1, 0, 0, 1, 1});
               relateToMin1Op->setCurrentId(&(this->mGU->CurOpId));
               relateToMin1Op->setInWorkSpace(this->mGU->connectWorkSpace(Join0OutWs));
               relateToMin1Op->setOutWorkSpace(this->mGU->connectWorkSpace(Min0OutWs));
               relateToMin1Op->setNewEdbBuf(this->emptyWS);
               relateToMin1Op->setEdb(this->mGU->connectNRQbi(Proj0EdbQbi));
               relateToMin1Op->setIdb(this->mGU->connectRQbi());
               relateToMin1Op->setAggrOffset(1);
               relateToMin1Op->setClearOut(true);
               relateToMin1Op->setStoreOut(true);
               relateToMin1Op->setUsingQbi(true);
               relateToMin1Op->setStoreOutQbi(this->mGU->connectRQbi());
               ops.push_back(relateToMin1Op);
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
     Experiment::relateToExperiment *exp = new Experiment::relateToExperiment(6000,  3000, true);
     exp->registerSchema();
     exp->registerPhysical();
     exp->runExperiment();
     delete exp;
     return 0;}
