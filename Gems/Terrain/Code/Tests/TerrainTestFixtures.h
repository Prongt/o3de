/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */
#pragma once

#include <AzTest/GemTestEnvironment.h>
#include <TerrainSystem/TerrainSystem.h>
#include <Components/TerrainSurfaceGradientListComponent.h>

namespace UnitTest
{
    // The Terrain unit tests need to use the GemTestEnvironment to load LmbrCentral, SurfaceData, and GradientSignal Gems so that these
    // systems can be used in the unit tests.
    class TerrainTestEnvironment
        : public AZ::Test::GemTestEnvironment
    {
    public:
        void AddGemsAndComponents() override;
        void PostCreateApplication() override;
    };

#ifdef HAVE_BENCHMARK
    //! The Benchmark environment is used for one time setup and tear down of shared resources
    class TerrainBenchmarkEnvironment
        : public AZ::Test::BenchmarkEnvironmentBase
        , public TerrainTestEnvironment

    {
    protected:
        void SetUpBenchmark() override
        {
            SetupEnvironment();
        }

        void TearDownBenchmark() override
        {
            TeardownEnvironment();
        }
    };
#endif

    // Base test fixture used for Terrain unit tests and benchmark tests
    class TerrainBaseFixture
    {
    public:
        void SetupCoreSystems();
        void TearDownCoreSystems();

        AZStd::unique_ptr<AZ::Entity> CreateEntity() const
        {
            return AZStd::make_unique<AZ::Entity>();
        }

        void ActivateEntity(AZ::Entity* entity) const
        {
            entity->Init();
            entity->Activate();
        }

        // Create an entity with a box shape and a transform.
        AZStd::unique_ptr<AZ::Entity> CreateTestBoxEntity(float boxHalfBounds) const;

        // Create an entity with a box shape and a transform.
        AZStd::unique_ptr<AZ::Entity> CreateTestBoxEntity(const AZ::Aabb& box) const;

        // Create an entity with a sphere shape and a transform.
        AZStd::unique_ptr<AZ::Entity> CreateTestSphereEntity(float shapeRadius) const;

        // Create and activate an entity with a gradient component of the requested type, initialized with test data.
        AZStd::unique_ptr<AZ::Entity> CreateAndActivateTestRandomGradient(const AZ::Aabb& spawnerBox, uint32_t randomSeed) const;

        AZStd::unique_ptr<AZ::Entity> CreateTestLayerSpawnerEntity(
            const AZ::Aabb& spawnerBox,
            const AZ::EntityId& heightGradientEntityId,
            const Terrain::TerrainSurfaceGradientListConfig& surfaceConfig) const;

        // Create a terrain system with reasonable defaults for testing, but with the ability to override the defaults
        // on a test-by-test basis.
        AZStd::unique_ptr<Terrain::TerrainSystem> CreateAndActivateTerrainSystem(
            float queryResolution = 1.0f,
            AZ::Aabb worldBounds = AZ::Aabb::CreateFromMinMax(AZ::Vector3(-128.0f), AZ::Vector3(128.0f))) const;

        void CreateTestTerrainSystem(const AZ::Aabb& worldBounds, float queryResolution, uint32_t numSurfaces);
        void DestroyTestTerrainSystem();

    protected:
        // State data for a full test terrain system setup.
        AZStd::unique_ptr<AZ::Entity> m_heightGradientEntity;
        AZStd::vector<AZStd::unique_ptr<AZ::Entity>> m_surfaceGradientEntities;
        AZStd::unique_ptr<AZ::Entity> m_terrainLayerSpawnerEntity;
        AZStd::unique_ptr<Terrain::TerrainSystem> m_terrainSystem;
    };

    class TerrainTestFixture
        : public TerrainBaseFixture
        , public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            SetupCoreSystems();
        }

        void TearDown() override
        {
            TearDownCoreSystems();
        }
    };

#ifdef HAVE_BENCHMARK
    class TerrainBenchmarkFixture
        : public TerrainBaseFixture
        , public ::benchmark::Fixture
    {
    public:
        void internalSetUp()
        {
            SetupCoreSystems();
        }

        void internalTearDown()
        {
            TearDownCoreSystems();
        }

    protected:
        void SetUp([[maybe_unused]] const benchmark::State& state) override
        {
            internalSetUp();
        }
        void SetUp([[maybe_unused]] benchmark::State& state) override
        {
            internalSetUp();
        }

        void TearDown([[maybe_unused]] const benchmark::State& state) override
        {
            internalTearDown();
        }
        void TearDown([[maybe_unused]] benchmark::State& state) override
        {
            internalTearDown();
        }
    };
#endif
}
