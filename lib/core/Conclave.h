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

#ifndef DECEPTION_CONCLAVE_H
#define DECEPTION_CONCLAVE_H
#include <map>
#include <core/Table.h>
namespace Deception {
    template<typename Interpreter>
    class Conclave {
    public:
        using Table_t = Table<Interpreter>;
        using TableReference = Table_t::SharedPtr;
        using BackingStore = std::map<std::string, TableReference>;
        Conclave() = default;
        Conclave(std::initializer_list<Table_t> list) {
            for (auto& a : list) {
                _backingStore.emplace_back(std::make_shared<Table_t>(a));
            }
        }
        Conclave(const Conclave&) = default;
        Conclave(Conclave&&) = default;
        auto front() const noexcept { return _backingStore.front(); }
        auto front() noexcept { return _backingStore.front(); }
        auto back() const noexcept { return _backingStore.back(); }
        auto back() noexcept { return _backingStore.back(); }
        auto size() const noexcept { return _backingStore.size(); }
        auto operator[](BackingStore::size_type index) const noexcept { return _backingStore[index]; }
        auto operator[](BackingStore::size_type index) noexcept { return _backingStore[index]; }
        template<typename ... Ts>
        TableReference newTable(Ts&&... args) noexcept {
            _backingStore.emplace_back(args...);
            return back();
        }
    private:
        BackingStore _backingStore;
    };
} // end namespace Deception


#endif //DECEPTION_CONCLAVE_H
