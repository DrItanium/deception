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
//
// Created by jwscoggins on 5/21/24.
//

#ifndef DECEPTION_INTERPRETER_H
#define DECEPTION_INTERPRETER_H
#include <istream>
#include <core/Table.h>
#include <core/Conclave.h>
namespace Deception {
    class Interpreter {
    public:
        using Conclave = Deception::Conclave<Interpreter>;
        using Table = Deception::Table<Interpreter>;
        using TableReference = Conclave::TableReference;
        Interpreter() = default;
        Interpreter(const Table& initialTable);
        Interpreter(std::initializer_list<Table> tables);
        void use(TableReference ptr);
        void restore();
        void run();
        char next();
        bool stopProcessing() const noexcept;
        template<typename ... Ts>
        TableReference newTable(Ts&& ... args) noexcept {
            return _tables.newTable(args...);
        }
        auto firstTable() const noexcept { return _tables.front(); }
        auto firstTable() noexcept { return _tables.front(); }
        auto lastTable() const noexcept { return _tables.back(); }
        auto lastTable() noexcept { return _tables.back(); }
        auto operator[](Conclave::BackingStore::size_type index) noexcept { return _tables[index]; }
        auto operator[](Conclave::BackingStore::size_type index) const noexcept { return _tables[index]; }
    private:
        std::stack<Table::SharedPtr> _executionStack;
        Table::SharedPtr _current;
        Conclave _tables;
        std::istream* _currentStream;
    };
} // end namespace Deception
#endif //DECEPTION_INTERPRETER_H
