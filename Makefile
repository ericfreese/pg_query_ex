CFLAGS = -g -O3 -std=c99 -pedantic -Wall -Wextra -Wno-unused-parameter

ERLANG_PATH = $(shell erl -eval 'io:format("~s", [lists:concat([code:root_dir(), "/erts-", erlang:system_info(version), "/include"])])' -s init stop -noshell)
CFLAGS += -I$(ERLANG_PATH)

ifeq ($(wildcard deps/libpg_query),)
	LIBPG_QUERY_PATH = ../libpg_query
else
	LIBPG_QUERY_PATH = deps/libpg_query
endif

CFLAGS += -I$(LIBPG_QUERY_PATH)

ifneq ($(OS),Windows_NT)
	CFLAGS += -fPIC

	ifeq ($(shell uname),Darwin)
		LDFLAGS += -dynamiclib -undefined dynamic_lookup
	endif
endif

all: priv/query.so

priv/query.so: src/query.c
	cc $(CFLAGS) -shared $(LDFLAGS) -o $@ src/query.c $(LIBPG_QUERY_PATH)/libpg_query.a

clean:
	$(RM) priv/query.so
