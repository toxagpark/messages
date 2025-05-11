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
		std::cout << "1. �������� ����� ���������\n"
			<< "2. ����� ���������\n"
			<< "3. �����\n"
			<< "����: ";

		if (!(std::cin >> choice)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		switch (choice) {
			
			case 1:
			{
				std::cout << "������� ����� ���������: ";
				std::string user_text;
				std::cin >> user_text;

				bool do_link = true;

				while (do_link) {
					std::cout << "���������� ������: ";
					std::string user_link;
					std::cin >> user_link;

					if (storage.count<Message>(sql::where(sql::c(&Message::hash) == user_link)) > 0) {
						std::cout << "��� ������ ��� ������, ���������� ������\n";
						continue;
					}

					Message new_message{ user_text, user_link };

					try {
						storage.insert(new_message);
						std::cout << "��������� ��������� � �������: " << user_link << "\n\n\n";
						do_link = false;
					}
					catch (const std::exception& e) {
						std::cerr << "������ ����������: " << e.what() << "\n\n\n";
						do_link = false;
					}
				}
				break;
			}

			case 2:
			{
				std::cout << "������� ������: ";
				std::string search_link;
				std::cin >> search_link;

				auto messages = storage.get_all<Message>(sql::where(sql::c(&Message::hash) == search_link));

				if (!messages.empty()) {
					std::cout << "������� ���������:\n" << messages[0].text << "\n\n\n";
				}
				else {
					std::cout << "��������� �� �������!\n\n\n";
				}
				break;
			}

			default:
				std::cout << "�������� �����\n\n\n";
				break;

		}

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	} while (choice != 3);
}

#endif WORK_WITH_USER_HPP