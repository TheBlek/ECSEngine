#pragma once

#include <vector>
#include <array>
#include <bitset>
#include <cassert>
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
	
	// WARNING!!! FOR INTERNAL USE ONLY - USES MAGIC. SEE CONSTRUCTOR
	void AddSignature(Signature &signature, unsigned int *signature_id) {
		ValidateSignatureID(*signature_id);
        signatures[*signature_id] = signature;
		(*signature_id)++;
	}

    void AddSignature(Signature&& signature, unsigned int *signature_id) {
        AddSignature(signature, signature_id);
    }

public:
    std::vector<Signature> signatures;

	template<typename ...Signatures>
    System(Engine& engine, Signatures... signatures) 
        : _engine(engine), _targets(sizeof...(Signatures)), _current_entities(sizeof...(Signatures)), signatures(sizeof...(Signatures)) {

		unsigned int signature_id = 0u;
		(AddSignature(signatures, &signature_id), ...);
	 }

    void AddEntity(Entity entity, size_t type) {
        assert(!IsEntityProccessed(entity, type) && "This entity has already been added");
        
        _targets[type].push_back(entity);
        _current_entities[type].set(entity);
    }

    void RemoveEntity(Entity entity, size_t type) {
        assert(IsEntityProccessed(entity, type) && "This entity hadn't been added");

        auto it = _targets[type].begin();
        for (; *it != entity; it++);

        _current_entities[type].reset(*it);
        _targets[type].erase(it);
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
