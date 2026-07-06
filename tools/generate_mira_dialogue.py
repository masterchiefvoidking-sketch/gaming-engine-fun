#!/usr/bin/env python3
"""Generate Mira dialogue pack with 100 lines for Apartment Life Demo."""

import json
from pathlib import Path

ROOMS = ["bedroom", "bathroom", "living_room", "dining_room", "kitchen", "wardrobe"]
MOODS = ["neutral", "happy", "shy", "romantic", "tired"]
STAGES = ["stranger", "acquaintance", "friend", "close_friend", "romantic_interest"]
OUTFIT_TAGS = ["casual", "sleepwear", "formal", "athletic", "dress"]

lines = []


def add(node_id, line, category, **conds):
    entry = {"id": node_id, "speaker": "mira", "line": line, "category": category}
    if conds:
        entry["conditions"] = conds
    lines.append(entry)


# Core interaction lines (required by events and viewer)
add("greet_morning", "Good morning. The apartment is peaceful today.", "greeting",
    time_of_day=["morning"])
add("greet_evening", "Welcome back. I was hoping you'd visit tonight.", "greeting",
    rooms=["living_room"], time_of_day=["evening"])
add("compliment_soft", "That's sweet of you... you always know what to say.", "compliment",
    min_stage="acquaintance")
add("compliment_blush", "M-my cheeks feel warm... don't tease me too much.", "compliment",
    min_stage="friend")
add("invite_dinner_yes", "I'd love to share dinner with you.", "meal", min_stage="friend")
add("invite_movie", "A movie night sounds perfect. Pick something cozy.", "event", min_stage="friend")
add("apology_soft", "It's okay. I'm glad we can talk things through.", "comfort")
add("gift_happy", "You remembered... this means more than you know.", "gift", min_stage="acquaintance")
add("goodnight_scene", "Goodnight. Sweet dreams.", "goodnight", rooms=["bedroom"])
add("rainy_window_talk", "The rain against the window is soothing, isn't it?", "ambient",
    rooms=["living_room"])
add("outfit_casual_1", "Do you like this casual look on me?", "outfit", outfit_tags=["casual"])
add("room_living_room_talk", "This living room feels like our own little world.", "room",
    rooms=["living_room"])
add("room_kitchen_1", "Coffee smells wonderful in the morning light.", "room", rooms=["kitchen"])

# 18 room-based lines (3 per room)
for room in ROOMS:
    for j in range(3):
        add(f"room_{room}_{j+1}",
            f"This {room.replace('_', ' ')} feels cozy when you're here.",
            "room", rooms=[room])

# 20 outfit-based reactions
for tag in OUTFIT_TAGS:
    for j in range(4):
        add(f"outfit_{tag}_{j+1}",
            f"Do you think this {tag} look suits me?",
            "outfit", outfit_tags=[tag])

# 20 mood-based reactions
for mood in MOODS:
    for j in range(4):
        add(f"mood_{mood}_{j+1}",
            f"I'm feeling a bit {mood} right now... thanks for noticing.",
            "mood")

# 20 memory callbacks
for i in range(20):
    add(f"memory_callback_{i+1}",
        "I still remember that moment we shared... it means a lot to me.",
        "memory", memories=[f"memory_{i+1}"])

# 20 relationship-stage lines
for stage in STAGES:
    for j in range(4):
        add(f"stage_{stage}_{j+1}",
            "Our relationship has grown. I'm glad we're at this stage together.",
            "relationship", min_stage=stage)

# Pad to exactly 100 lines
while len(lines) < 100:
    n = len(lines) + 1
    add(f"extra_line_{n}", f"Every quiet moment here with you feels special. ({n})", "general")

lines = lines[:100]

out = Path(__file__).resolve().parents[1] / "data/apartment_life/romance_sim/dialogue.json"
out.parent.mkdir(parents=True, exist_ok=True)
out.write_text(json.dumps(lines, indent=2, ensure_ascii=False) + "\n")
print(f"Wrote {len(lines)} dialogue lines to {out}")
