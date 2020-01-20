CFLAGS = -g -O3 -ansi -pedantic -Wall -Wextra -Wno-unused-parameter

ERLANG_PATH = $(shell erl -eval 'io:format("~s", [lists:concat([code:root_dir(), "/erts-", erlang:system_info(version), "/include"])])' -s init stop -noshell)
CFLAGS += -I$(ERLANG_PATH)

all: priv/query.so

priv/query.so: src/query.c
	  cc -fPIC $(CFLAGS) -Ideps/libpg_query deps/libpg_query/libpg_query.a -dynamiclib -undefined dynamic_lookup -o priv/query.so src/query.c