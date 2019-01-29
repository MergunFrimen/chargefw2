//
// Created by krab1k on 8.11.18.
//

#pragma once

#include <boost/config.hpp>

#include "../method.h"


class Dummy : public Method {
public:
    explicit Dummy() : Method("Dummy", {}, {}, {}, {}) {}

    virtual ~Dummy() = default;

    std::vector<double> calculate_charges(const Molecule &molecule) const override;
};

extern "C" BOOST_SYMBOL_EXPORT Dummy method;
Dummy method;
