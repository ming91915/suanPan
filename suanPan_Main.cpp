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

    // const auto A = make_shared<Concrete01>(1, -.002, -30, BackboneType::KPSU);
    // const vector<unsigned> B{ 10, 15, 25, 25, 35, 35, 45, 45, 55, 55 };
    // auto M = material_tester(A, B, -.0001);
    // M.save("K", raw_ascii);

    argument_parser(argc, argv);

    cout << "Finished: " << T.toc() << "\n";
    return 0;
}
