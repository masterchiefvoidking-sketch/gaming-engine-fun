# Phase 6: Anime Romance AI + Relationship Simulation

## Overview

Phase 6 delivers the core relationship simulation layer for adult, anime-style apartment romance and life-sim experiences. Characters feel emotionally responsive: they remember past interactions, react to rooms, outfits, schedules, gifts, moods, boundaries, and shared experiences.

All systems are modular, data-driven, and legally original.

## Core Systems

| System | Module | Description |
|--------|--------|-------------|
| Dialogue engine | `eve::ai::DialogueEngine` | Branching, mood/room/outfit/time/stage-conditioned lines |
| Character memory | `eve::ai::MemorySystem` | Templates, creation, recall, dialogue callbacks |
| Relationship progression | `eve::ai::RelationshipStage` | Stranger → Partner with gated content |
| Mood system | `eve::ai::MoodSystem` | Transitions from compliments, gifts, meals, rejection |
| Boundaries & consent | `eve::ai::BoundaryConsentSystem` | Action checks, rejection lines, consent state |
| Daily schedule AI | `eve::ai::AdaptiveScheduleAI` | Mood/weather/invitation-aware routine adaptation |
| Room behavior | `eve::ai::RoomBehaviorSystem` | Per-room actions and dialogue hooks |
| Outfit reactions | `eve::ai::OutfitReactionSystem` | Tag/outfit-based comments and stat deltas |
| Romance events | `eve::ai::RomanceEventSystem` | One-time story beats with memory rewards |
| Scene interruption | `eve::ai::SceneInterruptionHandler` | Priority-based scene stacking |
| Orchestrator | `eve::ai::RelationshipSimulation` | Unified talk/compliment/dinner/outfit/event/save API |

## Character State Model

`CharacterSimState` tracks mood, energy, comfort, trust, affection, respect, stress, confidence, shyness, familiarity, current outfit/room/activity, privacy level, consent, personality, preferences, boundaries, recent interactions, and relationship stage.

## Relationship Stages

1. **Stranger** — minimal dialogue, high boundaries
2. **Acquaintance** — greetings, light conversation
3. **Friend** — shared meals, room comfort
4. **Close friend** — deeper talks, outfit comments
5. **Romantic interest** — compliments, date planning
6. **Partner** — full romance content (still consent-gated)

No stage bypasses boundary or consent checks.

## Sample Character: Aiko

Original anime-style adult female profile in `data/romance_sim/`:

| File | Contents |
|------|----------|
| `character_profile.json` | Personality, preferences, default outfit |
| `boundaries.json` | Blocked actions (hug, intimate scenes, etc.) |
| `daily_schedule.json` | Wake → sleep routine |
| `dialogue.json` | 55 dialogue lines |
| `memory_templates.json` | 20 memory templates |
| `outfit_reactions.json` | 15 outfit reactions |
| `room_reactions.json` | 10 room behaviors |
| `event_templates.json` | 10 romance events |

## Vertical Slice: Rainy Evening Apartment

```bash
./build/apps/rainy_evening_demo/eve-rainy-evening-demo data
```

The demo lets the player:

1. Talk with Aiko in the living room (rainy evening)
2. Offer a compliment
3. Invite her to dinner
4. Trigger outfit-aware dialogue
5. Observe mood and relationship value changes
6. See memory callbacks in later conversation
7. Hit boundary responses (e.g. hug blocked)
8. Save and reload relationship state

## Editor Tools

Panel scaffolds in `romance_sim_tools.hpp`:

- Dialogue graph editor
- Memory debugger
- Relationship inspector
- Mood inspector
- Schedule editor (with adaptive resolution)
- Event template editor
- Boundary/consent rules editor
- Outfit reaction editor
- Room behavior editor

## API Example

```cpp
#include <eve/ai/relationship_simulation.hpp>

eve::ai::RelationshipSimulation sim;
sim.initialize("data");

sim.talk("rainy_evening_start");
sim.offer_compliment();
sim.invite_to_dinner();
sim.trigger_outfit_dialogue();
sim.trigger_event("rainy_day_conversation");

sim.save_state("/tmp/aiko_save.json");
sim.load_state("/tmp/aiko_save.json");
```

## Tests

```bash
ctest --test-dir build -R "Romance|Boundary|Dialogue|Memory|Mood|Outfit|Room|Adaptive|Rainy"
```

Coverage includes relationship progression, consent/boundary checks, dialogue conditions, memory create/recall, mood changes, outfit/room selection, schedule adaptation, save/load persistence, and the rainy evening vertical slice.

## Data Format

Dialogue nodes support `conditions` for `min_stage`, `rooms`, `time_of_day`, `memories`, `min_trust`, and `min_affection`. Choices apply trust/affection deltas and optional memory tags.

Memory templates define `description`, `emotional_weight`, and `callbacks` for recall lines.

Event templates define `dialogue_id`, `memory_template`, optional `room`, and reward values.
