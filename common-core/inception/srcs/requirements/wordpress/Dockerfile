FROM debian:bookworm

RUN apt-get update -y && \
	apt-get install php8.2 php8.2-fpm php8.2-mysql curl -y

RUN curl https://raw.githubusercontent.com/wp-cli/builds/gh-pages/phar/wp-cli.phar -o /usr/local/bin/wp && \
	chmod +x /usr/local/bin/wp


COPY ./conf/www.conf /etc/php/8.2/fpm/pool.d/www.conf
COPY ./tools/entrypoint.sh /usr/local/bin/entrypoint.sh

RUN chmod +x /usr/local/bin/entrypoint.sh



ENTRYPOINT [ "bash", "/usr/local/bin/entrypoint.sh" ]