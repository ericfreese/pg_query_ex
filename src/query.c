#include <string.h>
#include <erl_nif.h>
#include <pg_query.h>

static ERL_NIF_TERM parse_query(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary input_bin, output_bin;
    PgQueryParseResult parse_result;
    ERL_NIF_TERM return_term;

    if (!enif_inspect_iolist_as_binary(env, argv[0], &input_bin)) {
        return enif_make_badarg(env);
    }

    char *input_str = malloc(input_bin.size + 1);
    memcpy(input_str, input_bin.data, input_bin.size);
    input_str[input_bin.size] = '\0';
    parse_result = pg_query_parse(input_str);

    if (parse_result.error) {
        enif_alloc_binary(strlen(parse_result.error->message), &output_bin);
        memcpy(output_bin.data, parse_result.error->message, output_bin.size);

        return_term = enif_make_tuple3(
            env,
            enif_make_atom(env, "error"),
            enif_make_binary(env, &output_bin),
            enif_make_int(env, parse_result.error->cursorpos)
        );
    } else {
        enif_alloc_binary(strlen(parse_result.parse_tree), &output_bin);
        memcpy(output_bin.data, parse_result.parse_tree, output_bin.size);

        return_term = enif_make_tuple2(
            env,
            enif_make_atom(env, "ok"),
            enif_make_binary(env, &output_bin)
        );
    }

    enif_release_binary(&input_bin);
    enif_release_binary(&output_bin);
    pg_query_free_parse_result(parse_result);
    free(input_str);

    return return_term;
}

static ERL_NIF_TERM fingerprint_query(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary input_bin, output_bin;
    PgQueryFingerprintResult fingerprint_result;
    ERL_NIF_TERM return_term;

    if (!enif_inspect_iolist_as_binary(env, argv[0], &input_bin)) {
        return enif_make_badarg(env);
    }

    char *input_str = malloc(input_bin.size + 1);
    memcpy(input_str, input_bin.data, input_bin.size);
    input_str[input_bin.size] = '\0';
    fingerprint_result = pg_query_fingerprint(input_str);

    if (fingerprint_result.error) {
        enif_alloc_binary(strlen(fingerprint_result.error->message), &output_bin);
        memcpy(output_bin.data, fingerprint_result.error->message, output_bin.size);

        return_term = enif_make_tuple3(
            env,
            enif_make_atom(env, "error"),
            enif_make_binary(env, &output_bin),
            enif_make_int(env, fingerprint_result.error->cursorpos)
        );
    } else {
        enif_alloc_binary(strlen(fingerprint_result.hexdigest), &output_bin);
        memcpy(output_bin.data, fingerprint_result.hexdigest, output_bin.size);

        return_term = enif_make_tuple2(
            env,
            enif_make_atom(env, "ok"),
            enif_make_binary(env, &output_bin)
        );
    }

    enif_release_binary(&input_bin);
    enif_release_binary(&output_bin);
    pg_query_free_fingerprint_result(fingerprint_result);
    free(input_str);

    return return_term;
}

static int load(ErlNifEnv *env, void **priv, ERL_NIF_TERM info) {
    pg_query_init();
    return 0;
}

static void unload(ErlNifEnv *env, void *priv) {
    pg_query_exit();
}

static ErlNifFunc nif_funcs[] = {
    {"raw_parse_query", 1, parse_query, 0},
    {"fingerprint_query", 1, fingerprint_query, 0}
};

ERL_NIF_INIT(Elixir.PgQuery, nif_funcs, &load, NULL, NULL, &unload)
