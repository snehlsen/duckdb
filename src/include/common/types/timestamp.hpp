//===----------------------------------------------------------------------===//
//                         DuckDB
//
// common/types/timestamp.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/common.hpp"

namespace duckdb {

//! The Date class is a static class that holds helper functions for the Timestamp
//! type.
class Timestamp {
public:
	//! Convert a string in the format "YYYY-MM-DD hh:mm:ss" to a timestamp object
	static timestamp_t FromString(string str);
	//! Convert a date object to a string in the format "YYYY-MM-DDThh:mm:ssZ"
	static string ToString(timestamp_t timestamp);

	static date_t GetDate(timestamp_t timestamp);

	static dtime_t GetTime(timestamp_t timestamp);
	//! Create a Timestamp object from a specified (date, time) combination
	static timestamp_t FromDatetime(date_t date, dtime_t time);
	//! Extract the date and time from a given timestamp object
	static void Convert(timestamp_t date, date_t &out_date, dtime_t &out_time);
};
} // namespace duckdb
