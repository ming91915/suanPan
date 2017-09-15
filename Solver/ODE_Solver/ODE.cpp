#include "ODE.h"

ODE::ODE(const unsigned& T, const unsigned& CT, const unsigned& D)
    : Tag(T, CT)
    , n_size(D) {}

void ODE::set_dimension(const unsigned& D) { n_size = D; }

const unsigned& ODE::get_dimension() const { return n_size; }

void ODE::print() { suanpan_info("ODE::print() is unimplemented.\n"); }

void ODE::operator()(const unsigned& D) { n_size = D; }

unsigned ODE::operator()() { return n_size; }
