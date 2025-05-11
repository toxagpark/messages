#ifndef WORK_WITH_USER_HPP
#define WORK_WITH_USER_HPP

#include <iostream>
#include <string>
#include "message.hpp"

void WorkWithUser() {
	auto storage = InitDatabase("messages.db");
	storage.sync_schema();

	int choice = 0;
	do {
		std::cout << "1. Добавить новое сообщение\n"
			<< "2. Найти сообщение\n"
			<< "3. ВЫХОД\n"
			<< "Ввод: ";

		if (!(std::cin >> choice)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		switch (choice) {
			
			case 1:
			{
				std::cout << "Введите текст сообщения: ";
				std::string user_text;
				std::cin >> user_text;

				bool do_link = true;

				while (do_link) {
					std::cout << "Придумайте ссылку: ";
					std::string user_link;
					std::cin >> user_link;

					if (storage.count<Message>(sql::where(sql::c(&Message::hash) == user_link)) > 0) {
						std::cout << "Эта ссылка уже занята, попробуйте другую\n";
						continue;
					}

					Message new_message{ user_text, user_link };

					try {
						storage.insert(new_message);
						std::cout << "Сообщение сохранено с ссылкой: " << user_link << "\n\n\n";
						do_link = false;
					}
					catch (const std::exception& e) {
						std::cerr << "Ошибка сохранения: " << e.what() << "\n\n\n";
						do_link = false;
					}
				}
				break;
			}

			case 2:
			{
				std::cout << "Введите ссылку: ";
				std::string search_link;
				std::cin >> search_link;

				auto messages = storage.get_all<Message>(sql::where(sql::c(&Message::hash) == search_link));

				if (!messages.empty()) {
					std::cout << "Найдено сообщение:\n" << messages[0].text << "\n\n\n";
				}
				else {
					std::cout << "Сообщение не найдено!\n\n\n";
				}
				break;
			}

			default:
				std::cout << "Неверный выбор\n\n\n";
				break;

		}

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	} while (choice != 3);
}

#endif WORK_WITH_USER_HPP