import getpass
from dataclasses import dataclass, asdict
from datetime import datetime
import json
from pathlib import Path
from typing import List, Optional

DATA_FILE = Path(__file__).parent / "prayers.json"

@dataclass
class PrayerRequest:
    id: int
    user: str
    title: str
    content: str
    answered: bool = False
    created_at: str = datetime.utcnow().isoformat()
    #answered at?

def load_prayers() -> List[PrayerRequest]:
    if not DATA_FILE.exists():
        return []
    try:
        raw = json.loads(DATA_FILE.read_text(encoding="utf-8"))
        return [PrayerRequest(**r) for r in raw]
    except Exception:
        return []
    
def save_prayers(prayers: List[PrayerRequest]) -> None:
    DATA_FILE.write_text(json.dumps([asdict(p) for p in prayers], indent=2), encoding="utf-8")

def next_id(prayers: List[PrayerRequest]) -> int:
    return max((p.id for p in prayers), default=0) + 1

def prompt_menu() -> str:
    print("\nCommands:")
    print("  1) Add prayer")
    print("  2) List recent")
    print("  3) View by id")
    print("  4) Mark answered")
    print("  5) Delete")
    print("  6) Quit")
    return input("Choose an option (1-6): ").strip()

def add_prayer(prayers: List[PrayerRequest], user: str) -> None:
    title = input("Title (short): ").strip()
    if not title:
        print("Title required.")
        return
    content = input("Prayer content: ").strip()
    pid = next_id(prayers)
    p = PrayerRequest(id=pid, user=user, title=title, content=content)
    prayers.append(p)
    save_prayers(prayers)
    print(f"Prayer added (id={p.id}).")

def list_recent(prayers: List[PrayerRequest], limit: int = 10) -> None:
    recent = sorted(prayers, key=lambda p: p.created_at, reverse=True)[:limit]
    if not recent:
        print("No prayers found.")
        return
    for p in recent:
        status = "Answered" if p.answered else "Open"
        created = p.created_at.split("T")[0]
        print(f"[{p.id}] {p.title} — {p.user} — {status} — {created}")

def view_by_id(prayers: List[PrayerRequest], pid: int) -> None:
    p = next((x for x in prayers if x.id == pid), None)
    if not p:
        print("Not found.")
        return
    print(f"\nID: {p.id}\nUser: {p.user}\nTitle: {p.title}\nCreated: {p.created_at}\nAnswered: {p.answered}\n\n{p.content}\n")

def mark_answered(prayers: List[PrayerRequest], pid: int) -> None:
    p = next((x for x in prayers if x.id == pid), None)
    if not p:
        print("Not found.")
        return
    if p.answered:
        print("Already marked answered.")
        return
    p.answered = True
    save_prayers(prayers)
    print("Marked as answered.")

def delete_prayer(prayers: List[PrayerRequest], pid: int) -> None:
    idx = next((i for i,x in enumerate(prayers) if x.id == pid), None)
    if idx is None:
        print("Not found.")
        return
    confirm = input("Confirm delete? (y/N): ").strip().lower()
    if confirm == "y":
        prayers.pop(idx)
        save_prayers(prayers)
        print("Deleted.")
    else:
        print("Cancelled.")

def main():
    print("Welcome to the Prayer Room.")
    username = input("Please enter your username: ").strip()
    if not username:
        print("Username required. Exiting.")
        return
    # keep password for UX parity; not verified in this simple CLI
    _ = getpass.getpass(f"Hello, {username}! Please enter your password: ")
    print(f"Thank you, {username}, for logging in.\n")

    prayers = load_prayers()

    while True:
        cmd = prompt_menu()
        if cmd == "1":
            add_prayer(prayers, username)
        elif cmd == "2":
            list_recent(prayers)
        elif cmd == "3":
            try:
                pid = int(input("Enter prayer id: ").strip())
                view_by_id(prayers, pid)
            except ValueError:
                print("Invalid id.")
        elif cmd == "4":
            try:
                pid = int(input("Enter prayer id to mark answered: ").strip())
                mark_answered(prayers, pid)
            except ValueError:
                print("Invalid id.")
        elif cmd == "5":
            try:
                pid = int(input("Enter prayer id to delete: ").strip())
                delete_prayer(prayers, pid)
            except ValueError:
                print("Invalid id.")
        elif cmd == "6" or cmd.lower() in ("q", "quit", "exit"):
            print("Goodbye.")
            break
        else:
            print("Unknown option. Choose 1-6.")


if __name__ == "__main__":
    main()