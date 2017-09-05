#include "MaterialTemplate.h"

#ifndef MT_MATERIALTEMPLATE
#define MT_MATERIALTEMPLATE 0
#endif

MaterialTemplate::MaterialTemplate(const unsigned& T)
    : Material(T, MT_MATERIALTEMPLATE) {}

void MaterialTemplate::initialize() {}

double MaterialTemplate::get_parameter(const unsigned& T) const { return 0.; }

unique_ptr<Material> MaterialTemplate::get_copy() { return make_unique<MaterialTemplate>(*this); }

int MaterialTemplate::update_trial_status(const vec& t_strain) { return 0; }

int MaterialTemplate::clear_status() { return 0; }

int MaterialTemplate::commit_status() { return 0; }

int MaterialTemplate::reset_status() { return 0; }
