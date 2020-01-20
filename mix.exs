defmodule Mix.Tasks.Compile.LibPgQuery do
  def run(_args) do
    {result, _errcode} = System.cmd("make", [])
    IO.binwrite(result)
  end
end

defmodule PgQuery.MixProject do
  use Mix.Project

  def project do
    [
      app: :pg_query,
      version: "0.1.0",
      elixir: "~> 1.9",
      start_permanent: Mix.env() == :prod,
      deps: deps(),
      compilers: [:lib_pg_query] ++ Mix.compilers
    ]
  end

  # Run "mix help compile.app" to learn about applications.
  def application do
    [
      extra_applications: [:logger]
    ]
  end

  # Run "mix help deps" to learn about dependencies.
  defp deps do
    [
      {:jason, "~> 1.1"},
      {:libpg_query, github: "lfittl/libpg_query", branch: '10-latest', app: false}
    ]
  end
end
