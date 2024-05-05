#include "pch.h"

#include <Util.h>

IDGenerator* IDGenerator::only_copy;
IDGenerator* IDGenerator::instance() {
    if (!only_copy) {
        only_copy = new IDGenerator();
    }
    return only_copy;
}