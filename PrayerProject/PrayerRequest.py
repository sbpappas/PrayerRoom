class PrayerRequest:
    toWhom: str = "YHWH"

    def __init__(self, requester_name: str, prayer_intent: str, date_requested: str = None, answered: bool = False):
        self.requester_name = requester_name
        self.prayer_intent = prayer_intent
        self.date_requested = date_requested
        self.answered = answered

    def mark_as_answered(self):
        self.answered = True
    
    def get_status(self):
        return "Answered" if self.answered else "Pending"
    
    def get_Intention(self):
        return self.prayer_intent

    def __str__(self):
        return f"Prayer Request by {self.requester_name}: {self.prayer_intent}"