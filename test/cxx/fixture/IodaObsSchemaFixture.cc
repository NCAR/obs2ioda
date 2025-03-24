#include "IodaObsSchemaFixture.h"


void IodaObsSchemaFixture::SetUp() {
    this->schema = YAML::LoadFile(IODA_SCHEMA_YAML);
    this->iodaSchema = std::make_shared<IodaObsSchema>(this->schema);
}

void IodaObsSchemaFixture::TearDown() {
}
