#include "ioda_obs_schema.h"

void IodaObsSchemaComponent::setNames(const YAML::Node &node,
                                      const std::string &category) {
    this->names =
            setSequence<std::vector<std::string> >(
                node, category);

    if (!names.empty()) {
        this->validName = this->names.at(0);
    }
}

IodaObsSchemaComponent::IodaObsSchemaComponent(
    std::string componentType,
    std::string name
): componentType(std::move(componentType)) {
    if (!name.empty()) {
        this->names.push_back(std::move(name));
        this->validName = this->names.at(0);
    }
}

const std::vector<std::string> &
IodaObsSchemaComponent::getNames() const {
    return this->names;
}

const std::string &IodaObsSchemaComponent::getValidName() const {
    return this->validName;
}

void IodaObsSchemaComponent::load(const YAML::Node &node) {
    setNames(node, this->componentType);
}


IodaObsAttribute::IodaObsAttribute(
    std::string name): IodaObsSchemaComponent(
    "Attribute", std::move(name)) {
}

IodaObsGroup::IodaObsGroup(std::string name): IodaObsSchemaComponent(
    "Group", std::move(name)
) {
}

IodaObsDimension::IodaObsDimension(
    std::string name): IodaObsSchemaComponent(
    "Dimension", std::move(name)) {
}

void IodaObsVariable::setDimensions(const YAML::Node &node) {
    this->m_dimensions =
            setSequence<std::vector<std::vector<std::string> > >(
                node, "Dimensions");
    if (!this->m_dimensions.empty()) {
        this->m_validDimensions = this->m_dimensions.at(0);
    }
}

IodaObsVariable::IodaObsVariable(
    std::string name): IodaObsSchemaComponent(
    "Variable", std::move(name)) {
}

void IodaObsVariable::load(const YAML::Node &node) {
    static constexpr std::array<const char *, 2> keys = {
        "Variable", "Dimension"
    };
    for (const auto &key: keys) {
        if (node[key] && node[key].begin() != node[key].end()) {
            this->setNames(node, key);
            this->setDimensions(node);
            break;
        }
    }
}

std::vector<std::vector<std::string> >
IodaObsVariable::getDimensions() const {
    return m_dimensions;
}

std::vector<std::string> IodaObsVariable::getValidDimensions() const {
    return m_validDimensions;
}

IodaObsSchema::IodaObsSchema(const YAML::Node &schema) {
    this->loadComponent<IodaObsAttribute>(
        schema, "Attributes", "Attribute", this->attributes
    );
    this->loadComponent<
        IodaObsGroup>(schema, "Groups", "Group", groups);
    this->loadComponent<IodaObsDimension>(
        schema, "Dimensions", "Dimension", this->dimensions
    );
    this->loadComponent<IodaObsVariable>(
        schema, "Variables", "Variable", this->variables
    );
    this->loadComponent<IodaObsVariable>(
        schema, "Dimensions", "Dimension", this->variables
    );
}

std::shared_ptr<const IodaObsAttribute> IodaObsSchema::getAttribute(
    const std::string &name
) {
    return this->getComponent(name, this->attributes,
                              this->attributeRegexPatterns);
}

std::shared_ptr<const IodaObsGroup> IodaObsSchema::getGroup(
    const std::string &name
) {
    return this->getComponent(name, this->groups,
                              this->groupRegexPatterns);
}

std::shared_ptr<const IodaObsDimension> IodaObsSchema::getDimension(
    const std::string &name
) {
    return this->getComponent(name, this->dimensions,
                              this->dimensionRegexPatterns);
}

std::shared_ptr<const IodaObsVariable> IodaObsSchema::getVariable(
    const std::string &name
) {
    return this->getComponent(name, this->variables,
                              this->variableRegexPatterns);
}

void IodaObsSchema::
addVariableRegexPattern(const std::string &pattern) {
    variableRegexPatterns.push_back(pattern);
}

void IodaObsSchema::addGroupRegexPattern(const std::string &pattern) {
    groupRegexPatterns.push_back(pattern);
}

void IodaObsSchema::
addAttributeRegexPattern(const std::string &pattern) {
    attributeRegexPatterns.push_back(pattern);
}

void IodaObsSchema::
addDimensionRegexPattern(const std::string &pattern) {
    dimensionRegexPatterns.push_back(pattern);
}
