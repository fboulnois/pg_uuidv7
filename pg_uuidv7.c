#include "postgres.h"

#include "fmgr.h"
#include "port/pg_bswap.h"
#include "utils/uuid.h"
#include "utils/timestamp.h"

#include <time.h>

/*
 * Number of microseconds between unix and postgres epoch
 */
#define EPOCH_DIFF_USECS ((POSTGRES_EPOCH_JDATE - UNIX_EPOCH_JDATE) * USECS_PER_DAY)

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(uuid_generate_v7);

#define UUID_RND_CACHE_LEN 64
int rnd_cache_ptr = UUID_RND_CACHE_LEN;
unsigned char random_cache[UUID_RND_CACHE_LEN];

static bool
cached_strong_random(void *buf, size_t len)
{
	if (len + rnd_cache_ptr >= UUID_RND_CACHE_LEN)
	{
		if (!pg_strong_random(random_cache, UUID_RND_CACHE_LEN))
			return false;
		rnd_cache_ptr = 0;
	}
	memcpy(buf, &random_cache[rnd_cache_ptr], len);
	rnd_cache_ptr += len;
	return true;
}

static uint32_t sequence_counter;
static uint64_t previous_timestamp = 0;

Datum uuid_generate_v7(PG_FUNCTION_ARGS)
{
	pg_uuid_t  *uuid = palloc(UUID_LEN);
	uint64_t tms;
	struct timespec ts;

	if (clock_gettime(CLOCK_REALTIME, &ts) != 0)
		ereport(ERROR,
				(errcode(ERRCODE_INTERNAL_ERROR),
				 errmsg("could not get CLOCK_REALTIME")));

	tms = ((uint64_t)ts.tv_sec * 1000) + ((uint64_t)ts.tv_nsec / 1000000);

	if (tms <= previous_timestamp)
	{
		/* Time did not increment from the previous generation, we must increment counter */
		++sequence_counter;
		if (sequence_counter > 0x3ffff)
		{
			/* We only have 18-bit counter */
			sequence_counter = 0;
			previous_timestamp++;
		}

		/* protection from leap backward */
		tms = previous_timestamp;

		/* fill everything after the timestamp and counter with random bytes */
		if (!cached_strong_random(&uuid->data[8], UUID_LEN - 8))
			ereport(ERROR,
					(errcode(ERRCODE_INTERNAL_ERROR),
					errmsg("could not generate random values")));

		/* most significant 4 bits of 18-bit counter */
		uuid->data[6] = (unsigned char)(sequence_counter >> 14);
		/* next 8 bits */
		uuid->data[7] = (unsigned char)(sequence_counter >> 6);
		/* least significant 6 bits */
		uuid->data[8] = (unsigned char)(sequence_counter);
	}
	else
	{
		/* fill everything after the timestamp with random bytes */
		if (!cached_strong_random(&uuid->data[6], UUID_LEN - 6))
			ereport(ERROR,
					(errcode(ERRCODE_INTERNAL_ERROR),
					errmsg("could not generate random values")));

		/*
		 * Left-most counter bits are initialized as zero for the sole purpose
		 * of guarding against counter rollovers.
		 * See section "Fixed-Length Dedicated Counter Seeding"
		 * https://datatracker.ietf.org/doc/html/draft-ietf-uuidrev-rfc4122bis-09#monotonicity_counters
		 */
		uuid->data[6] = (uuid->data[6] & 0xf7);

		sequence_counter = ((uint32_t)uuid->data[8] & 0x3f) +
							(((uint32_t)uuid->data[7]) << 6) +
							(((uint32_t)uuid->data[6] & 0x0f) << 14);

		previous_timestamp = tms;
	}

	/* Fill in time part */
	uuid->data[0] = (unsigned char)(tms >> 40);
	uuid->data[1] = (unsigned char)(tms >> 32);
	uuid->data[2] = (unsigned char)(tms >> 24);
	uuid->data[3] = (unsigned char)(tms >> 16);
	uuid->data[4] = (unsigned char)(tms >> 8);
	uuid->data[5] = (unsigned char)tms;

	/*
	 * Set magic numbers for a "version 7" (pseudorandom) UUID, see
	 * http://tools.ietf.org/html/rfc ???
	 * https://datatracker.ietf.org/doc/html/draft-peabody-dispatch-new-uuid-format#name-creating-a-uuidv7-value
	 */
	/* set version field, top four bits are 0, 1, 1, 1 */
	uuid->data[6] = (uuid->data[6] & 0x0f) | 0x70;
	/* set variant field, top two bits are 1, 0 */
	uuid->data[8] = (uuid->data[8] & 0x3f) | 0x80;

	PG_RETURN_UUID_P(uuid);
}

PG_FUNCTION_INFO_V1(uuid_v7_to_timestamptz);

Datum uuid_v7_to_timestamptz(PG_FUNCTION_ARGS)
{
	pg_uuid_t *uuid = PG_GETARG_UUID_P(0);
	uint64_t ts;

	memcpy(&ts, &uuid->data[0], 6);
	ts = pg_ntoh64(ts) >> 16;
	ts = 1000 * ts - EPOCH_DIFF_USECS;

	PG_RETURN_TIMESTAMPTZ(ts);
}

PG_FUNCTION_INFO_V1(uuid_timestamptz_to_v7);

Datum uuid_timestamptz_to_v7(PG_FUNCTION_ARGS)
{
	pg_uuid_t *uuid = palloc(UUID_LEN);
	bool zero = false;
	uint64_t tms;

	TimestampTz ts = PG_GETARG_TIMESTAMPTZ(0);

	if (!PG_ARGISNULL(1))
		zero = PG_GETARG_BOOL(1);

	if (zero)
		memset(uuid, 0, UUID_LEN);
	else if (!pg_strong_random(uuid, UUID_LEN))
		ereport(ERROR,
				(errcode(ERRCODE_INTERNAL_ERROR),
				 errmsg("could not generate random values")));

	tms = ((uint64_t)ts + EPOCH_DIFF_USECS) / 1000;
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
