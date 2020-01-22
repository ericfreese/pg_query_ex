defmodule PgQuery do
  @moduledoc """
    Collection of method from the libpg_query library.

    The original library can be found at https://github.com/lfittl/libpg_query/tree/10-latest
  """

  @on_load :load_nifs 

  @doc false
  def load_nifs() do
    :erlang.load_nif('./priv/query', 0) 
  end
  
  @doc """
  Parses one or more sql queries (delimited by `;`), returns a map describing the query.

  ## Examples
      iex> PgQuery.parse_query("SELECT name from")            
      {:error, "syntax error at end of input", 17}

      iex> PgQuery.parse_query("SELECT name from users")            
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
                      "location" => 17,
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
                  }
                ]
              }
            }
          }
        }
      ]}
  """
  @spec parse_query(sql :: String.t()) :: {:error, String.t(), number()} | {:ok, list(map())}
  def parse_query(sql) do
    case raw_parse_query(sql) do
      {:ok, json_text} -> Jason.decode(json_text)
      error -> error
    end
  end

  @doc """
    Same as `parse_query/1` but the result is a json string.
  """
  @spec raw_parse_query(sql :: String.t()) :: {:error, String.t()} | {:ok, String.t()}
  def raw_parse_query(_sql) do
    raise "NIF not loaded"
  end

  @doc """
  Returns a fingerprint for a given query, ignoring formatting, values in where clauses etc.
  
  Precise rules can be found at https://github.com/lfittl/libpg_query/wiki/Fingerprinting
  
  ## Examples

      iex> PgQuery.fingerprint_query("SELECT name from users where id = 1")
      {:ok, "021ecc4fb237583f0d5612ffc00eadc799741785c0"}

      iex> PgQuery.fingerprint_query("SELECT name from users where id = 2")
      {:ok, "021ecc4fb237583f0d5612ffc00eadc799741785c0"}
      
      iex> PgQuery.fingerprint_query("SELECT age from users where id = 2")
      {:ok, "02c60fb7b029a0d057ad1ef510ddd66356e4425336"}
    
  """
  @spec fingerprint_query(sql :: String.t()) :: {:error, String.t()} | {:ok, String.t()}
  def fingerprint_query(_sql) do
    raise "NIF not loaded"
  end
end
