#include "ODE.h"

ODE::ODE(const unsigned& T, const unsigned& CT, const unsigned& D)
    : User_Function(T, CT)
    , num_dim(D) {}

ODE::~ODE() {}

void ODE::set_dimension(const unsigned& D) { num_dim = D; }

const unsigned& ODE::get_dimension() const { return num_dim; }

void ODE::print() { suanpan_info("ODE::print() is unimplemented.\n"); }

void ODE::operator()(const unsigned& D) { num_dim = D; }

unsigned ODE::operator()() { return num_dim; }
