//
// Created by krab1k on 23/10/18.
//

#include <utility>
#include "atom.h"
#include "../element.h"
#include "../periodic_table.h"


Atom::Atom(size_t idx, const Element *element, double x, double y, double z, std::string atom_name, size_t residue_id,
           std::string residue_name) : index_{idx}, element_{element}, atom_name_{std::move(atom_name)},
                                       residue_id_{residue_id}, residue_{std::move(residue_name)} {
    pos_[0] = x;
    pos_[1] = y;
    pos_[2] = z;
}
