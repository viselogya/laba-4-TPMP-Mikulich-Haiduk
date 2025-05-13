#pragma once

struct MasterRepairStats;
struct RepairWithCarInfo;
struct BrandRepairInfo;
struct WorkshopStats;
struct FullRepairInfo;


class AutoSalonDatabaseManager
{
public:

	explicit AutoSalonDatabaseManager(const std::filesystem::path& m_databasePath,
		const std::filesystem::path& databaseSchemePath);

	bool IsUserExist(const std::string username, const std::string password) const;

	std::vector<MasterRepairStats> GetRepairsByWorkshopAndPeriod(int workshopId, const std::string& startDate, 
		const std::string& endDate) const;

	std::vector<RepairWithCarInfo> GetRepairsAndCarsByMechanic(int mechanicId) const;

	std::vector<BrandRepairInfo> GetRepairsByCarBrands() const;

	std::vector<WorkshopStats> GetTotalRepairsAndRevenuePerWorkshop() const;

	std::vector<FullRepairInfo> GetTopWorkshopRepairsAndCars() const;

	int GetRepairCountByWorkshopAndDate(int workshopId, const std::string& date) const;


private:

	void CreateTables(const std::filesystem::path& databaseSchemePath) const;

	std::string GetDatabaseSchemaByPath(const std::filesystem::path& databaseSchemePath) const;
	

	sqlite3_stmt* PrepareStatement(const char* sqlRequest) const;

private:

	sqlite3* m_database{ nullptr };

};

