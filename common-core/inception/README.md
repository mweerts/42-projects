_This project has been created as part of the 42 curriculum by maxweert._

# Inception

## Description

This project builds a small containerized web stack with Docker Compose: Nginx terminating TLS, WordPress running on PHP-FPM, and MariaDB as the database. It is intended to demonstrate container orchestration, secret handling, and persistent data management in a reproducible way.

## Project Description and Design Choices

- **Docker usage and sources**: All sources live under `srcs/requirements` with separate build contexts for `nginx`, `wordpress`, and `mariadb`. Secrets are mounted from the `secrets/` directory. Persistent data is stored via bind mounts under `/home/${USERNAME}/data` (WordPress files) and `/home/${USERNAME}/data/mariadb` (database). The Makefile wires the main lifecycle commands.
- **Virtual Machines vs Docker**: VMs virtualize hardware and run full guest OSes with higher overhead; Docker containers share the host kernel, start faster, and are lighter for isolated services like this stack.
- **Secrets vs Environment Variables**: Secrets are read from files in `secrets/` at runtime, reducing exposure in process lists and images. Environment variables are used for non-sensitive configuration such as database name and usernames.
- **Docker Network vs Host Network**: A user-defined bridge network (`inception`) provides service-to-service DNS (`wordpress`, `mariadb`) and isolation. Host networking is avoided to reduce port conflicts and leakage.
- **Docker Volumes vs Bind Mounts**: Bind mounts into host directories (`/home/${USERNAME}/data/...`) make data visible on the host for backups and inspection. Named volumes exist in Compose, but they map to these bind paths for persistence across rebuilds.

## Instructions

1. **Prerequisites**: Docker, Docker Compose, and Make. On Linux/WSL2, ensure your user can run Docker. Export `USERNAME` if your login differs from `mweerts` so bind mounts point to the right host path.
2. **Prepare configuration**:
   - Create `.env` with at least: `USERNAME=...`, `DOMAIN_NAME=maxweert.42.fr`, `MARIADB_DATABASE=wordpress`, `MARIADB_USER=wp_user`.
   - Populate secrets files in `secrets/`: `mariadb_root_password.txt`, `mariadb_password.txt`, `wp_admin_password.txt`, `wp_user_password.txt` (one line per file).
3. **Initialize data directories**: `make setup_dirs` (creates `/home/${USERNAME}/data/mariadb` and `/home/${USERNAME}/data/wordpress`).
4. **Build and start**: `make` (runs build then up) or `make up` to start containers in detached mode. Logs: `make logs` or `docker compose -f srcs/docker-compose.yml logs -f nginx wordpress mariadb`.
5. **Access the site**: Add `maxweert.42.fr` pointing to your host IP (e.g., `127.0.0.1`) in your `/etc/hosts` (or Windows hosts file). Then browse `https://maxweert.42.fr` and accept the self-signed certificate.
6. **Stop/clean**: `make down` to stop; `make fclean` to remove images and volumes.

## Resources and AI Usage

- Docker: https://docs.docker.com/
- Docker Compose: https://docs.docker.com/compose/
- Nginx: https://nginx.org/en/docs/
- MariaDB: https://mariadb.com/kb/en/documentation/
- WordPress CLI: https://developer.wordpress.org/cli/commands/
- AI usage: GPT-5.1-Codex-Max was used to draft documentation, summarize configuration choices, sanity-check commands, and assist with a few shell/bash commands
