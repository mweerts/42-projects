# Usa un'immagine base ufficiale
FROM ubuntu:22.04

# Imposta la directory di lavoro
WORKDIR /usr/local/app/
# Installa le dipendenze (incluso python3 e pip)
RUN apt-get update && \
    apt-get install -y python3 python3-pip make curl siege valgrind && \
    apt-get clean

# Copia tutto il contenuto della repo nel container
COPY . .

# (Opzionale) Compila il programma
RUN make re .
RUN chmod 777 ./webserv
RUN mkdir -p www && chmod 777 www
RUN ls -l


# Imposta la directory dove si trova lo script
WORKDIR /usr/local/app/srcs-tester/py-tester/

# Esegui lo script Python
CMD ["python3", "scritp-siege-valgrind.py"]
