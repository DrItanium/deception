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
    Interpreter::Interpreter(std::initializer_list<Conclave::InputEntry> tables, std::initializer_list<StreamType> streamStack, char term) : _tables(tables), _inputStreams(streamStack), _terminationCharacter(term) { }
    Interpreter::Interpreter(std::initializer_list<Conclave::InputEntry> tables, char term) : Interpreter(tables, {std::experimental::make_observer<std::istream>(&std::cin)}, term) { }

    void
    Interpreter::use(const std::string& name) {
        use(_tables.find(name));
    }
    void
    Interpreter::use(TableReference ptr) {
        if (getCurrentTable()) {
            getCurrentTable()->leaveTable(*this);
        }
        if (ptr) {
            _executionStack.push(ptr);
            getCurrentTable()->enterTable(*this);
        }
    }
    void
    Interpreter::restore() {
        if (!_executionStack.empty()) {
            getCurrentTable()->leaveTable(*this);
            _executionStack.pop();
        }
    }
    void
    Interpreter::run() {
        do {
            if (auto current = next(); stopProcessing()) {
                break;
            } else {
                (*getCurrentTable())(current, *this);
            }
        } while (true);
    }

    void
    Interpreter::terminate() noexcept {
        _executing = false;
    }

    std::optional<Value>
    Interpreter::popElement() noexcept {
        if (_dataStack.empty())  {
            return std::nullopt;
        } else {
            Value result = _dataStack.back();
            _dataStack.pop_back();
            return result;
        }
    }
    bool
    Interpreter::dataStackEmpty() const noexcept { return _dataStack.empty(); }

    bool
    Interpreter::currentStreamValid() const noexcept {
        return !_inputStreams.empty() && std::visit([](auto&& stream) { return stream.operator bool(); }, getCurrentStream());
    }
    const Interpreter::StreamType&
    Interpreter::getCurrentStream() const noexcept {
        return _inputStreams.back();
    }
    Interpreter::StreamType&
    Interpreter::getCurrentStream() noexcept {
        return _inputStreams.back();
    }
    Interpreter::StreamResult
    Interpreter::next() {
        if (!currentStreamValid()) {
            // see if we can go to the next entry in the stream set
            if (_inputStreams.empty()) {
                // we have nothing left to process so just mark the interpreter as done and terminate, return the termination character as well
                terminate();
                return std::nullopt;
            }
            // we are done so go back to the previous one
            restoreInputStream();
        }
        return std::visit([](auto&& stream) -> char { return static_cast<char>(stream->get()); }, getCurrentStream());
    }

    bool
    Interpreter::stopProcessing() const noexcept {
        return !_executing || _inputStreams.empty();
    }
    void
    Interpreter::restoreInputStream() {
        _inputStreams.pop_back();
    }
    void
    Interpreter::useInputStream(const std::string& stream) {
        useInputStream(std::make_shared<std::stringstream>(stream));
    }
    void
    Interpreter::useFromStack() {
        if ()
    }
} // end namespace Deception
