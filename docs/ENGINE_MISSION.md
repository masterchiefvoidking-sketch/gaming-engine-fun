# EVE Engine Mission

EVE Engine is **not** attempting to become another Unreal Engine or Unity.

## Purpose

Build beautiful, high-performance, **anime-inspired**, **story-driven relationship and life simulation** experiences.

Every engineering decision should support a solo developer creating a polished relationship simulator with:

- Original expressive characters
- Beautiful animation
- A cozy apartment setting
- Meaningful dialogue and relationship progression

## Supported Platforms

| Priority | Platform |
|----------|----------|
| Primary | Windows |
| Secondary | Web (WebGPU / WebAssembly) |
| Future | Android, iOS |

## Performance Targets

- **Desktop:** 60–144 FPS
- **Web:** 60 FPS
- Stable memory usage
- Fast loading
- Minimal dependencies
- Low editor startup time

## In Scope

Every new system must directly improve one of:

- Characters
- Animation
- Apartments
- AI
- Dialogue
- Graphics
- Creator workflow

## Explicitly Out of Scope

The engine does **not** support:

| Category | Examples |
|----------|----------|
| Multiplayer | Networking, dedicated servers, MMO systems |
| Marketplace | Online asset store, cloud publishing, creator economy |
| Live service | Battle pass, subscriptions, always-online, required telemetry |
| General engine | Console SDKs, competitive gameplay infrastructure |

## Offline First

The engine must work **completely offline**. No telemetry requirements. No subscription gates.

## Local Content Workflow

Instead of a marketplace, EVE provides:

- Local asset library
- Project templates
- Reusable asset packs
- Import/export tools
- Backup manager

## Save System

One unified save format covering:

- Characters, relationships, dialogue, memories
- Apartments, inventory, wardrobe, photos
- Story progress, settings

## Web Deployment

One-click publishing generates:

- WebAssembly build manifest (toolchain integration planned)
- WebGPU renderer configuration
- Asset compression settings
- Progressive loading
- Save persistence hooks
- Responsive UI and mobile-friendly controls

## Mobile Readiness

Systems are designed for future touch support:

- Gesture camera
- Touch interaction
- Optimized shaders
- Battery-conscious rendering
- Dynamic quality scaling

## Success Metric

> If a solo developer can create a polished, anime-inspired relationship simulator from start to finish using this engine, the project has succeeded.

See also: `docs/PHASE9_ENGINE_DIRECTION.md`, `engine/platform/include/eve/platform/engine_mission.hpp`.
