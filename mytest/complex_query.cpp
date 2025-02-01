#include <iostream>
#include <utility>
#include "duckdb.hpp"
#include "duckdb/parser/parser.hpp"
#include "duckdb/planner/binder.hpp"
#include "duckdb/planner/planner.hpp"

using namespace duckdb;

int main() {
	DuckDB db(nullptr);
	Connection con(db);
	auto context = con.context;
	con.Query("CREATE TABLE inte0 (i INTEGER);");
	con.Query("CREATE TABLE inte1 (i INTEGER);");
	con.Query("INSERT INTO inte0 VALUES (3);INSERT INTO inte0 VALUES (4);INSERT INTO inte0 VALUES (3);");
	con.Query("INSERT INTO inte1 VALUES (3);INSERT INTO inte1 VALUES (4);INSERT INTO inte1 VALUES (5);");

	string query = "SELECT * FROM inte0 INNER JOIN inte1 ON inte0.i = inte1.i;";
	auto result = con.Query(query);
	result->Print();
}
