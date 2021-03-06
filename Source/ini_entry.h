//
// Created by Thomas Povinelli on 7/28/21.
//

#ifndef PARSEINI_INI_ENTRY_H
#define PARSEINI_INI_ENTRY_H

#include <sstream>
#include <string>
#include <vector>
#include "ini_section.h"
#include "utils.h"

namespace tom {

struct ini_entry {
private:
    std::string key_;
    std::string value_;

public:
    std::weak_ptr<ini_section> parent;

    bool operator ==(ini_entry const& rhs) const noexcept;

    bool operator !=(ini_entry const& rhs) const noexcept;

    ini_entry(ini_entry const& other);

    ini_entry(ini_entry&& other) noexcept;

    ini_entry& operator =(ini_entry const& other);

    ini_entry& operator =(ini_entry&& other) noexcept;

    ini_entry(std::weak_ptr<ini_section> parent, std::string key, std::string value);

    [[nodiscard]] std::string const& key() const noexcept;

    [[nodiscard]] std::string const& value() const noexcept;

    template <typename T>
    struct adapt_to {
        static_assert(std::is_default_constructible<T>::value, "Can only adapt to default constructable types");

        T operator ()(std::string const& value) {
            std::stringstream geek(value);
            T                 x;
            geek >> x;
            return x;
        }
    };

    template <typename T, typename AdapterFunc = adapt_to<T> >
    T adapt_value(AdapterFunc adapter = adapt_to<T>{ }) const {
        return adapter(value_);
    }

    operator std::tuple<std::string, std::string>() const;
};

}  // namespace tom
#endif  // PARSEINI_INI_ENTRY_H
