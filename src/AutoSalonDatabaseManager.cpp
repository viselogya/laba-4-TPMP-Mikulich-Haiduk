#include "pch.h"
#include "AutoSalonDatabaseManager.h"
#include "Models.h"


AutoSalonDatabaseManager::AutoSalonDatabaseManager(const std::filesystem::path& m_databasePath,
	const std::filesystem::path& databaseSchemePath)
{
	if (sqlite3_open(m_databasePath.string().data(), &m_database) != SQLITE_OK) {
		sqlite3_close(m_database);

		throw std::runtime_error("error with database path");
	}

	CreateTables(databaseSchemePath);
}


void AutoSalonDatabaseManager::CreateTables(const std::filesystem::path& databaseSchemePath) const
{
	char* error;

	if (sqlite3_exec(m_database, "PRAGMA foreign_keys = ON;", nullptr, nullptr, &error) != SQLITE_OK) {

		sqlite3_free(error);
		sqlite3_close(m_database);

		throw std::runtime_error(std::format("sqlite3_exec error with enable PRAMG foreign_keys = ON: {}", error));
	}

	std::string scheme = GetDatabaseSchemaByPath(databaseSchemePath);

	if (sqlite3_exec(m_database, scheme.data(), nullptr, nullptr, &error) == SQLITE_ERROR) {
		sqlite3_free(error);
		sqlite3_close(m_database);

		throw std::runtime_error(std::format("sqlite3_exec error with create tables: {}", error));
	}
}


std::string AutoSalonDatabaseManager::GetDatabaseSchemaByPath(const std::filesystem::path& databaseSchemePath) const
{

	std::ifstream schemeStream(databaseSchemePath.string());

	if (!schemeStream) {
		throw std::runtime_error("error with database scheme path");
	}

	std::string scheme;
	
	std::string line;
	while (std::getline(schemeStream, line))
	{
		scheme += line;
	}

	return scheme;
}

sqlite3_stmt* AutoSalonDatabaseManager::PrepareStatement(const char* sqlRequest) const {
	sqlite3_stmt* result = nullptr;

	if (sqlite3_prepare_v2(m_database, sqlRequest, -1, &result, nullptr) != SQLITE_OK) {
		throw std::runtime_error(std::format("Error while preparing statement in SqliteDatabaseManager! GLE: {}",
			sqlite3_errmsg(m_database)));
	}

	return result;
}


bool AutoSalonDatabaseManager::IsUserExist(const std::string username, const std::string password) const
{
	const char* isUserExistsByNameAndPasswordSqlRequest = "SELECT EXISTS(SELECT 1 FROM Users WHERE username = ? AND password = ?);";
	sqlite3_stmt* isUserExistsByNameAndPassword = PrepareStatement(isUserExistsByNameAndPasswordSqlRequest);

	sqlite3_bind_text(isUserExistsByNameAndPassword, 1, username.data(), static_cast<int>(username.size()), SQLITE_STATIC);
	sqlite3_bind_text(isUserExistsByNameAndPassword, 2, password.data(), static_cast<int>(password.size()), SQLITE_STATIC);
	bool isExists = false;

	if (sqlite3_step(isUserExistsByNameAndPassword) == SQLITE_ROW) {
		isExists = (sqlite3_column_int(isUserExistsByNameAndPassword, 0) == 1);
	}
	else {
		throw std::runtime_error(std::format("SQL IsUserExists(by name and password) error: {}", sqlite3_errmsg(m_database)));
	}

	sqlite3_reset(isUserExistsByNameAndPassword);
	sqlite3_clear_bindings(isUserExistsByNameAndPassword);

	return isExists;
}


std::vector<MasterRepairStats> AutoSalonDatabaseManager::GetRepairsByWorkshopAndPeriod(int workshopId, const std::string& startDate, const std::string& endDate) const {
	const char* sql = "SELECT Mechanic.mechanic_id, Mechanic.last_name || ' ' || IFNULL(Mechanic.first_name, '') || ' ' || IFNULL(Mechanic.middle_name, ''), Repair.repair_type, COUNT(*) "
		"FROM Repair "
		"JOIN Mechanic ON Mechanic.mechanic_id = Repair.mechanic_id "
		"WHERE Repair.workshop_id = ? AND Repair.start_date >= ? AND Repair.end_date <= ? "
		"GROUP BY Mechanic.mechanic_id, Repair.repair_type;";

	sqlite3_stmt* stmt = PrepareStatement(sql);
	sqlite3_bind_int(stmt, 1, workshopId);
	sqlite3_bind_text(stmt, 2, startDate.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, endDate.c_str(), -1, SQLITE_STATIC);

	std::vector<MasterRepairStats> result;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		MasterRepairStats stats;
		stats.mechanicId = sqlite3_column_int(stmt, 0);
		stats.fullName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		stats.repairType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		stats.count = sqlite3_column_int(stmt, 3);
		result.push_back(stats);
	}

	sqlite3_finalize(stmt);
	return result;
}

