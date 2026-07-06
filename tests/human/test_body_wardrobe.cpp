#include <eve/human/body_interaction_system.hpp>
#include <eve/human/body_regions.hpp>
#include <eve/human/interaction_rules.hpp>
#include <gtest/gtest.h>

TEST(BodyRegionTest, StringConversion) {
    EXPECT_STREQ(eve::human::to_string(eve::human::BodyRegion::Torso), "torso");
    EXPECT_EQ(eve::human::body_region_from_string("hips"), eve::human::BodyRegion::Hips);
}

TEST(BodyRegionTest, RayQueryTargetsVolume) {
    eve::human::BodyRegionVolume torso;
    torso.region = eve::human::BodyRegion::Torso;
    torso.center = {0.0f, 1.15f, 0.0f};
    torso.half_extents = {0.18f, 0.22f, 0.12f};

    eve::human::BodyInteractionRegistry registry;
    eve::human::BodyInteractionSystem system(registry);
    const eve::human::BodyRegionTarget target =
        system.query_region({torso}, {0.0f, 1.15f, 2.0f}, {0.0f, 0.0f, -1.0f});
    EXPECT_TRUE(target.hovered);
    EXPECT_EQ(target.region, eve::human::BodyRegion::Torso);
}

TEST(BodyInteractionTest, PermissionChecksBlockLowTrust) {
    eve::human::BodyInteractionRegistry registry;
    eve::human::BodyInteractionDefinition def;
    def.id = "test";
    def.region = eve::human::BodyRegion::Face;
    eve::human::BodyRegionAction action;
    action.id = "compliment";
    action.requirements.min_trust = 0.8f;
    def.actions.push_back(action);
    registry.register_definition(std::move(def));

    eve::human::BodyInteractionSystem system(registry);
    eve::human::InteractionContextSnapshot context;
    context.trust = 0.2f;
    const auto result = system.execute("compliment", context);
    EXPECT_FALSE(result.success);
    EXPECT_FALSE(result.rule_result.allowed);
}

TEST(BodyInteractionTest, LoadsFromDataFile) {
    eve::human::BodyInteractionRegistry registry;
    ASSERT_TRUE(registry.load_from_file(EVE_DATA_DIR "/interactions/body_regions.json"));
    EXPECT_GT(registry.definition_count(), 0u);
    EXPECT_GT(registry.action_count(), 0u);
}

TEST(BodyRegionTest, LoadsVolumesFromFile) {
    std::vector<eve::human::BodyRegionVolume> volumes;
    ASSERT_TRUE(eve::human::load_body_volumes_from_file(
        EVE_DATA_DIR "/characters/default/body_definition.json", volumes));
    EXPECT_EQ(volumes.size(), 13u);
}
