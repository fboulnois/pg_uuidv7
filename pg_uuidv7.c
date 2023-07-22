#include "postgres.h"

#include "fmgr.h"
#include "port/pg_bswap.h"
#include "utils/uuid.h"
#include "utils/timestamp.h"

#include <time.h>

/*
 * Number of milliseconds between unix and postgres epoch
 */
#define EPOCH_DIFF_MS ((POSTGRES_EPOCH_JDATE - UNIX_EPOCH_JDATE) * USECS_PER_DAY / 1000)

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(uuid_generate_v7);

Datum uuid_generate_v7(PG_FUNCTION_ARGS)
{
	pg_uuid_t *uuid = palloc(UUID_LEN);
	struct timespec ts;
	uint64_t tms;

	if (!pg_strong_random(uuid, UUID_LEN))
		ereport(ERROR,
				(errcode(ERRCODE_INTERNAL_ERROR),
				 errmsg("could not generate random values")));

	/*
	 * Set first 48 bits to unix epoch timestamp
	 */
	if (clock_gettime(CLOCK_REALTIME, &ts) != 0)
		ereport(ERROR,
				(errcode(ERRCODE_INTERNAL_ERROR),
				 errmsg("could not get CLOCK_REALTIME")));

	tms = ((uint64_t)ts.tv_sec * 1000) + ((uint64_t)ts.tv_nsec / 1000000);
	tms = pg_hton64(tms << 16);
	memcpy(&uuid->data[0], &tms, 6);

	/*
	 * Set magic numbers for a "version 7" UUID, see
	 * https://www.ietf.org/archive/id/draft-ietf-uuidrev-rfc4122bis-00.html#name-uuid-version-7
	 */
	uuid->data[6] = (uuid->data[6] & 0x0f) | 0x70; /* 4 bit version [0111] */
	uuid->data[8] = (uuid->data[8] & 0x3f) | 0x80; /* 2 bit variant [10]   */

	PG_RETURN_UUID_P(uuid);
}

PG_FUNCTION_INFO_V1(uuid_v7_to_timestamptz);

Datum uuid_v7_to_timestamptz(PG_FUNCTION_ARGS)
{
	pg_uuid_t *uuid = PG_GETARG_UUID_P(0);

	uint64_t tms;
	memcpy(&tms, &uuid->data[0], 6);
	tms = pg_ntoh64(tms) >> 16;
	tms = (tms - EPOCH_DIFF_MS) * 1000;

	PG_RETURN_TIMESTAMPTZ(tms);
}
