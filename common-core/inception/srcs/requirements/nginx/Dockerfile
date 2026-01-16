FROM debian:bookworm

RUN apt-get update -y && \
	apt-get install nginx openssl -y

RUN mkdir -p /etc/nginx/ssl \
	&& openssl req -x509 -nodes -days 365 -newkey rsa:2048 \
	-subj "/C=BE/ST=Bruxelles/L=Bruxelles/O=42/CN=maxweert.42.fr" \
	-keyout /etc/nginx/ssl/nginx.key \
	-out /etc/nginx/ssl/nginx.crt

COPY ./conf/default.conf /etc/nginx/conf.d/default.conf

CMD ["nginx", "-g", "daemon off;"]

