//===----------------------------------------------------------------------===//
//                         DuckDB
//
// parser/statement/alter_table_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "parser/column_definition.hpp"
#include "parser/expression.hpp"
#include "parser/parsed_data.hpp"
#include "parser/sql_node_visitor.hpp"
#include "parser/sql_statement.hpp"
#include "parser/tableref.hpp"

namespace duckdb {

class AlterTableStatement : public SQLStatement {
public:
	AlterTableStatement(unique_ptr<AlterTableInformation> info)
	    : SQLStatement(StatementType::ALTER), info(std::move(info)){};
	virtual ~AlterTableStatement() {
	}

	virtual string ToString() const {
		return "ALTER TABLE";
	}
	virtual unique_ptr<SQLStatement> Accept(SQLNodeVisitor *v) {
		return v->Visit(*this);
	}

	virtual bool Equals(const SQLStatement *other_) {
		if (!SQLStatement::Equals(other_)) {
			return false;
		}
		throw NotImplementedException("Equality not implemented!");
	}

	unique_ptr<TableRef> table;
	unique_ptr<AlterTableInformation> info;
};

} // namespace duckdb