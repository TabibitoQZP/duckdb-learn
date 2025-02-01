#include "duckdb.hpp"
#include <iostream>

using namespace std;
using namespace duckdb;

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
	con.Query("CREATE TABLE integers (i INTEGER)");
	con.Query("INSERT INTO integers VALUES (1), (2), (3), (999)");

	con.CreateVectorizedFunction<int, int>("udf_vectorized_int", &udf_vectorized<int>);

	con.Query("SELECT udf_vectorized_int(i) FROM integers")->Print();
	return 0;
}
