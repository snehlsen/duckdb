#include "execution/operator/schema/physical_create_index.hpp"

#include "catalog/catalog_entry/schema_catalog_entry.hpp"
#include "catalog/catalog_entry/table_catalog_entry.hpp"
#include "execution/expression_executor.hpp"
#include "main/client_context.hpp"

using namespace duckdb;
using namespace std;

void PhysicalCreateIndex::createARTIndex(ScanStructure *ss, DataChunk *intermediate, vector<TypeId> *result_types,
                                         DataChunk *result) {
	auto art = make_unique<ART>(*table.storage, column_ids, types, *result_types, move(expressions),
	                            move(unbound_expressions));
	// now we start incrementally building the index
	while (true) {
		intermediate->Reset();
		// scan a new chunk from the table to index
		table.storage->CreateIndexScan(*ss, column_ids, *intermediate);
		if (intermediate->size() == 0) {
			// finished scanning for index creation
			// release all locks
			break;
		}
		// resolve the expressions for this chunk
		ExpressionExecutor executor(intermediate);
		executor.Execute(art->expressions, *result);
		//
		art->Insert(*result, intermediate->data[intermediate->column_count - 1]);
	}
	table.storage->indexes.push_back(move(art));
}

void PhysicalCreateIndex::GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state) {
	if (column_ids.size() == 0) {
		throw NotImplementedException("CREATE INDEX does not refer to any columns in the base table!");
	}

	auto &schema = *table.schema;
	if (!schema.CreateIndex(context.ActiveTransaction(), info.get())) {
		// index already exists, but error ignored because of CREATE ... IF NOT
		// EXISTS
		return;
	}

	// create the chunk to hold intermediate expression results
	if (expressions.size() > 1) {
		throw NotImplementedException("Multidimensional indexes not supported yet");
	}

	DataChunk result;
	vector<TypeId> result_types;
	for (auto &expr : expressions) {
		result_types.push_back(expr->return_type);
	}
	result.Initialize(result_types);

	column_ids.push_back(COLUMN_IDENTIFIER_ROW_ID);

	ScanStructure ss;
	table.storage->InitializeScan(ss);

	DataChunk intermediate;
	auto types = table.GetTypes(column_ids);
	intermediate.Initialize(types);

	switch (info->index_type) {
	case IndexType::ART:
		createARTIndex(&ss, &intermediate, &result_types, &result);
		break;
	default:
		assert(0);
		break;
	}

	chunk.data[0].count = 1;
	chunk.data[0].SetValue(0, Value::BIGINT(0));

	state->finished = true;
}
