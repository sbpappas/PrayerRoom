from pathlib import Path
import json
from typing import Dict
import secrets
import hashlib
from datetime import datetime

USERS_FILE = Path(__file__).parent / "users.json"

def load_users() -> Dict[str, dict]:
    if not USERS_FILE.exists():
        return {}
    try:
        return json.loads(USERS_FILE.read_text(encoding="utf-8")) #returns file as a dict
    except Exception as e:
        raise RuntimeError(f"Failed to load users: {e}")

def save_users(users: Dict[str, dict]) -> None:
    USERS_FILE.write_text(json.dumps(users, indent=2), encoding="utf-8")

def _hash_password(password: str, salt: bytes, iterations: int = 120_000) -> str:
    dk = hashlib.pbkdf2_hmac("sha256", password.encode("utf-8"), salt, iterations) #sha 256 encryption
    return dk.hex()

def create_user(username: str, password: str) -> bool:
    username = username.strip().lower()
    users = load_users() # gets file
    if username in users:
        return False
    salt = secrets.token_bytes(16)
    iterations = 120_000
    pwd_hash = _hash_password(password, salt, iterations)
    users[username] = {
        "user ID": username,
        "salt": salt.hex(),
        "hash": pwd_hash,
        "iterations": iterations,
        "created_at": datetime.utcnow().isoformat()
    }
    save_users(users)
    return True

def verify_user(username: str, password: str) -> bool:
    username = username.lower()
    userfile = load_users()
    info = userfile.get(username)
    if not info:
        return False
    salt = bytes.fromhex(info["salt"])
    iterations = int(info.get("iterations", 120_000))
    expected = info["hash"]
    return _hash_password(password, salt, iterations) == expected