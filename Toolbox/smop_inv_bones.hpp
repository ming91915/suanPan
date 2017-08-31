#pragma once

class smop_inv
{
public:
    template <typename T1> static void apply(T1& out, const SmOp<T1, smop_inv>& in)
    {
        arma_extra_debug_sigprint();

        out = in.m;

        if(sp_inv(out) != 0) {
            out.reset();
            arma_stop_runtime_error("sp_inv(): matrix seems singular");
        }
    }
};
