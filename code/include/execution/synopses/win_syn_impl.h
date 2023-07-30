#ifndef _STREAMLOG_PROTOTYPE_WIN_SYN_IMPL_
#define _STREAMLOG_PROTOTYPE_WIN_SYN_IMPL_

#ifndef _WIN_SYN_
#include "execution/synopses/win_syn.h"
#endif

#include <iostream>
#include <fstream>

namespace Execution {
	class WindowSynopsisImpl : public WindowSynopsis {
	private:
		unsigned int           id;

		/// System log
		std::ostream          &LOG;
		
        std::vector<Tuple> *store;
	public:		
		WindowSynopsisImpl (unsigned int id, std::ostream& LOG);
		~WindowSynopsisImpl();
		

		int insertTuple (Tuple tuple, Timestamp timestamp);
		bool isEmpty() const;
		int getOldestTuple (Tuple& tuple, Timestamp& timestamp);
		int deleteOldestTuple();
        int scan (unsigned int    scanId);
	};
}

#endif