FROM n0madic/alpine-gcc:7.4.0

RUN apk add --no-cache cmake python3 make

WORKDIR /workdir
COPY example .

RUN cmake . && make

CMD python3 run.py .