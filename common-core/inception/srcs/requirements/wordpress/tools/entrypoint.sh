#!/usr/bin/env bash
set -euo pipefail

# Provide minimal HTTP context for wp-cli to silence HTTP_HOST warnings
HTTP_HOST_VALUE="${DOMAIN_NAME:-localhost}"
export HTTP_HOST="$HTTP_HOST_VALUE"
export SERVER_NAME="$HTTP_HOST_VALUE"

if [ -z "${MARIADB_PASSWORD:-}" ] && [ -f /run/secrets/mariadb_password ]; then
	MARIADB_PASSWORD="$(cat /run/secrets/mariadb_password)"
fi

if [ ! -f /var/www/html/wp-config-sample.php ]; then
  wp core download --path=/var/www/html --allow-root
  chown -R www-data:www-data /var/www/html
fi

if [ ! -f /var/www/html/wp-config.php ]; then
  wp config create --allow-root --dbname="$MARIADB_DATABASE" --dbuser="$MARIADB_USER" --dbpass="$MARIADB_PASSWORD" --dbhost=mariadb --path=/var/www/html
fi

if ! wp core is-installed --allow-root --path=/var/www/html; then
    wp core install --allow-root --url=https://$DOMAIN_NAME --title=$WP_TITLE --admin_user=$WP_ADMIN_USER \
  --admin_password=$(cat /run/secrets/wp_admin_password) --admin_email=$WP_ADMIN_EMAIL --path=/var/www/html --skip-email
fi

if ! wp user get $WP_USER --allow-root --path=/var/www/html; then
  wp user create $WP_USER $WP_USER_EMAIL --allow-root --role=author --user_pass=$(cat /run/secrets/wp_user_password) --path=/var/www/html
fi

exec php-fpm8.2 -F