//
// Created by krab1k on 13.11.18.
//

#include <cmath>
#include <mkl_lapacke.h>
#include <mkl.h>

#include "delre.h"
#include "../structures/molecule.h"
#include "../structures/bond.h"
#include "../parameters.h"


std::vector<double> DelRe::calculate_charges(const Molecule &molecule) const {

    const size_t n = molecule.atoms().size();
    const size_t m = molecule.bonds().size();
    std::vector<double> q(n, 0);

    auto *A = static_cast<double *>(mkl_calloc(n * n, sizeof(double), 64));
    auto *b = static_cast<double *>(mkl_calloc(n, sizeof(double), 64));
    auto *ipiv = static_cast<int *>(mkl_calloc(n, sizeof(int), 64));

    for (size_t i = 0; i < n; i++) {
        auto &atom_i = molecule.atoms()[i];
        b[i] = -parameters_->atom()->parameter(atom::delta)(atom_i);
        A[i * n + i] = -1.0;
    }

    for (const auto &bond: molecule.bonds()) {
        size_t i = bond.first().index();
        size_t j = bond.second().index();
        A[i * n + j] = parameters_->bond()->parameter(bond::gammaA)(bond);
        A[j * n + i] = parameters_->bond()->parameter(bond::gammaB)(bond);
    }

    auto n_int = static_cast<int>(n);
    LAPACKE_dgesv(LAPACK_ROW_MAJOR, n_int, 1, A, n_int, ipiv, b, 1);

    for (size_t k = 0; k < m; k++) {
        const auto &bond = molecule.bonds()[k];
        size_t i = bond.first().index();
        size_t j = bond.second().index();
        double dq = (b[i] - b[j]) / (2 * parameters_->bond()->parameter(bond::eps)(bond));
        q[i] -= dq;
        q[j] += dq;
    }

    mkl_free(A);
    mkl_free(b);
    mkl_free(ipiv);

    return q;
}
