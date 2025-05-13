#include "pch.h"
#include "Auth.h"
#include "AutoSalonDatabaseManager.h"

Auth::Auth(AutoSalonDatabaseManager& databaseManager)
	:
	m_databaseManager(databaseManager)
{}

bool Auth::Authenicate() const
{
	std::cout << "Write your name and password\n";
	std::string name;
	std::string password;

	std::cin >> name >> password;

	return m_databaseManager.IsUserExist(name, password);
}