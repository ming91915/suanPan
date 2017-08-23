/**
 * @class Tie
 * @brief A Tie class.
 *
 * The Tie class.
 *
 * @author T
 * @date 29/07/2017
 * @version 0.1.0
 * @file Tie.h
 */

#ifndef TIE_H
#define TIE_H

#include "MPC.h"

class Tie final : public MPC
{
    unsigned node_i;
    unsigned dof_i;
    unsigned node_j;
    unsigned dof_j;

public:
    Tie(const unsigned& T,   // tag
        const unsigned& S,   // step tag
        const unsigned& NA,  // node a
        const unsigned& DA,  // dof a
        const unsigned& NB,  // node b
        const unsigned& DB); // dof b
    Tie(const unsigned& S,
        const unsigned& NA,
        const unsigned& DA,
        const unsigned& NB,
        const unsigned& DB);

    int process(const shared_ptr<Domain>&) override;
};

#endif
