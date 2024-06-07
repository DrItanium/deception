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
// Created by jwscoggins on 6/6/24.
//

#ifndef DECEPTION_MEMORYSPACE_H
#define DECEPTION_MEMORYSPACE_H
#include <memory>
#include <cstdint>
#include <optional>
#include <core/Value.h>
#include <iterator>
namespace Deception {
    /**
     * @brief A block of memory which holds onto characters not bytes.
     */
    class MemorySpace {
    public:
        /**
         * @brief Construct a memory space with a capacity less than four giga characters
         * @param capacity The number of bytes that make up this memory space; Use the no argument version of the constructor for exactly 4G
         */
        explicit MemorySpace(Address capacity);
        /**
         * @brief construct a memory space with a capacity of exactly four giga characters
         */
        explicit MemorySpace();
        /**
         * Get the number of characters stored in this memory pool
         * @return The number of characters available in this memory pool
         */
        [[nodiscard]] constexpr auto size() const noexcept { return _capacity; }
        [[nodiscard]] char& get(Address index) noexcept { return _backingStorage[index]; }
        [[nodiscard]] const char& get(Address index) const noexcept { return _backingStorage[index]; }
        [[nodiscard]] char& operator[](Address index) noexcept { return get(index); }
        [[nodiscard]] const char& operator[](Address index) const noexcept { return get(index); }
        auto rbegin() noexcept { return std::reverse_iterator(_backingStorage.get() + _capacity); }
        auto rbegin() const noexcept { return std::reverse_iterator(_backingStorage.get() + _capacity); }
        auto crbegin() const noexcept { return std::reverse_iterator(_backingStorage.get() + _capacity); }
        auto rend() noexcept { return std::reverse_iterator(_backingStorage.get()); }
        auto rend() const noexcept { return std::reverse_iterator(_backingStorage.get()); }
        auto crend() const noexcept { return std::reverse_iterator(_backingStorage.get()); }
        auto begin() const noexcept { return _backingStorage.get(); }
        auto begin() noexcept { return _backingStorage.get(); }
        auto cbegin() const noexcept { return _backingStorage.get(); }
        auto end() const noexcept { return _backingStorage.get() + _capacity; }
        auto end() noexcept { return _backingStorage.get() + _capacity; }
        auto cend() const noexcept { return _backingStorage.get() + _capacity; }
    private:
        std::size_t _capacity;
        std::unique_ptr<char[]> _backingStorage;
    };
}


#endif //DECEPTION_MEMORYSPACE_H
