#pragma once

#include "logger.hpp"
#include "component.hpp"
#include "system.hpp"
#include "entity.hpp"

#include <cassert>
#include <unordered_map>
#include <utility>
#include <chrono>
#include <memory>
#include <random>

class Engine {
public:
    std::mt19937 rng;

    Engine() {
        _signatures = PackedArray<Signature, MAX_ENTITIES>();
        _components = PackedArray<IComponentArray*, MAX_ENTITIES>();
        _systems = PackedArray<std::shared_ptr<System>, MAX_SYSTEMS>();
        _name_to_component_index = std::unordered_map<std::string, Component>();
        _time = 0;
        _last_update = std::chrono::high_resolution_clock::now();

        std::random_device rd;
        rng = std::mt19937(rd());
    }

    float GetSimulationTime() {
        return _time;
    }

    Entity CreateEntity() {
        return _signatures.AddData(Signature());
    }

    void DeleteEntity(Entity entity) {
        _signatures.RemoveData(entity);

        for (int i = 0; i < _components.GetSize(); i++) {
            _components.entries[i]->OnEntityDeletion(entity);
        }

		for (size_t i = 0; i < _systems.size(); i++) {
			auto system = _systems.entries[i];
			for (size_t j = 0; j < system->GetSignatureCount(); j++) {
				if (system->IsEntityProccessed(entity, j))
					system->RemoveEntity(entity, j);
			}
		}
    }

    Signature& GetSignature(Entity entity) {
        return _signatures.GetData(entity);
    }

    template<typename T>
    void RegisterComponentType() {

        std::string type_name = typeid(T).name();

        assert(_name_to_component_index.find(type_name) == _name_to_component_index.end() && "This component has been already registered");

        IComponentArray* component_array = new ComponentArray<T>();
        _name_to_component_index.insert(make_pair(type_name, _components.AddData(component_array)));
    }

    template<typename ...Args>
    void RegisterComponentTypes() {
        (RegisterComponentType<Args>(), ...);
    }

    template<typename T>
    T& AddComponent(Entity entity) {
        SetComponent(entity, T());
		return GetComponent<T>(entity);
    }

    template<typename T>
    void SetComponent(Entity entity, const T& component) {
        VerifyComponentRegistration<T>();

        ComponentArray<T>& component_array = GetComponentArray<T>();
        component_array.SetData(entity, component);

        Signature& signature = GetSignature(entity);
        signature.AddComponent(GetComponentID<T>());


        for (int i = 0; i < _systems.GetSize(); i++) {
            auto system = _systems.entries[i];

            // We need to check for every signature system has
            for (int j = 0; j < system->GetSignatureCount(); j++) {
                // If it is not proccessed but is valid now
                if (!system->IsEntityProccessed(entity, j) && 
                    signature.IsSufficientFor( system->signatures[j] )) {

                    system->AddEntity(entity, j);
                }
            }
        }
    }

    template<typename T>
    void RemoveComponent(Entity entity) {
        VerifyComponentRegistration<T>();

        ComponentArray<T> component_array = GetComponentArray<T>();
        component_array.RemoveData(entity);

        Signature& signature = GetSignature(entity);
        signature.RemoveComponent(GetComponentID<T>());

        for (int i = 0; i < _systems.GetSize(); i++) {
            System* system = _systems.entries[i];

            // We need to check for every signature system has
            for (int j = 0; j < system->GetSignatureCount(); j++) {

                // If it is proccessed but not valid now - remove
                if (system->IsEntityProccessed(entity, j) && 
                    !signature.IsSufficientFor( system->signatures[j] )) {

                    system->RemoveEntity(entity, j);
                }
            }
        }
    }

    template<typename T>
    T& GetComponent(Entity entity) {

        ComponentArray<T>& component_array = GetComponentArray<T>();

        return component_array.GetData(entity);
    }

    template<typename T>
    std::vector<std::reference_wrapper<T>> GetComponentList(std::vector<Entity>& entities) {
        std::vector<std::reference_wrapper<T>> result;
        result.reserve(entities.size());

        for (Entity entity : entities)
            result.push_back(std::reference_wrapper<T>(GetComponent<T>(entity)));

        return result;
    }

	template<typename T, typename ...Args>
	T& RegisterSystem(Args... args) {
		std::shared_ptr<T> system_ptr = std::make_shared<T>(*this, args...);
		AppendSystemPtr(std::static_pointer_cast<System>(system_ptr));
		return *system_ptr;
	}

    template<typename ...Args>
    void RegisterSystems() {
        (RegisterSystem<Args>(), ...);
    }

    Entity GetEntityCount() const {
        return _signatures.GetSize();
    }

    void Update(float dt=-1.0f) {
        if (dt == -1.0f) {
            auto now = std::chrono::high_resolution_clock::now();
            dt = std::chrono::duration<float>(now - _last_update).count();
            _last_update = now;
        }

        _time += dt;

        for (int i = 0; i < _systems.GetSize(); i++) {
            _systems.entries[i]->Update(dt);
        }
    }

    void RunForSeconds(double duration, float dt=-1.0f) {
        while (_time < duration)
            Update(dt);
    }

    template<typename T> 
    Component GetComponentID() {
        VerifyComponentRegistration<T>();
        std::string type_name = typeid(T).name();

        return _name_to_component_index.find(type_name)->second;
    }

    template<typename T>
    void AddComponentToSignature(Signature& signature) {
        signature.AddComponent(GetComponentID<T>());
    }

    template<typename ...Params>
    Signature ConstructSignature(){
        Signature signature;
        (AddComponentToSignature<Params>(signature), ...);
        return signature;
    }
private:	
    PackedArray<Signature, MAX_ENTITIES> _signatures;
    PackedArray<IComponentArray*, MAX_ENTITIES> _components;
    PackedArray<std::shared_ptr<System>, MAX_SYSTEMS> _systems;
    std::unordered_map<std::string, Component> _name_to_component_index;

    float _time;
    std::chrono::time_point<std::chrono::high_resolution_clock> _last_update;
    
	void AppendSystemPtr(std::shared_ptr<System> system) {
        _systems.AddData(system);

        // For every entity check if it is required by the system
        for (int i = 0; i < GetEntityCount(); i++) {
            for (int j = 0; j < system->GetSignatureCount(); j++) {
                if (_signatures.entries[i].IsSufficientFor(system->signatures[j])) {
                    system->AddEntity(i, j);
                }
            }
        }
    }

    template<typename T>
    ComponentArray<T>& GetComponentArray() {
        IComponentArray*& base_array = _components.GetData(GetComponentID<T>());
        return static_cast<ComponentArray<T>&>(*base_array);
    }

    template<typename T>
    void VerifyComponentRegistration() {
        std::string type_name = typeid(T).name();

        assert(_name_to_component_index.find(type_name) != _name_to_component_index.end() && "This component hasn't been registered");
    }
};
