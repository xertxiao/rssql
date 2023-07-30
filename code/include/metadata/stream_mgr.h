#ifndef STREAMLOG_PROTOTYPE_STREAM_MGR_H
#define STREAMLOG_PROTOTYPE_STREAM_MGR_H

#include "schema.h"

namespace Metadata {
    class StreamManager {
    private:
        std::vector<Schema> schemaLists;
    public:
        StreamManager();
        ~StreamManager();


        int registerStream(Schema stream, size_t &retId);



        bool getStreamId(std::string streamName, size_t &retId) const;


        size_t getNumAttrs(size_t streamId) const;


        bool getAttrId(size_t streamId, std::string attrName, size_t& attrId) const;


        Type getAttrType(size_t streamId, size_t attrId) const;


        void printState();

    };
}

#endif //STREAMLOG_PROTOTYPE_STREAM_MGR_H
