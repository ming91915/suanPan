////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017 Theodore Chang
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////

#include "RCM.h"

uvec RCM(const vector<uvec>& A, const uvec& E) {
#ifdef SUANPAN_DEBUG
    wall_clock TM;
    TM.tic();
#endif

    const auto S = E.n_elem;

    uvec G = sort_index(E);
    uvec M(S, fill::zeros);
    uvec R(S, fill::zeros);

    uword IDXA = 0, IDXB = S - 1, IDXC = S - 1;

    while(IDXA < S) {
        if(IDXB == IDXC) {
            while(IDXA < S && M(G(IDXA)) == 1) ++IDXA;
            if(IDXA == S) break;
            R(IDXC--) = G(IDXA);
            M(G(IDXA++)) = 1;
        }
        for(const auto& IDX : A[R(IDXB--)])
            if(M(IDX) != 1) {
                R(IDXC--) = IDX;
                M(IDX) = 1;
            }
    }

#ifdef SUANPAN_DEBUG
    suanpan_debug("RCM algorithm takes %.5E seconds.\n", TM.toc());
#endif

    return R;
}

template <typename eT> uvec RCM(const SpMat<eT>& MEAT) {
#ifdef SUANPAN_DEBUG
    //! Check if the matrix is square.
    if(!MEAT.is_square()) throw logic_error("RCM() can only be applied to square matrix.\n");

    wall_clock TM;
    TM.tic();
#endif

    //! Get the size of the square matrix.
    auto S = MEAT.n_cols;

    //! Compute the number of degree of each node. Need update once Armadillo is improved.
    uvec E(S);
    //#pragma omp parallel for schedule(dynamic, SUANPAN_CHUNK)
    for(unsigned I = 0; I < S; ++I) E(I) = MEAT.col(I).n_nonzero;

    vector<uvec> A;
    A.reserve(S);
    //#pragma omp parallel for schedule(dynamic, SUANPAN_CHUNK)
    for(unsigned K = 0; K < S; ++K) {
        unsigned J = 0;
        uvec IDX(E(K));
        for(auto L = MEAT.begin_col(K); L != MEAT.end_col(K); ++L) IDX(J++) = L.row();
        A.emplace_back(IDX(sort_index(E(IDX))));
    }

    //! Get the indices array in increasing order of degree.
    uvec G = sort_index(E);

    //! Now define the mask vector to indicate if one node is numbered or not.
    uvec M(S, fill::zeros);
    //! Define the new order vector.
    uvec R(S, fill::zeros);

    //! Preparation is ready.
    //! The G stores all vertices increasing order of degree.
    //! The adjacency stores the neighbor vertices for each vertex sorted according to the number of degree.
    //! Now start to loop over all nodes.

    //! Define one position indicator IDXA for numbered vertices and two other position indicator IDXB and IDXC for looping.
    uword IDXA = 0, IDXB = S - 1, IDXC = S - 1;

    //! While the sum of mask does not equal to the size of matrix, there are vertices not numbered in the system, the algorithm should continue.
    while(IDXA < S) {
        //! IDXC will always point to the end of the vector. For any time when IDXB==IDXC, both indicators reach the end of reorder vector, i.e., there is no sub level any more (forms an independent subset). The graph cannot grow any more, then get the vertex with minimum degree as the new start point.
        if(IDXB == IDXC) {
            //! IDXA should be less that S so that G(IDXA) does not overflow.
            while(IDXA < S && M(G(IDXA)) == 1) ++IDXA;
            //! Once IDXA hits S, there is no unnumbered vertex in the graph. Quit the loop.
            if(IDXA == S) break;
            //! Push in first unnumbered element in the list.
            R(IDXC--) = G(IDXA);
            //! Label it as renumbered and move IDXA to next position.
            M(G(IDXA++)) = 1;
        }
        //! Now we at least has one root, which is indicated by the indicator IDXB, in our graph, push in all children into the vector. As they are already sorted, we can simply push in. When the loop is finished, move IDXB to next position, which may have another root or the children of current root.
        for(const auto& IDX : A[R(IDXB--)])
            if(M(IDX) != 1) {
                R(IDXC--) = IDX;
                M(IDX) = 1;
            }
    }

#ifdef SUANPAN_DEBUG
    suanpan_debug("RCM algorithm takes %.5E seconds.\n", TM.toc());
#endif

    return R;
}

template <typename eT> uvec RCM(const Mat<eT>& MEAT) { return RCM(SpMat<eT>(MEAT)); }
