FROM postgres:15-bullseye

RUN apt-get update && apt-get -y upgrade \
  && apt-get install -y build-essential libpq-dev postgresql-server-dev-all

WORKDIR /srv
COPY . /srv

RUN make

RUN tar -czvf pg_uuidv7.tar.gz pg_uuidv7--1.2.sql pg_uuidv7.control pg_uuidv7.so \
  && sha256sum pg_uuidv7--1.2.sql pg_uuidv7.control pg_uuidv7.so pg_uuidv7.tar.gz > SHA256SUMS
