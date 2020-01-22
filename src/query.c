#include <erl_nif.h>
#include <string.h>

#include <pg_query.h>

static ERL_NIF_TERM
parse_query(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary query_bin;
    PgQueryParseResult result;
    char *query;

    if (!enif_inspect_iolist_as_binary(env, argv[0], &query_bin)) {
        return enif_make_badarg(env);
    }

    query = strndup((char *)query_bin.data, query_bin.size);
    result = pg_query_parse(query);

    free(query);

    if (result.error) {
        ErlNifBinary return_bin;
        return_bin.size = sizeof(char) * strlen(result.error->message);
        return_bin.data = malloc(return_bin.size);
        strcpy((char*)return_bin.data, result.error->message);
        pg_query_free_parse_result(result);
        return enif_make_tuple3(env, enif_make_atom(env, "error"), enif_make_binary(env, &return_bin), enif_make_int(env, result.error->cursorpos));
    } else {
        ErlNifBinary return_bin;
        return_bin.size = sizeof(char) * strlen(result.parse_tree);
        return_bin.data = malloc(return_bin.size);
        strcpy((char*)return_bin.data, result.parse_tree);
        pg_query_free_parse_result(result);
        return enif_make_tuple2(env, enif_make_atom(env, "ok"), enif_make_binary(env, &return_bin));
    }
}


static ERL_NIF_TERM
fingerprint_query(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary query_bin;
    PgQueryFingerprintResult result;
    char *query;

    if (!enif_inspect_iolist_as_binary(env, argv[0], &query_bin)) {
        return enif_make_badarg(env);
    }

    query = strndup((char *)query_bin.data, query_bin.size);
    result = pg_query_fingerprint(query);

    free(query);

    if (result.error) {
        ErlNifBinary return_bin;
        return_bin.size = sizeof(char) * strlen(result.error->message);
        return_bin.data = malloc(return_bin.size);
        strcpy((char*)return_bin.data, result.error->message);
        pg_query_free_fingerprint_result(result);
        return enif_make_tuple3(env, enif_make_atom(env, "error"), enif_make_binary(env, &return_bin), enif_make_int(env, result.error->cursorpos));
    } else {
        ErlNifBinary return_bin;
        return_bin.size = sizeof(char) * strlen(result.hexdigest);
        return_bin.data = malloc(return_bin.size);
        strcpy((char*)return_bin.data, result.hexdigest);
        pg_query_free_fingerprint_result(result);
        return enif_make_tuple2(env, enif_make_atom(env, "ok"), enif_make_binary(env, &return_bin));
    }
}

static int load(ErlNifEnv *env, void **priv, ERL_NIF_TERM info) {
    pg_query_init();
    return 0;
}

static int
reload(ErlNifEnv *env, void **priv, ERL_NIF_TERM info) {
    return 0;
}

static int
upgrade(ErlNifEnv *env, void **priv, void **old_priv, ERL_NIF_TERM info) {
    return 0;
}

static void
unload(ErlNifEnv *env, void *priv) { }

static ErlNifFunc nif_funcs[] = {
    {"raw_parse_query", 1, parse_query, 0},
    {"fingerprint_query", 1, fingerprint_query, 0}};

ERL_NIF_INIT(Elixir.PgQuery, nif_funcs, &load, &reload, &upgrade, &unload)
