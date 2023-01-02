MODULES = pg_uuidv7
EXTENSION = pg_uuidv7
DATA = pg_uuidv7--1.0.sql

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
