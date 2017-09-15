#ifndef ELEMENTTEMPLATE_H
#define ELEMENTTEMPLATE_H

#include <Element/Element.h>

class ElementExample : public Element {
    static const unsigned m_node, m_dof;

    const double thickness;

    double area = 0.;

    mat strain_mat;

    unique_ptr<Material> m_material;

public:
    ElementExample(const unsigned&, const uvec&, const unsigned&, const double& = 1.);

    void initialize(const shared_ptr<DomainBase>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;

    void print() override;
};

#endif
