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
#include <optional>
#include <stack>
#include <memory>
#include <map>
#include <core/Concepts.h>
// each action table is different so there is no header

// Each table is made up of 256 entries, if they are not valid
using Table = std::map<char, std::function<void()>>;
using ActionTable = std::shared_ptr<Table>;
using Conclave = std::vector<ActionTable>;
Conclave tables;
std::stack<ActionTable> tableStack;
ActionTable currentTable = nullptr;
ActionTable getCurrentTable() {
    return currentTable;
}
int nestingDepth = 0;
void
resetTable() {
    while (!tableStack.empty()) {
        tableStack.pop();
    }
    currentTable = tables[0]; // go back to the initial table 
    nestingDepth = 0;
}
void 
restoreTable() {
    if (!tableStack.empty()) {
        currentTable = tableStack.top();
        tableStack.pop();
    }
}
void 
use(ActionTable target) {
    if (currentTable) {
        tableStack.push(currentTable);
    }
    currentTable = target;
}
/**
 * @param construct a new table and point to it!
 */
ActionTable
newTable() {
    tables.emplace_back(std::make_shared<Table>());
    return tables.back();
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
setupInitialInterpreter() {
    auto basicTable = newTable(); 
    auto parenLookupTable = newTable();
    auto singleLineCommentTable = newTable();
    resetTable();
    basicTable->emplace('#', [singleLineCommentTable]() { use(singleLineCommentTable); });
    // this is an example
    basicTable->emplace('(',
                        [parenLookupTable]() {
                            // consume characters until you find a matching ')'
                            // but in this case we just need to switch to a different table
                            use(parenLookupTable);
                            nestingDepth = 1;
                        });
    singleLineCommentTable->emplace('\n', []() { restoreTable(); });
    parenLookupTable->emplace('(', []() { ++nestingDepth; });
    parenLookupTable->emplace(')', []() {
        --nestingDepth;
        if (nestingDepth <= 0) {
            nestingDepth = 0;
            restoreTable(); // go back to the previous table
        }
    });
    basicTable->emplace('?',
                        [basicTable]() {
                            std::cout << "available words" << std::endl;
                            for (auto a : *basicTable) {
                                std::cout << a.first << std::endl;
                            }
                        });
}
void 
runInterpreter() {
    std::cout << "CTRL-D to quit" << std::endl;
    while (true) {
        if (auto theCharacter = nextCharacter(std::cin); theCharacter) {
            // now do a table lookup
            if (auto lookup = getCurrentTable()->find(*theCharacter); lookup != getCurrentTable()->end()) {
                lookup->second();
            }
        } else {
            // in this case, we want to perform error handling which only
            // happens when we run out of characters, we should break since you
            // pressed ctrl-d
            break;
        }
    }
}

int main(int argc, char** argv) {
    setupInitialInterpreter();
    runInterpreter();
    return 0;
}
