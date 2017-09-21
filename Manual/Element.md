Intro to Design Pattern of Element
==================================

------------------------------------------------------------------------

Some Basic Background Knowledge
-------------------------------

### What Is Element?

Elements are the key components in FEM analysis. A valid element connects several individual nodes together. From the perspective of geometry, an element can be any geometric shapes, such as line, triangle, quadrilateral, polygon, tetrahedron, hexahedron, wedge, pyramid, prism, you name it.

### What Does An Element Do?

A proper element should, as said above, connect several nodes (at least two, like a truss element as you may know) together and provide information about how the change of one node will affect the other one(s). In other words, an element should define the relationship between the connected nodes so that the individual nodes can form a self-sealed system which will response to external excitation, load, etc.

### What Is An Element's Scope?

The scope of an conventional element should be restricted to the region defined by this element, may contain edges, faces, volumes, etc. However, a valid element does not have to possess any geometric attributes, as long as the relationship betweens nodes is provided. The region outside of the element should not be handled by this element.

### What Is The Input Of An Element?

Since elements are independent from each other, an element only handles with the information of the connected nodes, no other data/information is needed.

### What Is The Output Of An Element?

As explained in the second problem, an element should react according to external load, which is contained in or stored as nodal data. The response may include various physical quantities, such as (for solid mechanics) stiffness/mass/damping matrix, resistance and temperature.

------------------------------------------------------------------------

Element Class In Detail
-----------------------

The **Element** class is designed to fulfill above tasks.

### Member Variables In Detail

First, as an element should have access to connected nodes to get information such as displacement, there should be a variable that holds all nodes. As the number of nodes varies from one element type to another, a standard vector is used. Besides, the element does not own the nodes and the connected nodes may be shared with other elements, we shall use a pointer that points to the real node object in the database.

``` cpp
vector<weak_ptr<Node>> node_ptr;
```

The element response relies on the material model used in an element. Typically, there should be one material model for one integration point. As we do not know how will the derived element class organized its data, we do not directly create the container for material models in the base class. Instead we store the tags of different material models that may be used in this element. As we separate creation and initialization of an element apart, the material tags can be defined during creation and later in initialization, we could get copies from the remote database.

``` cpp
uvec material_tag;
```

Before further definitions of basic mechanical quantities, we shall first know the size of those quantities. Hence we shall store how many nodes the element connects and how many DoFs per node. This in fact should be held by node objects but the connected node may have more DoFs than needed.

``` cpp
unsigned num_node = 0;
unsigned num_dof = 0;
```

Correspondingly, the node tags and DoF tags should be stored in case of further reuse.

``` cpp
uvec node_encoding;
uvec dof_encoding;
```

Now we now the size of vectors and matrices, we can create containers for those quantities.

``` cpp
vec resistance;
mat mass;
mat damping;
mat stiffness;
mat initial_stiffness;
```

Additionally, the Element class defines a non-linear geometry switch to enable/disable large deformation formulation. By such, the linear and nonlinear version can be combined for some elements.

``` cpp
bool nlgeom = false;
```

### Member Methods In Detail

Member methods include getters of above member variables for outside usage.

``` cpp
    const unsigned& get_dof_number() const;
    const unsigned& get_node_number() const;
    const uvec& get_dof_encoding() const;
    const uvec& get_node_encoding() const;

    const vector<weak_ptr<Node>>& get_node_ptr() const;

    virtual const vec& get_resistance() const;
    virtual const mat& get_mass() const;
    virtual const mat& get_damping() const;
    virtual const mat& get_stiffness() const;
    virtual const mat& get_initial_stiffness() const;
```

The later five methods are declared as virtual methods so that they could be overridden in derived classes if the developer does not want to use the built-in member variables.

Now it comes to the interesting part, which should be carefully implemented by the derived classes. The easiest ones are three status controlling methods.

``` cpp
virtual int commit_status() = 0;
virtual int clear_status() = 0;
virtual int reset_status() = 0;
```

Basically they are the wrappers of the corresponding methods in material models. So simply call the same method in attached material models.

There are two additional methods. The first one is to initialize the element before executing the analysis and should be in charge of validation of all required data **except for any node object or material model related checks**.

``` cpp
virtual void initialize(const shared_ptr<DomainBase>&) = 0;
```

This method will be called by the analysis control flow, after constructing the element (model creation), before incremental iteration (problem solving). So this method should

1.  define all necessary parameters which may be used in analysis,
2.  get copies of material models for self use,
3.  get pointers of node objects connected and
4.  potentially some other operations.

Another one is in charge of updating status.

``` cpp
virtual int update_status() = 0;
```

This method gets for example trial displacement via pointers of connected node objects, and calculates the trial status for material model and then calls the corresponding method in material model to update the status. At final stage, it gets the response from material models and forms the stiffness and resistance for the trial status.

------------------------------------------------------------------------

Summary
-------

### Member Variables Summary

To sum up, the **Element** class has 12 member variables which should be (partially) defined/initialized. They are all defined as protected variables so that they can be directly accessed by derived classes. Although this breaks the encapsulation of the **Element** class, we have to do some trade-off for some convenience. Other design patterns such as getters/setters of private variables or defining those variables in each derived class are too complicated as they will be frequently accessed by the all Element classes.

### Member Methods Summary

To implement a derived Element class. The minimum requirement is to **override the last five methods**, if developers use the build-in containers as the communication interface. If not, other methods should also be overridden to provide corresponding quantities.

In the simplest (most) cases, three out of those five methods simply call the methods of the same name in defined material models. So ideally, the main methods need to be overridden are

``` cpp
virtual void initialize(const shared_ptr<DomainBase>&) = 0;
virtual int update_status() = 0;
```

A vivid example will be exhibited in this template class.
