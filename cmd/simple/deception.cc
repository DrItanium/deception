/*
deception
Copyright (c) 2024 and beyond, Joshua Scoggins
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR 
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <iostream>
#include <functional>
#include <cstdint>
#include <array>
#include <optional>
// each action table is different so there is no header

struct Entry {
    std::function<void()> _body = nullptr;
    bool operator() const noexcept {
        return _body != nullptr;
    }
    /// @todo this is customizable but this is the simplest interpreter
    /// possible
    void operator()() {
        if (_body) {
            _body();
        }
    }
};


// Each table is made up of 256 entries, if they are not valid
using Table = std::array<std::function<void()>, 256>;
Table basicTable;
Table* currentTable = &basicTable;
void 
setupInitialInterpreter() {
    // do nothing right now
}
std::optional<uint8_t>
nextCharacter(std::istream& inputStream) noexcept {
    uint8_t nextCharacter = inputStream.get();
    if (inputStream.fail()) {
        return std::nullopt;
    } else {
        return nextCharacter;
    }
}
void 
runInterpreter() {
    std::cout << "CTRL-D to quit" << std::endl;
    while (true) {
        if (auto theCharacter = nextCharacter(std::cin); theCharacter) {
            // now do a table lookup
            (*currentTable)[theCharacter](); // then just invoke it
        } else {
            // in this case, we want to perform error handling which only
            // happens when we run out of characters, we should break since you
            // pressed ctrl-d
            break;
        }
    }
}

int main(int argc, char*[] argv) {
    setupInitialInterpreter();
    runInterpreter();
    return 0;
}
