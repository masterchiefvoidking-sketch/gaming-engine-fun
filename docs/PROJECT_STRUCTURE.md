# EVE Engine Project Structure

Personal single-player projects use this layout. The repository maps engine code under `engine/` and game projects under `Game/`.

## Engine (`engine/`)

| Spec Path | Repository Path |
|-----------|-----------------|
| `/Engine/Core` | `engine/core/` |
| `/Engine/Renderer` | `engine/render/` |
| `/Engine/Animation` | `engine/animation/` |
| `/Engine/Character` | `engine/human/` |
| `/Engine/Apartment` | `engine/world/` |
| `/Engine/AI` | `engine/ai/` |
| `/Engine/Dialogue` | `engine/ai/` (dialogue engine) |
| `/Engine/Wardrobe` | `engine/human/` (wardrobe system) |
| `/Engine/Camera` | `engine/render/` (inspection camera) |
| `/Engine/Physics` | `engine/physics/` |
| `/Engine/UI` | `engine/interaction/` |
| `/Engine/Editor` | `engine/editor/` |
| `/Engine/Tools` | `engine/build/`, `apps/` |

## Projects (`Game/`)

| Spec Path | Repository Path |
|-----------|-----------------|
| `/Projects/ApartmentLifeDemo` | `Game/ApartmentLifeDemo/` |

## Assets (per project `Assets/`)

| Spec Folder | Project Path |
|-------------|--------------|
| Characters | `Assets/Characters/` |
| Hair | `Assets/Characters/Hairstyles/` |
| Clothing | `Assets/Characters/Wardrobe/` |
| Furniture | `Assets/Furniture/` |
| Rooms | `Assets/Apartments/` |
| Animations | `Assets/Animations/` |
| Materials | embedded in JSON / future `Assets/Materials/` |
| Audio | future `Assets/Audio/` |
| Shaders | engine runtime (`engine/render/`) |

## Templates (`data/templates/`)

| Spec Template | File |
|---------------|------|
| Blank | `blank_template.json` |
| ApartmentSim | `apartment_sim_template.json` |
| VisualNovel | `visual_novel_template.json` |
| LifeSim | `life_sim_template.json` |

## Build Output (`Build/`)

| Spec Path | Generated Path |
|-----------|----------------|
| `/Build/Windows` | `Build/windows_development/`, `Build/windows_release/` |
| `/Build/Web` | `Build/web_development/`, `Build/web_release/` |
| `/Build/Mobile` | future `Build/android/`, `Build/ios/` |

## Personal Workflow

1. Open Creator Studio or editor
2. Customize character, apartment, dialogue/events
3. Use Export panel → validate → cook → build
4. Run locally via `launch.json` in output folder
5. Archive manually with project archive export

Offline-first. No marketplace, cloud, or accounts.
