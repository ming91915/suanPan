#include "MaterialTemplate.h"

#ifndef MT_MATERIALTEMPLATE
#define MT_MATERIALTEMPLATE 0
#endif

MaterialTemplate::MaterialTemplate(const unsigned& T)
    : Material(T, MT_MATERIALTEMPLATE)
{
}

void MaterialTemplate::initialize() {}

double MaterialTemplate::getParameter(const unsigned& T) const { return 0.; }

unique_ptr<Material> MaterialTemplate::getCopy()
{
    return make_unique<MaterialTemplate>(*this);
}

int MaterialTemplate::updateTrialStatus(const vec& t_strain) { return 0; }

int MaterialTemplate::clearStatus() { return 0; }

int MaterialTemplate::commitStatus() { return 0; }

int MaterialTemplate::resetStatus() { return 0; }
