# Phase 16 — Content Production

Produce the first complete original content pack for Apartment Life Demo using existing engine systems only.

## Content Pack

**Apartment Life Demo — Content Pack 01** (`ContentPack01/`)

| Deliverable | Location |
|-------------|----------|
| Character profile | ContentPack01/CharacterProfile.md |
| Wardrobe catalog | ContentPack01/WardrobeCatalog.md |
| Apartment layout | ContentPack01/ApartmentLayout.md |
| Dialogue (150+) | ContentPack01/DialogueDatabase.json |
| Events (10) | ContentPack01/EventDatabase.json |
| Camera (12) | ContentPack01/CameraPresets.json |
| Lighting (10) | ContentPack01/LightingPresets.json |
| Photo mode (8) | ContentPack01/PhotoModePresets.json |
| Validation | ContentPack01/ContentValidationReport.md |

## Character: Mira

Original adult anime-inspired female character, age 22. Not based on any copyrighted property.

## Content Rules

- Original content only
- Adult 18+, offline-first, single-player
- No copyrighted characters, outfits, names, or layouts

## Runtime Sync

Content is synced to `data/apartment_life/romance_sim/`, `data/camera/`, `data/anime/`, and `Game/ApartmentLifeDemo/Assets/`.

## Tests

```bash
ctest --test-dir build -R ContentPack01
```

## No Engine Expansion

This phase adds content only. Polish modules updated for new lighting/camera preset IDs.
