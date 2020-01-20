defmodule PgQuery do
  @moduledoc """
  Documentation for PgQuery.
  """

  @on_load :load_nifs 

  def load_nifs() do
    :erlang.load_nif('./priv/query', 0) 
  end
  
  def parse_query(sql) do
    case raw_parse_query(sql) do
      {:ok, json_text} -> Jason.decode(json_text)
      error -> error
    end
  end

  def raw_parse_query(_sql) do
    raise "NIF not loaded"
  end
end
