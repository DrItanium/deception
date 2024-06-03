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
#include <core/Value.h>
#include <core/Table.h>
#include <core/Conclave.h>
namespace Deception {
    class Interpreter {
    public:
        using Conclave = Deception::Conclave<Interpreter>;
        using Table = Deception::Table<Interpreter>;
        using TableReference = Conclave::TableReference;
        using ListEntry = typename Conclave::InputEntry;
        Interpreter() = default;
        Interpreter(std::initializer_list<ListEntry> tables);
        void use(const std::string& name);
        void use(TableReference ptr);
        void restore();
        void run();
        char next();
        bool stopProcessing() const noexcept;
        [[nodiscard]] TableReference getCurrentTable() noexcept { return _current; }
        auto operator[](const Conclave::BackingStore::key_type& index) noexcept { return _tables[index]; }
        auto operator[](Conclave::BackingStore::key_type&& index) noexcept { return _tables[index]; }
        void terminate() noexcept;
        std::optional<Value> popElement() noexcept {
           if (_dataStack.empty())  {
               return std::nullopt;
           } else {
               Value result = _dataStack.top();
               _dataStack.pop();
               return result;
           }
        }
        template<typename T>
        void pushElement(T value) noexcept {
            _dataStack.push(value);
        }
        bool dataStackEmpty() const noexcept { return _dataStack.empty(); }
    private:
        std::stack<Value> _dataStack;
        std::stack<Table::SharedPtr> _executionStack;
        Table::SharedPtr _current = nullptr;
        Conclave _tables;
        std::istream* _currentStream = nullptr;
        bool _executing = true;
    };
} // end namespace Deception
#endif //DECEPTION_INTERPRETER_H
