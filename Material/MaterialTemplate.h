/**
* @class MaterialTemplate
* @brief A MaterialTemplate class.
*
* The Material class defines material models for FEM analysis.
*
* The design pattern of this class is slightly different than the one of the Element
* class. The main one is that, the Material model needs to be copied for Element local use
* while the Element object does not need to be duplicated.
*
* As suanPan uses smart pointers for dynamic memory management, the copy of a Material
* prototype is defined as a unique pointer so that at Element level, there is no need to
* handle the lifetime of the Material object. Although the `new` way is still possible,
* there is a danger of memory leak like dangling pointers.
*
* The Material class itself defines a couple of member variables. Currently, strain,
* stress, strain rate, general purpose history variables and stiffness quantities for both
* current status and trial status are supported.
*
* Correspondingly, there are getters to return the trial status of the Material model.
* They are
* ```cpp
* virtual const vec& getStrain() const;
* virtual const vec& getStrainRate() const;
* virtual const vec& getStress() const;
* virtual const mat& getStiffness() const;
* virtual const mat& getInitialStiffness() const;
* ```
* There is no need to override those methods if one decides to use pre-defined status
* variables. As defined as virtual functions, the users are free to ignore all pre-defined
* variables and define their own status variables. In that case, the above methods should
* be overridden.
*
* Similar to other classes, there is a initialize method to allow the user to initialize
* the object without creating a new object.
* ```cpp
* virtual void initialize() = 0;
* ```
* This should be overridden in derived class to initialize the status variables. If your
* Material model does not need to re-initialize, it is okay to place all initializations
* inside of the constructors.
*
* The method that is in charge of returning a copy is called
* ```cpp
* virtual unique_ptr<Material> getCopy() = 0;
* ```
* By calling the copy constructor, this method can be implemented in a very concise way.
* ```cpp
* unique_ptr<Material> getCopy() { return make_unique<MaterialName>(*this); };
* ```
* According to the rule of three or the rule of five, since here the method calls the
* default copy constructor of a derived class, if the derived class is so complicated that
* the default constructor cannot handle, it is necessary to define the copy/assignment
* (move) constructors and the destructor.
*
* The Material class defines four updating method.
* ```cpp
* virtual int updateIncreStatus(const vec&); // incre_strain
* virtual int updateIncreStatus(const vec&, const vec&); // incre_strain incre_strain_rate
* virtual int updateTrialStatus(const vec&); // trial_strain
* virtual int updateTrialStatus(const vec&, const vec&); // trial_strain trial_strain_rate
* ```
* As the Material model only talks to Element objects, the developers are free to
* implement any of them, even all four methods, providing that the corresponding Element
* calls the proper method.
*
* The other status control methods are also need to be implemented.
* ```cpp
* virtual int clearStatus() = 0;
* virtual int commitStatus() = 0;
* virtual int resetStatus() = 0;
* ```
*
* There is an additional method called
* ```cpp
* virtual double getParameter(const unsigned& = 0) const;
* ```
* This is universal method to return material properties based on the input argument, as
* some Element objects may need some material properties to compute particular data.
*
* @author T
* @date 19/08/2017
* @version 0.1.1
* @file MaterialTemplate.h
* @addtogroup Material
* @{
*/

#ifndef MaterialTemplate_H
#define MaterialTemplate_H

#include "Material.h"

class MaterialTemplate : public Material
{
public:
    explicit MaterialTemplate(const unsigned& = 0);

    void initialize() override;

    double getParameter(const unsigned& = 0) const override;

    unique_ptr<Material> getCopy() override;

    int updateTrialStatus(const vec&) override;

    int clearStatus() override;
    int commitStatus() override;
    int resetStatus() override;
};

#endif

//! @}
