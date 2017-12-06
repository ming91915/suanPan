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

    const auto A = make_shared<CDP>();
    A->Material::initialize();
    A->initialize();

    vec step(std::initializer_list<double>{ -1E-4, .2E-4, .2E-4, 0., 0., 0. });
    // step = -step;
    vector<double> B;

    for(auto I = 0; I < 200; I++) {
        A->update_incre_status(step);
        A->commit_status();
        B.emplace_back(A->get_stress().at(0));
    }
    vec C(B);

    C.save("K", raw_ascii);

    suanpan_info("Finished in %.3F seconds.\n", T.toc());

    return 0;
}

// const auto A = make_shared<RambergOsgood>();
// const vector<unsigned> B{ 100, 200, 220, 240, 260, 280 };
// auto C = material_tester(A, B, 0.0001);
// C.save("K", raw_ascii);

// ABM2 B(1, make_shared<ODE_INSTANCE>(), 20, false);
// B.get_ode()->update_current_variable(vec{ 1 });
// B.get_ode()->update_incre_time(1.);
// B.analyze();
// B.get_ode()->get_trial_variable().print();