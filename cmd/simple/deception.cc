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
#include <optional>
#include <sstream>
#include <core/Interpreter.h>
#include <core/Codes.h>

// each action table is different so there is no header

// Each table is made up of 256 entries, if they are not valid
void displayCurrentTableContents(Deception::Interpreter& interpreter, char) {
    for (const auto& a : *(interpreter.getCurrentTable())) {
        std::cout << a.first << std::endl;
    }
}
int main(int argc, char** argv) {
    std::stringstream builder;
    Deception::Interpreter theInterpreter{
            {
                    {"single line comment", { {'\n', [](Deception::Interpreter& interpreter, char) { interpreter.restore(); }} } },
                    {"read string",
                     {
                             {{'\n', [](auto& i, char) { i.restore(); }}},
                             [&builder](auto& i) {
                                 builder.str("");
                                 },
                             [&builder](auto& i) {
                                 auto str = builder.str();
                                 std::cout << "You entered " << str << std::endl;
                             },
                             [&builder](auto& i, char c) { builder.put(c); }
                     }
                     },
                    {"core",
                     {
                             { Deception::Opcodes::Ascii::EOT, [](auto& interpreter, char) { interpreter.terminate(); } },
                             { '#', [](Deception::Interpreter& interpreter, char) {interpreter.use("single line comment"); } },
                             { '!', [](auto& interpreter, char) { interpreter.use("read string"); }},
                             { '?', displayCurrentTableContents },
                     }
                    }
            }
    };
    theInterpreter.use("core");
    std::cout << "CTRL-D to quit" << std::endl;
    theInterpreter.run();
    return 0;
}
