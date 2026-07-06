# Known Issues

## Bug Severity

| Level | Description | Release Policy |
|-------|-------------|----------------|
| P0 | Cannot launch, save, export, or data corruption | **Blocked** |
| P1 | Major broken feature or editor crash | **Blocked** |
| P2 | Visible broken behavior | Must fix or document workaround |
| P3 | Polish, typo, minor UX | May remain if documented |

## Current Status

No open P0 or P1 bugs at RC1.

## P2 (with workarounds)

_None at release._

## P3 (documented)

- Creator Studio is CLI-only; full GUI viewport is planned for a future phase.
- Web first-load may take several seconds while progressive loading warms caches.
- Some interaction entries are marked `placeholder: true` pending final animation hooks.
