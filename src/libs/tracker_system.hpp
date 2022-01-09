#pragma once

template<typename T, typename K>
class TrackerSystem : public System {
    
    std::function<T()> _function1;
    std::function<K()> _function2;

    std::vector<std::pair<T, K>> _data;

    bool _is_tracking;
public:
    TrackerSystem(Engine& engine) : System(engine, 0) {
        _is_tracking = false;
    }

    TrackerSystem(Engine& engine, T& value1, K& value2) : System(engine, 0) {
        _function1 = [&](){return value1;};
        _function2 = [&](){return value2;};

        _is_tracking = true;
    }

    TrackerSystem(Engine& engine, T& value1, std::function<K()> func2) : System(engine, 0) {
        _function1 = [&](){return value1;};
        _function2 = func2;

        _is_tracking = true;
    }

    TrackerSystem(Engine& engine, std::function<T()> func1, K& value2) : System(engine, 0) {
        _function1 = func1;
        _function2 = [&](){return value2;};

        _is_tracking = true;
    }

    TrackerSystem(Engine& engine, std::function<T()> func1, std::function<K()> func2) : System(engine, 0) {
        _function1 = func1;
        _function2 = func2;

        _is_tracking = true;
    }
    
    void Update(float dt) override {
        if (_is_tracking)
            _data.push_back({_function1(), _function2()});
    }

	std::vector<std::pair<T, K>> operator()() const {
		return _data;
	}

    std::vector<std::pair<T, K>> Pop() {
		auto tmp = _data;
		Reset();
        return tmp;
    }

    void Reset() {
        _data.clear();
    }
};
