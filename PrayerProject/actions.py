from typing import List
from .models import PrayerRequest
from .storage import save_prayers, next_id
from datetime import datetime

def add_prayer(prayers: List[PrayerRequest], user: str, title: str, content: str) -> PrayerRequest:
    pid = next_id(prayers)
    p = PrayerRequest(id=pid, user=user, title=title, content=content, created_at=datetime.utcnow().isoformat())
    prayers.append(p)
    save_prayers(prayers)
    return p

def list_recent(prayers: List[PrayerRequest], limit: int = 10):
    recent = sorted(prayers, key=lambda p: p.created_at, reverse=True)[:limit]
    return recent

def find_by_id(prayers: List[PrayerRequest], pid: int):
    return next((x for x in prayers if x.id == pid), None)

def mark_answered(prayers: List[PrayerRequest], pid: int) -> bool:
    p = find_by_id(prayers, pid)
    if not p or p.answered:
        return False
    p.answered = True
    save_prayers(prayers)
    return True

def delete_prayer(prayers: List[PrayerRequest], pid: int) -> bool:
    idx = next((i for i,x in enumerate(prayers) if x.id == pid), None)
    if idx is None:
        return False
    prayers.pop(idx)
    save_prayers(prayers)
    return True