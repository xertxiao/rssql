#ifndef _STREAMLOG_PROTOTYPE_QUERY_OUTPUT_
#define _STREAMLOG_PROTOTYPE_QUERY_OUTPUT_

#ifndef _TYPES_
#include "common/types.h"
#endif

namespace Interface {


	class QueryOutput {
	public:
		virtual ~QueryOutput() {}
		

		virtual int setNumAttrs(unsigned int numAttrs) = 0;
		

		virtual int setAttrInfo(unsigned int attrPos, 
								Type         attrType,
								unsigned int attrLen) = 0;
		

		virtual int start() = 0;
	

		virtual int putNext(const char *tuple, unsigned int len) = 0;


		virtual int end() = 0;
	};
}

#endif
