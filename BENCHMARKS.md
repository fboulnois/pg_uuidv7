# Benchmarks

## Summary

On average, `pg_uuidv7` is as fast as the native `gen_random_uuid()` function in
Postgres. In a worst case scenario it was up to 2% slower, however run-to-run
variations of `pgbench` were >2%, which likely means that in a real system the
performance impact of `pg_uuidv7` is negligible.

## Methods

Performance benchmarks were evaluated using Postgres 17 and `pgbench`. The
following functions were benchmarked:

* the native `gen_random_uuid()` function which is built in since Postgres 13
* the `uuid_generate_v7()` function from this extension (`pg_uuidv7`)
* a pure sql version of `uuid_generate_v7()` from [kjmph](https://gist.github.com/kjmph/5bd772b2c2df145aa645b837da7eca74)

See the [benchmark directory](test/benchmark) for the implementation.

## Results

```
pgbench --client=8 --jobs=8 --transactions=200000 --file=${TEST}.sql

-- SELECT gen_random_uuid();
scaling factor: 1
query mode: simple
number of clients: 8
number of threads: 8
maximum number of tries: 1
number of transactions per client: 200000
number of transactions actually processed: 1600000/1600000
number of failed transactions: 0 (0.000%)
latency average = 0.073 ms
initial connection time = 4.815 ms
tps = 110325.912399 (without initial connection time)

-- pg_uuidv7 C extension
scaling factor: 1
query mode: simple
number of clients: 8
number of threads: 8
maximum number of tries: 1
number of transactions per client: 200000
number of transactions actually processed: 1600000/1600000
number of failed transactions: 0 (0.000%)
latency average = 0.073 ms
initial connection time = 5.422 ms
tps = 109623.347197 (without initial connection time)

-- sql function r27
scaling factor: 1
query mode: simple
number of clients: 8
number of threads: 8
maximum number of tries: 1
number of transactions per client: 200000
number of transactions actually processed: 1600000/1600000
number of failed transactions: 0 (0.000%)
latency average = 0.088 ms
initial connection time = 5.146 ms
tps = 90448.451336 (without initial connection time)
```
