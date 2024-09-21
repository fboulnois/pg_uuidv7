# `pg_uuidv7`: Use the new v7 UUIDs in Postgres

A tiny Postgres extension to create valid [version 7 UUIDs](https://www.ietf.org/archive/id/draft-ietf-uuidrev-rfc4122bis-00.html#name-uuid-version-7)
in Postgres.

These are regular Postgres UUIDs, so they can be used as primary keys, converted
to and from strings, included in indexes, etc:

```sql
SELECT uuid_generate_v7();

           uuid_generate_v7
--------------------------------------
 018570bb-4a7d-7c7e-8df4-6d47afd8c8fc
(1 row)
```

The timestamp component of these UUIDs can be extracted:

```sql
SELECT uuid_v7_to_timestamptz('018570bb-4a7d-7c7e-8df4-6d47afd8c8fc');
   uuid_v7_to_timestamptz
----------------------------
 2023-01-02 04:26:40.637+00
(1 row)
```

Timestamps can be converted to v7 UUIDs:

```sql
SELECT uuid_timestamptz_to_v7('2023-01-02 04:26:40.637+00');
        uuid_timestamptz_to_v7
--------------------------------------
 018570bb-4a7d-7630-a5c4-89b795024c5d
(1 row)

-- for date range queries set the second argument to true to zero the random bits
SELECT uuid_timestamptz_to_v7('2023-01-02 04:26:40.637+00', true);
        uuid_timestamptz_to_v7
--------------------------------------
 018570bb-4a7d-7000-8000-000000000000
(1 row)
```

`uuid_generate_v7()` is nearly as fast as the native `gen_random_uuid()`
function. See the [benchmarks](BENCHMARKS.md) for more details.

## Background

Version 7 UUIDs have a few advantages. They include a 48-bit Unix timestamp with
millisecond accuracy and will overflow far in the future (10899 AD). They also
include 74 random bits which means [billions can be created every second](https://en.wikipedia.org/wiki/Birthday_problem#Probability_table)
without collisions. Because of their structure they are globally sortable and
can be created in parallel in a distributed system.

## Quickstart

> [!IMPORTANT]
> These instructions are for x86_64 Linux. On other architectures follow the
[build instructions](#build) instead.

1. Download the [latest `.tar.gz` release](https://github.com/fboulnois/pg_uuidv7/releases)
and extract it to a temporary directory
2. Copy `pg_uuidv7.so` for your Postgres version into the Postgres module
directory
3. Copy `pg_uuidv7--1.6.sql` and `pg_uuidv7.control` into the Postgres extension
directory
4. Enable the extension in the database using `CREATE EXTENSION pg_uuidv7;`

```sh
# example shell script to install pg_uuidv7
cd "$(mktemp -d)"
curl -LO "https://github.com/fboulnois/pg_uuidv7/releases/download/v1.6.0/{pg_uuidv7.tar.gz,SHA256SUMS}"
tar xf pg_uuidv7.tar.gz
sha256sum -c SHA256SUMS
PG_MAJOR=$(pg_config --version | sed 's/^.* \([0-9]\{1,\}\).*$/\1/')
cp "$PG_MAJOR/pg_uuidv7.so" "$(pg_config --pkglibdir)"
cp pg_uuidv7--1.6.sql pg_uuidv7.control "$(pg_config --sharedir)/extension"
psql -c "CREATE EXTENSION pg_uuidv7;"
```

## Build

### Build locally

`pg_uuidv7` only requires the `libpq` headers and Postgres extension tools to
build the code. On Debian, these headers are included in the `libpq-dev` and
`postgresql-server-dev-all` packages.

To build the code run `make`. To install the extension locally run `make install`.

### Build using Docker

A [`Dockerfile`](Dockerfile) is available to build the code using the official
Postgres Docker image:

```sh
docker build . --tag pg_uuidv7
```

A prebuilt x86_64 version of this image is on GitHub:

```sh
docker pull ghcr.io/fboulnois/pg_uuidv7:1.6.0
```

The prebuilt image [is similar](https://github.com/fboulnois/pg_uuidv7/pull/29#issuecomment-1996102946)
to a vanilla Postgres instance so the extension needs to be enabled manually or
with an initialization script with `CREATE EXTENSION pg_uuidv7;`.

## Test

A separate [`Dockerfile`](test/Dockerfile) is available to build the extension
against a specific version of Postgres and run the regression tests:

```sh
docker build . --file test/Dockerfile --tag pgxn-test
docker run --rm -it pgxn-test /bin/sh
# once in container
pg-start 16
pg-build-test
```

## Contributing

See the [CONTRIBUTING.md](CONTRIBUTING.md) file for more details. In short,
follow the style guidelines, agree to the Developer Certificate of Origin, and
submit a PR.
