#ifndef _STREAMLOG_PROTOTYPE_TABLE_SOURCE_
#define _STREAMLOG_PROTOTYPE_TABLE_SOURCE_

#include <vector>
#include "execution/internals/element.h"

namespace Interface {


	
	class TableSource {
	public:	
		virtual ~TableSource() {}	
		

		virtual int start() = 0;
		

		virtual int end() = 0;
	};
}

#endif
