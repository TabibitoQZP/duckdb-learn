//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/statement/call_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/parser/parsed_expression.hpp"
#include "duckdb/parser/sql_statement.hpp"
#include "duckdb/common/vector.hpp"

namespace duckdb {

// CallStatement会真正意义上调用函数, 可能UDF也会在这里调用
class CallStatement : public SQLStatement {
public:
	static constexpr const StatementType TYPE = StatementType::CALL_STATEMENT;

public:
	CallStatement();

	// 这个function, 就有很多种类, 比如Scalar, vector之类的
	unique_ptr<ParsedExpression> function;

protected:
	CallStatement(const CallStatement &other);

public:
	unique_ptr<SQLStatement> Copy() const override;
	string ToString() const override;
};
} // namespace duckdb
