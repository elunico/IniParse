//
// Created by Thomas Povinelli on 7/28/21.
//

//#include "ini_section.h"
#include "utils.h"
#include <iostream>
#include <memory>
#include "ini_entry.h"

namespace tom {

std::ostream& operator <<(std::ostream& os, ini_entry const& self) {
    os << self.key() << "=" << self.value();
    return os;
}

std::ostream& operator <<(std::ostream& os, ini_section const& self) {
    os << "[" << self.name << "]\n";

    auto const& entries = self.entries();

    for (auto const& entry : entries) {
        std::shared_ptr<ini_entry> const& ptr = entry.lock();
        if (ptr == nullptr)
            os << "std::weak_ptr<ini_entry>(nullptr)";
        else
            os << *ptr;
        os << "\n";
    }

    return os;
}

}  // namespace tom
