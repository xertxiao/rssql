import re
import sqlparse

# global
FIELD_DICT = {}
AGGR_IDX = -1

def conRQbi():
    return 'mGU->connectRQbi()'

def conRegNRQbi(nrqbis, id):
    for strid in nrqbis:
        if id in strid:
            return 'mGU->connectNRQbi({})'.format(id)
    return 'mGU->registerNRQbi({})'.format(id)

def conRegWS(wses, id):
    for strid in wses:
        if id in strid:
            return 'mGU->connectWorkSpace({})'.format(id)
    return 'mGU->registerWorkSpace({})'.format(id)
    
def conEmpyWs():
    return 'emptyWS'

def conEmpyQbi():
    return 'emptyQbi'

class ASTNode:
    
    def __init__(self, sql, OpName):
        self.OpName = OpName
        self.InWorkSpace = 'emptyWS'
        self.OutWorkSpace = 'emptyWS'
        self.newEdbBuf = 'emptyWS'
        self.edb = 'emptyQbi'
        self.idb = 'emptyQbi'
        self.clearOut = None
        self.storeOut = None
        self.storeOutQbi = None
        self.aggrOffset = -1

        self.inWS, self.outWS, self.edbBuf, self.edbQbi, self.idbQbi = \
            '{}InWs'.format(OpName), \
            '{}OutWs'.format(OpName), \
            '{}EdbBuf'.format(OpName), \
            '{}EdbQbi'.format(OpName), \
            '{}IdbQbi'.format(OpName)


    def set_intr(self, aggr_field_offset):
        pass

    def check_connect(self, ws, qbi, aggr_offset):
        # iws
        if 'empty' in self.InWorkSpace:
            self.InWorkSpace = conEmpyWs()
        elif self.InWorkSpace in ws:
            self.InWorkSpace = conRegWS(ws, self.InWorkSpace)
        else:
            self.InWorkSpace = conRegWS(ws, self.InWorkSpace)
            ws.append(self.InWorkSpace)

        # ows
        if 'empty' in self.OutWorkSpace:
            self.OutWorkSpace = conEmpyWs()
        elif self.OutWorkSpace in ws:
            self.OutWorkSpace = conRegWS(ws, self.OutWorkSpace)
        else:
            self.OutWorkSpace = conRegWS(ws, self.OutWorkSpace)
            ws.append(self.OutWorkSpace)

        # ebuf
        if 'empty' in self.newEdbBuf:
            self.newEdbBuf = conEmpyWs()
        elif self.newEdbBuf in ws:
            self.newEdbBuf = conRegWS(ws, self.newEdbBuf)
        else:
            self.newEdbBuf = conRegWS(ws, self.newEdbBuf)
            ws.append(self.newEdbBuf)

        # edb
        if 'empty' in self.edb:
            self.edb = conEmpyQbi()
        elif self.edb in qbi:
            self.edb = conRegNRQbi(qbi, self.edb)
        else:
            self.edb = conRegNRQbi(qbi, self.edb)
            qbi.append(self.edb)

        # idb
        if 'empty' in self.idb:
            self.idb = conEmpyQbi()
        else:
            self.idb = conRQbi()

        # clear out, store out
        if self.clearOut:
            self.clearOut = 'true'
        else:
            self.clearOut = 'false'

        if self.storeOut:
            self.storeOut = 'true'
        else:
            self.storeOut = 'false'
        
        # storeOQbi
        if self.type in ['ssrc', 'join']:
            self.storeOutQbi = conEmpyQbi()
        elif self.type in ['min', 'sum']:
            self.storeOutQbi = conRQbi()
        elif self.type == 'count':
            #TODO: sequence? add first?
            self.storeOutQbi = conRegNRQbi(qbi, self.idbQbi)
            # exit(1)
            if self.idbQbi not in qbi:
                qbi.append(self.idbQbi)
        else:
            self.storeOutQbi = conRegNRQbi(qbi, self.edbQbi)

        # aggr offset
        self.aggrOffset = str(aggr_offset)




        return ws, qbi

    def print_debug_info(self):
        print("==============={}==============".format(self.OpName))
        print("InWorkSpace: {}".format(self.InWorkSpace))
        print("OutWorkSpace: {}".format(self.OutWorkSpace))
        print("neewEdbBuf: {}".format(self.newEdbBuf))
        print("setEdb: {}".format(self.edb))
        print("setIdb: {}".format(self.idb))
        print("clearOut: {}".format(self.clearOut))
        print("storeOut: {}".format(self.storeOut))
        print("storeOutQbi: {}".format(self.storeOutQbi))
        print("aggrOffset: {}".format(self.aggrOffset))
        print("inWS: {}".format(self.inWS))
        print("outWS: {}".format(self.outWS))
        print("edbBuf: {}".format(self.edbBuf))
        print("edbQbi: {}".format(self.edbQbi))
        print("idbQbi: {}".format(self.idbQbi))
        # print("================================")

