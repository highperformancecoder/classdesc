FROM highperformancecoder/builttravisciimage
COPY . /root
RUN cd /root && make TRAVIS=1 travis-test

