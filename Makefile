MODULES = pg_uuidv7
EXTENSION = pg_uuidv7
DATA = sql/pg_uuidv7--1.7.sql

TESTS = $(wildcard test/sql/*.sql)
REGRESS = $(patsubst test/sql/%.sql,%,$(TESTS))
REGRESS_OPTS = --inputdir=test

PG_CONFIG ?= pg_config
PG_VERSION_MAJOR := $(shell $(PG_CONFIG) --version | sed -E 's/^PostgreSQL ([0-9]{2}).*$$/\1/')
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

.PHONY: FORCE
$(EXTENSION).bc: .built-with-pg-$(PG_VERSION_MAJOR)
.built-with-pg-$(PG_VERSION_MAJOR): FORCE
	if [ -f $@ ]; then echo "Still with Pg $(PG_VERSION_MAJOR)."; else touch $@; fi
.NOTINTERMEDIATE: .built-with-pg-$(PG_VERSION_MAJOR)
EXTRA_CLEAN = .built-with-pg-$(PG_VERSION_MAJOR)
