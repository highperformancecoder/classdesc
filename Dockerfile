FROM highperformancecoder/builttravisciimage
COPY . /root
RUN ln -sf /usr/bin/g++-13 /usr/bin/g++
RUN cd /root && make -j4 AEGIS=1 DEBUG=1 sure

