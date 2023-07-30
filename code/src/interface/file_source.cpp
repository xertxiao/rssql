#include "interface/file_source.h"
#include <sstream>
#include <iostream>


namespace Interface{

    int Interface::FileSource::start() {
        if (!input.is_open () || input.bad ()) return -1;
        return 0;
    }

    int Interface::FileSource::end() {
        return 0;
    }

    int Interface::FileSource::parseTuple(const std::string& lineBuffer, Execution::Element &element) {
        std::istringstream iss(lineBuffer);
        std::vector<float> tempTuple;
        float tempVal;
        while (iss >> tempVal) tempTuple.push_back(tempVal);
        element.timestamp = static_cast<unsigned int>(tempTuple.back());
        element.dpt = static_cast<unsigned int>(tempTuple.back());
        element.kind = Execution::E_PLUS;
        element.tuple = {tempTuple.begin(), tempTuple.end() - 1};
        return 0;
    }

    int FileSource::getNextBatch(std::vector<Execution::Element> &elementVec) {
        std::string lb;
        Execution::Element tempE;
        while (std::getline(input, lb)) {
            parseTuple(lb, tempE);
            if (tempE.timestamp > windowRight) {
                input.seekg(oldPos);
                this->windowRight += windowSlide;
                break;
            } else {
                elementVec.push_back(tempE);
            }
            oldPos = input.tellg();
        }

        if (input.eof()) {
            return 1;
        }
        return 0;
    }

}
