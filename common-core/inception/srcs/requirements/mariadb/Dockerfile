FROM debian:bookworm

RUN apt-get update -y \
	&& apt-get install -y mariadb-server mariadb-client bash \
	&& rm -rf /var/lib/apt/lists/*

COPY conf/50-server.cnf /etc/mysql/mariadb.conf.d/50-server.cnf
COPY ./tools/entrypoint.sh /usr/local/bin/entrypoint.sh

RUN chmod +x /usr/local/bin/entrypoint.sh

EXPOSE 3306

ENTRYPOINT [ "bash", "/usr/local/bin/entrypoint.sh" ]