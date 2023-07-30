#include "execution/qbi/qbi.h"


unsigned long Execution::Qbi::getWindowLeft() const {
    return mWindowLeft;
}

void Execution::Qbi::setWindowLeft(unsigned long windowLeft) {
    Qbi::mWindowLeft = windowLeft;
}

unsigned long Execution::Qbi::getWindowRight() const {
    return mWindowRight;
}

void Execution::Qbi::setWindowRight(unsigned long windowRight) {
    Qbi::mWindowRight = windowRight;
}

unsigned long Execution::Qbi::getSlide() const {
    return mSlide;
}

void Execution::Qbi::setSlide(unsigned long slide) {
    Qbi::mSlide = slide;
}

//void Execution::Qbi::insertBySlideNoAggr(Slide key, Element val) {
//    auto it = mIndex.find(key);
//    if (it == mIndex.end()) {
//        auto map = std::unordered_map<std::string, std::vector<Element>>{{"", std::vector<Element>{val}}};
//        mIndex[key] = map;
//    } else {
//        mIndex[key][""].push_back(val);
//    }
//}

void Execution::Qbi::insertBySlide(Slide key, Element val, size_t aggrOffset) {
    auto it = mIndex.find(key);
    std::string hashKey = "";
    for (int i = 0; i < aggrOffset; i++) hashKey += std::to_string(val.tuple[i]);
    if (it == mIndex.end()) {
        Element* nE = new Element(val);
        auto map = std::unordered_map<std::string, Element *>{{hashKey, nE}};
        mIndex[key] = map;
    } else {
        auto hashIt = mIndex[key].find(hashKey);
        if (hashIt == mIndex[key].end()) {
            Element* nE = new Element(val);
            mIndex[key][hashKey] = nE;
        } else {
            *(mIndex[key][hashKey]) = val;
        }
    }
}

int Execution::Qbi::getAtomsByRByElement(Element e, size_t aggrOffset, std::vector<Element *> &outVec){
    std::string hashKey = "";
    for (int i = 0; i < aggrOffset; i++) hashKey += std::to_string(e.tuple[i]);
    for (auto kv : mIndex) {
        auto it = kv.second.find(hashKey);
        if (it != kv.second.end()) {
            outVec.push_back(kv.second[hashKey]);
        }
    }
    return 0;
}

int Execution::Qbi::getAtomsByVByTuple(Tuple t, std::vector<Element> &outVec){
    std::string hashKey = "";
    for (auto e : t) hashKey += std::to_string(e);
    for (auto kv : mIndex) {
        auto it = kv.second.find(hashKey);
        if (it != kv.second.end()) {
            outVec.push_back(*kv.second[hashKey]);
        }
    }
    return 0;
}

void Execution::Qbi::clearAll() {
    for (auto kv : mIndex) {
        auto temp = kv.second;
        for (auto kv2 : temp) {
            delete kv2.second;
        }
    }
    mIndex.clear();
}
