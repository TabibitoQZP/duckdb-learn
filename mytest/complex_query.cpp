#include "duckdb.hpp"

using namespace duckdb;

int main() {
	DuckDB db(nullptr);
	Connection con(db);
	con.Query("CREATE TABLE inte0 (i INTEGER);");
	con.Query("CREATE TABLE inte1 (i INTEGER);");
	con.Query("INSERT INTO inte0 VALUES (3);INSERT INTO inte0 VALUES (4);INSERT INTO inte0 VALUES (3);");
	con.Query("INSERT INTO inte1 VALUES (3);INSERT INTO inte1 VALUES (4);INSERT INTO inte1 VALUES (5);");
	auto result = con.Query("SELECT * FROM inte0 INNER JOIN inte1 ON inte0.i = inte1.i;");
	result->Print();
}
