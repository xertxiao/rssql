#ifndef STREAMLOG_PROTOTYPE_ELEMENT_H
#define STREAMLOG_PROTOTYPE_ELEMENT_H

#include <ostream>
#include "tuple.h"
#include "common/types.h"

namespace Execution {
    enum ElementKind {
        E_PLUS, E_MINUS, E_HEARTBEAT
    };

    struct Element {
        ElementKind  kind;
        // TODO: will making it a pointer speed up?
        Tuple        tuple;
        Timestamp    timestamp;
        Dpt          dpt;

//        Element(Element const &newE) :  kind (E_PLUS), tuple (newE.tuple),
//                timestamp (newE.timestamp), dpt(newE.dpt)  {}

        Element () : kind (E_PLUS), tuple (0), timestamp (0), dpt(0) {}

        
        Element (unsigned long _size, Timestamp _ts, Dpt _dpt)
                : kind (E_PLUS), tuple (_size), timestamp (_ts), dpt(_dpt) {}


        Element (ElementKind _k, Tuple _t, Timestamp _ts, Dpt _dpt)
                : kind (_k), tuple (_t), timestamp (_ts), dpt(_dpt) {}

//        static Element Heartbeat (Timestamp _tstamp) {
//            Element e;
//
//            e.kind = E_HEARTBEAT;
//            e.timestamp = _tstamp;
//
//            return e;
//        }

        bool operator==(const Element &rhs) const {
            return tuple == rhs.tuple;
        }

        bool operator!=(const Element &rhs) const {
            return !(rhs == *this);
        }

        bool operator<(const Element &rhs) const {
            return tuple < rhs.tuple;
        }

        bool operator>(const Element &rhs) const {
            return rhs < *this;
        }

        bool operator<=(const Element &rhs) const {
            return !(rhs < *this);
        }

        bool operator>=(const Element &rhs) const {
            return !(*this < rhs);
        }




        friend std::ostream &operator<<(std::ostream &os, const Element &element) {
             os << "kind: " << element.kind << " tuple: " << element.tuple[0] << ", " << element.tuple[1] << " timestamp: " << element.timestamp;
//            os << "kind: " << element.kind << " tuple: " << element.tuple[0] << ", " << element.tuple[1] << ", " << element.tuple[2] << " timestamp: " << element.timestamp;
            os << " dpt: "<<element.dpt;

            return os;
        }
    };
}
#endif //STREAMLOG_PROTOTYPE_ELEMENT_H
