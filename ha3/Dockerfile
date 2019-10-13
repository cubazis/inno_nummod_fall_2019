FROM ubuntu:16.04

MAINTAINER <grebenkin@phystech.edu>

RUN apt-get update && apt-get -qq -y install \
    wget \
    make \
    autoconf

RUN  apt-get install -y software-properties-common
RUN add-apt-repository ppa:jonathonf/python-3.6
RUN  apt-get update
RUN  apt-get install -y gcc g++

RUN wget http://www.cmake.org/files/v3.10/cmake-3.10.1.tar.gz
RUN tar -xvzf cmake-3.10.1.tar.gz
RUN cd cmake-3.10.1 && ./configure && make && make install
RUN rm cmake-3.10.1.tar.gz

RUN apt-get install -y build-essential python3.6 python3.6-dev python3-pip python3.6-venv
RUN python3.6 -m pip install pip --upgrade

WORKDIR /workdir