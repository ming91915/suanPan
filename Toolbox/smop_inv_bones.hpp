#pragma once
#include "Toolbox"

class smop_inv
{
public:
    template <typename T1>
    static void apply(SymmMat<typename T1::elem_type>& out, const SmOp<T1, smop_inv>& in)
    {
        arma_extra_debug_sigprint();

        out = in.m;

        if(sp_inv(out) != 0) {
            out.reset();
            arma_stop_runtime_error("sp_inv(): matrix seems singular");
        }
    }
};
