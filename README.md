# `pg_uuidv7`: Use the new v7 UUIDs in Postgres

A tiny Postgres extension to create valid [version 7 UUIDs](https://www.ietf.org/archive/id/draft-ietf-uuidrev-rfc4122bis-00.html#name-uuid-version-7)
in Postgres.

These are regular Postgres UUIDs, so they can be used as primary keys, converted
to and from strings, included in indexes, etc:

```
SELECT uuid_generate_v7();

           uuid_generate_v7           
--------------------------------------
 018570bb-4a7d-7c7e-8df4-6d47afd8c8fc
(1 row)
```

## Background

Version 7 UUIDs have a few advantages. They include a 48-bit Unix timestamp with
millisecond accuracy and will overflow far in the future (10899 AD). They also
include 74 random bits which means [billions can be created every second](https://en.wikipedia.org/wiki/Birthday_problem#Probability_table)
without collisions. Because of their structure they are globally sortable and
can be created in parallel in a distributed system.

## Build

### Build locally

`pg_uuidv7` only requires the `libpq` headers and Postgres extension tools to
build the code. On Debian, these headers are included in the `libpq-dev` and
`postgresql-server-dev-all` packages.

To build the code run `make`.

### Build using Docker

A [`Dockerfile`](Dockerfile) is available to build the code using the official
Postgres Docker image:

```sh
docker build . --tag pg_uuidv7
```
