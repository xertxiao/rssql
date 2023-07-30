#ifndef STREAMLOG_PROTOTYPE_TYPES_H
#define STREAMLOG_PROTOTYPE_TYPES_H

typedef unsigned int Timestamp;
typedef unsigned int TimeDuration;
typedef unsigned int Hash;

/// Derivation Time Path
typedef unsigned int Dpt;



/*
For now, we only consider int and float type attributes. Since we can now perform serialization more easily
**/

enum Type {
    INT,                // 4 byte integer
    FLOAT,              // 4 byte floating point
};

// Lengths of the data types
static const unsigned int INT_SIZE   = sizeof(int);
static const unsigned int FLOAT_SIZE = sizeof(float);
static const unsigned int TIMESTAMP_SIZE = sizeof (Timestamp);


#endif //STREAMLOG_PROTOTYPE_TYPES_H
