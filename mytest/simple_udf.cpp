#include "duckdb.hpp"
#include <iostream>

using namespace std;
using namespace duckdb;

bool bigger_than_four(int value) {
	return value > 4;
}

int32_t udf_date(int32_t a) {
    return a;
}

/*
* This vectorized function copies the input values to the result vector
*/
template<typename TYPE>
static void udf_vectorized(DataChunk &args, ExpressionState &state, Vector &result) {
    // set the result vector type
    result.vector_type = VectorType::FLAT_VECTOR;
    // get a raw array from the result
    auto result_data = FlatVector::GetData<TYPE>(result);

    // get the solely input vector
    auto &input = args.data[0];
    // now get an orrified vector
    VectorData vdata;
    input.Orrify(args.size(), vdata);

    // get a raw array from the orrified input
    auto input_data = (TYPE *)vdata.data;

    // handling the data
    for (idx_t i = 0; i < args.size(); i++) {
        auto idx = vdata.sel->get_index(i);
        if ((*vdata.nullmask)[idx]) {
            continue;
        }
        result_data[i] = input_data[idx];
    }
}

int main() {
	DuckDB db(nullptr);
	Connection con(db);
	// create a table
	con.Query("CREATE TABLE integers (i INTEGER, j INTEGER)");

	// insert three rows into the table
	con.Query("INSERT INTO integers VALUES (3, 4), (5, 6), (7, NULL)");

	auto result = con.Query("SELECT * FROM integers");
	if (result->HasError()) {
		cerr << result->GetError() << endl;
	} else {
		cout << result->ToString() << endl;
	}


	con.CreateScalarFunction<bool, int>("bigger_than_four", &bigger_than_four);
	con.Query("SELECT i FROM (VALUES(3), (5)) tbl(i) WHERE bigger_than_four(i)")->Print();


	con.Query("CREATE TABLE dates (d DATE)");
	con.Query("INSERT INTO dates VALUES ('1992-01-01')");

	con.CreateScalarFunction<int32_t, int32_t>("udf_date", {LogicalType::DATE}, LogicalType::DATE, &udf_date);

	con.Query("SELECT udf_date(d) FROM dates")->Print();

	return 0;
}
