#include "metadata/global_unit.h"



Execution::WorkSpace *Execution::GlobalUnit::registerWorkSpace(unsigned int &newWSId) {
    auto * newWS = new WorkSpace();
    unsigned int idx = mWSCol.size();
    mWSCol.push_back(newWS);
    newWSId = idx;
    return newWS;
}

Execution::Qbi *Execution::GlobalUnit::registerNRQbi(unsigned int &newQbiId) {
    auto * newQbi = new Qbi();
    unsigned int idx = mNRQCol.size();
    mNRQCol.push_back(newQbi);
    newQbiId = idx;
    return newQbi;
}

Execution::WorkSpace *Execution::GlobalUnit::connectWorkSpace(unsigned int id) {
    return mWSCol[id];
}

Execution::Qbi *Execution::GlobalUnit::connectNRQbi(unsigned int id) {
    return mNRQCol[id];
}

Execution::Qbi *Execution::GlobalUnit::connectRQbi() {
    return mRQ;
}

void Execution::GlobalUnit::advanceAll() {
    for (auto qbi  : this->mNRQCol) qbi->advanceWindow();
    this->mRQ->advanceWindow();
}

void Execution::GlobalUnit::clearAllIdb() {
    for (auto qbi  : this->mNRQCol) qbi->clearAll();
    this->mRQ->clearAll();
}




