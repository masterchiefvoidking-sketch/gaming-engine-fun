# Save & Backup Guide

## Save Slots

- Default slot: `data/saves/apartment_life_slot.json`
- Game saves: `Game/ApartmentLifeDemo/Saves/`
- Format: EVE unified save (version 1)

## What Persists

- Character outfit, room, mood
- Relationship trust/affection/stage
- Dialogue progress and story flags
- Completed events and memories
- Apartment room and time-of-day
- Photo gallery entries
- Settings (volume, quality, touch controls)

## Stress Testing

Phase 14 validates 10 slots × 100 save/load cycles including corruption restore.

## Backup & Archive

```bash
# Full personal product cut creates BackupArchive under Releases/
./build/apps/personal_product_cut/eve-personal-product-cut
```

Manual archive via `ProjectArchiveEnhanced`:

- Export project (excludes `Build/` and `Backups/` by default)
- Optional: include saves and screenshots
- Validate manifest before restore

## Restore

1. Validate archive manifest (`archive_manifest.json`)
2. Import to a clean directory
3. Open in Creator Studio
4. Confirm no missing references via validation gate