class SSourceNode(ASTNode):

    def __init__(self, sql, OpName):
        super().__init__(sql, OpName)
        self.type = 'ssrc'
        self.sql = sqlparse.parse(sql)[0]
        self.tokens = [token.value for token in self.sql.flatten() if token.ttype == sqlparse.tokens.Name]
        self.sName, self.inFields = self.tokens[0], self.tokens[1:]
        self.inFields.remove('Ts')
        self.inFields = ['{}.{}'.format(self.sName, t) for t in self.inFields]
        self.outFields = self.inFields

        # Setting attributes
        self.OutWorkSpace = self.outWS
        self.InWorkSpace = self.InWorkSpace
        self.source = 'fileSource'
        self.newEdbBuf = self.newEdbBuf
        self.aggrOffset = 0
        self.clearOut = True
        self.storeOut = False
        # 0 for ss
        self.aggrOffset = 0


        

    def print_debug_info(self):
        super().print_debug_info()
        print("sql: {}".format(self.sql.value))
        print("sName: {}".format(self.sName))
        print("inFields: {}".format(self.inFields))
        print("outFields: {}".format(self.outFields))
        print("================================")

class ProjNode(ASTNode):

    def __init__(self, sql, OpName):
        super().__init__(sql, OpName)
        self.type = 'proj'
        self.sql = sqlparse.parse(sql)[0]
        # print("proj sql:{}".format(self.sql))
        self.tokens = [token.value for token in self.sql.flatten() if token.ttype == sqlparse.tokens.Name or token.ttype == sqlparse.tokens.Number.Float]
        # self.tokens = [(token.value, token.ttype) for token in self.sql.flatten()] 
        # self.tokens = [(token.value, token, token.ttype) for token in self.sql.flatten()]
        # print("proj tokens:{}".format(self.tokens))
        self.sName, self.inFields = self.tokens[-1], self.tokens[:-1]
        self.inFields.remove('Ts')
        # self.inFields = ['{}.{}'.format(self.sName, t) for t in self.inFields]
        self.outFields = ['{}.{}'.format(self.sName, t) for t in self.inFields]
        self.inFields = None

    # def set_in_field(self, par):
    #     self.inFields = par.outFields
        self.ProjEvalOp = 'Execution::FLT_CPY'
        self.ProjEvalOne = 'Execution::FLT_ONE'
        self.ProjEvalOpIdx = []
        self.ProjEvalOneIdx = []

    def set_intr(self, aggr_field_offset):
        # set cpy
        for idx in range(len(self.outFields)):
            if "1.0" in self.outFields[idx]:
                self.ProjEvalOneIdx.append([(0, 0), (0, 0), (1, 1)])
            else:
                self.ProjEvalOpIdx.append([(0, idx), (0, 0), (1, idx)])
    
    def print_debug_info(self):
        super().print_debug_info()
        print("sql: {}".format(self.sql.value))
        print("sName: {}".format(self.sName))
        print("inFields: {}".format(self.inFields))
        print("outFields: {}".format(self.outFields))
        print("ProjEvalOp: {} ||| {}".format(self.ProjEvalOp, self.ProjEvalOpIdx))
        print("ProjEvalOne: {} ||| {}".format(self.ProjEvalOne, self.ProjEvalOneIdx))
        print("================================")

