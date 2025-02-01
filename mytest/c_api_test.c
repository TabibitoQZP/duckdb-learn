#include "stdio.h"
#include "duckdb.h"

int main() {
	unsigned long long sz = duckdb_vector_size();
	printf("%lld", sz);
	return 0;
}
