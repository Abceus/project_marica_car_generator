#pragma once

#include <functional>
#include <map>

template <typename... ARGS>
class CallbackCollection {
public:
    using CallbackType = std::function<void(ARGS...)>;
    using IDType = size_t;

public:
    IDType Add(const CallbackType& callback) {
        callbacks.emplace(currentId, callback);
        return currentId++;
    }

    void Remove(IDType id) {
        callbacks.erase(id);
    }

    void Call(ARGS... args) {
        for (const auto& [id, callback] : callbacks) {
            callback(args...);
        }
    }

private:
    std::map<IDType, CallbackType> callbacks;
    IDType currentId = 0;
};