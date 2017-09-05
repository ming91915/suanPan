/**
* @class CLoad
* @brief A CLoad class.
*
* The CLoad class is in charge of handling concentrated load.
*
* @author T
* @date 23/07/2017
* @file CLoad.h
*/

#ifndef CLOAD_H
#define CLOAD_H

#include "Load.h"

class CLoad : public Load {
    double pattern = 0.;

    uvec nodes;
    uvec dofs;

public:
    explicit CLoad(const unsigned& = 0, const unsigned& = 0, const double& = 0., const uvec& = {}, const unsigned& = 0, const shared_ptr<Amplitude>& = nullptr);
    CLoad(const unsigned&, const double&, const uvec&, const unsigned&, const shared_ptr<Amplitude>& = nullptr);
    CLoad(const unsigned&, const unsigned&, const double&, const uvec&, const uvec&, const shared_ptr<Amplitude>& = nullptr);
    CLoad(const unsigned&, const double&, const uvec&, const uvec&, const shared_ptr<Amplitude>& = nullptr);
    ~CLoad();

    int process(const shared_ptr<Domain>&) override;
};

#endif
