#ifndef OBJECT_REGISTRY_H
#define OBJECT_REGISTRY_H
#include <unordered_map>
#include <string>

template<typename T>
class ObjectRegistry {
public:

    void addObject(int objectID,
        std::shared_ptr<T> object) {
        objectRegistry[objectID] = std::move(object);
    }

    std::shared_ptr<T> getObject(const int objectID) {
        const auto it = objectRegistry.find(objectID);
        if (it == objectRegistry.end()) {
            throw std::runtime_error(
                ("Object ID not found in the Object map: " +
                 std::to_string(objectID)).c_str()
            );
        }
        return it->second;
    }
    void removeObject(int objectID) {
        auto objectIterator = this->objectRegistry.find(objectID);
        if (objectIterator == this->objectRegistry.end()) {
            throw std::runtime_error(
                ("Object ID not found in the object registry: " +
                 std::to_string(objectID)).c_str()
            );
        }
        this->objectRegistry.erase(objectIterator);
    }

private:
    std::unordered_map<int, std::shared_ptr<T> >
    objectRegistry;
};
#endif //OBJECT_REGISTRY_H
