#pragma once

#include "constants.hpp"
#include <bitset>

struct Signature {
    void AddComponent(Component id);

    void RemoveComponent(Component id);

    void Reset();

    bool IsSufficientFor(Signature& gate);

    std::bitset<MAX_COMPONENTS> components;
private:
    void VerifyID(Component id);
};