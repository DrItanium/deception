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
#include <core/Interpreter.h>
#include <iostream>
namespace Deception {
    Interpreter::Interpreter(std::initializer_list<Conclave::InputEntry> tables) : _tables(tables), _currentStream(&std::cin) { }
    void
    Interpreter::use(const std::string& name) {
        use(_tables.find(name));
    }
    void
    Interpreter::use(TableReference ptr) {
        if (_current) {
            _current->leaveTable(*this);
            _executionStack.push(_current);
        }
        if (ptr) {
            _current = ptr;
            _current->enterTable(*this);
        }
    }
    void
    Interpreter::restore() {
        if (!_executionStack.empty()) {
            _current->leaveTable(*this);
            _current = _executionStack.top();
            _executionStack.pop();
        }
    }
    void
    Interpreter::run() {
        do {
            if (auto current = next(); stopProcessing()) {
                break;
            } else {
                (*_current)(current, *this);
            }
        } while (true);
    }
    char
    Interpreter::next() {
        return static_cast<char>(_currentStream->get());
    }
    bool
    Interpreter::stopProcessing() const noexcept {
        return _currentStream->fail() || !_executing;
    }
    void
    Interpreter::terminate() noexcept {
        _executing = false;
    }
} // end namespace Deception
