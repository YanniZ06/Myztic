#pragma once

#include <SDL_stdinc.h>

class IDGenerator {
public:
    static IDGenerator* instance();
    uint32_t next() { return _id++; }
private:
    IDGenerator() : _id(0) {}

    static IDGenerator* only_copy;
    uint32_t _id;
};