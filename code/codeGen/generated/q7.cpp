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
     class subOrgExperiment : public Experiment {
     private:
          std::vector<Execution::Operator> *instOps;
          std::vector<Execution::Operator *> ops;
          std::string fileSourceName = "../../data/q7.dat";
          Interface::FileSource *fileSource;
          unsigned int WINDOW_LEN = 0;
          unsigned int WINDOW_SLIDE = 0;
          Execution::WorkSpace *emptyWS;
          Execution::Qbi *emptyQbi;
     public:
          subOrgExperiment(unsigned int w, unsigned int s, bool q) {
               this->WINDOW_SLIDE = s;
               this->WINDOW_LEN = w;
               this->useQBI = q;
               this->instOps = new std::vector<Execution::Operator>();
               this->emptyWS = new Execution::WorkSpace(0);
               this->emptyQbi = new Execution::Qbi();
               this->fileSource = new Interface::FileSource(this->fileSourceName, WINDOW_LEN, WINDOW_SLIDE);
               this->mGU->CurOpId = 0;
          }
          ~subOrgExperiment() {
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
               Execution::StreamSourceOp  *subOrgSSOp = new Execution::StreamSourceOp(0, reinterpret_cast<std::ostream &>(stderr));
               unsigned int SSInWs, SSOutWs, SSEdbBuf, SSEdbQbi, SSIdbQbi;
               subOrgSSOp->setSource(this->fileSource);
               subOrgSSOp->setCurrentId(&(this->mGU->CurOpId));
               subOrgSSOp->setInWorkSpace(this->emptyWS);
               subOrgSSOp->setOutWorkSpace(this->mGU->registerWorkSpace(SSOutWs));
               subOrgSSOp->setNewEdbBuf(this->emptyWS);
               subOrgSSOp->setEdb(this->emptyQbi);
               subOrgSSOp->setIdb(this->emptyQbi);
               subOrgSSOp->setAggrOffset(2);
               subOrgSSOp->setClearOut(true);
               subOrgSSOp->setStoreOut(false);
               subOrgSSOp->setUsingQbi(true);
               subOrgSSOp->setStoreOutQbi(this->emptyQbi);
               ops.push_back(subOrgSSOp);
               Execution::ProjectOp  *subOrgProj0Op = new Execution::ProjectOp(1, reinterpret_cast<std::ostream &>(stderr));
               unsigned int Proj0InWs, Proj0OutWs, Proj0EdbBuf, Proj0EdbQbi, Proj0IdbQbi;
               subOrgProj0Op->mProjEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 0, 0, 0, 1, 0});
               subOrgProj0Op->mProjEval->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 1, 0, 0, 1, 1});
               subOrgProj0Op->setCurrentId(&(this->mGU->CurOpId));
               subOrgProj0Op->setInWorkSpace(this->mGU->connectWorkSpace(SSOutWs));
               subOrgProj0Op->setOutWorkSpace(this->mGU->registerWorkSpace(Proj0OutWs));
               subOrgProj0Op->setNewEdbBuf(this->mGU->registerWorkSpace(Proj0EdbBuf));
               subOrgProj0Op->setEdb(this->mGU->registerNRQbi(Proj0EdbQbi));
               subOrgProj0Op->setIdb(this->emptyQbi);
               subOrgProj0Op->setAggrOffset(2);
               subOrgProj0Op->setClearOut(true);
               subOrgProj0Op->setStoreOut(true);
               subOrgProj0Op->setUsingQbi(true);
               subOrgProj0Op->setStoreOutQbi(this->mGU->connectNRQbi(Proj0EdbQbi));
               ops.push_back(subOrgProj0Op);
               Execution::RecursionOp  *subOrgRecOp = new Execution::RecursionOp(2, reinterpret_cast<std::ostream &>(stderr));
               unsigned int RecInWs, RecOutWs, RecEdbBuf, RecEdbQbi, RecIdbQbi;
               subOrgRecOp->setCurrentId(&(this->mGU->CurOpId));
               subOrgRecOp->setInWorkSpace(this->mGU->connectWorkSpace(Proj0OutWs));
               subOrgRecOp->setOutWorkSpace(this->mGU->registerWorkSpace(RecOutWs));
               subOrgRecOp->setNewEdbBuf(this->emptyWS);
               subOrgRecOp->setEdb(this->emptyQbi);
               subOrgRecOp->setIdb(this->mGU->connectRQbi());
               subOrgRecOp->setAggrOffset(2);
               subOrgRecOp->setClearOut(true);
               subOrgRecOp->setStoreOut(true);
               subOrgRecOp->setUsingQbi(true);
               subOrgRecOp->setStoreOutQbi(this->mGU->registerNRQbi(RecEdbQbi));
               ops.push_back(subOrgRecOp);
               Execution::BinaryJoinOp  *subOrgJoin0Op = new Execution::BinaryJoinOp(3, reinterpret_cast<std::ostream &>(stderr));
               unsigned int Join0InWs, Join0OutWs, Join0EdbBuf, Join0EdbQbi, Join0IdbQbi;
               subOrgJoin0Op->mPredEval->addInstr(Execution::BInstr{Execution::FLT_EQ, 0, 0, 1, 0, nullptr, nullptr});
               subOrgJoin0Op->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_CPY, 0, 1, 0, 0, 2, 0});
               subOrgJoin0Op->mJoinOut->addInstr(Execution::AInstr{Execution::FLT_CPY, 1, 1, 0, 0, 2, 1});
               subOrgJoin0Op->setCurrentId(&(this->mGU->CurOpId));
               subOrgJoin0Op->setInWorkSpace(this->mGU->connectWorkSpace(RecOutWs));
               subOrgJoin0Op->setOutWorkSpace(this->mGU->connectWorkSpace(Proj0OutWs));
               subOrgJoin0Op->setNewEdbBuf(this->mGU->connectWorkSpace(Proj0OutWs));
               subOrgJoin0Op->setEdb(this->mGU->connectNRQbi(Proj0EdbQbi));
               subOrgJoin0Op->setIdb(this->emptyQbi);
               subOrgJoin0Op->setAggrOffset(2);
               subOrgJoin0Op->setClearOut(true);
               subOrgJoin0Op->setStoreOut(false);
               subOrgJoin0Op->setUsingQbi(true);
               subOrgJoin0Op->setStoreOutQbi(this->emptyQbi);
               ops.push_back(subOrgJoin0Op);
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
     Experiment::subOrgExperiment *exp = new Experiment::subOrgExperiment(3000,  100, true);
     exp->registerSchema();
     exp->registerPhysical();
     exp->runExperiment();
     delete exp;
     return 0;}
