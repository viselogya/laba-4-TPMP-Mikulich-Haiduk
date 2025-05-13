#pragma once

class AutoSalonDatabaseManager;

class Auth
{
public:

	explicit Auth(AutoSalonDatabaseManager& databaseManager);

	bool Authenicate() const;

private:

	AutoSalonDatabaseManager& m_databaseManager;
};

