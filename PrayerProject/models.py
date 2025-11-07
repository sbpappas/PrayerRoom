from dataclasses import dataclass, asdict
from datetime import datetime

# 

@dataclass
class PrayerRequest:
    id: int
    user: str
    title: str
    content: str
    answered: bool = False
    created_at: str = datetime.utcnow().isoformat()

    def to_dict(self):
        return asdict(self)