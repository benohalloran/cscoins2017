FROM ubuntu:latest


RUN apt-get update && apt-get install -y apt-utils 
RUN apt-get install -y make cmake libssl-dev zlib1g-dev git gcc g++
COPY . cscoins2017
RUN cd cscoins2017 && git submodule update --init --recursive && make

CMD cd cscoins2017 && ./client
