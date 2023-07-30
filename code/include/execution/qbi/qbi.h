#ifndef STREAMLOG_PROTOTYPE_QBI_H
#define STREAMLOG_PROTOTYPE_QBI_H

#include <unordered_map>
#include "execution/internals/element.h"

#include <iostream>
#include <cmath>

namespace Execution {
    // A mSlide just denote how long a window slides each time.
    typedef unsigned long Slide;

    class Qbi {
    private:

        // For Sliding Window
        unsigned long mWindowLeft;
        unsigned long mWindowRight;
        Slide mSlide;
        /// Access via slide
        std::unordered_map<Slide,
            std::unordered_map<std::string, Element *>> mIndex;



    public:
        Qbi() = default;
        ~Qbi() {
            for (auto kv : mIndex) {
                for (auto kv2 : kv.second){
                    delete kv2.second;
                }
            }
        }

        unsigned long getWindowLeft() const;

        void setWindowLeft(unsigned long windowLeft);

        unsigned long getWindowRight() const;

        void setWindowRight(unsigned long windowRight);

        Slide getSlide() const;

        void setSlide(Slide slide);

        void insertBySlideNoAggr(Slide slide, Element e);

        void insertBySlide(Slide key, Element val, size_t aggrOffset);




        int getAllAtoms(std::vector<Element> &outVec) {
            // TODO: This might be bottleneck?
            for(auto kv : mIndex) {
                for (auto kv2 : kv.second) {
                    outVec.push_back(*(kv2.second));
                }
            }
            return 0;
        }

        unsigned long long getAllSize(){
            unsigned long long res = 0;
            for(const auto& kv : mIndex) {
                for (const auto& kv2 : kv.second) {
                    res += sizeof(*kv2.second);
                }
            }
            return res;
        }

        void advanceWindow() {

            Slide toExpire = floor(mWindowLeft/mSlide);

            for (auto kv : mIndex[toExpire]) {
                delete kv.second;
            }
            mIndex.erase(toExpire);

            mWindowLeft += mSlide;
            mWindowRight += mSlide;
        }

        int getAtomsByRByElement(Element e, size_t aggrOffset, std::vector<Element *> &outVec);
        int getAtomsByVByTuple(Tuple t, std::vector<Element> &outVec);

        void clearAll();
    };
}
#endif //STREAMLOG_PROTOTYPE_QBI_H
