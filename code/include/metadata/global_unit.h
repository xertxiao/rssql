#ifndef STREAMLOG_PROTOTYPE_GLOBAL_UNIT_H
#define STREAMLOG_PROTOTYPE_GLOBAL_UNIT_H

#include <vector>
#include <unordered_map>
#include "execution/internals/element.h"
#include "execution/qbi/qbi.h"

namespace Execution {

    // A vector of Element as WorkSpace
    typedef std::vector<Execution::Element> WorkSpace;




    class GlobalUnit {
        // For Workspace, or Queue
        typedef std::vector<WorkSpace *> WorkSpaceCollection;
        typedef std::unordered_map<unsigned int, unsigned int> WorkSpaceMap;


        // For Qbi of non-recur
        typedef std::vector<Qbi *> NonRecurQbiCollection;
        typedef std::unordered_map<unsigned int, unsigned int> NonRecurQbiMap;

        // For Qbi of recur, currently only one
        typedef Qbi * RecurQbi;

    private:
        // For Workspace, or Queue
        WorkSpaceCollection mWSCol;
        WorkSpaceMap mWSMap;

        // For Qbi of non-recur
        NonRecurQbiCollection mNRQCol;
        NonRecurQbiMap  mNRQMap;

        // For Qbi of recur
        RecurQbi mRQ;




    public:
        unsigned int CurOpId;

        GlobalUnit() {
            mRQ = new Qbi();
        };
        ~GlobalUnit() {
            for (auto p : mWSCol) delete p;
            for (auto p : mNRQCol) delete p;
            delete mRQ;
        };

        void setWindowsAll(unsigned long wl, unsigned long wr, Slide slide) {
            for (auto q : mNRQCol) {
                q->setWindowLeft(wl);
                q->setWindowRight(wr);
                q->setSlide(slide);
            }

            mRQ->setWindowLeft(wl);
            mRQ->setWindowRight(wr);
            mRQ->setSlide(slide);
        }

        WorkSpace * registerWorkSpace(unsigned int &newWSId);
        Qbi * registerNRQbi(unsigned int &newQbiId);

        WorkSpace * connectWorkSpace(unsigned int opId);
        Qbi * connectNRQbi(unsigned int opId);
        Qbi * connectRQbi();

        void advanceAll();

        void clearAllIdb();

        unsigned long long getAllSize(){
            unsigned long long temp = 0;
            for (const auto& qbi : mNRQCol) temp += qbi->getAllSize();
            temp += mRQ->getAllSize();
            return temp;
        }
    };
}

#endif //STREAMLOG_PROTOTYPE_GLOBAL_UNIT_H
