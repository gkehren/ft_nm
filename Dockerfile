FROM alpine:latest

RUN apk update && apk add binutils gcc make musl-dev

COPY . /app

WORKDIR /app

CMD ["/bin/sh"]
