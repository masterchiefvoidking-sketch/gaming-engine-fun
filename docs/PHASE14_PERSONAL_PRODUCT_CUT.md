# Phase 14 — Personal Product Cut

Turn Apartment Life Demo into a complete personal-use product build.

## Mission

Finish the loop:

1. Open editor
2. Customize character, wardrobe, apartment, dialogue, events
3. Test play inside editor
4. Save project
5. Export Windows and Web builds
6. Reopen project cleanly
7. Archive and restore safely

## New Systems

| Module | Purpose |
|--------|---------|
| `engine/product/` | Release checklist, content completion, save stress, archive, bundle |
| `apps/personal_product_cut/` | Orchestrates full personal product cut |
| `tests/product/` | Automated validation for Phase 14 |

## Running the Cut

```bash
cmake --build build --target eve-personal-product-cut
./build/apps/personal_product_cut/eve-personal-product-cut
```

Output lands in `Releases/ApartmentLifeDemo_RC1/`.

## Feature Lock

No new major systems. Frozen at character customization, anime rendering, wardrobe, six room areas, dialogue, relationship/memory state, room interactions, scene events, photo mode, save/load, Windows/Web export, and local backup.

## Exclusions

No multiplayer, console, marketplace, cloud accounts, live-service, ads, analytics, social sharing, online store, DRM, or external login.

## Documentation

- [Setup Guide](personal_product/SETUP.md)
- [Editor Guide](personal_product/EDITOR_GUIDE.md)
- [Export Guide](personal_product/EXPORT_GUIDE.md)
- [Asset Import Guide](personal_product/ASSET_IMPORT.md)
- [Character Customization](personal_product/CHARACTER_GUIDE.md)
- [Wardrobe Guide](personal_product/WARDROBE_GUIDE.md)
- [Dialogue & Events](personal_product/DIALOGUE_EVENTS_GUIDE.md)
- [Save & Backup](personal_product/SAVE_BACKUP_GUIDE.md)
- [Known Issues](personal_product/KNOWN_ISSUES.md)
- [Roadmap](personal_product/ROADMAP.md)
