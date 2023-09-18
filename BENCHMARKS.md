# Benchmarks

## Summary

On average, `pg_uuidv7` is nearly as fast as the native `gen_random_uuid()`
function in Postgres. It is perhaps 2% slower in a worst case scenario, however
run-to-run variations of `pgbench` were >2%, which likely means that in a real
system the performance impact of `pg_uuidv7` is negligible.

## Methods

Performance benchmarks were evaluated using `pgbench`. The following functions
were benchmarked:

* the native `gen_random_uuid()` function which is built in since Postgres 13
* the `uuid_generate_v7()` function from this extension (`pg_uuidv7`)
* a pure sql version of `uuid_generate_v7()` from [kjmph](https://gist.github.com/kjmph/5bd772b2c2df145aa645b837da7eca74)

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
latency average = 0.076 ms
initial connection time = 5.039 ms
tps = 105898.857590 (without initial connection time)

-- pg_uuidv7 C extension
scaling factor: 1
query mode: simple
number of clients: 8
number of threads: 8
maximum number of tries: 1
number of transactions per client: 200000
number of transactions actually processed: 1600000/1600000
number of failed transactions: 0 (0.000%)
latency average = 0.076 ms
initial connection time = 4.553 ms
tps = 104904.866366 (without initial connection time)

-- sql function r24
scaling factor: 1
query mode: simple
number of clients: 8
number of threads: 8
maximum number of tries: 1
number of transactions per client: 200000
number of transactions actually processed: 1600000/1600000
number of failed transactions: 0 (0.000%)
latency average = 0.089 ms
initial connection time = 4.965 ms
tps = 89461.762254 (without initial connection time)
```
