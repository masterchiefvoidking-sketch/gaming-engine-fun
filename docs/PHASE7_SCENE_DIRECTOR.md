# Phase 7: Scene Director + Daily Life Event Engine

## Overview

Phase 7 delivers a cinematic scene director that turns apartment life, romance progression, room interactions, outfits, moods, and memories into believable interactive scenes. The apartment feels alive, reactive, emotional, and visually polished.

## Core Systems (`eve::scene`)

| System | Description |
|--------|-------------|
| `SceneDirector` | Orchestrates scenes: participants, room, dialogue, camera, cues, outcomes |
| `EventScheduler` | Time/weather/mood/relationship-gated event selection with cooldowns |
| `SceneStateMachine` | Idle → Loading → Entering → Playing → Completed state flow |
| `SceneConditionEvaluator` | 15+ condition types: stage, mood, room, outfit, memory, weather, flags |
| `RoomEventTriggerSystem` | Room + interaction → scene template mapping |
| `CameraShotSequencer` | Cinematic shot playback with FOV, DOF, duration, ease |
| `DialogueSceneSequencer` | Beat-synchronized dialogue via `DialogueEngine` |
| `AnimationCueSystem` | 20 gesture/expression cues (sit, smile, blush, eye contact, etc.) |
| `LightingCueSystem` | Room lighting preset transitions |
| `MusicCueSystem` | Ambient/music track cues with fade |
| `EmotionalOutcomeSystem` | Stat deltas, memory creation, dialogue/event unlocks |

## Scene Director Controls

- Who is in the scene
- Where it happens
- Interaction trigger
- Relationship/mood/consent/privacy gates
- Dialogue sequence
- Camera shots
- Animation and expression cues
- Lighting and music
- Memory and relationship outcomes
- Save/load state

## Event Types

Morning greeting, coffee together, getting ready, wardrobe preview, cooking together, shared dinner, movie night, rainy evening talk, balcony conversation, bedroom goodnight, living room relaxation, mirror outfit reaction, date preparation, apology, comfort, celebration, gift reaction, photo-mode pose.

## Camera Shots

Wide room, medium conversation, face/eye close-up, full-body outfit, over-the-shoulder, mirror, sitting, walking follow, window/sunset, rainy window, dining two-shot, wardrobe orbit.

## Vertical Slice: Rainy Movie Night

```bash
./build/apps/rainy_movie_night_demo/eve-rainy-movie-night-demo data
```

Scene flow:

1. Rain begins outside
2. Character enters living room
3. Warm lamp lighting activates
4. Player invites her to watch a movie
5. Dialogue branches on trust and mood
6. Camera switches: wide → sofa → close-up
7. Facial expressions and gestures
8. Shared memory created
9. Relationship values update
10. Scene recallable in future dialogue

## Data Files (`data/scene_director/`)

| File | Count |
|------|-------|
| `scene_templates.json` | 10 templates |
| `rainy_movie_night.json` | Full demo scene |
| `camera_shots.json` | 10 presets |
| `animation_cues.json` | 20 cues |
| `lighting_presets.json` | 10 presets |
| `music_cues.json` | 10 cues |
| `event_conditions.json` | 15 conditions |
| `emotional_outcomes.json` | 15 outcomes |
| `room_triggers.json` | 10 room triggers |

## Editor Tools

Panel scaffolds in `scene_director_tools.hpp`:

- Scene graph editor
- Event template editor
- Shot sequencer
- Dialogue scene editor
- Animation cue timeline
- Lighting cue timeline
- Music cue timeline
- Event condition debugger
- Scene outcome preview
- Room trigger editor

## API Example

```cpp
#include <eve/scene/scene_director.hpp>
#include <eve/ai/relationship_simulation.hpp>

eve::ai::RelationshipSimulation relationship;
relationship.initialize("data");

eve::scene::SceneDirector director;
director.initialize("data", relationship);
director.attach_camera(camera);
director.attach_lighting(lighting);

director.start_scene("rainy_movie_night");
while (director.state_machine().is_active()) {
    director.advance(1.0f / 60.0f);
}
director.save_state("/tmp/scene_save.json");
```

## Tests

```bash
ctest --test-dir build -R "Scene|EventScheduler|CameraShot|AnimationCue|RainyMovie"
```

Coverage: condition evaluation, event eligibility, cooldowns, camera sequencing, animation dispatch, emotional outcomes, memory creation, save/load, privacy gating, and the Rainy Movie Night vertical slice.

## Integration

Phase 7 builds on Phase 6 (`RelationshipSimulation`, `DialogueEngine`, `MemorySystem`) and Phase 5 (anime rendering, camera, lighting, expressions, motion).
