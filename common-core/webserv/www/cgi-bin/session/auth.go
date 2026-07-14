package main

import (
	"crypto/rand"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"math/big"
	"os"
	"path/filepath"
	"strconv"
	"strings"
	"time"
)

const (
	CookieName           = "SESSIONID"
	SessionMaxAgeSeconds = 3600
	DefaultSessionsRel   = "./sessions.json"
	MaxRequestBodyBytes  = 1 << 20 // cap for stdin body
	TimeLayout           = "2006-01-02 15:04:05"
)

type SessionData struct {
	Username   string `json:"username"`
	Password   string `json:"password"`
	CreatedAt  int64  `json:"created_at"`  // epoch seconds
	LastAccess int64  `json:"last_access"` // epoch seconds
	IP         string `json:"ip"`
}

type Sessions map[string]SessionData

func main() {
	switch os.Getenv("QUERY_STRING") {
	case "login":
		requireMethod("POST")
		handleLogin()
	case "logout":
		requireMethod("POST")
		handleLogout()
	case "user":
		requireMethod("GET")
		handleUser()
	default:
		writeError(400, "Invalid request")
	}
}

func requireMethod(expected string) {
	method := os.Getenv("REQUEST_METHOD")
	if method == "" || method != expected {
		writeError(400, "Invalid method")
		os.Exit(0)
	}
}

func checkUserPassword(username, password string) bool {
	return username == "test" && password == "test" ||
		username == "admin" && password == "password123" ||
		username == "user" && password == "user123"
}

func handleLogin() {
	body, err := readRequestBody()
	if err != nil {
		writeError(400, "Invalid request body")
		return
	}

	var req struct {
		Username string `json:"username"`
		Password string `json:"password"`
	}
	dec := json.NewDecoder(strings.NewReader(string(body)))
	dec.DisallowUnknownFields()
	if err := dec.Decode(&req); err != nil {
		writeError(400, "Invalid JSON payload")
		return
	}

	username := strings.TrimSpace(req.Username)
	password := strings.TrimSpace(req.Password)
	if username == "" || password == "" {
		writeError(401, "Missing username or password")
		return
	}

	if !checkUserPassword(username, password) {
		writeError(401, "Invalid username or password")
		return
	}

	sessions, _ := loadSessions()
	cleanupExpiredSessions(sessions, SessionMaxAgeSeconds)

	sessionID, err := generateSessionID(32)
	if err != nil {
		writeError(500, "Failed to generate session ID")
		return
	}

	now := time.Now().Unix()
	ip := os.Getenv("REMOTE_ADDR")
	if ip == "" {
		ip = "unknown"
	}
	sessions[sessionID] = SessionData{
		Username:   username,
		Password:   password,
		CreatedAt:  now,
		LastAccess: now,
		IP:         ip,
	}
	if err := saveSessions(sessions); err != nil {
		writeError(500, "Internal server error")
		return
	}

	writeOK(
		map[string]any{
			"ok":         true,
			"username":   username,
			"session_id": fmt.Sprintf("%s...", mask(sessionID, 8)),
		},
		buildSetCookie(sessionID, SessionMaxAgeSeconds),
	)
}

func handleLogout() {
	sessionID := getCookieValue(CookieName)
	if sessionID == "" {
		writeError(400, "No active session found")
		return
	}

	sessions, _ := loadSessions()
	if _, ok := sessions[sessionID]; ok {
		delete(sessions, sessionID)
		if err := saveSessions(sessions); err != nil {
			writeError(500, "Failed to logout")
			return
		}
		writeOK(map[string]any{
			"ok":         true,
			"logged_out": true,
		}, buildClearCookie())
		return
	}
	writeError(401, "Invalid or expired session")
}

func handleUser() {
	sessionID := getCookieValue(CookieName)
	if sessionID == "" {
		writeError(401, "Unauthorized")
		return
	}

	sessions, _ := loadSessions()
	cleanupExpiredSessions(sessions, SessionMaxAgeSeconds)

	s, ok := sessions[sessionID]
	if !ok {
		writeError(401, "Invalid or expired session")
		return
	}

	now := time.Now()
	s.LastAccess = now.Unix()
	sessions[sessionID] = s
	_ = saveSessions(sessions)

	created := time.Unix(s.CreatedAt, 0)
	lastAcc := time.Unix(s.LastAccess, 0)

	resp := map[string]any{
		"ok":       true,
		"username": s.Username,
		"session_info": map[string]any{
			"session_id":             fmt.Sprintf("%s...", mask(sessionID, 8)),
			"created_at":             created.Format(TimeLayout),
			"last_access":            lastAcc.Format(TimeLayout),
			"session_duration":       humanDuration(now.Sub(created)),
			"time_since_last_access": humanDuration(now.Sub(lastAcc)),
			"ip_address":             s.IP,
			"total_sessions":         len(sessions),
		},
	}
	writeOK(resp, buildSetCookie(sessionID, SessionMaxAgeSeconds))
}

