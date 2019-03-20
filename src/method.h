//
// Created by krab1k on 31/10/18.
//

#pragma once

#include <string>
#include <vector>
#include <utility>

#include "structures/molecule.h"


enum class RequiredFeatures {
    BOND_INFO,
    BOND_DISTANCES,
    DISTANCE_TREE
};


class Parameters;


struct MethodOption {
    std::string name;
    std::string description;
    std::string type;
    std::string default_value;
    std::vector<std::string> choices;
};


class Method {
protected:
    const std::string name_{};
    const std::vector<std::string> common_parameters_{};
    const std::vector<std::string> atom_parameters_{};
    const std::vector<std::string> bond_parameters_{};
    const std::map<std::string, MethodOption> options_{};

    std::map<std::string, std::string> option_values_{};

    Parameters *parameters_{nullptr};

    ~Method() = default;

public:
    Method(std::string name, std::vector<std::string> common, std::vector<std::string> atom,
           std::vector<std::string> bond, std::map<std::string, MethodOption> options) :
            name_{std::move(name)},
            common_parameters_{std::move(common)},
            atom_parameters_{std::move(atom)},
            bond_parameters_{std::move(bond)},
            options_{std::move(options)} {}

    std::vector<std::string> common_parameters() { return common_parameters_; }

    std::vector<std::string> atom_parameters() { return atom_parameters_; }

    std::vector<std::string> bond_parameters() { return bond_parameters_; }

    Parameters *parameters() { return parameters_; }

    void set_parameters(Parameters *parameters);

    bool has_parameters() {
        return (common_parameters_.size() + atom_parameters_.size() + bond_parameters_.size()) != 0;
    }

    virtual std::vector<RequiredFeatures> get_requirements() const;

    virtual bool is_suitable_for_molecule(const Molecule &) const;

    virtual bool is_suitable_for_large_molecule() const { return true; }

    virtual std::vector<double> calculate_charges(const Molecule &molecule) const = 0;

    std::string name() { return name_; }

    std::map<std::string, MethodOption> get_options() const { return options_; }

    template<typename T>
    T get_option_value(const std::string &name) const;

    void set_option_value(const std::string &name, const std::string &value) {
        option_values_[name] = value;
    }
};


template<>
std::string Method::get_option_value<std::string>(const std::string &name) const;

template<>
double Method::get_option_value<double>(const std::string &name) const;

template<>
int Method::get_option_value<int>(const std::string &name) const;


class EEMethod : public Method {
    std::map<std::string, MethodOption> augment_options(std::map<std::string, MethodOption> options) const {
        options["type"] = {"type", "Type of a solver", "str", "full", {"full", "cutoff"}};
        options["radius"] = {"radius", "Radius for cutoff", "double", "12", {}};
        return options;
    }

protected:
    virtual std::vector<double> solve_system(const std::vector<const Atom *> &atoms, double total_charge) const = 0;

    ~EEMethod() = default;

public:
    EEMethod(std::string name, std::vector<std::string> common, std::vector<std::string> atom,
             std::vector<std::string> bond, std::map<std::string, MethodOption> options) :
            Method(std::move(name), std::move(common), std::move(atom), std::move(bond), augment_options(
                    std::move(options))) {}

    std::vector<double> calculate_charges(const Molecule &molecule) const override;

    std::vector<RequiredFeatures> get_requirements() const override {
        return {RequiredFeatures::DISTANCE_TREE};
    }
};
