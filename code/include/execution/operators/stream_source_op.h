#ifndef STREAMLOG_PROTOTYPE_STREAM_SOURCE_OP_H
#define STREAMLOG_PROTOTYPE_STREAM_SOURCE_OP_H

#include <vector>
#include "execution/operators/operator.h"
#include "common/types.h"
#include "execution/queues/queue.h"
#include "interface/table_source.h"
#include "interface/file_source.h"

namespace Execution {
	class StreamSourceOp : public Operator {
	private:
		unsigned int   mId;


		// Attribute specification: these may not be in the input tuple
		// format, so we do not use the usual AEval for copying ... [[
		// explanation ]]
		struct Attr {
			Type type;
			unsigned int len;
		};

		// Specification of the attributes in the input stream
        std::vector<Attr> attrs;
		unsigned int numAttrs;

		static const unsigned int TIMESTAMP_OFFSET = 0;
		static const unsigned int DATA_OFFSET = TIMESTAMP_SIZE;

		// Source who is feeding us the tuples
		Interface::FileSource *source;

		Timestamp lastInputTs;
		Timestamp lastOutputTs;


		std::ostream& LOG;

        //TODO: Temp, use vector fot data for debugging purpose
        std::vector<Execution::Element> *data;
        size_t dataRead = 0;
        size_t prevDatRead = 0;

    public:
		StreamSourceOp (unsigned int id, std::ostream& LOG);
		virtual ~StreamSourceOp ();


		int addAttr (Type type, unsigned int len, Column outCol);

		int initialize ();

		int run ();


        // Temp for data

        void setData(std::vector<Element> *data);
        void setSource(Interface::FileSource *newSrc);


    };
}


#endif //STREAMLOG_PROTOTYPE_STREAM_SOURCE_OP_H
