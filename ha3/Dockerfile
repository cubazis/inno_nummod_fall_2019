FROM alpine

RUN apk add --no-cache cmake gcc g++ python3 make

WORKDIR /workdir
COPY example .

RUN cmake . && make

CMD python3 run.py .