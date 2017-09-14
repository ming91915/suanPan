/**
 * @class Proto01
 * @brief A Proto01 class.
 *
 * [10.1002/nme.1620381102](http://onlinelibrary.wiley.com/doi/10.1002/nme.1620381102/full)
 *
 * @author T
 * @date 13/09/2017
 * @version 0.2.0
 * @file Proto01.h
 * @addtogroup Membrane
 * @ingroup Element
 * @{
 */

#ifndef PROTO01_H
#define PROTO01_H

#include <Element/Element.h>

class Proto01 : public Element {
    struct IntegrationPoint {
        vec coor;
        double weight, jacob_det;
        mat P, A, B, BI;
        unique_ptr<Material> m_material;
    };

    static const unsigned m_node, m_dof;

    static mat mapping;

    const double thickness;

    vector<unique_ptr<IntegrationPoint>> int_pt;

    mat HI, HIL, HILI; // constant matrices

    mat trial_ht, current_ht;

    vec trial_disp, current_disp;     // displacement
    vec trial_lambda, current_lambda; // enhanced strain
    vec trial_alpha, current_alpha;   // strain
    vec trial_beta, current_beta;     // stress

    mat initial_qtitt;
    mat trial_qtitt, current_qtitt; // eq. 65
    mat trial_qtifi, current_qtifi; // eq. 65

public:
    Proto01(const unsigned&, const uvec&, const unsigned&, const double& = 1.);

    void initialize(const shared_ptr<Domain>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;

    vector<vec> record(const OutputList&) override;

    void print() override;
};

#endif

//! @}
