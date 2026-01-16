#!/usr/bin/env bash
set -euo pipefail

if [ -z "${MARIADB_ROOT_PASSWORD:-}" ] && [ -f /run/secrets/mariadb_root_password ]; then
	MARIADB_ROOT_PASSWORD="$(cat /run/secrets/mariadb_root_password)"
fi

if [ -z "${MARIADB_PASSWORD:-}" ] && [ -f /run/secrets/mariadb_password ]; then
	MARIADB_PASSWORD="$(cat /run/secrets/mariadb_password)"
fi

required_vars="MARIADB_ROOT_PASSWORD MARIADB_DATABASE MARIADB_USER MARIADB_PASSWORD"

for var in $required_vars; do
	# eval to read the value of the variable name in $var; POSIX-friendly substitute for ${!var}.
	eval "val=\${$var-}"
	if [ -z "${val:-}" ]; then
		echo "[mariadb] Missing required env: $var" >&2
		exit 1
	fi
done

mkdir -p /run/mysqld
chown -R mysql:mysql /run/mysqld /var/lib/mysql

SOCKET="/run/mysqld/mysqld.sock"
PIDFILE="/run/mysqld/mariadb.pid"
fresh_db=0

if [ ! -d /var/lib/mysql/mysql ]; then
	fresh_db=1
	mariadb-install-db --user=mysql --ldata=/var/lib/mysql >/dev/null
fi

# Start a temporary server with networking disabled so we can run SQL with grants active
mysqld --user=mysql --skip-networking --socket="$SOCKET" --datadir=/var/lib/mysql --pid-file="$PIDFILE" &

ROOT_FLAGS=""
[ "$fresh_db" -eq 0 ] && ROOT_FLAGS="-p${MARIADB_ROOT_PASSWORD}"

for i in $(seq 30); do
	if mysqladmin --protocol=socket --socket="$SOCKET" -uroot $ROOT_FLAGS ping >/dev/null 2>&1; then
		break
	fi
	sleep 1
done

if ! mysqladmin --protocol=socket --socket="$SOCKET" -uroot $ROOT_FLAGS ping >/dev/null 2>&1; then
	echo "[mariadb] bootstrap mysqld did not start" >&2
	exit 1
fi

if [ "$fresh_db" -eq 1 ]; then
	mariadb --protocol=socket --socket="$SOCKET" -uroot <<EOF
ALTER USER IF EXISTS 'root'@'localhost' IDENTIFIED VIA mysql_native_password USING PASSWORD('${MARIADB_ROOT_PASSWORD}');
CREATE USER IF NOT EXISTS 'root'@'%' IDENTIFIED VIA mysql_native_password USING PASSWORD('${MARIADB_ROOT_PASSWORD}');
ALTER USER 'root'@'%' IDENTIFIED VIA mysql_native_password USING PASSWORD('${MARIADB_ROOT_PASSWORD}');
GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' WITH GRANT OPTION;
CREATE DATABASE IF NOT EXISTS \`${MARIADB_DATABASE}\` CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
CREATE USER IF NOT EXISTS '${MARIADB_USER}'@'%' IDENTIFIED BY '${MARIADB_PASSWORD}';
GRANT ALL PRIVILEGES ON \`${MARIADB_DATABASE}\`.* TO '${MARIADB_USER}'@'%';
FLUSH PRIVILEGES;
EOF
	ROOT_FLAGS="-p${MARIADB_ROOT_PASSWORD}"
else
	mariadb --protocol=socket --socket="$SOCKET" -uroot $ROOT_FLAGS <<EOF
ALTER USER IF EXISTS 'root'@'localhost' IDENTIFIED VIA mysql_native_password USING PASSWORD('${MARIADB_ROOT_PASSWORD}');
CREATE USER IF NOT EXISTS 'root'@'%' IDENTIFIED VIA mysql_native_password USING PASSWORD('${MARIADB_ROOT_PASSWORD}');
ALTER USER 'root'@'%' IDENTIFIED VIA mysql_native_password USING PASSWORD('${MARIADB_ROOT_PASSWORD}');
CREATE DATABASE IF NOT EXISTS \`${MARIADB_DATABASE}\` CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
CREATE USER IF NOT EXISTS '${MARIADB_USER}'@'%' IDENTIFIED BY '${MARIADB_PASSWORD}';
ALTER USER '${MARIADB_USER}'@'%' IDENTIFIED BY '${MARIADB_PASSWORD}';
GRANT ALL PRIVILEGES ON \`${MARIADB_DATABASE}\`.* TO '${MARIADB_USER}'@'%';
FLUSH PRIVILEGES;
EOF
fi

mysqladmin --protocol=socket --socket="$SOCKET" -uroot $ROOT_FLAGS shutdown >/dev/null

exec mysqld_safe --datadir=/var/lib/mysql --user=mysql