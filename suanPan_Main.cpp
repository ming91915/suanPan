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

    argument_parser(argc, argv);

    cout << "Finished: " << T.toc() << "\n";
    return 0;
}

void example_concrete01() {
    Concrete01 A(1, -2E-3, -30, BackboneType::TSAI);

    vector<double> strain, stress;

    for(auto I = 0; I < 50; ++I) {
        A.update_incre_status(vec{ -1E-4 });
        strain.emplace_back(A.get_strain().at(0));
        stress.emplace_back(A.get_stress().at(0));
        A.commit_status();
    }
    for(auto I = 0; I < 70; ++I) {
        A.update_incre_status(vec{ 1E-4 });
        strain.emplace_back(A.get_strain().at(0));
        stress.emplace_back(A.get_stress().at(0));
        A.commit_status();
    }
    for(auto I = 0; I < 100; ++I) {
        A.update_incre_status(vec{ -1E-4 });
        strain.emplace_back(A.get_strain().at(0));
        stress.emplace_back(A.get_stress().at(0));
        A.commit_status();
    }

    mat K(strain.size(), 2);
    K.col(0) = vec(strain);
    K.col(1) = vec(stress);

    K.save("K", raw_ascii);
}