class RecurNode(ASTNode):

    def __init__(self, sql, OpName):
        super().__init__(sql, OpName)
        self.type = 'recur'
        self.sql = sqlparse.parse(sql)[0]
        self.tokens = [token.value for token in self.sql.flatten() if token.ttype == sqlparse.tokens.Name] 
        self.sName, self.inFields = self.tokens[0], self.tokens[1:]
        self.inFields.remove('Ts')

        if 'min' in self.inFields:
            self.aggrIdx = self.inFields.index('min')
            self.inFields.remove('min')
            self.aggrType = 'min'
        elif 'max' in self.inFields:
            self.aggrIdx = self.inFields.index('max')
            self.inFields.remove('max')
            self.aggrType = 'max'
        elif 'count' in self.inFields:
            self.aggrIdx = self.inFields.index('count') 
            self.inFields.remove('count')
            self.aggrType = 'count'
        elif 'sum' in self.inFields:
            self.aggrIdx = self.inFields.index('sum') 
            self.inFields.remove('sum')
            self.aggrType = 'sum'
        else:
            self.aggrIdx = -1
            self.aggrType = 'none'

        # self.inFields = ['{}.{}'.format(self.sName, t) for t in self.inFields]
        self.outFields = ['{}.{}'.format(self.sName, t) for t in self.inFields]
        self.inFields = None
        self.aggrOffset = self.aggrIdx
        # print("Recnode: infield:" + str(self.inFields))

    def print_debug_info(self):
        super().print_debug_info()
        print("sql: {}".format(self.sql.value))
        print("sName: {}".format(self.sName))
        print("inFields: {}".format(self.inFields))
        print("outFields: {}".format(self.outFields))
        print("aggrType: {}".format(self.aggrType))
        print("================================")


class MinNode(ASTNode):

    def __init__(self, sql, schema, OpName):
        super().__init__(sql, OpName)
        self.type = 'min'
        self.schema = sqlparse.parse(schema)[0]
        self.sql = sqlparse.parse(sql)[0]
        self.inFields = []
        self.outFields = []
        self.aggrIdx = -1

        self.CompareEvalOp = 'Execution::FLT_LT'
        self.CompareEvalIdx = []
        self.InitEvalOp = 'Execution::FLT_CPY'
        self.InitEvalIdx = []
        self.UpdateEvalOp = 'Execution::FLT_CPY'
        self.UpdateEvalIdx = []

        self.tokens = [token.value for token in self.sql.flatten() if token.ttype == sqlparse.tokens.Name or token.ttype == sqlparse.tokens.Number.Float]
        # self.tokens = [(token.value, token.ttype) for token in self.sql.flatten()] 
        # self.tokens = [(token.value, token, token.ttype) for token in self.sql.flatten()]
        # print("proj tokens:{}".format(self.tokens))
        self.sName, self.inFields = self.tokens[-1], self.tokens[:-1]
        self.inFields.remove('Ts')
        # self.inFields = ['{}.{}'.format(self.sName, t) for t in self.inFields]
        self.outFields = ['{}.{}'.format(self.sName, t) for t in self.inFields]
        self.inFields = None


        # self.tokens = [token.value for token in self.sql.flatten() if token.ttype == sqlparse.tokens.Name] 
        # self.sName, self.inFields = self.tokens[0], self.tokens[1:]
        # print("SSSS:" + str(self.inFields))
        # exit(1)
        # if 'Ts' in self.inFields: self.inFields.remove('Ts')
        # self.outFields = ['{}.{}'.format(self.sName, t) for t in self.inFields]
        # self.inFields = None

    def set_intr(self, aggr_field_offset):
        # set compare
        self.CompareEvalIdx.append([(0, aggr_field_offset), (1, aggr_field_offset), (None, None)])

        # set init
        for idx in range(len(self.outFields)):
            self.InitEvalIdx.append([(0, idx), (0, 0), (2, idx)])

        # set update
        self.UpdateEvalIdx.append([(0, aggr_field_offset), (0, 0), (1, aggr_field_offset)])

    def print_debug_info(self):
        super().print_debug_info()
        print("sql: {}".format(self.sql.value))
        print("schema: {}".format(self.schema.value))
        # print("sName: {}".format(self.sName))
        print("inFields: {}".format(self.inFields))
        print("outFields: {}".format(self.outFields))
        print("CompareEval: {} ||| {}".format(self.CompareEvalOp, self.CompareEvalIdx))
        print("InitEval: {} ||| {}".format(self.InitEvalOp, self.InitEvalIdx))
        print("UpdateEval: {} ||| {}".format(self.UpdateEvalOp, self.UpdateEvalIdx))
        print("================================")


