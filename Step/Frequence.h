/**
 * @class Frequence
 * @brief A Frequence class.
 * @author T
 * @date 03/07/2017
 * @file Frequence.h
 * @addtogroup Step
 * @{
 */

#ifndef FREQUENCE_H
#define FREQUENCE_H

#include "Step.h"

class Frequence : public Step
{
    unsigned eigen_number;

public:
    explicit Frequence(const unsigned& = 0, const unsigned& = 4);

    int initialize() override;

    int analyze() override;

    void set_eigen_number(const unsigned&);
    const unsigned& get_eigen_number() const;
};

#endif

//! @}
