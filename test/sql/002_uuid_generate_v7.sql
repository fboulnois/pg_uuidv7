-- set up variables for testing
\set uuidv7 '018570bb-4a7d-7000-8000-000000000000'

-- test the uuid_generate_v7 function
SELECT uuid_generate_v7() > :'uuidv7';
