/*******************************************************************************
 * Copyright (C) 2017 Theodore Chang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
/**
 * @class Material
 * @brief A Material abstract base class.
 * @author T
 * @date 29/11/2017
 * @version 0.1.1
 * @file Material.h
 * @addtogroup Material
 * @{
 */

#ifndef MATERIAL_H
#define MATERIAL_H

#include <Domain/Tag.h>
#include <Section/ParameterType.h>

enum class MaterialType : unsigned { D0 = 0, D1 = 1, D2 = 3, D3 = 6 };
enum class PlaneType { S, E, N };

class DomainBase;
enum class OutputType;

using std::vector;

class Material : public Tag {
protected:
    double density = 0.; /**< density */

    vec current_strain;      /**< current status */
    vec current_strain_rate; /**< current status */
    vec current_stress;      /**< current status */
    vec current_stress_rate; /**< current status */
    vec current_history;     /**< current status */

    vec trial_strain;      /**< trial status */
    vec trial_strain_rate; /**< trial status */
    vec trial_stress;      /**< trial status */
    vec trial_stress_rate; /**< trial status */
    vec trial_history;     /**< trial status */

    vec incre_strain;      /**< incremental status */
    vec incre_strain_rate; /**< incremental status */
    vec incre_stress;      /**< incremental status */
    vec incre_stress_rate; /**< incremental status */

    mat initial_stiffness; /**< stiffness matrix */
    mat current_stiffness; /**< stiffness matrix */
    mat trial_stiffness;   /**< stiffness matrix */
public:
    const bool initialized = false;

    const MaterialType material_type;

    explicit Material(const unsigned = 0,      // tag
        const unsigned = CT_MATERIAL,          // class tag
        const MaterialType = MaterialType::D1, // material type
        const double = 0.                      // density
    );

    virtual ~Material();

    virtual void initialize(const shared_ptr<DomainBase>& = nullptr) = 0;

    virtual double get_parameter(const ParameterType& = ParameterType::DENSITY) const;

    virtual const vec& get_strain() const;
    virtual const vec& get_strain_rate() const;
    virtual const vec& get_stress() const;
    virtual const mat& get_stiffness() const;
    virtual const mat& get_initial_stiffness() const;

    virtual unique_ptr<Material> get_copy() = 0;

    int update_incre_status(const double);
    int update_incre_status(const double, const double);
    int update_trial_status(const double);
    int update_trial_status(const double, const double);

    virtual int update_incre_status(const vec&);
    virtual int update_incre_status(const vec&, const vec&);
    virtual int update_trial_status(const vec&);
    virtual int update_trial_status(const vec&, const vec&);

    virtual int clear_status() = 0;
    virtual int commit_status() = 0;
    virtual int reset_status() = 0;

    virtual vector<vec> record(const OutputType&);
};

namespace suanpan {
unique_ptr<Material> make_copy(const shared_ptr<Material>&);
unique_ptr<Material> make_copy(const unique_ptr<Material>&);
}

#endif

//! @}
