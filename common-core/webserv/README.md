# webserv

## Quick start (Docker)
- Using make:
  ```
  make docker
  ```
- Build:
  ```
  docker build -t webserv:dev .
  ```
- Run:
  ```
  docker run --rm --name webserv -p 8080:8080 -p 8081:8081 webserv:dev
  ```
- Exec into the container (non-root):
  ```
  docker exec -it webserv bash
  ```
- Stop:
  ```
  docker rm -f webserv
  ```

Notes:
- Container listens on 0.0.0.0 and serves files from /usr/local/app/www/.
- PHP, python and Go CGI are enabled in the container (`/usr/bin/php-cgi`, `/usr/bin/go`).

## Local build and run (macOS/Linux)
Prereqs: g++ (C++98), make.

- Build:
  ```make```
- Run (CMake dev run with your config):
  ```make cdev -c config/default.conf```
- Or run the binary directly:
  ```./webserv -c config/default.conf```

Ports:
- Default configs expose 8080 and 8081.
- Visit http://127.0.0.1:8080/ (or the port you configured).

## Command-line flags
- -c <path>: path to configuration file (config/default.conf by default).
- -v: verbose logs.

## Configuration basics (Nginx-like)
Important: root does not support relative paths yet. Use absolute paths for:
- root
- upload_dir
- tmp_folder

Minimal example: 
```
server {
	listen 8080;
	host 127.0.0.1;
	server_name local;
	root /absolute/path/to/webserv/www/; # absolute path required
	index index.html;
	autoindex on;
	client_max_body_size 52428800; # 50 MB
	upload_dir /absolute/path/to/webserv/www/uploads;

	location /upload {
      allow_methods POST GET;
	  client_max_body_size 1048576000; #1GB
  	}

	location cgi-bin {
		root /absolute/path/to/cgi-bin/;
		cgi_path /usr/bin/python3 /bin/bash /path/to/go /path/to/php-cgi;
		cgi_ext .py .sh .go .php;
	}
}
```

- The server uses the exact paths in `cgi_path` (it does not search PATH).

## Useful endpoints
- GET /           -> serves static files from root
- GET /status     -> JSON status
- GET /config     -> JSON server config
- /upload, /uploads -> upload/list/delete depending on your config

## Make targets
- all: build `webserv`
- run: run `./webserv` (uses the default config path; pass flags manually if needed)
- clean / fclean / re: usual cleanup/rebuild
- cbuild: CMake build (creates `build/`)
- cdev: CMake build + run with verbose flag
- crun: CMake build + run

## Troubleshooting
- Port already in use:
  - Change `listen` values or stop the conflicting service.
