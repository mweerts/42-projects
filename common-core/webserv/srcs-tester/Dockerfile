# Usa un'immagine base ufficiale
FROM ubuntu:22.04
EXPOSE 8080
EXPOSE 8081

# Imposta la directory di lavoro
WORKDIR /usr/local/app/
# Installa le dipendenze (incluso python3 e pip)
RUN apt-get update && \
    apt-get install -y python3 python3-pip make curl siege valgrind && \
    apt-get clean

COPY . .

# (Opzionale) Compila il programma
# RUN make .
# RUN chmod 777 ./webserv
RUN mkdir -p www && chmod 755 www
RUN mkdir -p www/cgi-bin && chmod 755 www/cgi-bin/*.py
RUN ls -l


# Imposta la directory dove si trova lo script
RUN make re .
WORKDIR /usr/local/app/srcs-tester/src/
RUN mv GeneratoreTester  /usr/local/app/srcs-tester/py-tester/.
WORKDIR /usr/local/app/srcs-tester/py-tester/

# Esegui lo script Python
CMD ["python3", "scritp-siege-valgrind.py"]
