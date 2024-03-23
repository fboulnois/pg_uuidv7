FROM postgres:16 AS env-build

# install build dependencies
RUN apt-get update && apt-get -y upgrade \
  && apt-get install -y build-essential libpq-dev postgresql-server-dev-all

WORKDIR /srv
COPY . /srv

# build extension for all supported versions
RUN for v in `seq 13 16`; do pg_buildext build-$v $v; done

# create tarball and checksums
RUN cp sql/pg_uuidv7--1.5.sql . && TARGETS=$(find * -name pg_uuidv7.so) \
  && tar -czvf pg_uuidv7.tar.gz $TARGETS pg_uuidv7--1.5.sql pg_uuidv7.control \
  && sha256sum pg_uuidv7.tar.gz $TARGETS pg_uuidv7--1.5.sql pg_uuidv7.control > SHA256SUMS

FROM postgres:16 AS env-deploy

# copy tarball and checksums
COPY --from=0 /srv/pg_uuidv7.tar.gz /srv/SHA256SUMS /srv/

# add extension to postgres
COPY --from=0 /srv/${PG_MAJOR}/pg_uuidv7.so /usr/lib/postgresql/${PG_MAJOR}/lib
COPY --from=0 /srv/pg_uuidv7.control /usr/share/postgresql/${PG_MAJOR}/extension
COPY --from=0 /srv/pg_uuidv7--1.5.sql /usr/share/postgresql/${PG_MAJOR}/extension

FROM scratch AS extension-deploy

COPY --from=0 /srv/${PG_MAJOR}/pg_uuidv7.so /usr/lib/postgresql/${PG_MAJOR}/lib
COPY --from=0 /srv/pg_uuidv7.control /usr/share/postgresql/${PG_MAJOR}/extension
COPY --from=0 /srv/pg_uuidv7--1.5.sql /usr/share/postgresql/${PG_MAJOR}/extension
