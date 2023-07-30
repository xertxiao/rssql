#include "execution/operators/proj_op.h"

namespace Execution {
    ProjectOp::ProjectOp (unsigned int id, std::ostream& LOG) : Operator(), mId(id) , LOG(LOG){
        this->mProjEval = new AEval();
        this->mEvalContext = new EvalContext();

        this->mProjEval->setEvalContext(this->mEvalContext);

    }
    ProjectOp::~ProjectOp (){
        delete this->mProjEval;
        delete this->mEvalContext;
    };


    int ProjectOp::run () {



        if (*(mCurrentId) == this->mId) {
            auto start = std::chrono::high_resolution_clock::now();
            if(this->DEBUG) std::cerr << "=====Proj-"<<this->mId<<"-Start===== " << std::endl;



            if (mClearOut) this->mOutWorkSpace->clear();

            std::vector<Element> tempOut;
            for (auto e : *(this->mInWorkSpace)) {
                Element outE {this->mProjEval->getInstrSize(), e.timestamp, e.dpt};
                mEvalContext->bind(&(e.tuple), PROJ_INPUT_ROLE);
                mEvalContext->bind(&(outE.tuple), PROJ_OUTPUT_ROLE);

                mProjEval->eval();

                tempOut.push_back(outE);
            }

            *(this->mOutWorkSpace) = tempOut;
            for (auto e : *(this->mOutWorkSpace)) if(this->DEBUG) std::cerr<<"Proj: "<<e<<std::endl;
            (*mCurrentId)++;

            this->condStoreComputedResult();
            if(this->DEBUG) std::cerr << "=====Proj-"<<this->mId<<"-End===== " << std::endl;
            // *(this->MicroSecDuration) += std::chrono::duration_cast<MicroSec>(std::chrono::high_resolution_clock::now() - start).count();
        }
        return 0;
    }

}