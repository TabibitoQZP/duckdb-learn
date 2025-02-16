#include "duckdb/parser/parser.hpp"

using namespace duckdb;

int main() {
	Parser parser;
	parser.ParseQuery("SELECT id1, id2 FROM tab WHERE age < 18;SELECT id1, id2 FROM tab WHERE age < 18;");
	return 0;
}
