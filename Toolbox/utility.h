/*******************************************************************************
 * Copyright (C) 2017 Theodore Chang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#ifndef UTILITY_H
#define UTILITY_H

#include <sstream>
#include <string>

template <typename T> T& modifier(const T& I) { return const_cast<T&>(I); }

template <typename T> T*& modifier(const T* const& I) { return const_cast<T*&>(I); }

template <typename T> T sign(const T& I) { return (I > 0.) - (I < 0.); }

using std::istringstream;
using std::string;

template <typename T> bool get_input(istringstream& I, T& O) { return !(I >> O).fail(); }

template <typename T> T get_input(istringstream& I) {
    T O;
    I >> O;
    return O;
}

template <typename T> bool get_optional_input(istringstream& I, T& O) { return !I.eof() && !(I >> O).fail(); }

bool is_equal(const char*, const char*);
bool is_equal(const string&, const char*);

bool is_true(const char*);
bool is_false(const char*);
bool is_true(const string&);
bool is_false(const string&);

#include <armadillo>
#include <memory>

template <typename T> arma::mat material_tester(const std::shared_ptr<T>& obj, const std::vector<unsigned>& idx, const double& incre) {
    std::vector<double> A{ 0 }, B{ 0 };
    arma::vec E{ incre };
    for(const auto& I : idx) {
        for(unsigned J = 0; J < I; ++J) {
            obj->update_incre_status(E);
            obj->commit_status();
            A.emplace_back(obj->get_strain().at(0));
            B.emplace_back(obj->get_stress().at(0));
        }
        E = -E;
    }

    arma::mat response(A.size(), 2);
    response.col(0) = arma::vec{ A };
    response.col(1) = arma::vec{ B };

    return response;
}

#endif
