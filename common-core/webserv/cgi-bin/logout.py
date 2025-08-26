#!/usr/bin/env python3
import os, json, time

SESSIONS_FILE = "./sessions.json"

def load_sessions():
    """Charge les sessions depuis le fichier JSON"""
    try:
        with open(SESSIONS_FILE, "r") as f:
            return json.load(f)
    except:
        return {}

def save_sessions(sessions):
    """Sauvegarde les sessions dans le fichier JSON"""
    try:
        with open(SESSIONS_FILE, "w") as f:
            json.dump(sessions, f, indent=2)
        return True
    except Exception as e:
        return False

def get_session_id_from_cookie():
    """Extrait l'ID de session depuis les cookies"""
    cookie = os.environ.get("HTTP_COOKIE", "")
    for part in cookie.split(";"):
        part = part.strip()
        if part.startswith("SESSIONID="):
            return part.split("=", 1)[1]
    return None

session_id = get_session_id_from_cookie()

if session_id:
    sessions = load_sessions()
    
    if session_id in sessions:
        username = sessions[session_id].get("username", "unknown")
        
        del sessions[session_id]
        
        if save_sessions(sessions):
            print("Content-Type: application/json")
            print("Set-Cookie: SESSIONID=deleted; Path=/; Expires=Thu, 01 Jan 1970 00:00:00 GMT; HttpOnly")
            print()
            print(json.dumps({
                "ok": True, 
                "message": f"Au revoir {username}!",
                "logged_out": True
            }))
        else:
            print("Status: 500 Internal Server Error")
            print("Content-Type: application/json")
            print()
            print(json.dumps({"ok": False, "error": "Erreur lors de la déconnexion"}))
    else:
        print("Status: 401 Unauthorized")
        print("Content-Type: application/json")
        print("Set-Cookie: SESSIONID=deleted; Path=/; Expires=Thu, 01 Jan 1970 00:00:00 GMT; HttpOnly")
        print()
        print(json.dumps({"ok": False, "error": "Session invalide ou expirée"}))
else:
    print("Status: 400 Bad Request")
    print("Content-Type: application/json")
    print()
    print(json.dumps({"ok": False, "error": "Aucune session active trouvée"}))
