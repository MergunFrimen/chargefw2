//
// Created by krab1k on 23/10/18.
//

#pragma once

#include <string>
#include <utility>


class Element {
    size_t Z_{};
    std::string symbol_;
    std::string name_;
    double electronegativity_{};
    double covalent_radius_{};
    double vdw_radius_{};
    int period_{};
    int group_{};
    double electron_affinity_{};
    double ionization_potential_{};

public:
    Element() = default;

    Element(size_t Z, std::string symbol, std::string name, double electronegativity, double covalent_radius,
            double vdw_radius, int period, int group, double electron_affinity, double ionization_potential)
            : Z_(Z), symbol_(std::move(symbol)), name_(std::move(name)), electronegativity_(electronegativity),
              covalent_radius_(covalent_radius), vdw_radius_(vdw_radius), period_(period), group_(group),
              electron_affinity_{electron_affinity}, ionization_potential_{ionization_potential} {}

    const std::string &symbol() const { return symbol_; }

    const std::string &name() const { return name_; }

    double electronegativity() const { return electronegativity_; }

    double covalent_radius() const { return covalent_radius_; }

    double vdw_radius() const { return vdw_radius_; }

    double electron_affinity() const { return electron_affinity_; }

    double ionization_potential() const { return ionization_potential_; }

    int group() const { return group_; }

    int valence_electron_count() const;

    int period() const { return period_; }

    size_t Z() const { return Z_; }
};
