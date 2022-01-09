#pragma once

#include <vector>
#include <array>
#include <bitset>
#include "entity.hpp"

class Engine;

class System {
protected:
    Engine& _engine;
    std::vector<std::vector<Entity>> _targets;
    std::vector<std::bitset<MAX_ENTITIES>> _current_entities;

    void ValidateSignatureID(size_t id) const{
        assert(id >= 0 && id < GetSignatureCount() && "Signature ID is out of bounds");
    }

public:
    std::vector<Signature> signatures;

    System(Engine& engine, size_t signature_count) 
        : _engine(engine), _targets(signature_count), _current_entities(signature_count), signatures(signature_count) { }

    void AddEntity(Entity entity, size_t type) {
        assert(!IsEntityProccessed(entity, type) && "This entity has already been added");
        
        _targets[type].push_back(entity);
        _current_entities[type].set(entity);
    }

    void RemoveEntity(Entity entity, size_t type) {
        assert(IsEntityProccessed(entity, type) && "This entity hadn't been added");

        auto it = _targets[type].begin();
        for (; *it != entity; it++);

		
		//Logger::LogAdvanced("Is this I was supposed to delete? - %d", *(it));
        _current_entities[type].reset(*it);
        _targets[type].erase(it);
    }

    void SetSignature(size_t id, Signature& signature) {
        ValidateSignatureID(id);
        signatures[id] = signature;
    }

    void SetSignature(size_t id, Signature&& signature) {
        SetSignature(id, signature);
    }

    bool IsEntityProccessed(Entity entity, size_t type) const {
        ValidateSignatureID(type);
        return _current_entities[type].test(entity);
    }

    size_t GetSignatureCount() const {
        return _targets.size();
    }

    void virtual Update(float dt) = 0;
};
