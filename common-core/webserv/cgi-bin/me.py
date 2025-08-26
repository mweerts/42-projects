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

def clean_expired_sessions(sessions, max_age=3600):
    """Nettoie les sessions expirées (plus de max_age secondes)"""
    current_time = time.time()
    expired_sessions = []
    
    for session_id, session_data in sessions.items():
        if current_time - session_data.get("last_access", 0) > max_age:
            expired_sessions.append(session_id)
    
    for session_id in expired_sessions:
        del sessions[session_id]
    
    return sessions

def format_duration(seconds):
    """Formate une durée en secondes en format lisible"""
    if seconds < 60:
        return f"{int(seconds)} secondes"
    elif seconds < 3600:
        return f"{int(seconds // 60)} minutes"
    else:
        hours = int(seconds // 3600)
        minutes = int((seconds % 3600) // 60)
        return f"{hours}h {minutes}m"

# Récupérer l'ID de session depuis les cookies
session_id = get_session_id_from_cookie()

if session_id:
    # Charger et nettoyer les sessions
    sessions = load_sessions()
    sessions = clean_expired_sessions(sessions)
    
    # Vérifier si la session existe et est valide
    if session_id in sessions:
        session_data = sessions[session_id]
        username = session_data.get("username", "unknown")
        created_at = session_data.get("created_at", time.time())
        last_access = session_data.get("last_access", time.time())
        ip = session_data.get("ip", "unknown")
        
        # Mettre à jour le timestamp de dernière activité
        sessions[session_id]["last_access"] = time.time()
        
        # Calculer la durée de session
        session_duration = time.time() - created_at
        time_since_last_access = time.time() - last_access
        
        # Sauvegarder la session mise à jour
        save_sessions(sessions)
        
        # Retourner les informations de l'utilisateur
        print("Content-Type: application/json")
        print()
        print(json.dumps({
            "ok": True,
            "username": username,
            "session_info": {
                "session_id": session_id[:8] + "...",  # Masquer une partie pour la sécurité
                "created_at": time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(created_at)),
                "last_access": time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(last_access)),
                "session_duration": format_duration(session_duration),
                "time_since_last_access": format_duration(time_since_last_access),
                "ip_address": ip,
                "total_sessions": len(sessions)
            },
            "message": f"Connecté en tant que {username}"
        }))
    else:
        # Session non trouvée ou expirée
        print("Status: 401 Unauthorized")
        print("Content-Type: application/json")
        print("Set-Cookie: SESSIONID=deleted; Path=/; Expires=Thu, 01 Jan 1970 00:00:00 GMT; HttpOnly")
        print()
        print(json.dumps({
            "ok": False, 
            "error": "Session invalide ou expirée",
            "redirect": "/login.html"
        }))
else:
    # Pas de session ID dans les cookies
    print("Status: 401 Unauthorized")
    print("Content-Type: application/json")
    print()
    print(json.dumps({
        "ok": False, 
        "error": "Non connecté",
        "redirect": "/login.html"
    }))
