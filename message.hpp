#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <sqlite_orm/sqlite_orm.h>
#include <string>

namespace sql = sqlite_orm;

struct Message {
	std::string text;
	std::string hash;
};

auto InitDatabase(const std::string& db_name) {
	return sql::make_storage(
		db_name,
		sql::make_table(
			"messages",
			sql::make_column("text", &Message::text),
			sql::make_column("hash", &Message::hash)
		)
	);
}

#endif MESSAGE_HPP