class CountNode(ASTNode):

    def __init__(self, sql, schema, OpName):
        super().__init__(sql, OpName)
        self.type = 'count'
        self.schema = sqlparse.parse(schema)[0]
        self.sql = sqlparse.parse(sql)[0]
        self.inFields = []
        self.outFields = []
        self.aggrIdx = -1



        self.CompareEvalOp = 'Execution::A_TRUE'
        self.CompareEvalIdx = []
        self.InitEvalOp = 'Execution::FLT_CPY'
        self.InitEvalIdx = []
        self.InitEvalOneOp = 'Execution::FLT_ONE'
        self.InitEvalOneIdx = []
        self.UpdateEvalOp = 'Execution::FLT_ADO'
        self.UpdateEvalIdx = []

        self.tokens = [token.value for token in self.sql.flatten() if token.ttype == sqlparse.tokens.Name] 
        self.sName, self.inFields = self.tokens[0], self.tokens[1:]
        if 'Ts' in self.inFields: self.inFields.remove('Ts')
        self.outFields = ['{}.{}'.format(self.sName, t) for t in self.inFields]
        self.inFields = None
        
    def set_intr(self, aggr_field_offset):
        # set compare
        self.CompareEvalIdx.append([(0, aggr_field_offset), (1, aggr_field_offset), (None, None)])

        # set init
        # for idx in range(len(self.outFields)):
        self.InitEvalIdx.append([(0, 0), (0, 0), (2, 0)])
        self.InitEvalOneIdx.append([(0, 1), (0, 0), (2, 1)])

        # set update
        self.UpdateEvalIdx.append([(0, 0), (0, 0), (1, aggr_field_offset)])



    
    def print_debug_info(self):
        super().print_debug_info()
        print("sql: {}".format(self.sql.value))
        print("schema: {}".format(self.schema.value))
        # print("sName: {}".format(self.sName))
        print("inFields: {}".format(self.inFields))
        print("outFields: {}".format(self.outFields))
        print("CompareEval: {} ||| {}".format(self.CompareEvalOp, self.CompareEvalIdx))
        print("InitEval: {} ||| {}".format(self.InitEvalOp, self.InitEvalIdx))
        print("UpdateEval: {} ||| {}".format(self.UpdateEvalOp, self.UpdateEvalIdx))
        print("================================")

class SumNode(ASTNode):

    def __init__(self, sql, schema, OpName):
        super().__init__(sql, OpName)
        self.schema = sqlparse.parse(schema)[0]
        self.type = 'sum'
        self.sql = sqlparse.parse(sql)[0]
        self.inFields = []
        self.outFields = []
        self.aggrIdx = -1


        self.CompareEvalOp = 'Execution::A_TRUE'
        self.CompareEvalIdx = []
        self.InitEvalOp = 'Execution::FLT_CPY'
        self.InitEvalIdx = []
        self.UpdateEvalOp = 'Execution::FLT_ADO'
        self.UpdateEvalIdx = []


        self.tokens = [token.value for token in self.sql.flatten() if token.ttype == sqlparse.tokens.Name] 
        self.sName, self.inFields = self.tokens[0], self.tokens[1:]
        if 'Ts' in self.inFields: self.inFields.remove('Ts')
        self.outFields = ['{}.{}'.format(self.sName, t) for t in self.inFields]
        self.inFields = None

    def set_intr(self, aggr_field_offset):
        # set compare
        self.CompareEvalIdx.append([(0, aggr_field_offset), (1, aggr_field_offset), (None, None)])

        # set init
        for idx in range(len(self.outFields)):
            self.InitEvalIdx.append([(0, idx), (0, 0), (2, idx)])

        # set update
        self.UpdateEvalIdx.append([(0, aggr_field_offset), (0, 0), (1, aggr_field_offset)])

    def print_debug_info(self):
        super().print_debug_info()
        print("sql: {}".format(self.sql.value))
        print("schema: {}".format(self.schema.value))
        # print("sName: {}".format(self.sName))
        print("inFields: {}".format(self.inFields))
        print("outFields: {}".format(self.outFields))
        print("CompareEval: {} ||| {}".format(self.CompareEvalOp, self.CompareEvalIdx))
        print("InitEval: {} ||| {}".format(self.InitEvalOp, self.InitEvalIdx))
        print("UpdateEval: {} ||| {}".format(self.UpdateEvalOp, self.UpdateEvalIdx))
        print("================================")

