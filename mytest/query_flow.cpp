#include "duckdb.hpp"
#include "duckdb/common/helper.hpp"
#include "duckdb/execution/executor.hpp"
#include "duckdb/execution/physical_plan_generator.hpp"
#include "duckdb/main/client_config.hpp"
#include "duckdb/main/client_context.hpp"
#include "duckdb/optimizer/optimizer.hpp"
#include "duckdb/parser/parser.hpp"
#include "duckdb/planner/planner.hpp"

using namespace duckdb;

int main(int argc, char** argv) {
	// 数据库的初始化
	DuckDB db(nullptr);

	// context初始化
	shared_ptr<ClientContext> context;
	context = make_shared_ptr<ClientContext>(db.instance->shared_from_this());

	// 初始化query
	string query = "CREATE TABLE integers (i INTEGER, j INTEGER);INSERT INTO integers VALUES (3, 4), (5, 6), (7, NULL);SELECT * FROM integers;";

	// 配置解析器的选项
	ParserOptions options;
	auto &client_config = ClientConfig::GetConfig(*context);
	options.preserve_identifier_case = client_config.preserve_identifier_case;
	options.integer_division = client_config.integer_division;
	options.max_expression_depth = client_config.max_expression_depth;
	options.extensions = &DBConfig::GetConfig(*context).parser_extensions;
	Parser parser(options);

	// 解析query
	parser.ParseQuery(query);
	vector<unique_ptr<SQLStatement>> statements = std::move(parser.statements);

	for (auto &statement: statements) {
		context->transaction.BeginTransaction();

		auto copy_statement = statement->Copy();
		// planner
		Planner planner(*context);
		// 注意, 有两个CreatePlan, 一个public一个private, 二者调用不同
		planner.CreatePlan(std::move(copy_statement));

		// optimizer
		Optimizer optimizer(*planner.binder, *context);
		auto plan = optimizer.Optimize(std::move(planner.plan));

		// physical planner
		PhysicalPlanGenerator physical_plan_generator(*context);
		auto physical_plan = physical_plan_generator.CreatePlan(std::move(plan));

		// executor
		Executor executor(*context);
		executor.Initialize(std::move(physical_plan));
		auto result_status = executor.ExecuteTask();
		// auto result = executor.GetResult();
		// result->Print();

		context->transaction.Commit();
	}
	return 0;
}
