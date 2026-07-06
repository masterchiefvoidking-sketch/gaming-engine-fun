# Dialogue & Events Guide

## Dialogue

- 100+ lines in `data/apartment_life/romance_sim/dialogue.json`
- Packages in `Game/ApartmentLifeDemo/Assets/Dialogue/`

Edit via Dialogue Graph editor. Each node needs valid `id`, `text`, and optional `choices`.

## Events

- 10 playable events in `Game/ApartmentLifeDemo/Assets/Events/`
- Templates in `data/apartment_life/romance_sim/event_templates.json`

Events support conditions (relationship level, room, time) and cooldowns.

## Relationship & Memory

Relationship state and memories persist through the unified save format. Dialogue progress and completed events are tracked in save slots.
