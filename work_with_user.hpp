#ifndef WORK_WITH_USER_HPP
#define WORK_WITH_USER_HPP

#include <iostream>
#include <string>
#include "message.hpp"
#include "link_generate.hpp"

void WorkWithUser() {
	auto storage = InitDatabase("messages.db");
	storage.sync_schema();
	DeleteOldMessages(storage);

	int choice = 0;
	do {
		std::cout << "1. Добавить новое сообщение\n"
			<< "2. Найти сообщение\n"
			<< "3. Вывод всех сообщений\n"
			<< "4. ВЫХОД\n"
			<< "Ввод: ";

		if (!(std::cin >> choice)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			choice = 0;
		}
		else {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}


		switch (choice) {
			
			case 1:
			{
				std::cout << "Введите текст сообщения: ";
				std::string user_text;
				std::getline(std::cin, user_text);

				bool do_link = true;

				while (do_link) {
					std::string user_link = GenerateRandomLink(8);

					if (storage.count<Message>(sql::where(sql::c(&Message::hash) == user_link)) > 0) {
						continue;
					}

					const auto now_time = std::time(nullptr);
					Message new_message{ user_text, user_link, now_time };

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

			case 3:
			{
				auto messages = storage.get_all<Message>();
				if (!messages.empty()) {
					for (const auto& message : messages) {
						std::cout << message.text << "\nLink: " << message.hash << "\n\n\n";
					}
				}
				else {
					std::cout << "Сообщений нет\n\n\n";
				}
				break;
			}

			case 4:
			{
				std::cout << "Выход\n\n\n";
				break;
			}

			default:
				std::cout << "Неверный выбор\n\n\n";
				break;

		}

	} while (choice != 4);
}

#endif WORK_WITH_USER_HPP