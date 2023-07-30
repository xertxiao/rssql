#ifndef _STREAMLOG_PROTOTYPE_TUPLE_ITER_
#define _STREAMLOG_PROTOTYPE_TUPLE_ITER_

#ifndef _TUPLE_
#include "execution/internals/tuple.h"
#endif

namespace Execution {
	class TupleIterator {
	public:
		virtual ~TupleIterator () {}
		
		/**
		 * Get the next tuple in this iteration
		 *
		 * @param   tuple   (output) next tuple
		 * @return           true if next tuple present,
		 *                   false if we reached the end.
		 */ 
		virtual bool getNext (Tuple& tuple) = 0;
	};
}

#endif
