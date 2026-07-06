# Phase 2: Apartment Life Simulation

## Overview

Phase 2 introduces a data-driven apartment interaction framework where characters live believable daily routines, interact with every room, and build evolving relationships.

## Systems

### Apartment Interaction Framework (`eve::world`)

| Component | Purpose |
|-----------|---------|
| `InteractionRegistry` | Loads JSON interaction definitions |
| `InteractionSystem` | Hover detection, context actions, animation/sound triggers |
| `ApartmentLayout` | 11-room apartment graph with connections |
| `ApartmentSimulation` | Time of day, weather, lighting, appliances, housekeeping |
| `ApartmentPersistence` | Save/load interactable and simulation state |

### Relationship & Social AI (`eve::ai`)

| Component | Purpose |
|-----------|---------|
| `RelationshipModel` | Trust, affection, respect, humor, memories, conversation history |
| `CharacterState` | Mood, stress, energy, personality, preferences |
| `WardrobeProfile` | Clothing categories, outfits, random generation |
| `DailySchedule` | Room/activity schedules per character |
| `SocialAI` | Conversation invites, shared activities, emotional reactions |

### Scripting (`eve::scripting`)

| Component | Purpose |
|-----------|---------|
| `IScriptHost` | Plugin-compatible script host interface |
| `PythonScriptHost` | Python bridge (stub mode, plugin-ready) |
| `VisualScriptGraph` | Visual scripting node graph data |
| `ScriptEngine` | Host registry and event dispatch |

### Editor (`eve::editor`)

| Tool | Purpose |
|------|---------|
| `ApartmentBuilder` | Load layout, spawn interactables, manage simulation state |

## Data-Driven Content

```
data/
  apartments/default/layout.json   # 11-room apartment layout
  interactions/room_interactions.json  # Context actions per object
```

### Interaction Definition Schema

```json
{
  "id": "bed",
  "display_name": "Bed",
  "room": "bedroom",
  "actions": [
    {
      "id": "sleep",
      "label": "Sleep",
      "category": "sleep",
      "animation": "anim_sleep",
      "sound": "sfx_bed",
      "duration": 3600,
      "ai_usable": true,
      "resulting_state": "in_use"
    }
  ]
}
```

## Rooms Supported

Bedroom, Bathroom, Living Room, Dining Room, Kitchen, Wardrobe/Dressing Room, Office, Balcony, Hallway, Laundry Room, Storage.

## Extension Points

- Add interaction JSON files under `data/interactions/`
- Register custom `IScriptHost` implementations for Python/visual scripting
- Extend `SocialAI` with new intent types and event reactions
- Plugin-compatible via existing `PluginManager`

## Testing

```bash
ctest --test-dir build --output-on-failure
```

Test suites: `eve_world_tests`, `eve_ai_tests`, `eve_scripting_tests`, `eve_editor_apartment_tests`
