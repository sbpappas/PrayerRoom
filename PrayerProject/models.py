from dataclasses import dataclass, asdict
from datetime import datetime

# this is a dataclass (mostly for storing data) 
# I might change to a normal class (object with methods and more formal control) later

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