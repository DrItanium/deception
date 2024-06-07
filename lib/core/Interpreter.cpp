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
    Interpreter::Interpreter(std::initializer_list<Conclave::InputEntry> tables, std::initializer_list<StreamType> streamStack, Address capacity) : _tables(tables), _inputStreams(streamStack), _memory(capacity) { }
    Interpreter::Interpreter(std::initializer_list<Conclave::InputEntry> tables, Address capacity) : Interpreter(tables, {std::experimental::make_observer<std::istream>(&std::cin)}, capacity) { }

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
            if (auto current = next(); stopProcessing() || !current) {
                break;
            } else {
                (*getCurrentTable())(*current, *this);
            }
        } while (true);
    }

    void
    Interpreter::terminate() noexcept {
        _executing = false;
    }

    Value
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
        return !_inputStreams.empty() && std::visit([](auto&& stream) { return stream->operator bool(); }, getCurrentStream());
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
        return !_executing || _inputStreams.empty() || !currentStreamValid();
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
    Interpreter::useInputStream(char c) {
        useInputStream(std::string(1, c));
    }
    void
    Interpreter::useFromStack() {
        if (auto top = popElement(); top) {
            if (std::holds_alternative<std::string>(*top)) {
                use(std::get<std::string>(*top));
                return;
            } else {
                std::cerr << "useFromStack: Top element not a string!" << std::endl;
            }
        } else {
            std::cerr << "useFromStack: stack is empty!" << std::endl;
        }
        //  report an error
    }
    void
    displayCurrentTableContents(Deception::Interpreter& interpreter, char) {
        auto theTable = interpreter.getCurrentTable();
        for (int i = 0; i < 0x100; ++i) {
            char c = static_cast<char>(i);
            if (auto result = theTable->lookup(c); result) {
                std::cout << c << std::endl;
            }
        }
    }
    void
    displayTopItemOnDataStack(Deception::Interpreter& interpreter, char) {
        if (interpreter.dataStackEmpty())  {
            std::cout << "data stack empty!" << std::endl;
        } else {
            std::cout << "top of stack" << std::endl;
            for (auto i = interpreter.dataStackReverseBegin(); i != interpreter.dataStackReverseEnd(); ++i) {
                if (auto internalValue = *i; internalValue) {
                    std::visit([](auto&& value) { std::cout << "- " << value << std::endl; }, *internalValue);
                } else {
                    std::cout << "- null" << std::endl;
                }
            }
            std::cout << "bottom of stack" << std::endl;
        }
    }
} // end namespace Deception