std::vector<RepairWithCarInfo> AutoSalonDatabaseManager::GetRepairsAndCarsByMechanic(int mechanicId) const {
	const char* sql = "SELECT Repair.repair_id, Repair.workshop_id, Repair.start_date, Repair.end_date, Repair.repair_type, Repair.cost, "
		"Car.license_plate, Car.brand, Car.manufacture_year, Car.owner_last_name "
		"FROM Repair "
		"JOIN Car ON Repair.license_plate = Car.license_plate "
		"WHERE Repair.mechanic_id = ?;";

	sqlite3_stmt* stmt = PrepareStatement(sql);
	sqlite3_bind_int(stmt, 1, mechanicId);

	std::vector<RepairWithCarInfo> result;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		RepairWithCarInfo repair;
		repair.repairId = sqlite3_column_int(stmt, 0);
		repair.workshopId = sqlite3_column_int(stmt, 1);
		repair.startDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		repair.endDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
		repair.repairType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
		repair.cost = sqlite3_column_double(stmt, 5);
		repair.licensePlate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
		repair.brand = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
		repair.manufactureYear = sqlite3_column_int(stmt, 8);
		repair.ownerLastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
		result.push_back(repair);
	}

	sqlite3_finalize(stmt);
	return result;
}


std::vector<BrandRepairInfo> AutoSalonDatabaseManager::GetRepairsByCarBrands() const {
	const char* sql = "SELECT Car.brand, Repair.workshop_id, Repair.start_date, Repair.end_date, Repair.repair_type, Mechanic.last_name "
		"FROM Repair "
		"JOIN Car ON Repair.license_plate = Car.license_plate "
		"JOIN Mechanic ON Repair.mechanic_id = Mechanic.mechanic_id;";

	sqlite3_stmt* stmt = PrepareStatement(sql);

	std::vector<BrandRepairInfo> result;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		BrandRepairInfo info;
		info.brand = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		info.workshopId = sqlite3_column_int(stmt, 1);
		info.startDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		info.endDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
		info.repairType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
		info.mechanicLastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
		result.push_back(info);
	}

	sqlite3_finalize(stmt);
	return result;
}


std::vector<WorkshopStats> AutoSalonDatabaseManager::GetTotalRepairsAndRevenuePerWorkshop() const {
	const char* sql = "SELECT Repair.workshop_id, COUNT(*) AS total_repairs, SUM(Repair.cost) AS total_revenue "
		"FROM Repair "
		"GROUP BY Repair.workshop_id;";

	sqlite3_stmt* stmt = PrepareStatement(sql);

	std::vector<WorkshopStats> result;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		WorkshopStats stats;
		stats.workshopId = sqlite3_column_int(stmt, 0);
		stats.totalRepairs = sqlite3_column_int(stmt, 1);
		stats.totalRevenue = sqlite3_column_double(stmt, 2);
		result.push_back(stats);
	}

	sqlite3_finalize(stmt);
	return result;
}


std::vector<FullRepairInfo> AutoSalonDatabaseManager::GetTopWorkshopRepairsAndCars() const {
	const char* sql = "WITH RepairCount AS ("
		"  SELECT workshop_id, COUNT(*) AS repair_count "
		"  FROM Repair "
		"  GROUP BY workshop_id "
		"  ORDER BY repair_count DESC "
		"  LIMIT 1 "
		") "
		"SELECT Repair.repair_id, Repair.workshop_id, Repair.license_plate, Repair.start_date, "
		"       Repair.end_date, Repair.repair_type, Repair.cost, Repair.mechanic_id, "
		"       Car.brand, Car.owner_last_name "
		"FROM Repair "
		"JOIN RepairCount ON Repair.workshop_id = RepairCount.workshop_id "
		"JOIN Car ON Repair.license_plate = Car.license_plate;";

	sqlite3_stmt* stmt = PrepareStatement(sql);

	std::vector<FullRepairInfo> result;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		FullRepairInfo repair;
		repair.repairId = sqlite3_column_int(stmt, 0);
		repair.workshopId = sqlite3_column_int(stmt, 1);
		repair.licensePlate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		repair.startDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
		repair.endDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
		repair.repairType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
		repair.cost = sqlite3_column_double(stmt, 6);
		repair.mechanicId = sqlite3_column_int(stmt, 7);
		repair.carBrand = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
		repair.ownerLastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
		result.push_back(repair);
	}

	sqlite3_finalize(stmt);
	return result;
}


int AutoSalonDatabaseManager::GetRepairCountByWorkshopAndDate(int workshopId, const std::string& date) const {
	const char* sql = "SELECT COUNT(*) "
		"FROM Repair "
		"WHERE Repair.workshop_id = ? AND Repair.start_date <= ? AND (Repair.end_date >= ? OR Repair.end_date IS NULL);";

	sqlite3_stmt* stmt = PrepareStatement(sql);
	sqlite3_bind_int(stmt, 1, workshopId);
	sqlite3_bind_text(stmt, 2, date.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, date.c_str(), -1, SQLITE_STATIC);

	int repairCount = 0;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		repairCount = sqlite3_column_int(stmt, 0);
	}

	sqlite3_finalize(stmt);
	return repairCount;
}
