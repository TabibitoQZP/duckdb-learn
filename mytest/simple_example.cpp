#include "duckdb.hpp"

using namespace duckdb;

int main() {
	DuckDB db(nullptr);

	Connection con(db);

	con.Query("CREATE TABLE integers(i INTEGER)");
	// con.Query("INSERT INTO integers VALUES (3);INSERT INTO integers VALUES (4);INSERT INTO integers VALUES (5);");
	// con.Query("SELECT * FROM (SELECT * FROM integers WHERE i > 0) WHERE i > 3 AND i < 5;");
	con.Query("INSERT INTO integers VALUES (3);INSERT INTO integers VALUES (4);INSERT INTO integers VALUES (5);");
	auto result = con.Query("SELECT * FROM integers WHERE i = 3;SELECT * FROM integers WHERE i = 4;SELECT * FROM integers WHERE i = 5;");
	result->Print();
}
