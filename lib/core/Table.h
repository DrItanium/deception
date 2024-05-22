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
namespace Deception {
    class Table {
    public:
        using ExecutionBody = std::function<void()>;
        using DispatchTable = std::map<char, ExecutionBody>;
    public:
        Table() = default;
        Table(std::initializer_list<DispatchTable::value_type> items) : _table(items) { }
        Table(const Table& other) = default;
        Table(Table&& other) = default;
        decltype(auto) end() noexcept { return _table.end(); }
        decltype(auto) end() const noexcept { return _table.end(); }
        decltype(auto) begin() noexcept { return _table.begin(); }
        decltype(auto) begin() const noexcept { return _table.begin(); }
        decltype(auto) find(char value) noexcept { return _table.find(value); }
        decltype(auto) find(char value) const noexcept { return _table.find(value); }
        template<typename ... Ts>
        decltype(auto) emplace(Ts&&... args) noexcept {
            return _table.emplace(args...);
        }
        decltype(auto) operator[](char&& value) noexcept { return _table.operator[](value); }
        decltype(auto) operator[](const char&& value) noexcept { return _table.operator[](value); }
        void run(char c) noexcept;
        void operator()(char c) noexcept { run(c); }
    private:
        DispatchTable _table;
    };
    using TableReference = std::shared_ptr<Table>;
} // end namespace Deception

#endif //DECEPTION_TABLE_H