class BiJoinNode(ASTNode):

    def __init__(self, sql, OpName):
        super().__init__(sql, OpName)
        self.sql = sql
        self.type = 'join'
        # self.sql = sqlparse.parse(sql)[0]
        self.inFields = []
        self.left, self.right = '', ''
        self.condition=''


        self.PredEvalOp = 'Execution::FLT_EQ'
        self.PredEvalIdx = []
        self.JOutEvalOp = 'Execution::FLT_CPY'
        self.JOutEvalIdx = []

    def set_intr(self, aggr_field_offset):
        # TODO: make sure finish this intr
        # set predEval
        # self.CompareEvalIdx.append([(0, aggr_field_offset), (1, aggr_field_offset), (None, None)])

        if 'ride' in self.condition:
            self.PredEvalIdx.append([(0, 1), (1, 0), (None, None)])
            self.JOutEvalIdx.append([(0, 0), (0, 0), (2, 0)])
            self.JOutEvalIdx.append([(1, 1), (0, 0), (2, 1)])
            self.JOutEvalIdx.append([(0, 2), (1, 2), (2, 2)])
        elif 'subOrg' in self.condition:
            self.PredEvalIdx.append([(0, 0), (1, 0), (None, None)])
            self.JOutEvalIdx.append([(0, 1), (0, 0), (2, 0)])
            self.JOutEvalIdx.append([(1, 1), (0, 0), (2, 1)])
        elif 'relateTo' in self.condition:
            self.PredEvalIdx.append([(0, 1), (1, 0), (None, None)])
            self.JOutEvalIdx.append([(0, 0), (0, 0), (2, 0)])
            self.JOutEvalIdx.append([(1, 1), (0, 0), (2, 1)])
        else:
            self.PredEvalIdx.append([(0, 0), (1, 0), (None, None)])
            if '0' in self.OpName:
                self.JOutEvalIdx.append([(0, 0), (0, 0), (2, 0)])
                self.JOutEvalIdx.append([(1, 1), (0, 1), (2, 1)])
            else:
                self.JOutEvalIdx.append([(0, 1), (0, 0), (2, 0)])
                self.JOutEvalIdx.append([(1, 1), (0, 0), (2, 1)])

    def print_debug_info(self):
        super().print_debug_info()
        print("sql: {}".format(self.sql))
        # print("sName: {}".format(self.sName))
        print("inFields: {}".format(self.inFields))
        print("left: {}".format(self.left))
        print("right: {}".format(self.right))
        print("condition: {}".format(self.condition))
        print("================================")

