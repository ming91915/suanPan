#pragma once

class glue_times_symm
{
public:
    template <typename T1, typename T2>
    arma_hot static void apply(Mat<typename T1::elem_type>& out,
        const Glue<T1, T2, glue_times_symm>& I)
    {
        typedef typename T1::elem_type eT;
        auto& A = I.A;
        auto& X = I.B;

        out = X;

        auto UPLO = 'U';
        auto N = static_cast<int>(A.n_size);
        eT ALPHA = 1.;
        auto INC = 1;
        eT BETA = 0.;

        if(is_float<eT>::value) {
            using T = float;
            suanpan::sspmv_(&UPLO, &N, (T*)&ALPHA, (T*)A.memptr(), (T*)X.memptr(), &INC,
                (T*)&BETA, (T*)out.memptr(), &INC);
        } else if(is_double<eT>::value) {
            using T = double;
            suanpan::dspmv_(&UPLO, &N, (T*)&ALPHA, (T*)A.memptr(), (T*)X.memptr(), &INC,
                (T*)&BETA, (T*)out.memptr(), &INC);
        }
    }
};
