FROM gcc:latest

WORKDIR /app

COPY aquawatt.c .

RUN gcc -Wall -Wextra aquawatt.c -o aquawatt -lm

ENTRYPOINT ["./aquawatt"]