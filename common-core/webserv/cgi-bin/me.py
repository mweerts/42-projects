#!/usr/bin/env python3
import os, json, time

SESSIONS_FILE = "/tmp/sessions.json"

def load_sessions():
    try:
        with open(SESSIONS_FILE, "r") as f:
            return json.load(f)
    except:
        return {}

cookie = os.environ.get("HTTP_COOKIE", "")
session_id = None
for part in cookie.split(";"):
    part = part.strip()
    if part.startswith("SESSIONID="):
        session_id = part.split("=", 1)[1]

sessions = load_sessions()

if session_id and session_id in sessions:
    username = sessions[session_id]["username"]
    sessions[session_id]["last_access"] = time.time()
    with open(SESSIONS_FILE, "w") as f:
        json.dump(sessions, f)

    print("Content-Type: application/json")
    print()
    print(json.dumps({"ok": True, "username": username}))
else:
    print("Status: 401 Unauthorized")
    print("Content-Type: application/json")
    print()
    print(json.dumps({"ok": False, "error": "Not logged in"}))
