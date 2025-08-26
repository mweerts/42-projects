#!/usr/bin/env python3
import os, json

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

cookie = os.environ.get("HTTP_COOKIE", "")
session_id = None
for part in cookie.split(";"):
    part = part.strip()
    if part.startswith("SESSIONID="):
        session_id = part.split("=", 1)[1]

sessions = load_sessions()
if session_id and session_id in sessions:
    del sessions[session_id]
    save_sessions(sessions)

print("Content-Type: application/json")
print("Set-Cookie: SESSIONID=deleted; Path=/; Expires=Thu, 01 Jan 1970 00:00:00 GMT")
print()
print(json.dumps({"ok": True, "message": "Logged out"}))
