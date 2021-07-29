//
// Created by Thomas Povinelli on 7/28/21.
//

//#include "ini_section.h"
#include "utils.h"
#include "ini_entry.h"
#include <iostream>
#include <memory>

namespace tom {



std::ostream& operator <<(std::ostream& os, ini_entry const& self) {
    os << self.key() << "=" << self.value();
    return os;
}

std::ostream& operator <<(std::ostream& os, ini_section const& self) {
    os << "[" << self.name << "]\n";
    for (std::shared_ptr<ini_entry> const &entry : self.entries()) {
//        os << "\t" << *entry << "\n";
        os << *entry << "\n";
    }

    return os;
}

}