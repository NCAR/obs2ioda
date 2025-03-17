#include "ioda_schema.h"

IodaSchema::IodaSchema(YAML::Node schema) {
    for (auto attribute: schema["Attributes"]) {
        if (attribute["Attribute"]) {
            auto iodaAttribute = std::make_shared<IodaAttribute>();
            iodaAttribute->name = attribute["Attribute"].begin()->as<std::string>();
            for (auto attr: attribute["Attribute"]) {
                this->attributes[attr.as<std::string>()] = iodaAttribute;
            }
        }
    }
    for (auto variable: schema["Variables"]) {
        if (variable["Variable"]) {
            std::shared_ptr<IodaVariable> iodaVariable = std::make_shared<IodaVariable>();
            iodaVariable->name = variable["Variable"].begin()->as<std::string>();
            for (auto var: variable["Variable"]) {
                this->variables[var.as<std::string>()] = iodaVariable;
            }
        }
    }

    for (auto dimension: schema["Dimensions"]) {
        if (dimension["Dimension"]) {
            auto iodaDimension = std::make_shared<IodaDimension>();
            iodaDimension->name = dimension["Dimension"].begin()->as<std::string>();
            for (auto dim: dimension["Dimension"]) {
                this->dimensions[dim.as<std::string>()] = iodaDimension;
            }
        }
    }

    for (auto group: schema["Groups"]) {
        if (group["Group"]) {
            auto iodaGroup = std::make_shared<IodaGroup>();
            iodaGroup->name = group["Group"].begin()->as<std::string>();
            for (auto grp: group["Group"]) {
                this->groups[grp.as<std::string>()] = iodaGroup;
            }
        }
    }
}

std::shared_ptr<IodaAttribute> IodaSchema::getAttribute(const std::string& name) {
    std::shared_ptr<IodaAttribute> iodaAttribute;
    if (this->attributes.find(name) != this->attributes.end()) {
        iodaAttribute = this->attributes[name];
    }
    else {
        iodaAttribute = std::make_shared<IodaAttribute>();
        iodaAttribute->name = name;
    }
    return iodaAttribute;
}

std::shared_ptr<IodaVariable> IodaSchema::getVariable(const std::string& name) {
    std::shared_ptr<IodaVariable> iodaVariable;
    if (this->variables.find(name) != this->variables.end()) {
        iodaVariable = this->variables[name];
    }
    else {
        iodaVariable = std::make_shared<IodaVariable>();
        iodaVariable->name = name;
    }
    return iodaVariable;
}

std::shared_ptr<IodaDimension> IodaSchema::getDimension(const std::string& name) {
    std::shared_ptr<IodaDimension> iodaDimension;
    if (this->dimensions.find(name) != this->dimensions.end()) {
        iodaDimension = this->dimensions[name];
    }
    else {
        iodaDimension = std::make_shared<IodaDimension>();
        iodaDimension->name = name;
    }
    return iodaDimension;
}

std::shared_ptr<IodaGroup> IodaSchema::getGroup(const std::string& name) {
    std::shared_ptr<IodaGroup> iodaGroup;
    if (this->groups.find(name) != this->groups.end()) {
        iodaGroup = this->groups[name];
    }
    else {
        iodaGroup = std::make_shared<IodaGroup>();
        iodaGroup->name = name;
    }
    return iodaGroup;
}
