from util import *
from antlr4 import *
from RSSQLLexer import *
from RSSQLParser import *

import argparse

class CodeGen:

    def __init__(self, instOps, w, s, dpath):
        self.noIndent = ''
        self.indent = '     '

        self.windowSize = w
        self.windowSlide = s

        self.query = instOps[0].sName

        self.instOps = instOps
        self.inDataPath = dpath
        self.outName = '../main.cpp'
        self.fileDp = None

    def writeToFile(self, lines):
        if self.fileDp:
            for line in lines:
                self.fileDp.write(line)
                self.fileDp.write('\n')
    
    def genCode(self):
        self.fileDp = open(self.outName, 'w')
        self.genHeaders()
        self.genClass()
        self.genMain()
        self.fileDp.close()
        self.fileDp = None

    def genCompareEval(self, opType, operation, t1, t2):
        CompareEvalLines = [
            '{}{}{}Op->mCompareEval->addInstr(Execution::BInstr{{Execution::{}, {}, {}, {}, {}, nullptr, nullptr}});'.\
                format(self.indent*3, self.query, opType, operation, t1[0], t1[1], t2[0], t2[1])
        ]
        self.writeToFile(CompareEvalLines)

    def genInitEval(self, opType, operation, t1, t2, t3):
        InitEvalLines = [
            '{}{}{}Op->mInitEval->addInstr(Execution::AInstr{{Execution::{}, {}, {}, {}, {}, {}, {}}});'.\
                format(self.indent*3, self.query, opType, operation, t1[0], t1[1], t2[0], t2[1], t3[0], t3[1])
        ]
        self.writeToFile(InitEvalLines)

    def genUpdateEval(self, opType, operation, t1, t2, t3):
        UpdateEvalLines = [
            '{}{}{}Op->mUpdateEval->addInstr(Execution::AInstr{{Execution::{}, {}, {}, {}, {}, {}, {}}});'.\
                format(self.indent*3, self.query, opType, operation, t1[0], t1[1], t2[0], t2[1], t3[0], t3[1])
        ]
        self.writeToFile(UpdateEvalLines)

    def genProjEval(self, opType, operation, t1, t2, t3):
        ProjEvalLines = [
            '{}{}{}Op->mProjEval->addInstr(Execution::AInstr{{Execution::{}, {}, {}, {}, {}, {}, {}}});'.\
                format(self.indent*3, self.query, opType, operation, t1[0], t1[1], t2[0], t2[1], t3[0], t3[1])
        ]
        self.writeToFile(ProjEvalLines)

    def genPredEval(self, opType, operation, t1, t2, t3):
        PredEvalLines = [
            '{}{}{}Op->mPredEval->addInstr(Execution::BInstr{{Execution::{}, {}, {}, {}, {}, nullptr, nullptr}});'.\
                format(self.indent*3, self.query, opType, operation, t1[0], t1[1], t2[0], t2[1], t3[0], t3[1])
        ]
        self.writeToFile(PredEvalLines)

    def genJOutEval(self, opType, operation, t1, t2, t3):
        JOutEvalLines = [
            '{}{}{}Op->mJoinOut->addInstr(Execution::AInstr{{Execution::{}, {}, {}, {}, {}, {}, {}}});'.\
                format(self.indent*3, self.query, opType, operation, t1[0], t1[1], t2[0], t2[1], t3[0], t3[1])
        ]
        self.writeToFile(JOutEvalLines)

    def genOpAttr(self, opType, inWS, outWS, newEDBBuf, edb, idb, aggr, clearOut, storeOut, storeOutQbi):
        OpAttrLines = [
            '{}{}{}Op->setCurrentId(&(this->mGU->CurOpId));'.format(self.indent*3, self.query, opType),
            # TODO: Not all have setSrc?
            # '{}{}{}Op->setSource(this->fileSource);'.format(self.indent*3, self.query, opType),
            '{}{}{}Op->setInWorkSpace(this->{});'.format(self.indent*3, self.query, opType, inWS),
            '{}{}{}Op->setOutWorkSpace(this->{});'.format(self.indent*3, self.query, opType, outWS),
            '{}{}{}Op->setNewEdbBuf(this->{});'.format(self.indent*3, self.query, opType, newEDBBuf),
            '{}{}{}Op->setEdb(this->{});'.format(self.indent*3, self.query, opType, edb),
            '{}{}{}Op->setIdb(this->{});'.format(self.indent*3, self.query, opType, idb),
            '{}{}{}Op->setAggrOffset({});'.format(self.indent*3, self.query, opType, aggr),
            '{}{}{}Op->setClearOut({});'.format(self.indent*3, self.query, opType, clearOut),
            '{}{}{}Op->setStoreOut({});'.format(self.indent*3, self.query, opType, storeOut),
            # always qbi
            '{}{}{}Op->setUsingQbi(true);'.format(self.indent*3, self.query, opType),
            '{}{}{}Op->setStoreOutQbi(this->{});'.format(self.indent*3, self.query, opType, storeOutQbi),
            '{}ops.push_back({}{}Op);'.format(self.indent*3, self.query, opType)
        ]

        self.writeToFile(OpAttrLines)

    def genSSourceOp(self, op, i):

        SSLines = [
            '{}Execution::StreamSourceOp  *{}{}Op = new Execution::StreamSourceOp({}, reinterpret_cast<std::ostream &>(stderr));'.format(self.indent*3, self.query, op.OpName, i),
            '{}unsigned int {}, {}, {}, {}, {};'.format(self.indent*3, op.inWS, op.outWS, op.edbBuf, op.edbQbi, op.idbQbi),
            '{}{}{}Op->setSource(this->fileSource);'.format(self.indent*3, self.query, op.OpName)
        ]

        self.writeToFile(SSLines)

        self.genOpAttr(op.OpName, op.InWorkSpace, op.OutWorkSpace, op.newEdbBuf, \
            op.edb, op.idb, op.aggrOffset, \
            op.clearOut, op.storeOut, op.storeOutQbi)

    def genRecOp(self, op, i):

        RecLines = [
            '{}Execution::RecursionOp  *{}{}Op = new Execution::RecursionOp({}, reinterpret_cast<std::ostream &>(stderr));'.format(self.indent*3, self.query, op.OpName, i),
            '{}unsigned int {}, {}, {}, {}, {};'.format(self.indent*3, op.inWS, op.outWS, op.edbBuf, op.edbQbi, op.idbQbi),
        ]

        self.writeToFile(RecLines)

        self.genOpAttr(op.OpName, op.InWorkSpace, op.OutWorkSpace, op.newEdbBuf, \
            op.edb, op.idb, op.aggrOffset, \
            op.clearOut, op.storeOut, op.storeOutQbi)


    def genMinOp(self, op, i):

        MinLines = [
            '{}// Note: though we init csop, it can also be used as min, so we used it for now, fix later'.format(self.indent*3),
            '{}Execution::CountSumOp  *{}{}Op = new Execution::CountSumOp({}, reinterpret_cast<std::ostream &>(stderr));'.format(self.indent*3, self.query, op.OpName, i),
            '{}unsigned int {}, {}, {}, {}, {};'.format(self.indent*3, op.inWS, op.outWS, op.edbBuf, op.edbQbi, op.idbQbi),
        ]

        self.writeToFile(MinLines)

        if op.CompareEvalIdx:
            for group in op.CompareEvalIdx:
                self.genCompareEval(op.OpName, 'FLT_LT', group[0], group[1])

        if op.InitEvalIdx:
            for group in op.InitEvalIdx:
                self.genInitEval(op.OpName, 'FLT_CPY', group[0], group[1], group[2])

        if op.UpdateEvalIdx:
            for group in op.UpdateEvalIdx:
                self.genUpdateEval(op.OpName, 'FLT_CPY', group[0], group[1], group[2])

        self.genOpAttr(op.OpName, op.InWorkSpace, op.OutWorkSpace, op.newEdbBuf, \
            op.edb, op.idb, op.aggrOffset, \
            op.clearOut, op.storeOut, op.storeOutQbi)

    def genProjOp(self, op, i):

        ProjLines = [
            '{}Execution::ProjectOp  *{}{}Op = new Execution::ProjectOp({}, reinterpret_cast<std::ostream &>(stderr));'.format(self.indent*3, self.query, op.OpName, i),
            '{}unsigned int {}, {}, {}, {}, {};'.format(self.indent*3, op.inWS, op.outWS, op.edbBuf, op.edbQbi, op.idbQbi),
        ]

        self.writeToFile(ProjLines)

        if op.ProjEvalOpIdx:
            for group in op.ProjEvalOpIdx:
                self.genProjEval(op.OpName, 'FLT_CPY', group[0], group[1], group[2])

        if op.ProjEvalOneIdx:
            for group in op.ProjEvalOneIdx:
                self.genProjEval(op.OpName, 'FLT_ONE', group[0], group[1], group[2])


        self.genOpAttr(op.OpName, op.InWorkSpace, op.OutWorkSpace, op.newEdbBuf, \
            op.edb, op.idb, op.aggrOffset, \
            op.clearOut, op.storeOut, op.storeOutQbi)


    def genJoinOp(self, op, i):

        JoinLines = [
            '{}Execution::BinaryJoinOp  *{}{}Op = new Execution::BinaryJoinOp({}, reinterpret_cast<std::ostream &>(stderr));'.format(self.indent*3, self.query, op.OpName, i),
            '{}unsigned int {}, {}, {}, {}, {};'.format(self.indent*3, op.inWS, op.outWS, op.edbBuf, op.edbQbi, op.idbQbi),
        ]

        self.writeToFile(JoinLines)

        if op.PredEvalIdx:
            for group in op.PredEvalIdx:
                self.genPredEval(op.OpName, 'FLT_EQ', group[0], group[1], group[2])
        
        if op.JOutEvalIdx:
            for group in op.JOutEvalIdx:
                self.genJOutEval(op.OpName, 'FLT_CPY', group[0], group[1], group[2])

        if self.query == 'pub':
            self.genJOutEval(op.OpName, 'FLT_ADD', (0,2), (1,2), (2,2))

        if '1' in op.OpName:
            self.genOpAttr(op.OpName, op.InWorkSpace.replace('RecOutWs', "Join0OutWs"), op.OutWorkSpace, op.newEdbBuf, \
                op.edb, op.idb, op.aggrOffset, \
                op.clearOut, op.storeOut, op.storeOutQbi)
        else:
            self.genOpAttr(op.OpName, op.InWorkSpace, op.OutWorkSpace, op.newEdbBuf, \
                op.edb, op.idb, op.aggrOffset, \
                op.clearOut, op.storeOut, op.storeOutQbi)

    def genCtOp(self, op, i):

        CtLines = [
            '{}Execution::CountSumOp  *{}{}Op = new Execution::CountSumOp({}, reinterpret_cast<std::ostream &>(stderr));'.format(self.indent*3, self.query, op.OpName, i),
            '{}unsigned int {}, {}, {}, {}, {};'.format(self.indent*3, op.inWS, op.outWS, op.edbBuf, op.edbQbi, op.idbQbi),
        ]

        self.writeToFile(CtLines)

        if op.CompareEvalIdx:
            for group in op.CompareEvalIdx:
                self.genCompareEval(op.OpName, 'A_TRUE', group[0], group[1])

        if op.InitEvalIdx:
            for group in op.InitEvalIdx:
                self.genInitEval(op.OpName, 'FLT_CPY', group[0], group[1], group[2])

        if op.InitEvalOneIdx:
            for group in op.InitEvalOneIdx:
                self.genInitEval(op.OpName, 'FLT_ONE', group[0], group[1], group[2])

        if op.UpdateEvalIdx:
            for group in op.UpdateEvalIdx:
                self.genUpdateEval(op.OpName, 'FLT_ADO', group[0], group[1], group[2])

        self.genOpAttr(op.OpName, op.InWorkSpace, op.OutWorkSpace, op.newEdbBuf, \
            op.edb, op.idb, op.aggrOffset, \
            op.clearOut, op.storeOut, op.storeOutQbi)
    
    def genSumOp(self, op, i):

        SumLines = [
            '{}Execution::CountSumOp  *{}{}Op = new Execution::CountSumOp({}, reinterpret_cast<std::ostream &>(stderr));'.format(self.indent*3, self.query, op.OpName, i),
            '{}unsigned int {}, {}, {}, {}, {};'.format(self.indent*3, op.inWS, op.outWS, op.edbBuf, op.edbQbi, op.idbQbi),
        ]

        self.writeToFile(SumLines)

        if op.CompareEvalIdx:
            for group in op.CompareEvalIdx:
                self.genCompareEval(op.OpName, 'A_TRUE', group[0], group[1])

        if op.InitEvalIdx:
            for group in op.InitEvalIdx:
                self.genInitEval(op.OpName, 'FLT_CPY', group[0], group[1], group[2])

        if op.UpdateEvalIdx:
            for group in op.UpdateEvalIdx:
                self.genUpdateEval(op.OpName, 'FLT_ADO', group[0], group[1], group[2])

        self.genOpAttr(op.OpName, op.InWorkSpace, op.OutWorkSpace, op.newEdbBuf, \
            op.edb, op.idb, op.aggrOffset, \
            op.clearOut, op.storeOut, op.storeOutQbi)

    def genHeaders(self):
        headerLines = [
            '#include "execution/operators/stream_source_op.h"',
            '#include "execution/operators/recursion_op.h"', 
            '#include "execution/operators/proj_op.h"', 
            '#include "execution/operators/bin_join_op.h"',
            '#include "execution/operators/select_op.h"',
            '#include "execution/operators/min_aggr.h"',
            '#include "execution/operators/count_sum_aggr.h"',
            '#include "experiments/experiment.h"',
            '#include "metadata/stream_mgr.h"',
            '#include "metadata/phy_op.h"',
            '#include "execution/qbi/qbi.h"',
            '#include "interface/file_source.h"',
            '#include<iostream>'
        ]
        self.writeToFile(headerLines)

    def genClass(self):
        classLines1 = [
            '{}namespace Experiment {{'.format(self.noIndent),
            '{}class {}Experiment : public Experiment {{'.format(self.indent, self.query),
            '{}private:'.format(self.indent),
            '{}std::vector<Execution::Operator> *instOps;'.format(self.indent*2),
            '{}std::vector<Execution::Operator *> ops;'.format(self.indent*2),
            '{}std::string fileSourceName = "{}";'.format(self.indent*2, self.inDataPath),
            '{}Interface::FileSource *fileSource;'.format(self.indent*2),
            '{}unsigned int WINDOW_LEN = 0;'.format(self.indent*2),
            '{}unsigned int WINDOW_SLIDE = 0;'.format(self.indent*2),
            '{}Execution::WorkSpace *emptyWS;'.format(self.indent*2),
            '{}Execution::Qbi *emptyQbi;'.format(self.indent*2),
            '{}public:'.format(self.indent),
        ]
        self.writeToFile(classLines1)
        self.genConstructor()
        self.genDestructor()
        self.genRegSchema()
        self.genRegOther()
        # This one is complex
        self.genRegPhy()
        self.genRunExp()

        classLines2 = [
            '{}}};'.format(self.indent),
            '{}}}'.format(self.noIndent)
        ]
        self.writeToFile(classLines2)
    
    def genConstructor(self):
        conLines = [
            '{}{}Experiment(unsigned int w, unsigned int s, bool q) {{'.format(self.indent*2, self.query),
            '{}this->WINDOW_SLIDE = s;'.format(self.indent*3),
            '{}this->WINDOW_LEN = w;'.format(self.indent*3),
            '{}this->useQBI = q;'.format(self.indent*3),
            '{}this->instOps = new std::vector<Execution::Operator>();'.format(self.indent*3),
            '{}this->emptyWS = new Execution::WorkSpace(0);'.format(self.indent*3),
            '{}this->emptyQbi = new Execution::Qbi();'.format(self.indent*3),
            '{}this->fileSource = new Interface::FileSource(this->fileSourceName, WINDOW_LEN, WINDOW_SLIDE);'.format(self.indent*3),
            '{}this->mGU->CurOpId = 0;'.format(self.indent*3),
            '{}}}'.format(self.indent*2)
        ]
        self.writeToFile(conLines)
        

    def genDestructor(self):
        desLines = [
            '{}~{}Experiment() {{'.format(self.indent*2, self.query),
            '{}delete this->instOps;'.format(self.indent*3),
            '{}delete this->emptyWS;'.format(self.indent*3),
            '{}delete this->emptyQbi;'.format(self.indent*3),
            '{}delete this->fileSource;'.format(self.indent*3),
            '{}}}'.format(self.indent*2)
        ]
        self.writeToFile(desLines)
    
    def genRegSchema(self):
        regSchLines = [
            '{}//Not needed here in this quries'.format(self.indent*2),
            '{}int registerSchema() {{return 0;}}'.format(self.indent*2)
        ]
        self.writeToFile(regSchLines)

    def genRegOther(self):
        regOtherLines = [
            '{}int registerOthers(unsigned int window, unsigned int slide, bool qbi) {{'.format(self.indent*2),
            '{}this->WINDOW_LEN = window;'.format(self.indent*3),
            '{}this->WINDOW_SLIDE = slide;'.format(self.indent*3),
            '{}this->useQBI = qbi;'.format(self.indent*3),
            '{}return 0;'.format(self.indent*3),
            '{}}}'.format(self.indent*2)
        ]
        self.writeToFile(regOtherLines)

    def genRegPhy(self):
        regPhyLines = [
            '{}int registerPhysical() {{'.format(self.indent*2),
        ]
        self.writeToFile(regPhyLines)

        for i, op in enumerate(self.instOps):
            if op.type == 'ssrc':
                self.genSSourceOp(op, i)
            elif op.type == 'proj':
                self.genProjOp(op, i)
            elif op.type == 'recur':
                self.genRecOp(op, i)
            elif op.type == 'min':
                self.genMinOp(op, i)
            elif op.type == 'count':
                self.genCtOp(op, i)
            elif op.type == 'sum':
                self.genSumOp(op, i)
            elif op.type == 'join':
                self.genJoinOp(op, i)
            else:
                pass


        regPhyLines2 = [
            '{}this->mGU->setWindowsAll(0, WINDOW_LEN, WINDOW_SLIDE);'.format(self.indent*3),
            '{}for (auto o : ops) {{'.format(self.indent*3),
            '{}o->setMgu(this->mGU);'.format(self.indent*4),
            '{}o->setUsingQbi(true);'.format(self.indent*4),
            '{}o->setDebug(false);'.format(self.indent*4),
            '{}}}'.format(self.indent*3),
            '{}return 0;'.format(self.indent*3),
            '{}}}'.format(self.indent*2)
        ]
        self.writeToFile(regPhyLines2)

    def genRunExp(self):
        runExpLines = [
            '{}int runExperiment() {{'.format(self.indent*2, self.query),
            '{}bool done = false;'.format(self.indent*3),
            '{}while (!done) {{'.format(self.indent*3),
            '{}for (auto op : ops) {{'.format(self.indent*4),
            '{}op->run();'.format(self.indent*5),
            '{}if (this->mGU->CurOpId == 99) {{'.format(self.indent*5),
            '{}done = true;'.format(self.indent*6),
            '{}break;'.format(self.indent*6),
            '{}}}'.format(self.indent*5),
            '{}}}'.format(self.indent*4),
            '{}}}'.format(self.indent*3),
            '{}return 0;'.format(self.indent*3),
            '{}}}'.format(self.indent*2)
        ]
        self.writeToFile(runExpLines)

    def genMain(self):
        mainLines = [
            '{}int main (int argc, char *argv[]) {{'.format(self.noIndent),
            '{}Experiment::{}Experiment *exp = new Experiment::{}Experiment({}, {}, true);'.format(self.indent, self.query, self.query, self.windowSize, self.windowSlide),
            '{}exp->registerSchema();'.format(self.indent),
            '{}exp->registerPhysical();'.format(self.indent),
            '{}exp->runExperiment();'.format(self.indent),
            '{}delete exp;'.format(self.indent),
            '{}return 0;}}'.format(self.indent)
        ]

        self.writeToFile(mainLines)
        pass


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--query", type=str, required=True, help="Absolute Path to the query to be processed")
    parser.add_argument("--dpath", type=str, required=True, help="Absolute Path to the data directory")
    args = parser.parse_args()

    query = args.query
    dpath = args.dpath

    q = ''

    with open(query, "r") as file:
        q = file.read()

    if q:
        pp = PreProcessor(q)
        pp = pp.parse_query()
        instops = iterateAllStatements(pp[0], pp[1], pp[2], pp[3], pp[4], pp[5], pp[6])
        cg = CodeGen(instops, pp[6], pp[7], dpath)
        cg.genCode()

