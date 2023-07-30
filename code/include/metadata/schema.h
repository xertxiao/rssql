#ifndef STREAMLOG_PROTOTYPE_SCHEMA_H
#define STREAMLOG_PROTOTYPE_SCHEMA_H


#include <string>
#include <vector>

#include "attribute.h"

namespace Metadata
{

    class Schema
    {
    private:
        std::string schemaName;
        std::vector<Attribute> attributeLists;
    public:
        Schema();
        ~Schema();
        std::string getSchemaName();
        std::vector<Attribute> getAttributeLists();
        void addAttr(Attribute newAttr);

    };
}

#endif //STREAMLOG_PROTOTYPE_SCHEMA_H
