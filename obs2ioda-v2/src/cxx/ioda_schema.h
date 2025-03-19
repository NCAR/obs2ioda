#ifndef IODASCHEMA_H
#define IODASCHEMA_H

#include "yaml-cpp/yaml.h"
#include <unordered_map>
struct IodaVariable {
    std::string name;
};

struct IodaDimension {
    std::string name;
};

struct IodaGroup {
    std::string name;
};

struct IodaAttribute {
    std::string name;
};

class IodaSchema {
    std::unordered_map<std::string, std::shared_ptr<IodaVariable>> variables;
    std::unordered_map<std::string, std::shared_ptr<IodaDimension>> dimensions;
    std::unordered_map<std::string, std::shared_ptr<IodaGroup>> groups;
    std::unordered_map<std::string, std::shared_ptr<IodaAttribute>> attributes;

public:
    explicit IodaSchema(YAML::Node schema);

    std::shared_ptr<IodaAttribute> getAttribute(const std::string& name);
    std::shared_ptr<IodaVariable> getVariable(const std::string& name);
    std::shared_ptr<IodaDimension> getDimension(const std::string& name);
    std::shared_ptr<IodaGroup> getGroup(const std::string& name);
};

#endif //IODASCHEMA_H
