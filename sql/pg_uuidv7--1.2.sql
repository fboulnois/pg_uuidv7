-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use '''CREATE EXTENSION "pg_uuidv7"''' to load this file. \quit

-- 48 bits for ms since unix epoch (rollover in 10899), 74 bits of randomness
CREATE FUNCTION uuid_generate_v7()
RETURNS uuid
AS 'MODULE_PATHNAME', 'uuid_generate_v7'
VOLATILE STRICT LANGUAGE C PARALLEL SAFE;

-- extract the timestamp from a v7 uuid
CREATE FUNCTION uuid_v7_to_timestamptz(uuid)
RETURNS timestamptz
AS 'MODULE_PATHNAME', 'uuid_v7_to_timestamptz'
VOLATILE STRICT LANGUAGE C PARALLEL SAFE;

-- create a v7 uuid from a timestamp
CREATE FUNCTION uuid_timestamptz_to_v7(timestamptz, zero bool = false)
RETURNS uuid
AS 'MODULE_PATHNAME', 'uuid_timestamptz_to_v7'
VOLATILE STRICT LANGUAGE C PARALLEL SAFE;
