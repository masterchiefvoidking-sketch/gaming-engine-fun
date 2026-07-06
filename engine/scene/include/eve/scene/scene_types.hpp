#pragma once

#include <eve/ai/relationship_stages.hpp>
#include <eve/ai/wardrobe.hpp>
#include <eve/core/math/math.hpp>
#include <eve/core/types.hpp>

#include <string>
#include <vector>

namespace eve::scene {

enum class SceneState : u8 {
    Idle = 0,
    Loading,
    Entering,
    Playing,
    WaitingChoice,
    Transitioning,
    Completed,
    Failed,
    Cooldown,
};

enum class EaseCurve : u8 {
    Linear = 0,
    EaseIn,
    EaseOut,
    EaseInOut,
    SmoothStep,
};

enum class SceneEventType : u8 {
  MorningGreeting = 0,
  CoffeeTogether,
  GettingReady,
  WardrobePreview,
  CookingTogether,
  SharedDinner,
  MovieNight,
  RainyEveningTalk,
  BalconyConversation,
  BedroomGoodnight,
  LivingRoomRelaxation,
  MirrorOutfitReaction,
  DatePreparation,
  ApologyConversation,
  ComfortConversation,
  Celebration,
  GiftReaction,
  PhotoModePose,
  Custom,
};

enum class AnimationCueId : u8 {
    Sit = 0,
    Stand,
    Turn,
    Walk,
    Lean,
    Smile,
    Laugh,
    Blush,
    LookAway,
    EyeContact,
    Wave,
    Stretch,
    HoldObject,
    PlaceObject,
    Eat,
    Drink,
    Read,
    WatchTv,
    MirrorPose,
    OutfitPreviewWalk,
    Custom,
};

enum class ShotPresetId : u8 {
    WideRoom = 0,
    MediumConversation,
    FaceCloseUp,
    EyeCloseUp,
    FullBodyOutfit,
    OverTheShoulder,
    Mirror,
    Sitting,
    WalkingFollow,
    WindowSunset,
    RainyWindow,
    DiningTwoShot,
    WardrobeOrbit,
    Custom,
};

struct SceneParticipant {
    std::string character_id;
    std::string spawn_room;
    std::string outfit_id;
};

struct SceneBeat {
    f32 start_time = 0.0f;
    std::string dialogue_node_id;
    std::string camera_shot_id;
    std::string animation_cue_id;
    std::string expression_id;
    std::string lighting_cue_id;
    std::string music_cue_id;
    std::string outcome_id;
    bool wait_for_choice = false;
};

struct SceneTemplate {
    std::string id;
    std::string label;
    SceneEventType type = SceneEventType::Custom;
    std::string room_id;
    std::string trigger_interaction;
    std::string condition_id;
    std::vector<SceneParticipant> participants;
    std::vector<SceneBeat> beats;
    std::string default_outcome_id;
    std::string memory_template_id;
    f32 cooldown_hours = 0.0f;
    bool one_time = false;
};

struct CameraShotDefinition {
    std::string id;
    std::string label;
    ShotPresetId preset = ShotPresetId::Custom;
    math::Vec3 position;
    math::Vec3 target;
    math::Vec3 focus_target;
    f32 fov_y_degrees = 35.0f;
    f32 focal_length_mm = 50.0f;
    f32 depth_of_field_strength = 0.0f;
    f32 duration_seconds = 2.0f;
    EaseCurve ease = EaseCurve::EaseInOut;
    std::string lighting_override_id;
    std::string animation_cue_id;
};

struct AnimationCueDefinition {
    std::string id;
    std::string label;
    AnimationCueId cue = AnimationCueId::Custom;
    std::string clip_id;
    std::string expression_id;
    f32 duration_seconds = 1.0f;
    f32 blend_weight = 1.0f;
};

struct LightingCueDefinition {
    std::string id;
    std::string label;
    std::string room_id;
    std::string preset_id;
    f32 transition_seconds = 1.0f;
};

struct MusicCueDefinition {
    std::string id;
    std::string label;
    std::string track_id;
    f32 volume = 0.7f;
    f32 fade_in_seconds = 1.0f;
    f32 fade_out_seconds = 1.0f;
    bool loop = true;
};

struct EventConditionDefinition {
    std::string id;
    std::string label;
    ai::RelationshipStage min_stage = ai::RelationshipStage::Stranger;
    ai::Mood required_mood = ai::Mood::Neutral;
    bool mood_required = false;
    std::vector<std::string> required_rooms;
    std::vector<std::string> required_outfits;
    std::vector<std::string> required_memories;
    std::vector<std::string> blocked_memories;
    std::vector<std::string> time_of_day;
    std::vector<std::string> weather;
    std::vector<std::string> story_flags;
    f32 min_trust = 0.0f;
    f32 min_affection = 0.0f;
    f32 min_comfort = 0.0f;
    f32 min_energy = 0.0f;
    f32 max_stress = 1.0f;
    bool require_consent = true;
    bool require_privacy = false;
};

struct EmotionalOutcomeDefinition {
    std::string id;
    std::string label;
    f32 trust_delta = 0.0f;
    f32 affection_delta = 0.0f;
    f32 respect_delta = 0.0f;
    f32 comfort_delta = 0.0f;
    f32 confidence_delta = 0.0f;
    f32 stress_delta = 0.0f;
    std::string memory_template_id;
    std::string unlock_dialogue_id;
    std::string unlock_event_id;
    f32 cooldown_hours = 0.0f;
};

struct SceneRuntimeContext {
    f32 time_of_day = 18.0f;
    u8 day_of_week = 0;
    std::string weather = "clear";
    std::string active_room;
    std::string active_outfit;
    ai::RelationshipStage stage = ai::RelationshipStage::Stranger;
    ai::Mood mood = ai::Mood::Neutral;
    f32 trust = 0.0f;
    f32 affection = 0.0f;
    f32 comfort = 0.0f;
    f32 energy = 1.0f;
    f32 stress = 0.0f;
    bool consent_granted = true;
    bool privacy_ok = true;
    std::vector<std::string> known_memories;
    std::vector<std::string> story_flags;
    std::vector<std::string> completed_events;
};

struct ScenePlaybackState {
    SceneState state = SceneState::Idle;
    std::string active_scene_id;
    std::string active_room_id;
    f32 elapsed_seconds = 0.0f;
    u32 current_beat_index = 0;
    std::string current_shot_id;
    std::string current_dialogue_line;
    std::vector<std::string> event_cooldowns;
};

[[nodiscard]] const char* to_string(SceneState state);
[[nodiscard]] const char* to_string(EaseCurve curve);

} // namespace eve::scene
