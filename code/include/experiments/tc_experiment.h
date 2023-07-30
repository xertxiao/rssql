
#ifndef STREAMLOG_PROTOTYPE_TC_EXPERIMENT_H
#define STREAMLOG_PROTOTYPE_TC_EXPERIMENT_H

#include "experiment.h"
#include "metadata/stream_mgr.h"
#include "metadata/phy_op.h"
#include "execution/qbi/qbi.h"
#include "interface/file_source.h"

namespace Experiment{

    class TCExperiment : public Experiment{

    public:
        unsigned long long *totalDur;
        std::vector<unsigned long long> *latencies;
        std::vector<unsigned long long> *memConsumption;


    private:
//        size_t arcId;
//        Metadata::StreamManager *strmMgr;
//        Physical::Operator *rootOps;
        std::vector<Execution::Operator> *instOps;
        std::vector<Execution::Operator *> ops;

        Execution::WorkSpace *emptyWS;
        Execution::Qbi *emptyQbi;

        std::string fileSourceName = "./dataset/twitter_small_ts.data";
        Interface::FileSource *fileSource;



        unsigned int WINDOW_LEN = 0;
        unsigned int WINDOW_SLIDE = 0;


        /// Temp for only debuging
//        std::vector<Execution::Element> sampleData = {
//                Execution::Element(Execution::E_PLUS, Execution::Tuple {1.0, 2.0}, 0, 0),
//                Execution::Element(Execution::E_PLUS, Execution::Tuple {2.0, 3.0}, 1, 1),
//                Execution::Element(Execution::E_PLUS, Execution::Tuple {3.0, 4.0}, 2, 2),
//                Execution::Element(Execution::E_PLUS, Execution::Tuple {4.0, 5.0}, 3, 3),
//                Execution::Element(Execution::E_PLUS, Execution::Tuple {6.0, 7.0}, 4, 4),
//        };
//


//        Workspace workspace;
//        std::vector<Execution::Element> newEdbBuf;

        Execution::Slide _tempSlide = 1;




        // TODO: temp for debugging
        unsigned long long timeTaken;

    public:
        TCExperiment(unsigned int w, unsigned int s, bool q);

        ~TCExperiment();
        int registerSchema();
        int registerPhysical();
        int runExperiment();
    };
}

#endif //STREAMLOG_PROTOTYPE_TC_EXPERIMENT_H
