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

#ifndef DECEPTION_TABLE_H
#define DECEPTION_TABLE_H
#include <map>
#include <functional>
#include <memory>
#include <stack>
#include <string>
#include <iostream>
#include <optional>
namespace Deception {
    template<typename Interpreter>
    struct GenericTable {
        using ExecutionBody = std::function<void(Interpreter& self, char character)>;
        using LookupResult = std::optional<ExecutionBody>;
        using SharedPtr = std::shared_ptr<GenericTable<Interpreter>>;
        GenericTable() = default;
        virtual ~GenericTable() = default;
        virtual LookupResult lookup(char c) = 0;
        virtual void run(char c, Interpreter& interpreter) {
            if (auto result = lookup(c); result) {
                (*result)(interpreter, c);
            } else {
                defaultImplementation(interpreter, c);
            }
        }
        void operator()(char c, Interpreter& i) noexcept { run(c, i); }
        virtual void enterTable(Interpreter&) { }
        virtual void leaveTable(Interpreter&) { }
        virtual void defaultImplementation(Interpreter&, char) { }
    };
    template<typename Interpreter>
    class Table : public GenericTable<Interpreter> {
    public:
        using Parent = GenericTable<Interpreter>;
        using ExecutionBody = Parent::ExecutionBody;
        using LookupResult = Parent::LookupResult;
        using TableEnterFunction = std::function<void(Interpreter&)>;
        using TableExitFunction = std::function<void(Interpreter&)>;
        using DispatchTable = std::map<char, ExecutionBody>;
        using SharedPtr = std::shared_ptr<Table<Interpreter>>;
        using InitializerList = std::initializer_list<typename DispatchTable::value_type>;
    private:
        static void fallbackNothing(Interpreter&, char) { }
        static void emptyEnterExitFunction(Interpreter&) {}
    public:
        Table() = default;
        Table(InitializerList items, TableEnterFunction onEnter = emptyEnterExitFunction, TableExitFunction onLeave = emptyEnterExitFunction, ExecutionBody defaultState = fallbackNothing) : _table(items), _onEnter(onEnter), _onLeave(onLeave), _fallback(defaultState) { }
        Table(const Table& other) = default;
        Table(Table&& other) = default;
        virtual ~Table() = default;
        decltype(auto) end() noexcept { return _table.end(); }
        decltype(auto) end() const noexcept { return _table.end(); }
        decltype(auto) begin() noexcept { return _table.begin(); }
        decltype(auto) begin() const noexcept { return _table.begin(); }
        decltype(auto) find(char value) noexcept { return _table.find(value); }
        decltype(auto) find(char value) const noexcept { return _table.find(value); }
        LookupResult lookup(char c) override {
            if (auto result = find(c);  result != end()) {
                return result->second;
            } else {
                return std::nullopt;
            }
        }

        template<typename ... Ts>
        decltype(auto) emplace(Ts&&... args) noexcept {
            return _table.emplace(args...);
        }
        void enterTable(Interpreter& interpreter) override {
            _onEnter(interpreter);
        }
        void leaveTable(Interpreter& interpreter) override {
            _onLeave(interpreter);
        }
        void defaultImplementation(Interpreter& interpreter, char c) override {
            _fallback(interpreter, c);
        }
    private:
        DispatchTable _table;
        TableEnterFunction _onEnter = emptyEnterExitFunction;
        TableEnterFunction _onLeave = emptyEnterExitFunction;
        ExecutionBody _fallback = fallbackNothing;
    };
    template<typename Interpreter>
    struct StringConstructionTable : public GenericTable<Interpreter> {
        using Parent = Deception::Table<Interpreter>;
        using LookupResult = Parent::LookupResult;
        explicit StringConstructionTable(char terminatorSymbol) : _terminatorChar(terminatorSymbol) { }
        ~StringConstructionTable() override = default;
        void enterTable(Interpreter& interpreter) override { interpreter.clearOutputStream(); }
        void leaveTable(Interpreter& interpreter) override { interpreter.moveOutputToStack(); }
        void defaultImplementation(Interpreter& interpreter, char c) override { interpreter.putIntoOutputStream(c); }

        LookupResult lookup(char c) override {
            if (c == _terminatorChar)  {
                return [](Interpreter& interpreter, char) { interpreter.restore(); };
            } else {
                return std::nullopt;
            }
        }
        [[nodiscard]] constexpr auto getTerminatorChar() const noexcept { return _terminatorChar; }

    private:
        char _terminatorChar;
    };
    template<typename Interpreter>
    struct DropCharactersUntil : public GenericTable<Interpreter> {
        using Parent = GenericTable<Interpreter>;
        explicit DropCharactersUntil(char until) : _terminatorChar(until) { }

        Parent::LookupResult lookup(char c) override {
            if (c == _terminatorChar) {
                return [](Interpreter& i, char) {
                    i.restore();
                };
            } else {
                return std::nullopt;
            }
        }

    private:
        char _terminatorChar;
    };
} // end namespace Deception

#endif //DECEPTION_TABLE_H
