FROM postgres:16

RUN apt-get update && apt-get -y upgrade \
  && apt-get install -y build-essential libpq-dev postgresql-server-dev-all

WORKDIR /srv
COPY . /srv

RUN for v in `seq 13 16`; do pg_buildext build-$v $v; done

RUN cp sql/pg_uuidv7--1.4.sql . && TARGETS=$(find * -name pg_uuidv7.so) \
  && tar -czvf pg_uuidv7.tar.gz $TARGETS pg_uuidv7--1.4.sql pg_uuidv7.control \
  && sha256sum pg_uuidv7.tar.gz $TARGETS pg_uuidv7--1.4.sql pg_uuidv7.control > SHA256SUMS

RUN cp ${PG_MAJOR}/pg_uuidv7.so /usr/lib/postgresql/${PG_MAJOR}/lib \
  && cp pg_uuidv7.control /usr/share/postgresql/${PG_MAJOR}/extension \
  && cp pg_uuidv7--1.4.sql /usr/share/postgresql/${PG_MAJOR}/extension
