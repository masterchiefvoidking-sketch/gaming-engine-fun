#!/usr/bin/env python3
"""Sync Mira wardrobe items from Game content into runtime anime wardrobe data."""

import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
GAME_WARDROBE = ROOT / "Game/ApartmentLifeDemo/Assets/Characters/Wardrobe"
ANIME_WARDROBE = ROOT / "data/anime/wardrobe_items.json"

CATEGORY_LAYER = {
    "casual": ("casualwear", "main_clothing"),
    "sleepwear": ("sleepwear", "main_clothing"),
    "formal": ("formalwear", "main_clothing"),
    "athletic": ("athleticwear", "main_clothing"),
    "dress": ("dresses", "main_clothing"),
    "shoes": ("shoes", "accessory"),
    "socks": ("socks", "accessory"),
    "jewelry": ("jewelry", "jewelry"),
    "accessories": ("accessories", "accessory"),
}

existing = json.loads(ANIME_WARDROBE.read_text())
existing_ids = {item["id"] for item in existing}

for path in sorted(GAME_WARDROBE.glob("mira_outfit_*.json")):
    item = json.loads(path.read_text())
    item_id = item["id"]
    if item_id in existing_ids:
        continue
    category = item.get("category", "casual")
    cat_name, layer = CATEGORY_LAYER.get(category, ("casualwear", "main_clothing"))
    existing.append({
        "id": item_id,
        "display_name": item.get("name", item_id),
        "category": cat_name,
        "layer": layer,
        "coverage_tags": ["torso", "legs"] if layer == "main_clothing" else ["accessory"],
        "compatibility_tags": ["mira", "apartment"],
        "tags": item.get("tags", []),
    })
    existing_ids.add(item_id)

ANIME_WARDROBE.write_text(json.dumps(existing, indent=2) + "\n")
print(f"Synced wardrobe: {len(existing)} items in {ANIME_WARDROBE}")
