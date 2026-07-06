# Phase 8: Creator Studio

## Overview

Phase 8 delivers a professional no-code/low-code content creation suite that separates **Engine**, **Editor**, **Game Content**, and **Mods**. Indie teams can build original anime-inspired relationship simulation games without modifying engine source code.

## Architecture

```
Engine/          Runtime systems (render, AI, scene, human, etc.)
Editor/          Creator Studio tools and panels
Game/            Per-game content projects (e.g. Apartment Life Demo)
Mods/            Optional content extensions per game
```

## Content Module (`eve::content`)

| System | Description |
|--------|-------------|
| `ContentDatabase` | Central asset registry for all content types |
| `ProjectLayout` | Standard `/Game` folder structure |
| `GameContentProject` | Unified project loader/saver |
| `CharacterContentLibrary` | Character definitions + appearance presets |
| `WardrobeDatabase` | Unlimited clothing items with full metadata |
| `ApartmentContentLibrary` | Apartments, furniture, object interactions |
| `DialogueContentLibrary` | Visual dialogue node packages |
| `EventContentLibrary` | Visual event graph definitions |
| `ModSystem` | Mod loading and extension |
| `AssetImporter` | FBX, glTF, USD, PNG, WAV, OGG, MP4 import |
| `LocalizationTable` | Multi-locale string tables |
| `UndoStack` | Undo/redo for editor operations |
| `HotReloadManager` | Hot reload for dialogue/events |
| `ProjectPackager` | One-click export (Windows, Linux, Steam Deck) |

## Creator Studio (`eve::editor::CreatorStudio`)

Editor panels:

- **Character Editor** — name, bio, personality, appearance, voice, schedule, dialogue packages
- **Wardrobe Editor** — clothing database with categories, variants, unlock conditions
- **Dialogue Graph Editor** — branching nodes with conditions, expressions, camera, music
- **Event Graph Editor** — start/condition/dialogue/camera/reward/memory/end nodes
- **Timeline Editor** — character, dialogue, camera, music, lighting, expression tracks
- **Photo Studio** — poses, expressions, DOF, filters, batch screenshots
- **Asset Browser** — search everywhere with relevance ranking

Editor UX features: dark theme config, dockable shell, undo/redo, autosave, hot reload, drag-and-drop asset import.

## Sample Project: Apartment Life Demo

```
Game/ApartmentLifeDemo/
  project.json
  Assets/
    Characters/       Mira (original adult character)
    Characters/Wardrobe/  20 clothing items
    Characters/Hairstyles/  5 hairstyles
    Characters/Expressions/   10 expressions
    Apartments/         Sunset Apartment
    Furniture/          20 furniture objects
    Dialogue/           20 conversation packages
    Events/             5 apartment events
    Localization/       EN/JA strings
  Saves/
  Mods/
```

## Running Creator Studio

```bash
./build/apps/creator_studio/eve-creator-studio Game/ApartmentLifeDemo
```

## Tests

```bash
ctest --test-dir build -R "Content|CreatorStudio"
```

Coverage: content database, project layout, undo/redo, wardrobe load, localization, asset import, packaging, full sample project load, Creator Studio open/export, photo studio capture.
