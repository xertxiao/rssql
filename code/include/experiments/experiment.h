#ifndef STREAMLOG_PROTOTYPE_EXPERIMENT_H
#define STREAMLOG_PROTOTYPE_EXPERIMENT_H

#include "execution/internals/element.h"
#include "metadata/global_unit.h"

namespace Experiment{

//    typedef std::pair<unsigned int, std::vector<Execution::Element> *> Workspace;

    class Experiment{
    protected:
        Execution::GlobalUnit * mGU;

        bool monitor = false;
        bool useQBI = false;

    public:
        Experiment() {
            mGU = new Execution::GlobalUnit();
        }
        virtual ~Experiment() {
            delete mGU;
        }

        virtual int registerSchema()=0;
        virtual int registerPhysical()=0;
        virtual int runExperiment()=0;


    };

}

#endif //STREAMLOG_PROTOTYPE_EXPERIMENT_H
