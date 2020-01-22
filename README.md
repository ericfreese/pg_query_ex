# PgQuery

A [libpg_query](https://github.com/lfittl/libpg_query/tree/10-latest) wrapper written in elixir.

Useful for parsing postgresql queries.

## Usage

### Fingerprint
Fingerprints queries will ignoring formatting and concrete values.
```elixir
iex> PgQuery.fingerprint_query("SELECT count(*) from users where age > 50") 
{:ok, "02dd673649193e38c3e5afb638f51614fee858ab1e"}
iex> PgQuery.fingerprint_query("SELECT count(*) from users where age < 50") 
{:ok, "02dd673649193e38c3e5afb638f51614fee858ab1e"}
iex> PgQuery.fingerprint_query("SELECT 1 + 2")               
{:ok, "025a33db2ae7cbfce37fa78c34b32e75b376eb6d16"}
```

### Query parsing
```elixir
iex> PgQuery.parse_query("SELECT name, age from users")
{:ok,
 [
   %{
     "RawStmt" => %{
       "stmt" => %{
         "SelectStmt" => %{
           "fromClause" => [
             %{
               "RangeVar" => %{
                 "inh" => true,
                 "location" => 22,
                 "relname" => "users",
                 "relpersistence" => "p"
               }
             }
           ],
           "op" => 0,
           "targetList" => [
             %{
               "ResTarget" => %{
                 "location" => 7,
                 "val" => %{
                   "ColumnRef" => %{
                     "fields" => [%{"String" => %{"str" => "name"}}],
                     "location" => 7
                   }
                 }
               }
             },
             %{
               "ResTarget" => %{
                 "location" => 13,
                 "val" => %{
                   "ColumnRef" => %{
                     "fields" => [%{"String" => %{"str" => "age"}}],
                     "location" => 13
                   }
                 }
               }
             }
           ]
         }
       }
     }
   }
 ]}
```

## Installation

If [available in Hex](https://hex.pm/docs/publish), the package can be installed
by adding `pg_query` to your list of dependencies in `mix.exs`:

```elixir
def deps do
  [
    {:pg_query, "~> 0.1.0"}
  ]
end
```

Documentation can be generated with [ExDoc](https://github.com/elixir-lang/ex_doc)
and published on [HexDocs](https://hexdocs.pm). Once published, the docs can
be found at [https://hexdocs.pm/pg_query](https://hexdocs.pm/pg_query).
