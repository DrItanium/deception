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
#include <core/Interpreter.h>
#include <core/Codes.h>

// Each table is made up of 256 entries, if they are not valid
using GenericTable = Deception::Interpreter::Conclave::GenericInputEntry;
using CustomTable = Deception::Interpreter::Conclave::CustomInputEntry;

template<typename Interpreter>
using StringConstructionTable = Deception::StringConstructionTable<Interpreter>;

int
main(int argc, char** argv) {
    Deception::Interpreter theInterpreter{
            {
                    GenericTable { "skip next character", {{}, [](auto&&) {}, [](auto&&) {}, [](auto& interpreter, char) { interpreter.restore(); } }},
                    CustomTable { "single line comment", std::make_shared<Deception::DropCharactersUntil<Deception::Interpreter>>('\n') },
                    CustomTable { "multi line comment", std::make_shared<Deception::DropCharactersUntil<Deception::Interpreter>>(')')},
                    CustomTable { "read string", std::make_shared<StringConstructionTable<Deception::Interpreter>>(Deception::Opcodes::TopLevelCodes::EndMakeString) },
                    CustomTable { "read line", std::make_shared<StringConstructionTable<Deception::Interpreter>>('\n') },
                    GenericTable {
                            "core", {
                                    { Deception::Opcodes::Ascii::EOT, [](auto& interpreter, char) { interpreter.terminate(); } },
                                    { 'q', [](auto& interpreter, char) { interpreter.useInputStream(Deception::Opcodes::Ascii::EOT); } },
                                    { '#', [](Deception::Interpreter& interpreter, char) {interpreter.use("single line comment"); } },
                                    { '!', [](auto& interpreter, char) { interpreter.use("read line"); }},
                                    { Deception::Opcodes::TopLevelCodes::StartMakeString, [](auto& interpreter, char) { interpreter.use("read string"); } },
                                    { Deception::Opcodes::TopLevelCodes::SkipNextCharacter, [](auto& interpreter, char) { interpreter.use("skip next character"); } },
                                    { Deception::Opcodes::TopLevelCodes::SwitchToTableFromStack, [](auto& interpreter, char) { interpreter.useFromStack(); } },
                                    { '.', Deception::displayTopItemOnDataStack },
                                    { '?', Deception::displayCurrentTableContents },
                                    { '(', [](auto& interpreter, char) { interpreter.use("multi line comment"); }},
                            }
                    }
            }
    };
    theInterpreter.use("core");
    std::cout << "CTRL-D to quit" << std::endl;
    theInterpreter.run();
    return 0;
}
