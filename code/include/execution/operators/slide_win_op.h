#ifndef _STREAMLOG_PROTOTYPE_SLIDE_WIN_OP_
#define _STREAMLOG_PROTOTYPE_SLIDE_WIN_OP_

#ifndef _OPERATOR_
#include "execution/operators/operator.h"
#endif

#ifndef _QUEUE_
#include "execution/queues/queue.h"
#endif


namespace Execution {
	class SlidingWindowOp : public Operator {
	private:
		/// System-wide mId
		unsigned int id;

		/// System log
		std::ostream &LOG;
		
		/// Input queue
		Queue *inputQueue;
		
		/// Output queue
		Queue *outputQueue;


		/// Timestamp-related
        Timestamp       windowStart;

		TimeDuration    windowSize;

        TimeDuration    slideSize;

		/// Timestamp of the last element dequeued from input queue.  0 if
		/// no tuple has been dequeued.  Note: an element might correspond
		/// to a data tuple or a heartbeat.
		Timestamp lastInputTs;
		
		/// Timestamp of the last element enqueued in the output queue. 0
		/// if no such element has been enqueued.
		Timestamp lastOutputTs;
		
	public:		
		SlidingWindowOp(unsigned int id, std::ostream &LOG);
		virtual ~SlidingWindowOp ();
		
		int setInputQueue (Queue *inputQueue);
		int setOutputQueue (Queue *outputQueue);
		int setWindowSize (unsigned int windowSize);
        int setWindowSlide (unsigned int slideSize);
		
		int run();

	private:
        int slide();
	};
}

#endif // _SLIDE_WIN_OP_