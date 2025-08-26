#!/usr/bin/env python3
import os, sys, json, random, string, time

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

def generate_session_id(n=32):
    """Génère un ID de session aléatoire"""
    chars = string.ascii_letters + string.digits
    return ''.join(random.choice(chars) for _ in range(n))

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

# Base de données simple des utilisateurs (en production, utiliser une vraie DB)
USERS = {
    "admin": "password123",
    "user": "user123",
    "test": "test"
}

# Lire le corps de la requête (POST JSON)
content_length = int(os.environ.get("CONTENT_LENGTH", 0))
body = sys.stdin.read(content_length) if content_length > 0 else ""

try:
    data = json.loads(body)
    username = data.get("username", "").strip()
    password = data.get("password", "").strip()
except:
    username = password = ""

# Vérification des credentials
if username and password and username in USERS and USERS[username] == password:
    # Charger et nettoyer les sessions existantes
    sessions = load_sessions()
    sessions = clean_expired_sessions(sessions)
    
    # Créer une nouvelle session
    session_id = generate_session_id()
    sessions[session_id] = {
        "username": username,
        "created_at": time.time(),
        "last_access": time.time(),
        "ip": os.environ.get("REMOTE_ADDR", "unknown")
    }
    
    # Sauvegarder les sessions
    if save_sessions(sessions):
        # Succès - envoyer la réponse avec le cookie
        print("Content-Type: application/json")
        print(f"Set-Cookie: SESSIONID={session_id}; Path=/; HttpOnly; SameSite=Strict; Max-Age=3600")
        print()
        print(json.dumps({
            "ok": True, 
            "username": username,
            "message": f"Bienvenue {username}!",
            "session_id": session_id[:8] + "..."  # Afficher seulement les premiers caractères pour debug
        }))
    else:
        # Erreur de sauvegarde
        print("Status: 500 Internal Server Error")
        print("Content-Type: application/json")
        print()
        print(json.dumps({"ok": False, "error": "Erreur interne du serveur"}))
else:
    # Échec de l'authentification
    print("Status: 401 Unauthorized")
    print("Content-Type: application/json")
    print()
    print(json.dumps({
        "ok": False, 
        "error": "Nom d'utilisateur ou mot de passe incorrect",
        "available_users": list(USERS.keys())  # Pour faciliter les tests
    }))
