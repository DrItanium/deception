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
#include <sstream>
#include <list>
#include <memory>
#include <experimental/memory>
#include <core/Value.h>
#include <core/Table.h>
#include <core/Conclave.h>
#include <core/MemorySpace.h>
namespace Deception {
    class Interpreter {
    public:
        using Conclave = Deception::Conclave<Interpreter>;
        using Table = Deception::Table<Interpreter>;
        using TableReference = Conclave::GenericTableReference;
        using ListEntry = typename Conclave::InputEntry;
        using DataStack = std::list<Value>;
        using ExecutionStack = std::stack<TableReference>;
        using StreamType = InputStream;
        using StreamStack = InputStreamStack;
        using StreamResult = StreamReadResult;
        Interpreter(std::initializer_list<ListEntry> tables, std::initializer_list<StreamType> startingStreamEntries, Address capacity = (256 * 1024 * 1024));
        Interpreter(std::initializer_list<ListEntry> tables, Address capacity = (256*1024*1024));
        void use(const std::string& name);
        void use(TableReference ptr);
        void useFromStack();
        void restore();
        void run();
        StreamResult next();
        bool stopProcessing() const noexcept;
        [[nodiscard]] TableReference getCurrentTable() noexcept { return _executionStack.empty() ? nullptr : _executionStack.top(); }
        auto operator[](const Conclave::BackingStore::key_type& index) noexcept { return _tables[index]; }
        auto operator[](Conclave::BackingStore::key_type&& index) noexcept { return _tables[index]; }
        void terminate() noexcept;
        Value popElement() noexcept;
        [[nodiscard]] bool dataStackEmpty() const noexcept;
        template<typename T>
        void pushElement(T value) noexcept {
            _dataStack.push_back(value);
        }
        auto dataStackBegin() const noexcept { return _dataStack.cbegin(); }
        auto dataStackEnd() const noexcept { return _dataStack.cend(); }
        auto dataStackReverseBegin() const noexcept { return _dataStack.crbegin(); }
        auto dataStackReverseEnd() const noexcept { return _dataStack.crend(); }
        [[nodiscard]] auto dataStackSize() const noexcept { return _dataStack.size(); }
        [[nodiscard]] bool currentStreamValid() const noexcept;
        StreamType& getCurrentStream() noexcept;
        const StreamType& getCurrentStream() const noexcept;
        template<typename T>
        void useInputStream(T stream) {
            _inputStreams.emplace_back(stream);
        }
        void useInputStream(const std::string& stream);
        void useInputStream(char c);
        void restoreInputStream();
        void clearOutputStream() {
            _currentOutputStream.str("");
        }
        void putIntoOutputStream(const std::string& str) {
            _currentOutputStream << str;
        }
        void putIntoOutputStream(char c) {
            _currentOutputStream.put(c);
        }
        void moveOutputToStack() {
            std::string tmp = _currentOutputStream.str();
            pushElement(tmp);
        }
        constexpr auto memoryCapacity() const noexcept { return _memory.size(); }
        /**
         * Return the list of all the opcodes previously executed minus the context of the tables used for execution (although it should not be that much of a problem overall to track!)
         * @return The sequence of operations executed in a single string (probably don't want to print this one!)
         */
        std::string getPreviousExecution() const noexcept { return _previousExecution.str(); }
    private:
        DataStack _dataStack;
        ExecutionStack _executionStack;
        Conclave _tables;
        bool _executing = true;
        StreamStack _inputStreams;
        std::stringstream _currentOutputStream;
        MemorySpace _memory;
        std::stringstream _previousExecution;
    private:
        static inline StreamType noStream{ ObservedInputStream (nullptr) };
    };
    void displayCurrentTableContents(Deception::Interpreter& interpreter, char);
    void displayTopItemOnDataStack(Deception::Interpreter& interpreter, char);
} // end namespace Deception
#endif //DECEPTION_INTERPRETER_H
