# Content Validation Report — Content Pack 01

**Pack:** Apartment Life Demo — Content Pack 01  
**Character:** Mira (original)  
**Date:** Generated at content production phase

## Summary

| Check | Status |
|-------|--------|
| Missing assets | PASS |
| Broken references | PASS (content_index regenerated) |
| Clothing compatibility | PASS (20 wardrobe items) |
| Dialogue nodes | PASS (150+ lines + legacy aliases) |
| Event conditions | PASS (10 events) |
| Camera anchors | PASS (12 presets) |
| Lighting presets | PASS (10 presets) |
| Photo mode presets | PASS (8 presets) |
| Object interactions | PASS (55 interactions) |
| Save/load persistence | PASS |
| Windows export validation | PASS |
| Web export validation | PASS |

## Content Counts

| Asset | Count | Required |
|-------|-------|----------|
| Dialogue lines | 159 | 150 |
| Events | 10 | 10 |
| Wardrobe items | 20 | 20 |
| Hairstyles | 5 | 5 |
| Expressions | 10 | 10 |
| Camera presets | 12 | 12 |
| Lighting presets | 10 | 10 |
| Photo presets | 8 | 8 |
| Object interactions | 55 | 50 |
| Saved outfits | 5 | 5 |
| Rooms | 6 | 6 |

## Notes

- All content is original placeholder metadata for personal-use demo.
- Expression files include eyes, eyebrows, mouth, head tilt, blush, animation cue, and dialogue hook.
- Bathroom and grooming events use privacy-aware dialogue (`bound_01`).
- Legacy dialogue aliases retained for editor/polish compatibility.

## Validation Command

```bash
ctest --test-dir build -R ContentPack01
./build/apps/personal_product_cut/eve-personal-product-cut
```
