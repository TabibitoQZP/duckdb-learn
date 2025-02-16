/*
* 可以根据test/planner/test_projection_binding.cpp来编写测试
*/
#include "duckdb/main/connection.hpp"
#include "duckdb/main/database.hpp"
#include "duckdb/parser/parser.hpp"
#include "duckdb/planner/planner.hpp"

using namespace duckdb;

unique_ptr<LogicalOperator> ParseLogicalTree(Connection& con, string query) {
	Parser parser;
	parser.ParseQuery(query.c_str());
	if (parser.statements.size() == 0 || parser.statements[0]->type != StatementType::SELECT_STATEMENT) {
		return nullptr;
	}

	Planner planner(*con.context);
	planner.CreatePlan(move(parser.statements[0]));
	return move(planner.plan);
}

int main() {
	DuckDB db;
	Connection con(db);
	con.Query("BEGIN TRANSACTION");
	con.Query("CREATE TABLE a (i INTEGER, j INTEGER)");

	auto op0 = ParseLogicalTree(con, "SELECT i FROM a ORDER BY i");
	auto op1 = ParseLogicalTree(con, "SELECT a.i FROM a ORDER BY i");
	auto op2 = ParseLogicalTree(con, "SELECT i FROM a ORDER BY a.i");
	auto op3 = ParseLogicalTree(con, "SELECT i AS k FROM a ORDER BY i");

	return 0;

}
