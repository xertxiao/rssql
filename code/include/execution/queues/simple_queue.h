#ifndef _STREAMLOG_PROTOTYPE_SIMPLE_QUEUE_
#define _STREAMLOG_PROTOTYPE_SIMPLE_QUEUE_

#include <ostream>

#ifndef _QUEUE_
#include "execution/queues/queue.h"
#endif

/**
 * A simple implementation of a queue. 
 */

namespace Execution {
	class SimpleQueue : public Queue {
	private:
		// System wide unique mId assigned to me.
		int id;
		
		// Maximum number of elements that I can enqueue
		unsigned int maxElements;
		
		// The number of elements that we currently have on the queue.
		unsigned int numElements;
		
		std::ostream& LOG;
		
	public:
		
		SimpleQueue (int id, std::ostream& LOG);
		virtual ~SimpleQueue();
		
		// Initialization routines.
		int initialize();
		
		// Inherited from Queue.
		bool enqueue (Element element);
		bool dequeue (Element &element);
		bool peek (Element &element) const;
		bool isFull () const;
		bool isEmpty () const;
	};
}
#endif