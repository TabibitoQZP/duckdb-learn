#include <iostream>
#include "duckdb.hpp"
#include "duckdb/main/connection.hpp"
#include "duckdb/main/database.hpp"
#include "duckdb/optimizer/optimizer.hpp"
#include "duckdb/parser/parser.hpp"
#include "duckdb/planner/operator/logical_projection.hpp"
#include "duckdb/planner/planner.hpp"

using namespace duckdb;
using namespace std;

unique_ptr<LogicalOperator> ParseLogicalTree(string query, Connection& con) {
	Parser parser;
	parser.ParseQuery(query.c_str());
	if (parser.statements.size() == 0 || parser.statements[0]->type != StatementType::SELECT_STATEMENT) {
		return nullptr;
	}
	Planner planner(*con.context);
	planner.CreatePlan(move(parser.statements[0]));
	return move(planner.plan);
}

void printLogicalOperator(LogicalOperator& op, int depth) {
	for (int i = 0; i < depth; i++) {
		cout << "  ";
	}
	string opName = LogicalOperatorToString(op.type);
	cout << opName;
	if (op.type == LogicalOperatorType::PROJECTION) {
		cout << '(' << ((LogicalProjection&)op).table_index << ')';
	}
	for (auto& e: op.expressions) {
		cout << ExpressionTypeToString(e->type) << ' ';
	}


	cout << endl;
	for (auto& c: op.children) {
		printLogicalOperator(*c, depth + 1);
	}
}

int main() {
	DuckDB db(nullptr);
	Connection con(db);
	con.Query("BEGIN TRANSACTION");
	Binder binder(*con.context);

	con.Query("CREATE TABLE integers(i integer, j integer, k integer )");

	Optimizer optimizer(binder, *con.context);
	auto orignalPlan = ParseLogicalTree("SELECT k FROM integers where i+j > 10 and j = 5 and i = k+1 ", con);
	printLogicalOperator(*orignalPlan, 0);
	auto optimizedPlan = optimizer.Optimize(std::move(orignalPlan));
	printLogicalOperator(*optimizedPlan, 0);
	return 0;
}