def iterateAllStatements(stream, rel, rec_schema, exit_r, recur_r, q, w):
    

    AGGR = False
    AGGR_OFFSET = 0
    PROJ_OFFSET = 0
    JOIN_OFFSET = 0
    InstOps = []

    ## Stream Src Node
    snode = SSourceNode(stream, 'SS')
    InstOps.append(snode)

    if rel:
        ctnode = CountNode(rel,rel,'Count{}'.format(AGGR_OFFSET))
        # set attri
        ctnode.OutWorkSpace = ctnode.outWS
        ctnode.InWorkSpace = snode.outWS
        # no idb
        ctnode.clearOut = True
        # store edb
        ctnode.storeOut = True
        # Alwasy store edb to edbqbi
        InstOps.append(ctnode)


    # Proj Node
    pjnode = ProjNode(exit_r, 'Proj{}'.format(PROJ_OFFSET))
    PROJ_OFFSET += 1

    if pjnode.sName == InstOps[-1].sName:
        pjnode.OutWorkSpace = pjnode.outWS
        pjnode.InWorkSpace = InstOps[-1].outWS
        pjnode.newEdbBuf = pjnode.edbBuf
        pjnode.edb = pjnode.edbQbi
        # no idb
        pjnode.clearOut = True
        # store edb
        pjnode.storeOut = True
        # Alwasy store edb to edbqbi
        pjnode.storeOutQbi = pjnode.edbQbi
        # TODO: 0 for edb ?
        pjnode.aggrOffset = 0
    else:
        print("pjnode: error matching snode")
        print(pjnode.sName)
        print(InstOps[-1].sName)
        exit(1)

    InstOps.append(pjnode)   
    ## Recursion Node
    rnode = RecurNode(rec_schema, 'Rec')

    # Deal with recur aggr
    if rnode.aggrType != 'none': 
        AGGR = True
        if rnode.aggrType == 'min':
            minnode = MinNode(exit_r,rec_schema,'Min{}'.format(AGGR_OFFSET))
            # Set exit r aggr
            minnode.OutWorkSpace = minnode.outWS
            minnode.InWorkSpace = pjnode.outWS
            minnode.edb = pjnode.edbQbi
            minnode.idb = 'connectRQbi'
            minnode.clearOut = True
            # store idb
            minnode.storeOut = True
            # Alwasy store edb to edbqbi
            minnode.storeOutQbi = 'connectRQbi'
            minnode.aggrOffset = rnode.aggrIdx
            AGGR_OFFSET += 1
            InstOps.append(minnode)   
        elif rnode.aggrType == 'sum':
            pass
        else:
            print("aggrNode1: Not Implemented")
            exit(1)

    aggr_field_offset = rnode.aggrIdx


    ## set attributes for r node
    rnode.OutWorkSpace = rnode.outWS
    rnode.InWorkSpace = InstOps[-1].outWS
    rnode.idb = 'connectRQbi'
    rnode.clearOut = True
    rnode.storeOut = True
    rnode.storeOutQbi = 'connectRQbi'
    rnode.aggrOffset = rnode.aggrIdx

    ## Notice the order
    InstOps.append(rnode)


    # if rnode.sName == snode.sName:
    #     rnode.InWorkSpace = snode.outWS

    recsel, recfrom, recwhere = sqlparse.parse(recur_r)[0].get_sublists()
    if type(recsel.tokens[0]) == sqlparse.sql.Function:
        recsel = recsel[1:]

    recsel_values=[]
    for token in recsel:
        if type(token) == sqlparse.sql.Operation or type(token) == sqlparse.sql.Identifier:
            recsel_values.append(token.value)

    recfrom_values=[]
    for token in recfrom:
        if type(token) == sqlparse.sql.Operation or type(token) == sqlparse.sql.Identifier:
            recfrom_values.append(token.value)

    recwhere_values=[]
    for token in recwhere:
        if type(token) == sqlparse.sql.Comparison:
            recwhere_values.append(token.value)


    if recfrom:
        _outer = rnode.sName
        recfrom_values.remove(rnode.sName)
        for _rel in recfrom_values: 
            newjoin = BiJoinNode(recur_r,'Join{}'.format(JOIN_OFFSET))
            JOIN_OFFSET += 1
            for c in recwhere_values:
                if _outer in c and _rel in c:
                    newjoin.condition = c
                    break
            newjoin.left, newjoin.right = _outer, _rel

            # Set attr for eaach join
            newjoin.InWorkSpace = rnode.outWS
            newjoin.OutWorkSpace = newjoin.outWS    
            # TODO: check this for PR?   
            newjoin.newEdbBuf = pjnode.outWS
            newjoin.edb = pjnode.edb
            # empty b/c not needed here
            newjoin.idb = 'emptyQbi'
            # TODO: join aggroffset = 0?
            newjoin.aggrOffset = 0
            newjoin.clearOut = True
            newjoin.storeOut = False
            newjoin.storeOutQbi = 'emptyQbi'

            InstOps.append(newjoin)
            _outer = _rel

        
    # print(recsel_values)
    # print(recfrom_values)
    # print(recwhere_values)


    if rnode.aggrType != 'none': 
        AGGR = True
        if rnode.aggrType == 'min':
            minnode = MinNode(exit_r,rec_schema,'Min{}'.format(AGGR_OFFSET))
            # Set exit r aggr
            minnode.OutWorkSpace = minnode.outWS
            minnode.InWorkSpace = InstOps[-1].outWS
            minnode.edb = pjnode.edbQbi
            minnode.idb = 'connectRQbi'
            minnode.clearOut = True
            # store idb
            minnode.storeOut = True
            # Alwasy store edb to edbqbi
            minnode.storeOutQbi = 'connectRQbi'
            minnode.aggrOffset = rnode.aggrIdx
            AGGR_OFFSET += 1
            InstOps.append(minnode)   
        elif rnode.aggrType == 'sum':
            sumnode = SumNode(exit_r,rec_schema,'Sum{}'.format(AGGR_OFFSET))
            # Set exit r aggr
            sumnode.OutWorkSpace = sumnode.outWS
            sumnode.InWorkSpace = InstOps[-1].outWS
            sumnode.edb = 'emptyWS'
            sumnode.clearOut = True
            # store idb
            sumnode.storeOut = False
            # Alwasy store edb to edbqbi
            sumnode.storeOutQbi = 'emptyQbi'
            sumnode.aggrOffset = 1
            AGGR_OFFSET += 1
            InstOps.append(sumnode)   
        else:
            print("aggrNode1: Not Implemented")
            exit(1)

    for op in InstOps:
        for op2 in InstOps:
            if op.InWorkSpace == op2.OutWorkSpace:
                op.inFields = op2.outWS
                # op.print_debug_info()
    existWs, existQbi = [], []
    InstOps[-1].OutWorkSpace = rnode.InWorkSpace
    if 'isSubOrg' in rec_schema:
        aggr_field_offset = 2
    for op in InstOps:
        # print("aggr_field_offset:{}".format(aggr_field_offset))
        op.set_intr(aggr_field_offset)
        # print(existWs)
        # print(existWs)
        existWs, existQbi = op.check_connect(existWs, existQbi, aggr_field_offset)
        # existWs += _existWs
        # existQbi += _existQbi
        # op.print_debug_info()
    return InstOps