func readRequestBody() ([]byte, error) {
	cl := os.Getenv("CONTENT_LENGTH")
	if cl == "" {
		return io.ReadAll(io.LimitReader(os.Stdin, MaxRequestBodyBytes))
	}
	n, err := strconv.Atoi(cl)
	if err != nil || n < 0 {
		return nil, errors.New("bad content length")
	}
	if n > MaxRequestBodyBytes {
		return nil, errors.New("request too large")
	}
	return io.ReadAll(io.LimitReader(os.Stdin, int64(n)))
}

func writeOK(body map[string]any, extraHeaders ...string) {
	fmt.Println("Status: 200 OK")
	fmt.Println("Content-Type: application/json")
	for _, h := range extraHeaders {
		fmt.Println(h)
	}
	fmt.Println()
	_ = json.NewEncoder(os.Stdout).Encode(body)
}

func writeError(statusCode int, message string) {
	fmt.Printf("Status: %d %s\n", statusCode, statusText(statusCode))
	fmt.Println("Content-Type: application/json")
	if statusCode == 400 || statusCode == 401 {
		fmt.Println(buildClearCookie())
	}
	fmt.Println()
	_ = json.NewEncoder(os.Stdout).Encode(map[string]any{"ok": false, "error": message})
}

// func writeError(statusCode int, body map[string]any) {
// 	fmt.Printf("Status: %d %s\n", statusCode, statusText(statusCode))
// 	fmt.Println("Content-Type: application/json")
// 	if statusCode == 400 || statusCode == 401 {
// 		fmt.Println(buildClearCookie())
// 	}
// 	fmt.Println()
// 	_ = json.NewEncoder(os.Stdout).Encode(body)
// }

func buildSetCookie(value string, maxAge int) string {
	return fmt.Sprintf("Set-Cookie: %s=%s; Path=/; HttpOnly; SameSite=Strict; Max-Age=%d", CookieName, value, maxAge)
}

func buildClearCookie() string {
	return fmt.Sprintf("Set-Cookie: %s=; Path=/; Max-Age=0; Expires=Thu, 01 Jan 1970 00:00:00 GMT; HttpOnly; SameSite=Strict", CookieName)
}

func getCookieValue(name string) string {
	raw := os.Getenv("HTTP_COOKIE")
	if raw == "" {
		return ""
	}
	parts := strings.Split(raw, ";")
	prefix := name + "="
	for i := range parts {
		c := strings.TrimSpace(parts[i])
		if strings.HasPrefix(c, prefix) {
			return strings.TrimPrefix(c, prefix)
		}
	}
	return ""
}

func sessionsPath() string {
	if v := os.Getenv("SESSIONS_FILE"); v != "" {
		return v
	}
	if v := os.Getenv("SCRIPT_FILENAME"); v != "" {
		return filepath.Join(filepath.Dir(v), "sessions.json")
	}
	return DefaultSessionsRel
}

func loadSessions() (Sessions, error) {
	path := sessionsPath()
	f, err := os.Open(path)
	if err != nil {
		return make(Sessions), nil
	}
	defer f.Close()
	var s Sessions
	if err := json.NewDecoder(f).Decode(&s); err != nil {
		return make(Sessions), nil
	}
	return s, nil
}

func saveSessions(s Sessions) error {
	path := sessionsPath()
	data, err := json.MarshalIndent(s, "", "  ")
	if err != nil {
		return err
	}
	tmp := path + ".tmp"
	if err := os.WriteFile(tmp, data, 0644); err != nil {
		return err
	}
	return os.Rename(tmp, path)
}

func cleanupExpiredSessions(s Sessions, maxAgeSec int64) {
	now := time.Now().Unix()
	for id, v := range s {
		if now-v.LastAccess > maxAgeSec {
			delete(s, id)
		}
	}
}

func generateSessionID(n int) (string, error) {
	const letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
	b := make([]byte, n)
	for i := 0; i < n; i++ {
		idx, err := rand.Int(rand.Reader, big.NewInt(int64(len(letters))))
		if err != nil {
			return "", err
		}
		b[i] = letters[idx.Int64()]
	}
	return string(b), nil
}

func mask(s string, visible int) string {
	if len(s) <= visible {
		return s
	}
	return s[:visible]
}

func humanDuration(d time.Duration) string {
	if d < time.Minute {
		return fmt.Sprintf("%d secondes", int(d.Seconds()))
	}
	if d < time.Hour {
		return fmt.Sprintf("%d minutes", int(d.Minutes()))
	}
	h := int(d.Hours())
	m := int(d.Minutes()) % 60
	return fmt.Sprintf("%dh %dm", h, m)
}

func statusText(code int) string {
	switch code {
	case 400:
		return "Bad Request"
	case 401:
		return "Unauthorized"
	case 500:
		return "Internal Server Error"
	default:
		return "Error"
	}
}
