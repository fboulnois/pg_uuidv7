-- set up variables for testing
\set uuidv7 '018570bb-4a7d-7000-8000-000000000000'
\set uuidtz '2023-01-02 04:26:40.637+00'

-- test converting a uuid to a timestamp
SELECT uuid_v7_to_timestamptz(:'uuidv7') = :'uuidtz';
