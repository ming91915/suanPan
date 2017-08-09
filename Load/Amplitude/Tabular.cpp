#include "Tabular.h"

Tabular::Tabular(const unsigned& T, const unsigned& CT)
    : Amplitude(T, CT)
{
}

Tabular::Tabular(const unsigned& T, const vec& TI, const vec& M)
    : Amplitude(T, CT_TABULAR)
    , time(TI)
    , magnitude(M)
{
    if(time.n_elem != magnitude.n_elem)
        throw logic_error("Tabular requires two vectors of the same size.\n");
}

Tabular::Tabular(const unsigned& T, const char* P)
    : Amplitude(T, CT_TABULAR)
{
    mat ext_data;
    ext_data.load(P);
    if(ext_data.n_cols == 2) {
        time = ext_data.col(0);
        magnitude = ext_data.col(1);
    } else if(ext_data.n_cols > 2)
        printf("Tabular reads more than two columns from the given file, check it.\n");
    else
        throw logic_error("Tabular requires two valid columns.\n");
}

Tabular::~Tabular() {}

double Tabular::getAmplitude(const double& T)
{
    uword IDX = 0;
    for(uword i = 0; i < time.n_elem; ++i)
        if(time(i) < T)
            IDX = i;
        else
            break;
    auto A = magnitude(IDX);
    if(IDX == 0)
        A = 0.;
    else if(IDX != time.n_elem - 1)
        A += (T - time(IDX)) * (magnitude(IDX + 1) - magnitude(IDX)) /
            (time(IDX + 1) - time(IDX));
    return A;
}

void Tabular::print() { printf("Tabular.\n"); }
