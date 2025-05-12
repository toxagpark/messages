#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <sqlite_orm/sqlite_orm.h>
#include <string>

namespace sql = sqlite_orm;

struct Message {
	std::string text;
	std::string hash;
	std::time_t start_time;
};

auto InitDatabase(const std::string& db_name) {
	return sql::make_storage(
		db_name,
		sql::make_table(
			"messages",
			sql::make_column("text", &Message::text),
			sql::make_column("hash", &Message::hash),
			sql::make_column("start_time", &Message::start_time)
		)
	);
}

template<typename Storage>
void DeleteOldMessages(Storage& storage) {
	const auto now = std::time(nullptr);
	const auto one_day_ago = now - 60;
	storage.remove_all<Message>(sql::where(sql::c(&Message::start_time) < one_day_ago));
}

#endif MESSAGE_HPP