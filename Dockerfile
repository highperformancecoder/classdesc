FROM ubuntu
ADD . /root
RUN apt-get update -qq -y
RUN apt-get install -y wget gnupg2 g++
RUN wget http://download.opensuse.org/repositories/home:hpcoder1/xUbuntu_18.04/Release.key
RUN apt-key add - < Release.key
RUN echo 'deb http://download.opensuse.org/repositories/home:/hpcoder1/xUbuntu_18.04/ /' >/tmp/hpcoders.list
RUN mv /tmp/hpcoders.list /etc/apt/sources.list.d/
RUN apt-get update -qq -y
RUN apt-get install -y --allow-unauthenticated meson libxml2 libboost-dev json-spirit 
RUN cd /root && TRAVIS=true CC=gcc CXX=g++ meson builddir && ninja -v -C builddir -j 1
