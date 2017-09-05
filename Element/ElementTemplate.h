/**
* @class ElementTemplate
* @brief The ElementTemplate class illustrates the basic formulation a typical Element class used in FEM analysis.
*
* ## What Is An Element?
* Element is the key component in FEM analysis. It connects individual nodes together.
*
* ## What Does An Element Do?
* A proper element should, as said above, connect several nodes (at least two) together and provide information about how the change of one node will affect the other nodes. In other words, an element should define the relationship between the connected nodes so that the individual nodes can form a system which will response to external excitation, load, etc.
*
* ## What Is An Element's Scope?
* The scope of an element should be restrained to the sub-domain defined by this element. The region outside of the element should not be handled by this element. It is rational.
*
* ## What Is The Input Of An Element?
* Since the element only handles the self-defined sub-domain, an element needs only the information of the connected nodes, no other data/information.
*
* ## What Is The Output Of An Element?
* As explained in the second problem, an element should react according to external load, which is contained in or stored as nodal data. Hence an element should output the response. The response may include various physical quantities, such as (for solid mechanics) stiffness/mass/damping matrix, resistance and temperature.
*
* ## Element Class In Detail
* With above information in mind, the Element class is designed to fulfill above tasks.
*
* ### Member Variables In Detail
* First, as an Element should have access to connected nodes to get information such as displacement, there should be a variable that holds all nodes. To reduce the size of the class, we use a standard vector of weak pointers that point to the real Node objects.
*
* ```cpp
* vector<weak_ptr<Node>> node_ptr;
* ```
*
* The response replies on the material model used in an element. Typically, there should be one material model for one integration point. As we do not know how will the derived element class organized its data, we do not directly create the container for material models in the base Element class. Instead we store the tags of different material models that may be used in this element.
*
* ```cpp
* uvec material_tag;
* ```
*
* As we separate creation and initialization of an element apart, the material tags can be defined during creation and later in initialization, we could get copies from the Domain object. Before further definitions of basic mechanical quantities, we shall first know the size of those quantities. Hence we shall store how many nodes the element connects and how many DoFs per node. This in fact should be held by Node class but the connected node may have more DoFs than needed.
*
* ```cpp
* unsigned num_node = 0;
* unsigned num_dof = 0;
* ```
*
* Correspondingly, the node tags and dof tags should be stored in case of further reuse.
*
* ```cpp
* uvec node_encoding;
* uvec dof_encoding;
* ```
*
* Now we now the size of vectors and matrices, we can create the containers for those quantities.
*
* ```cpp
* vec resistance;
* mat mass;
* mat damping;
* mat stiffness;
* mat initial_stiffness;
* ```
*
* Additionally, the Element class defines a non-linear geometry switch to enable/disable large deformation formulation. By such the linear and nonlinear version can be combined for some elements.
*
* ```cpp
* bool nlgeom = false;
* ```
*
* ## Member Variables Summary
* To sum up, the Element class has 12 member variables which should be (partially) defined/initialized. They are all defined as protected variables so that they can be directly accessed by the derived element class. Although this breaks the encapsulation of the Element class, we have to do some trade-off for some convenience. Other design patterns such as getters/setters of private variables or defining those variables in each derived class are too complicated as they will be frequently accessed by the all Element classes.
*
* ### Member Methods In Detail
* Member methods include getters of above member variables for outside usage.
*
* ```cpp
* const unsigned& get_dof_number() const;
* const unsigned& getNumberNode() const;
* const uvec& getEncodingDOF() const;
* const uvec& getEncodingNode() const;
*
* const vector<weak_ptr<Node>>& getNodePtr() const;
*
* virtual const vec& getResistance() const;
* virtual const mat& get_mass() const;
* virtual const mat& get_damping() const;
* virtual const mat& get_stiffness() const;
* virtual const mat& get_initial_stiffness() const;
* ```
*
* The later five methods are declared as virtual methods so that they could be overridden in derived classes if the developer does not want to use the built-in member variables.
*
* Now it comes to the interesting part, which should be carefully implemented by the derived classes. The easiest ones are three status control methods.
*
* ```cpp
* virtual int commit_status() = 0;
* virtual int clear_status() = 0;
* virtual int reset_status() = 0;
* ```
*
* Basically they are the wrappers of the corresponding methods in material models. So simply call the same method in attached material models.
*
* There are two more additional methods. The first one is to initialize the element before executing the analysis.
*
* ```cpp
* virtual void initialize(const shared_ptr<Domain>&) = 0;
* ```
*
* This method will be called by the model control flow, after constructing the element (creation), before incremental iteration (problem solving). So this method should 1) define all necessary parameters which may be used in analysis, 2) get copies of material models for self use, 3) get pointers of Node objects connected and 4) potentially some other operations.
*
* Another one is in charge of updating status.
*
* ```cpp
* virtual int update_status() = 0;
* ```
*
* This method get trial displacement via pointers of connected Node objects, and calculate the trial status for material model and then call the corresponding method in material model to update the status. Then get the response from material models and forms the stiffness and resistance for the trial status.
*
* ## Member Methods Summary
* To implement a derived Element class. The minimum requirement is to implement **only** five methods, three out of these five methods simply call the methods of the same name in meterial model. So in fact, **only two methods should be overridden**. They are
*
* ```cpp
* virtual void initialize(const shared_ptr<Domain>&) = 0;
* virtual int update_status() = 0;
* ```
*
* A vivid example will be exhibited in this template class.
*
* @author T
* @date 05/09/2017
* @version 0.1.2
* @file ElementTemplate.h
* @addtogroup Element
* @{
*/

#ifndef ELEMENTTEMPLATE_H
#define ELEMENTTEMPLATE_H

#include "Element.h"

class ElementTemplate : public Element {
    /**
     * @brief As a universal practice, we define two static constants to represent the number of nodes and the number of DoFs. This is not necessary but only for clearness.
     */
    static const unsigned m_node;
    static const unsigned m_dof;

    double thickness = 0.; /**< thickness */

    double area = 0.; /**< area */

    mat strain_mat;

    unique_ptr<Material> m_material; /**< store material model */
public:
    ElementTemplate(const unsigned&, const uvec&, const unsigned&, const double& = 1.);

    void initialize(const shared_ptr<Domain>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;
};

#endif

//! @}