class PreProcessor:
    def __init__(self, query):
        self.query = query

    def _parse_stream(self):
        match = re.match(r"CREATE STREAM (.*?) (WITH|CREATE)", self.query)
        if match:
            result = match.group(1)
            # print(f"_parse_stream: {result}")
            return result
        else:
            # print("No _parse_stream")
            return None

    def _parse_relation(self):
        match = re.search(r"CREATE RELATION (.*?) (WITH|CREATE)", self.query)
        if match:
            result = match.group(1)
            # print(f"_parse_relation: {result}")
            return result
        else:
            # print("No _parse_relation")
            return None

    def _parse_recur_schema(self):
        match = re.search(r"WITH recursive (.*?) AS \(", self.query)
        if match:
            result = match.group(1)
            # print(f"_parse_recur_schema: {result}")
            return result
        else:
            # print("No _parse_recur_schema")
            return None

    def _parse_exit(self):
        # result = re.findall(r"AS \((.*?)(?=UNION)", self.query)
        # output = result[0].replace("(","").replace(")","")
        match = re.search(r"(?s:.*)AS \((.*?)\) UNION", self.query)
        if match:
            result = match.group(1)
            # print("_parse_exit:{}".format(result))
            return result
        else:
            # print("No _parse_exit")
            return None
    
    def _parse_recur(self):
        match = re.search(r"UNION \((.*?)\) SELECT", self.query)
        if match:
            result = match.group(1)
            # print(f"_parse_recur: {result}")
            return result
        else:
            # print("No _parse_recur")
            return None

    def _parse_query(self):
        match = re.search(r"(?s:.*)(SELECT (.*?) FROM.*) WINDOW", self.query)
        if match:
            result = match.group(1)
            # print(f"_parse_query match: {result}")
            return result
        else:
            match = re.search(r"(?s:.*)(SELECT (.*?) FROM.*)", self.query)
            result = match.group(1)
            # print(f"_parse_query match: {result}")
            return result

    def _parse_window(self):
        match = re.search(r"WINDOW\[(.*)\]", self.query)
        if match:
            result = match.group(1)
            # print(f"_parse_window match: {result}")
            return result
        else:
            # print("No _parse_window")
            return None

    def parse_query(self):
        stream = self._parse_stream()
        rel = self._parse_relation()
        rec_schema = self._parse_recur_schema()
        exit_r = self._parse_exit()
        rec_r = self._parse_recur()
        q = self._parse_query()
        w = self._parse_window()
        # print(w.split(','))
        if not w:
            w1 = w2 = '0'
        elif len(w.split(',')) == 2:
            w1, w2 = w.split(',')
        elif len(w.split(',')) == 1:
            w1 = w2 = w
        else:
            w1 = w2 = '0'
        return (stream, rel, rec_schema, exit_r, rec_r, q, w1, w2)
        # return (stream_name, relation_name, recursive_name, union_select, select_query, window)


