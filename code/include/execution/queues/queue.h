#ifndef _STREAMLOG_PROTOTYPE_QUEUE_
#define _STREAMLOG_PROTOTYPE_QUEUE_



#include "execution/internals/element.h"

namespace Execution {


	class Queue {
        public:
            virtual ~Queue () {}
            
            /// Enqueue a new element into the queue.
            virtual bool enqueue (Element element) = 0;
            
            /// Dequeue the oldest element from the queue.
            virtual bool dequeue (Element &element) = 0;
            
            /// Peek the oldest element without dequeueing.
            virtual bool peek (Element &element) const = 0;
            
            // return true if the queue is full, false otherwise.  
            virtual bool isFull () const = 0;
            
            // return true if the queue is empty, false otherwise.
            virtual bool isEmpty () const = 0;
	};
}

#endif