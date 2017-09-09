/**
 * @class Proto01
 * @brief A Proto01 class.
 *
 * [10.1002/nme.1620381102](http://onlinelibrary.wiley.com/doi/10.1002/nme.1620381102/full)
 *
 * @author T
 * @date 30/07/2017
 * @version 0.1.0
 * @file Proto01.h
 * @addtogroup Membrane
 * @ingroup Element
 * @{
 */

#ifndef Proto01_H
#define Proto01_H

#include <Element/Element.h>

class Proto01 : public Element {
    struct IntegrationPoint {
        vec coor;
        double weight, jacob_det;
        mat jacob, pn_pxy;
        mat P, A, B, BI;
        unique_ptr<Material> m_material;
    };

    static const unsigned m_node;
    static const unsigned m_dof;

    static mat mapping;

    const double thickness;

    vector<unique_ptr<IntegrationPoint>> int_pt;

    mat ele_coor;

    mat HI, HIL, HILI; // constant matrices

    vec FI;         // variadic vectors
    mat HT, QT, TT; // variadic matrices

    vec trial_disp;   // displacement
    vec trial_lambda; // enhanced strain
    vec trial_alpha;  // strain
    vec trial_beta;   // stress

    vec current_disp;
    vec current_lambda;
    vec current_alpha;
    vec current_beta;

    mat initial_qtitt;
    mat trial_qtitt; // eq. 65
    mat trial_qtifi; // eq. 65
    mat current_qtitt;
    mat current_qtifi;

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
