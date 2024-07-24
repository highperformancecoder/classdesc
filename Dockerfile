FROM highperformancecoder/builttravisciimage
COPY . /root
RUN cd /root && make AEGIS=1 DEBUG=1 sure