if __name__ == "__main__":
    main()



# dpath = './data.dat'

# q2 = 'CREATE STREAM subOrg (Ts: timestamp, Org: int, SubOrg: int) WITH recursive isSubOrg(Ts, Org, SubOrg) AS (SELECT Ts, Org, SubOrg FROM subOrg) UNION (SELECT greatest(subOrg.Ts, isSubOrg.Ts), isSubOrg.Org, subOrg.SubOrg FROM subOrg, isSubOrg WHERE isSubOrg.SubOrg = subOrg.Org) SELECT Org, SubOrg FROM isSubOrg WINDOW[6000, 3000]'
# q3 = 'CREATE STREAM ride (Ts: timestamp, PickUp: str, DropTo: str, Fare: double) WITH recursive bestPrice (Ts, PickUp, DropTo, min() AS BestFare) AS (SELECT Ts, PickUp, DropTo, Fare FROM ride) UNION (SELECT greatest(ride.Ts, bestPrice.Ts), bestPrice.PickUp, ride.DropTo, bestPrice.BestFare + ride.Fare FROM ride, bestPrice WHERE ride.PickUp = bestPrice.DropTo) SELECT PickUp, DropTo, BestFare FROM bestPrice WINDOW[60]'
# q4 = 'CREATE STREAM relateTo (Ts: timestamp, X: int, Y: int) WITH recursive ealiestOrg (Ts, X, min() AS Eid) AS (SELECT Ts, X, X FROM relateTo) UNION (SELECT greatest(relateTo.Ts, ealiestOrg.Ts), relateTo.Y, ealiestOrg.Eid FROM relateTo, ealiestOrg WHERE relateTo.X = ealiestOrg.X) SELECT X, Eid FROM ealiestOrg'
# q6 = 'CREATE STREAM pub (Ts: timestamp, PaperId: int, PaperRef: int) CREATE RELATION totalRef (Ts, PaperId, count() AS R) AS (SELECT Ts, PaperId, PaperRef FROM pub) WITH recursive prank (Ts, P, sum() AS V) AS (SELECT Ts, P, 1.0 FROM totalRef) UNION (SELECT greatest(prank.Ts, pub.Ts, totalRef.Ts), pub.PapertotalRef, prank.V / totalRef.R FROM totalRef, pub, prank WHERE prank.P = pub.PaperId, prank.P = totalRef.PaperId) SELECT P, R FROM prank WINDOW[360, 30]'
# q7 = 'CREATE STREAM subOrg (Ts: timestamp, Org: int, SubOrg: int) WITH recursive isSubOrg(Ts, Org, SubOrg) AS (SELECT Ts, Org, SubOrg FROM subOrg) UNION (SELECT greatest(subOrg.Ts, isSubOrg.Ts), isSubOrg.Org, subOrg.SubOrg FROM subOrg, isSubOrg WHERE isSubOrg.SubOrg = subOrg.Org) SELECT Org, SubOrg FROM isSubOrg'
# pp = PreProcessor(q4)
# pp = pp.parse_query()
# instops = iterateAllStatements(pp[0], pp[1], pp[2], pp[3], pp[4], pp[5], pp[6])
# cg = CodeGen(instops, pp[6], pp[7], dpath)
# cg.genCode()
# # cg = CodeGen(instops)
