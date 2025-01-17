#include "planner/operator/logical_any_join.hpp"

using namespace duckdb;
using namespace std;

LogicalAnyJoin::LogicalAnyJoin(JoinType type, LogicalOperatorType logical_type) : LogicalJoin(type, logical_type) {
}

string LogicalAnyJoin::ParamsToString() const {
	return "[" + condition->ToString() + "]";
}

count_t LogicalAnyJoin::ExpressionCount() {
	assert(expressions.size() == 0);
	return 1;
}

Expression *LogicalAnyJoin::GetExpression(index_t index) {
	assert(index == 0);
	return condition.get();
}

void LogicalAnyJoin::ReplaceExpression(
    std::function<unique_ptr<Expression>(unique_ptr<Expression> expression)> callback, index_t index) {
	assert(index == 0);
	condition = callback(move(condition));
}
