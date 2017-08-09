#include "ODE.h"

ODE::ODE(const unsigned& T, const unsigned& CT, const unsigned& D)
    : User_Function(T, CT)
    , num_dim(D)
{
}

ODE::~ODE() {}

void ODE::setDimension(const unsigned& D) { num_dim = D; }

const unsigned& ODE::getDimension() const { return num_dim; }

void ODE::print() { printf("ODE::print() is unimplemented.\n"); }
