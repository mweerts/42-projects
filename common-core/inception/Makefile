SERVICES = mariadb nginx wordpress

all: setup_dirs build up

setup_dirs:
	mkdir -p /home/maxweert/data/mariadb
	mkdir -p /home/maxweert/data/wordpress

build:
	docker compose -f srcs/docker-compose.yml build ${SERVICES}

up:
	docker compose -f srcs/docker-compose.yml up -d ${SERVICES}

down:
	docker compose -f srcs/docker-compose.yml down ${SERVICES}

restart:
	docker compose -f srcs/docker-compose.yml restart ${SERVICES}

clean: 
	docker compose -f srcs/docker-compose.yml down --rmi all --remove-orphans
	docker system prune -af --volumes

fclean:
	docker compose -f srcs/docker-compose.yml down --rmi all --volumes --remove-orphans
	docker system prune -af --volumes

logs:
	docker compose -f srcs/docker-compose.yml logs -f ${SERVICES}

logs-%:
	docker compose -f srcs/docker-compose.yml logs -f $*

exec-%:
	docker exec -it $* bash
	

re: fclean all