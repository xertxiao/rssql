#ifndef _STREAMLOG_PROTOTYPE_TUPLE_LAYOUT_
#define _STREAMLOG_PROTOTYPE_TUPLE_LAYOUT_

#ifndef _PHY_OP_
#include "metadata/phy_op.h"
#endif

namespace Metadata {


	
	class TupleLayout {
	private:
		

		static unsigned int TUPLE_ALIGN;
		

        std::vector<unsigned int> cols;


		unsigned int numAttrs;
		

		unsigned int numBytes;
		
	public:
		

		TupleLayout ();		
		
	
	
		TupleLayout (Physical::Operator *op);

		~TupleLayout();
		

		int addAttr (Type type, unsigned int len, unsigned int &col);
		

		int addFixedLenAttr (Type type, unsigned int &col);
		

		int addCharPtrAttr (unsigned int &col);
		
	
		int addTimestampAttr (unsigned int &col);
		

		unsigned int getTupleLen() const;
		
	
		unsigned int getColumn (unsigned int pos) const;
		

		static int getOpColumn (Physical::Operator *op, unsigned int pos,
								unsigned int &col);

		static int getOpTupleLen (Physical::Operator *op,
								  unsigned int &len);								  
		
	private:		
		static unsigned int getTupleAlign ();
	};
	
	class ConstTupleLayout {
	private:

		unsigned int numAttrs;
		
		std::vector<float> fVals;

		TupleLayout *tupleLayout;
		
	public:
		ConstTupleLayout ();
		~ConstTupleLayout ();				
		
		int addFloat (float fval, unsigned int &col);

	};
}

#endif
