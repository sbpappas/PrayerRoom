from pathlib import Path
import json
from typing import List
from .models import PrayerRequest

DATA_FILE = Path(__file__).parent / "prayers.json"

def load_prayers() -> List[PrayerRequest]: # retrieve the data file
    if not DATA_FILE.exists():
        return []
    try:
        raw = json.loads(DATA_FILE.read_text(encoding="utf-8"))
        return [PrayerRequest(**r) for r in raw] #thought this was cool
        # ** unpacks a dictionary into keyword arguments. 
            # keys become parameter name, values become values
    except Exception as e:
        raise RuntimeError(f"Failed to load prayers: {e}")

def save_prayers(prayers: List[PrayerRequest]) -> None:
    DATA_FILE.write_text(json.dumps([p.to_dict() for p in prayers], indent=2), encoding="utf-8")

def next_prayer_id(prayers: List[PrayerRequest]) -> int: 
    return max((p.id for p in prayers), default=0) + 1
    # iterates over prayers and gets id of each prayer object
    # finds the next available id by getting MAX and adding one.

def next_user_id(users: List[str]) -> int:
    return max((u.id for u in users), default = 0) + 1