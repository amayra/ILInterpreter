#pragma once

#include "types.hpp"

#include <stack>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>
#include <cstring>
#include "logger.hpp"

namespace ili {

    struct VariableBase {
        Type type;
    };

    template<typename T>
    struct Variable : public VariableBase {
        T value;
    };

    class Method;
    class DLL;


    struct Context {
        DLL *dll = nullptr;

        u8 *stackPointer = nullptr;
        u8 *framePointer = nullptr;
        u8 *stack;

        Type *typeStackPointer = nullptr;
        Type *typeFramePointer = nullptr;
        Type *typeStack;

        std::unordered_map<std::string, std::function<void()>> nativeFunctions;



        Type getTypeOnStack(u16 pos = 0) {
            return *(typeStackPointer - 1 - pos);
        }

        template<typename T>
        T pop() {
            T ret;

            typeStackPointer--;
            stackPointer -= sizeof(T);

            std::memset(&ret, 0x00, sizeof(T));
            std::memcpy(&ret, stackPointer, getTypeSize(getTypeOnStack()));

            Logger::debug("Popped %d bytes from stack: %08lx", sizeof(T), ret);

            return ret;
        }

        template<typename T>
        void push(Type type, T val) {

            std::memcpy(stackPointer, &val, getTypeSize(type));
            *typeStackPointer = type;

            typeStackPointer++;
            stackPointer += sizeof(T);

            Logger::debug("Pushed %d bytes onto stack: %08lx", sizeof(T), val);
        }
    };

}