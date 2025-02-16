#include "duckdb.hpp"
#include "duckdb/execution/physical_plan_generator.hpp"
#include "duckdb/optimizer/optimizer.hpp"
#include "duckdb/parser/parser.hpp"
#include "duckdb/planner/planner.hpp"
#include "duckdb/transaction/transaction.hpp"
#include "duckdb/transaction/transaction_context.hpp"
#include "duckdb/transaction/transaction_manager.hpp"

using namespace duckdb;

int main() {
	// 初始化db, context
	DuckDB db(nullptr);
	unique_ptr<ClientContext> context = make_unique<ClientContext>(db);

	// 实际query路径
	// Connection con(db);
	// con.Query("dd");

	string query;

	// parser
	Parser parser;
	parser.ParseQuery(query);

	// 注意, CreatePreparedStatement做了planner, optimizer和physical planner
	for (auto &statement:parser.statements) {
		// planner
		Planner planner(*context);
		planner.CreatePlan(std::move(statement));

		// optimizer
		Optimizer optimizer(planner.binder, *context);
		auto plan = optimizer.Optimize(move(planner.plan));

		// physical_plan
		PhysicalPlanGenerator physical_planner(*context);
		auto physical_plan = physical_planner.CreatePlan(move(plan));

		// optimizer
	}

	return 0;
}
