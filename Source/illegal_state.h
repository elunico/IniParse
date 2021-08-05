//
// Created by Thomas Povinelli on 8/3/21.
//

#ifndef PARSEINI_ILLEGAL_STATE_H
#define PARSEINI_ILLEGAL_STATE_H
#include <exception>
#include <string>

namespace tom {
class illegal_state : std::exception {

public:
    explicit illegal_state(std::string string);

    std::string message;

    [[nodiscard]] const char* what() const noexcept override;
};
}


#endif //PARSEINI_ILLEGAL_STATE_H
