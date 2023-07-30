#ifndef STREAMLOG_PROTOTYPE_ATTRIBUTE_H
#define STREAMLOG_PROTOTYPE_ATTRIBUTE_H

#include <string>
#include "common/types.h"

namespace Metadata
{

    class Attribute
    {
    private:
        std::string attrName;
        Type attrType;

    public:
        Attribute();
        Attribute(std::string name, Type type) : attrName(name), attrType(type) {}
        ~Attribute();
        Type getAttrType() { return attrType; }
        std::string getAttrName() { return attrName; }
        void setAttrType(Type type) { attrType = type; }
        void setAttrName(std::string name) { attrName = name; }
    };
}

#endif //STREAMLOG_PROTOTYPE_ATTRIBUTE_H
