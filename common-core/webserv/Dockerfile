FROM debian:bookworm-slim AS builder
RUN apt-get update \
 && apt-get install -y --no-install-recommends g++ make ca-certificates \
 && rm -rf /var/lib/apt/lists/*
WORKDIR /src
COPY . /src
RUN make -j"$(nproc)"

FROM debian:bookworm-slim AS runtime
RUN apt-get update \
 && apt-get install -y --no-install-recommends \
      python3 bash php-cgi golang ca-certificates \
 && rm -rf /var/lib/apt/lists/*

RUN useradd -m -u 10001 web

# App layout
RUN mkdir -p /usr/local/app/config /usr/local/app/www \
 && chown -R web:web /usr/local/app
RUN mkdir -p /usr/local/app/www/uploads \
 && chown -R web:web /usr/local/app/www/uploads
WORKDIR /usr/local/app

COPY --chown=web:web www/ /usr/local/app/www/
COPY --chown=web:web cgi-bin/ /usr/local/app/cgi-bin/

COPY --from=builder /src/webserv /usr/local/bin/webserv

COPY config/default-container.conf /usr/local/app/config/default.conf
RUN chown -R web:web /usr/local/app/config

EXPOSE 8080 8081

USER web
ENV CONFIG_FILE=/usr/local/app/config/default.conf
ENTRYPOINT ["/usr/local/bin/webserv","-c","/usr/local/app/config/default.conf"]
