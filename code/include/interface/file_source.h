#ifndef _STREAMLOG_PROTOTYPE_FILE_SOURCE_
#define _STREAMLOG_PROTOTYPE_FILE_SOURCE_

#include "interface/table_source.h"

#include "common/constants.h"
#include "execution/internals/element.h"

#include <fstream>

using Interface::TableSource;

namespace Interface {
	
	class FileSource : public TableSource {
	private:
		/// Source file input
		std::ifstream input;
        unsigned int windowRight;
        unsigned int windowSlide;
        std::streampos oldPos;
		
	public:
        FileSource(std::string fileName,unsigned int windowLen, unsigned int windowSlide)
                : windowRight(windowLen), windowSlide(windowSlide), input(fileName) {}

        ~FileSource () = default;
		
		int start ();
		int getNextBatch (std::vector<Execution::Element> &elementVec);
        int end ();
		
	private:
		static int parseTuple (const std::string& lineBuffer, Execution::Element &element);
        //int computeOffsets ();
	};
}

#endif
