#ifndef _STREAMLOG_PROTOTYPE_PLAN_MGR_
#define _STREAMLOG_PROTOTYPE_PLAN_MGR_



#include <ostream>

#ifndef _LOGOP_
#include "querygen/logop.h"
#endif

#ifndef _OPERATOR_
#include "execution/operators/operator.h"
#endif

#ifndef _QUERY_OUTPUT_
#include "interface/query_output.h"
#endif

#ifndef _TABLE_SOURCE_
#include "interface/table_source.h"
#endif

#ifndef _TABLE_MGR_
#include "metadata/table_mgr.h"
#endif

#ifndef _SCHEDULER_
#include "execution/scheduler/scheduler.h"
#endif

namespace Metadata {

	class PlanManager {
	public:
		virtual ~PlanManager () {}
		

		virtual int addBaseTable (unsigned int tableId,
								  Interface::TableSource *source) = 0;		
		

		
		virtual int addLogicalQueryPlan(unsigned int queryId,
										Logical::Operator *logPlan,
										Interface::QueryOutput *output) = 0;


		virtual int getQuerySchema (unsigned int queryId,
									char *schemaBuf,
									unsigned int schemaBufLen) = 0;
		

		virtual int map(unsigned int queryId, unsigned int tableId) = 0;
		

		virtual int optimize_plan () = 0;


		virtual int add_aux_structures () = 0;
		

		
		virtual int getXMLPlan (char *planBuf, unsigned int planBufLen) = 0;
		

		
		virtual int instantiate () = 0;
		

		virtual int initScheduler (Execution::Scheduler *sched) = 0;
		

		static PlanManager *newPlanManager(TableManager *tableMgr,
										   std::ostream& LOG);
		
#ifdef _DM_

		virtual void printPlan() = 0;		
#endif

		/// debug
		virtual int printStat () = 0;

#ifdef _SYS_STR_
		
		
		virtual int addMonitorPlan (unsigned int monitorId,
									Logical::Operator *logPlan,
									Interface::QueryOutput *output,
									Execution::Scheduler *sched) = 0;
#endif
		
	};
}

#endif
