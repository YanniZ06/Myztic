#pragma once

#include <SDL_stdinc.h>
#include <map>
// #include <Application.h>

namespace Myztic {
    class IDGenerator {
    public:
        // Accesses the ID generator for that specific id-type and creates a new one if the id-type doesnt exist yet
        inline static IDGenerator* instanceOf(const char* type) {
            if (!idTable[type]) {
                idTable[type] = new IDGenerator();
            }
            return idTable[type];
        }

        // Removes the ID generator entry for that specific id-type and returns true if one was found, otherwise false
        inline static bool removeInstance(const char* type) {
            if (idTable[type]) {
                idTable.erase(type);
                return true;
            }
            return false;
        }

        // Gets the next ID in line, used with instanceOf
        inline uint32_t next() { return _id++; }
    private:
        IDGenerator() : _id(0) {}

        static std::map<const char*, IDGenerator*> idTable;
        uint32_t _id;
    };
}
