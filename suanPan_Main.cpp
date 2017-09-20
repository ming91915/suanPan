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

void example_concrete01() {
    auto A = make_shared<Concrete01>(1, -.002, -30, BackboneType::TSAI);
    vector<double> B;
    vector<double> C;

    for(size_t i = 0; i < 10; i++) {
        A->update_incre_status(vec{ -.0001 });
        A->commit_status();
        B.emplace_back(A->get_strain().at(0));
        C.emplace_back(A->get_stress().at(0));
    }

    for(size_t i = 0; i < 20; i++) {
        A->update_incre_status(vec{ .0001 });
        A->commit_status();
        B.emplace_back(A->get_strain().at(0));
        C.emplace_back(A->get_stress().at(0));
    }
    for(size_t i = 0; i < 50; i++) {
        A->update_incre_status(vec{ -.0001 });
        A->commit_status();
        B.emplace_back(A->get_strain().at(0));
        C.emplace_back(A->get_stress().at(0));
    }
    for(size_t i = 0; i < 60; i++) {
        A->update_incre_status(vec{ .0001 });
        A->commit_status();
        B.emplace_back(A->get_strain().at(0));
        C.emplace_back(A->get_stress().at(0));
    }
    for(size_t i = 0; i < 90; i++) {
        A->update_incre_status(vec{ -.0001 });
        A->commit_status();
        B.emplace_back(A->get_strain().at(0));
        C.emplace_back(A->get_stress().at(0));
    }
    for(size_t i = 0; i < 100; i++) {
        A->update_incre_status(vec{ .0001 });
        A->commit_status();
        B.emplace_back(A->get_strain().at(0));
        C.emplace_back(A->get_stress().at(0));
    }
    for(size_t i = 0; i < 130; i++) {
        A->update_incre_status(vec{ -.0001 });
        A->commit_status();
        B.emplace_back(A->get_strain().at(0));
        C.emplace_back(A->get_stress().at(0));
    }

    mat D(B.size(), 2);
    D.col(0) = vec(B);
    D.col(1) = vec(C);

    D.save("K", raw_ascii);
}

int main(int argc, char** argv) {
    wall_clock T;
    T.tic();

    argument_parser(argc, argv);

    // example_concrete01();

    cout << "Finished: " << T.toc() << "\n";
    return 0;
}
