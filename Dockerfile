FROM debian:latest

RUN apt-get update && apt-get install -y \
	gcc \
	gcc-multilib \
	vim \
	file \
	valgrind \
	make

COPY . /app

WORKDIR /app

CMD ["/bin/bash"]
