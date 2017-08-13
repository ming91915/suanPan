#ifndef ELEMENTTEMPLATE_H
#define ELEMENTTEMPLATE_H

#include <Element/Element.h>

class ElementExample : public Element
{
    static const unsigned m_node;
    static const unsigned m_dof;

    double thickness = 0.;

    double area = 0.;

    mat strain_mat;

    unique_ptr<Material> m_material;

public:
    ElementExample(const unsigned&, const uvec&, const unsigned&, const double& = 1.);

    void initialize(const shared_ptr<Domain>&) override;
    int updateStatus() override;
    int commitStatus() override;
    int clearStatus() override;
    int resetStatus() override;

    void print() override;
};

#endif