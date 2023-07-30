#ifndef _STREAMLOG_PROTOTYPE_TUPLE_
#define _STREAMLOG_PROTOTYPE_TUPLE_

#include<vector>
namespace Execution {
	

	typedef unsigned int Column;

    typedef unsigned int Row;

    typedef std::vector<float> Tuple;

}

// Routines to access columns within tuples

//#define ICOL(t,c)   ( ( (int *)   (t) ) [c] )
//#define FCOL(t,c)   ( ( (float *) (t) ) [c] )
//#define BCOL(t,c)   ( (t) [c] )
//#define CCOL(t,c)   (t + c)

#endif
