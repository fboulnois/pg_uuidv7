-- set up variables for testing
\set uuidv7 '018570bb-4a7d-7000-8000-000000000000'
\set uuidts '2023-01-02 04:26:40.637'

-- test converting a timestamp to a uuid
SELECT uuid_timestamp_to_v7(:'uuidts') > :'uuidv7';

-- test converting a timestamp to a uuid and zero out the random bits
SELECT uuid_timestamp_to_v7(:'uuidts', true) = :'uuidv7';
