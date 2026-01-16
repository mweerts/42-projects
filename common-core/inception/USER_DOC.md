# User Documentation

## Services Provided

- **Nginx**: HTTPS reverse proxy and TLS termination.
- **WordPress (PHP-FPM)**: Website and admin UI.
- **MariaDB**: Persistent database for WordPress.

## Starting and Stopping

- Start (build + up): `make` or `make up`
- Stop: `make down`
- Logs: `make logs` (all) or `make logs-X` for a specific service (`X` = nginx | wordpress | mariadb)

## Accessing the Site and Admin

1. Add to your hosts file (Linux/macOS `/etc/hosts`, Windows `C:\Windows\System32\drivers\etc\hosts`):
   - `127.0.0.1 maxweert.42.fr` (or your host IP if remote)
2. Browse: `https://maxweert.42.fr`
3. Admin panel: `https://maxweert.42.fr/wp-admin`

## Credentials

- Admin user: value of `WP_ADMIN_USER` in `.env`
- Admin password: contents of `secrets/wp_admin_password.txt`
- Regular user: value of `WP_USER` in `.env`
- Regular user password: `secrets/wp_user_password.txt`
- Database user/password: `MARIADB_USER` in `.env`, password in `secrets/mariadb_password.txt`

## Checking Service Health

- WordPress: `make logs-wordpress`
- Nginx: `make logs-nginx`
- MariaDB: `make logs-mariadb`
- Process status: `docker compose -f srcs/docker-compose.yml ps`

## Managing Credentials

- Secrets live as files in `secrets/` (one line per file). Update the file, then restart the affected service: `docker compose -f srcs/docker-compose.yml up -d wordpress mariadb`.

## Stopping and Cleaning Up

- Stop: `make down`
- Remove everything (images, volumes): `make fclean` (data loss for DB and WordPress uploads when volumes are removed).
