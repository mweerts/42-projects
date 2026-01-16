# Developer Documentation

## Environment Setup

- Prerequisites: Docker, Docker Compose, Make.
- Ensure your user can run Docker (on WSL2, Docker Desktop or service must be running).
- Export `USERNAME` if different from `maxweert` so bind mounts point to `/home/${USERNAME}/data/...`.

## Configuration and Secrets

- Create `.env` at repo root with at least:
  - `USERNAME=your_login`
  - `DOMAIN_NAME=maxweert.42.fr`
  - `MARIADB_DATABASE=wordpress`
  - `MARIADB_USER=wp_user`
  - `WP_ADMIN_USER=admin`, `WP_USER=author`, plus any emails/titles.
- Secrets (one line each) in `secrets/`:
  - `mariadb_root_password.txt`
  - `mariadb_password.txt`
  - `wp_admin_password.txt`
  - `wp_user_password.txt`

## Build and Launch

- Prepare data dirs: `make setup_dirs`
- Build: `make build`
- Start: `make up` (detached) or `docker compose -f srcs/docker-compose.yml up --build`
- Logs: `make logs` or `docker compose -f srcs/docker-compose.yml logs -f nginx wordpress mariadb`
- Stop: `make down`
- Clean images/volumes: `make fclean`

## Managing Containers and Volumes

- Status: `docker compose -f srcs/docker-compose.yml ps`
- Shell inside a service: `docker exec -it wordpress bash` (or `nginx`, `mariadb`)
- Remove volumes if you need a clean DB/site (data loss): `docker volume rm mariadb wordpress`
- Data paths on host (bind mounts): `/home/${USERNAME}/data/mariadb` and `/home/${USERNAME}/data/wordpress`
- Access to the database: `docker compose -f srcs/docker-compose.yml exec mariadb mariadb -u"USER" -p"PASSWORD"`

## Where Things Live

- Compose file: `srcs/docker-compose.yml`
- Service builds: `srcs/requirements/nginx`, `srcs/requirements/wordpress`, `srcs/requirements/mariadb`
- Secrets: `secrets/`
- Persistent data: bind mounts under `/home/${USERNAME}/data/*`

## Notes

- Nginx serves TLS with a self-signed cert baked at build time; browsers will warn until you add an exception or trust the cert.
- The stack uses the user-defined bridge network `inception` for internal DNS (`wordpress`, `mariadb`).
