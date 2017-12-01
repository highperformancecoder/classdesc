FROM jpakkane/mesonci:zesty
ADD . /root
RUN wget http://download.opensuse.org/repositories/home:hpcoder1/xUbuntu_17.10/Release.key
RUN echo 'deb http://download.opensuse.org/repositories/home:/hpcoder1/xUbuntu_17.10/ /' >/tmp/hpcoders.list
RUN mv /tmp/hpcoders.list /etc/apt/sources.list.d/
RUN apt-get update -qq -y
RUN apt-key add - < Release.key
RUN apt-get install -y --allow-unauthenticated meson libxml2 libboost-dev json-spirit
RUN cd /root && TRAVIS=true CC=gcc CXX=g++ meson builddir && ninja -C builddir
