/**
* @class Material
* @brief A Material abstract base class.
* @author T
* @date 27/07/2017
* @version 0.1.0
* @file Material.h
* @addtogroup Material
* @{
*/

#ifndef MATERIAL_H
#define MATERIAL_H

#include <Domain/Tag.h>

class Material : public Tag
{
protected:
    double density = 0.; /**< density */

    vec current_strain;      /**< current status */
    vec current_strain_rate; /**< current status */
    vec current_stress;      /**< current status */
    vec current_history;     /**< current status */

    vec trial_strain;      /**< trial status */
    vec trial_strain_rate; /**< trial status */
    vec trial_stress;      /**< trial status */
    vec trial_history;     /**< trial status */

    vec incre_strain;      /**< incremental status */
    vec incre_stress;      /**< incremental status */
    vec incre_strain_rate; /**< incremental status */

    mat initial_stiffness; /**< stiffness matrix */
    mat current_stiffness; /**< stiffness matrix */
    mat trial_stiffness;   /**< stiffness matrix */
public:
    explicit Material(const unsigned& = 0, const unsigned& = CT_MATERIAL);

    virtual ~Material();

    virtual void initialize() = 0;

    virtual double get_parameter(const unsigned& = 0) const;

    virtual const vec& get_strain() const;
    virtual const vec& get_strain_rate() const;
    virtual const vec& get_stress() const;
    virtual const mat& get_stiffness() const;
    virtual const mat& get_initial_stiffness() const;

    virtual unique_ptr<Material> get_copy() = 0;

    virtual int update_incre_status(const vec&);
    virtual int update_incre_status(const vec&, const vec&);
    virtual int update_trial_status(const vec&);
    virtual int update_trial_status(const vec&, const vec&);

    virtual int clear_status() = 0;
    virtual int commit_status() = 0;
    virtual int reset_status() = 0;
};

#endif

//! @}
