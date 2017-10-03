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

#include <suanPan>

int main(int argc, char** argv) {
    wall_clock T;
    T.tic();

    // argument_parser(argc, argv);

    double C;
    vec A = { 0.669, 0.560, 1.310 };
    A /= 1000;
    // vec A = { 0.5,-0.2,0.6 };
    auto B = nominal_to_principal_strain(A, C);

    cout << C / datum::pi * 180. << endl;
    A(2) /= 2.;
    (B * A).print();

    suanpan_info("Finished in %.3F seconds.\n", T.toc());

    return 0;
}

// const auto A = make_shared<RambergOsgood>();
// const vector<unsigned> B{ 100, 200, 220, 240, 260, 280 };
// auto C = material_tester(A, B, 0.0001);
// C.save("K", raw_ascii);
