#include <gtest/gtest.h>
#include "object_registry.h"

class ObjectRegistryTest : public ::testing::Test {
protected:
    struct Vec3 {
        float x, y, z;
    };
    ObjectRegistry<Vec3> objectRegistry;
};

TEST_F(ObjectRegistryTest, AddAndRetrieveObject) {
    auto vec3 = std::make_shared<Vec3>(Vec3{1.0f, 2.0f, 3.0f});
    objectRegistry.addObject(42, vec3);
    auto retrieved = objectRegistry.getObject(42);
    EXPECT_EQ(retrieved->x, 1.0f);
    EXPECT_EQ(retrieved->y, 2.0f);
    EXPECT_EQ(retrieved->z, 3.0f);
}

TEST_F(ObjectRegistryTest, RemoveObject) {
    auto vec3 = std::make_shared<Vec3>(Vec3{4.0f, 5.0f, 6.0f});
    objectRegistry.addObject(24, vec3);
    objectRegistry.removeObject(24);
    EXPECT_THROW(objectRegistry.getObject(24), std::runtime_error);
}

TEST_F(ObjectRegistryTest, GetMissingObjectThrows) {
    EXPECT_THROW(objectRegistry.getObject(999), std::runtime_error);
}

TEST_F(ObjectRegistryTest, RemoveMissingObjectThrows) {
    EXPECT_THROW(objectRegistry.removeObject(999), std::runtime_error);
}

