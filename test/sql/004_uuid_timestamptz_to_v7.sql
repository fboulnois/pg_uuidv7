-- set up variables for testing
\set uuidv7 '018570bb-4a7d-7000-8000-000000000000'
\set uuidtz '2023-01-02 04:26:40.637+00'
\set uuid '2023-01-02 04:26:40.637'

-- test converting a timestamptz to a uuid
SELECT uuid_timestamptz_to_v7(:'uuidtz') > :'uuidv7';

-- test converting a timestamp to a uuid
SELECT uuid_timestamp_to_v7(:'uuid') > :'uuidv7';

-- test converting a timestamptz to a uuid and zero out the random bits
SELECT uuid_timestamptz_to_v7(:'uuidtz', true) = :'uuidv7';

-- test converting a timestamp to a uuid and zero out the random bits
SELECT uuid_timestamp_to_v7(:'uuid', true) = :'uuidv7';
