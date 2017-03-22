FROM alpine:3.5

RUN apk add --update openssl-dev zlib openssh git build-base linux-headers
COPY . cscoins2017
RUN cd cscoins2017 && git submodule update --init --recursive && make

ENTRYPOINT ['./client']
