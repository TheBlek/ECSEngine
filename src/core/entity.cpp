#include "entity.hpp"
#include "constants.hpp"
#include <cassert>
#include <bitset>

void Signature::VerifyID(Component id) {
    assert(id >= 0 && id < components.size() && "Given component id is out of range");
}

void Signature::AddComponent(Component id) {
    VerifyID(id);
    components.set(id);
}

void Signature::RemoveComponent(Component id) {
    VerifyID(id);
    components.reset(id);
}

bool Signature::IsSufficientFor(Signature& gate) {
    return (components & gate.components).count() == gate.components.count();
}

void Signature::Reset() {
    components.reset();
}