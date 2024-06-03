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

/*
 * This file contains the concept of a value which can be stashed into the an interpreter or whatever else we want.
 * When working with deception, we setup execution tables that perform an action and then stash the result back in
 * the interpreter. It is a stack like what one sees in forth but far more robust. It uses std::variant to make sure
 * that different kinds of types can be returned. It is also optional so that we can denote a failure as well
 *
 */

#ifndef DECEPTION_VALUE_H
#define DECEPTION_VALUE_H

#include <variant>
#include <optional>
#include <string>
#include <cstdint>

namespace Deception {
    using Integer = int64_t;
    using Ordinal = uint64_t;
    using Character = char;
    using Boolean = bool;
    /**
     * A given value that can be put into the stack as needed
     */
    using RawValue = std::variant<std::string, Integer, Ordinal, Character, Boolean>;
    /**
     * @brief a Value is a thing that can be null or contain a value, it is generally something that is pushed onto the interpreter stack in cases where that makes sense!
     */
    using Value = std::optional<RawValue>;
} // end namespace Deception
#endif //DECEPTION_VALUE_H
