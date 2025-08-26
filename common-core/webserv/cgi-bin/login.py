#!/usr/bin/env python3
import os, sys, json, random, string, time

SESSIONS_FILE = "/tmp/sessions.json"

def load_sessions():
    try:
        with open(SESSIONS_FILE, "r") as f:
            return json.load(f)
    except:
        return {}

def save_sessions(sessions):
    with open(SESSIONS_FILE, "w") as f:
        json.dump(sessions, f)

def generate_session_id(n=16):
    chars = string.ascii_letters + string.digits
    return ''.join(random.choice(chars) for _ in range(n))

# Lire le corps de la requête (POST JSON)
content_length = int(os.environ.get("CONTENT_LENGTH", 0))
body = sys.stdin.read(content_length) if content_length > 0 else ""

try:
    data = json.loads(body)
    username = data.get("username")
    password = data.get("password")
except:
    username = password = None

# Pour l’exemple, on accepte n’importe quel username/password non vide
if username and password:
    sessions = load_sessions()
    session_id = generate_session_id()
    sessions[session_id] = {"username": username, "last_access": time.time()}
    save_sessions(sessions)

    print("Content-Type: application/json")
    print(f"Set-Cookie: SESSIONID={session_id}; Path=/; HttpOnly")
    print()
    print(json.dumps({"ok": True, "username": username}))
else:
    print("Status: 401 Unauthorized")
    print("Content-Type: application/json")
    print()
    print(json.dumps({"ok": False, "error": "Invalid credentials"}))
