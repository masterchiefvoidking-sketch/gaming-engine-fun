# Asset Import Guide

## Supported Paths

Place assets under `Game/ApartmentLifeDemo/Assets/`:

- `Characters/` — character definitions, expressions, hairstyles
- `Characters/Wardrobe/` — outfit JSON files
- `Apartments/` — room layouts and furniture placement
- `Dialogue/` — dialogue packages
- `Events/` — event definitions
- `Interactions/` — object interaction definitions
- `Animations/` — animation clip metadata

## Import Workflow

1. Drop JSON asset files into the correct folder
2. Open project in Creator Studio (hot-reload picks up changes)
3. Run validation via Export panel
4. Save project

## Placeholders

Assets marked `"placeholder": true` are original stand-ins, not final art. Replace with your own original content before distribution.
