import getpass
from typing import Optional
from .storage import load_prayers
from .users import create_user, verify_user
from .actions import add_prayer, list_recent, find_by_id, mark_answered, delete_prayer

def auth_menu() -> Optional[str]:
    while True:
        print("\n1) Login\n2) Sign up\n3) Quit")
        choice = input("Choose (1-3): ").strip()
        if choice == "1":
            username = input("Username: ").strip()
            password = getpass.getpass("Password: ")
            if verify_user(username, password):
                print("Login successful.")
                return username.strip().lower()
            else:
                print("Invalid username or password.")
        elif choice == "2":
            username = input("Choose a username: ").strip()
            if not username:
                print("Username required.")
                continue
            password = getpass.getpass("Choose a password: ")
            password2 = getpass.getpass("Confirm password: ")
            if password != password2:
                print("Passwords do not match.")
                continue
            if create_user(username, password):
                print("User created. You may now log in.")
            else:
                print("Username already exists.")
        elif choice in ("3", "q", "quit", "exit"):
            return None

def main():
    print("Welcome to the Prayer Room.")
    user = auth_menu()
    if not user:
        print("Goodbye.")
        return

    prayers = load_prayers()

    while True:
        print("\nCommands:\n 1) Add prayer\n 2) List recent\n 3) View by id\n 4) Mark answered\n 5) Delete\n 6) Quit")
        cmd = input("Choose an option (1-6): ").strip()
        if cmd == "1":
            title = input("Title (short): ").strip() #take out whitespace
            if not title:
                print("Title required.")
                continue
            content = input("Prayer content: ").strip()
            p = add_prayer(prayers, user, title, content)
            print(f"Prayer added (id={p.id}).")
        elif cmd == "2":
            recent = list_recent(prayers)
            if not recent:
                print("No prayers found.")
            for p in recent:
                status = "Answered" if p.answered else "Open"
                print(f"[{p.id}] {p.title} — {p.user} — {status} — {p.created_at.split('T')[0]}")
        elif cmd == "3":
            try:
                pid = int(input("Enter prayer id: ").strip())
                p = find_by_id(prayers, pid)
                if not p:
                    print("Not found.")
                else:
                    print(f"\nID: {p.id}\nUser: {p.user}\nTitle: {p.title}\nCreated: {p.created_at}\nAnswered: {p.answered}\n\n{p.content}\n")
            except ValueError:
                print("Invalid id.")
        elif cmd == "4":
            try:
                pid = int(input("Enter prayer id to mark answered: ").strip())
                if mark_answered(prayers, pid):
                    print("Marked as answered.")
                else:
                    print("Not found or already answered.")
            except ValueError:
                print("Invalid id.")
        elif cmd == "5":
            try:
                pid = int(input("Enter prayer id to delete: ").strip())
                confirm = input("Confirm delete? (y/N): ").strip().lower()
                if confirm == "y" and delete_prayer(prayers, pid):
                    print("Deleted.")
                else:
                    print("Cancelled or not found.")
            except ValueError:
                print("Invalid id.")
        elif cmd == "6" or cmd.lower() in ("q", "quit", "exit"):
            print("Goodbye.")
            break
        else:
            print("Unknown option. Choose 1-6.")

if __name__ == "__main__":
    main()