#ifndef _STREAMLOG_PROTOTYPE_WIN_SYN_
#define _STREAMLOG_PROTOTYPE_WIN_SYN_



#ifndef _TUPLE_
#include "execution/internals/tuple.h"
#endif

#ifndef _TYPES_
#include "common/types.h"
#endif

namespace Execution {
class WindowSynopsis {
	public:
		virtual ~WindowSynopsis() {}
		
		virtual int insertTuple (Tuple tuple, 
								 Timestamp timestamp) = 0;
		
		virtual bool isEmpty() const = 0;
		
        virtual int scan (unsigned int    scanId) = 0;

		virtual int getOldestTuple (Tuple& tuple, 
									Timestamp& timestamp) = 0;
		
		virtual int deleteOldestTuple () = 0;
	};
}

#endif // _WIN_SYN_