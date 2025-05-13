#include "pch.h"
#include "AutoSalonDatabaseManager.h"
#include "Models.h"
#include "Auth.h"

namespace config 
{
	const std::filesystem::path databasePath("../database/autosalon.db");
	const std::filesystem::path databaseSchemePath("../databaseScheme/scheme.sql");
}

void PrintMasterRepairStats(const AutoSalonDatabaseManager& dbManager);
void PrintRepairWithCarInfo(const AutoSalonDatabaseManager& dbManager);
void PrintWorkshopRepairStats(const AutoSalonDatabaseManager& dbManager);
void PrintWorkshopRevenueStats(const AutoSalonDatabaseManager& dbManager);
void PrintMostRepairsWorkshop(const AutoSalonDatabaseManager& dbManager);
void PrintWorkshopRepairCountStats(const AutoSalonDatabaseManager& dbManager);
void PrintWorkshopRepairCountOnDate(const AutoSalonDatabaseManager& dbManager);

int main() {
	try
	{
		auto databaseManager = std::make_unique<AutoSalonDatabaseManager>(config::databasePath, config::databaseSchemePath);
		auto auth = std::make_unique<Auth>(*databaseManager);

		while (!auth->Authenicate()) {}

        while (true) {
            std::cout << "1. Get master repair statistics" << std::endl;
            std::cout << "2. Get repair information with car info" << std::endl;
            std::cout << "3. Get workshop repair statistics by brand" << std::endl;
            std::cout << "4. Get workshop revenue stats" << std::endl;
            std::cout << "5. Get workshop with most repairs" << std::endl;
            std::cout << "6. Get workshop repair count statistics" << std::endl;
            std::cout << "7. Get workshop repair count on a specific date" << std::endl;
            std::cout << "0. Exit" << std::endl;
            std::cout << "Enter your choice: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
            case 1:

                break;
            case 2:
                PrintRepairWithCarInfo(*databaseManager);
                break;
            case 3:
                PrintWorkshopRepairStats(*databaseManager);
                break;
            case 4:
                PrintWorkshopRevenueStats(*databaseManager);
                break;
            case 5:
                PrintMostRepairsWorkshop(*databaseManager);
                break;
            case 6:
                PrintWorkshopRepairCountStats(*databaseManager);
                break;
            case 7:
                PrintWorkshopRepairCountOnDate(*databaseManager);
                break;
            case 0:
                std::cout << "Exiting..." << std::endl;
                return 0;
            default:
                std::cout << "Invalid choice, please try again." << std::endl;
            }
        }
	}
	catch (const std::exception& ex)
	{
		std::cout << std::format("Error! GLE: {}", ex.what());
	}
}

void PrintMasterRepairStats(const AutoSalonDatabaseManager& dbManager) {
    // Çàïðîñèòü ââîä ïîëüçîâàòåëÿ äëÿ âûáîðà àâòîìàñòåðñêîé è ïåðèîäà
    int workshopId;
    std::string startDate, endDate;
    std::cout << "Enter workshop ID: ";
    std::cin >> workshopId;
    std::cout << "Enter start date (YYYY-MM-DD): ";
    std::cin >> startDate;
    std::cout << "Enter end date (YYYY-MM-DD): ";
    std::cin >> endDate;

    std::vector<MasterRepairStats> stats = dbManager.GetRepairsByWorkshopAndPeriod(workshopId, startDate, endDate);
    for (const auto& stat : stats) {
        stat.Print();
    }
}

void PrintRepairWithCarInfo(const AutoSalonDatabaseManager& dbManager) {
    int mechanicId;
    std::cout << "Enter mechanic ID: ";
    std::cin >> mechanicId;

    std::vector<RepairWithCarInfo> repairs = dbManager.GetRepairsAndCarsByMechanic(mechanicId);
    for (const auto& repair : repairs) {
        repair.Print();
    }
}

void PrintWorkshopRepairStats(const AutoSalonDatabaseManager& dbManager) {
    std::vector<BrandRepairInfo> repairs = dbManager.GetRepairsByCarBrands();
    for (const auto& repair : repairs) {
        repair.Print();
    }
}

void PrintWorkshopRevenueStats(const AutoSalonDatabaseManager& dbManager) {
    std::vector<WorkshopStats> stats = dbManager.GetTotalRepairsAndRevenuePerWorkshop();
    for (const auto& stat : stats) {
        stat.Print();
    }
}

void PrintMostRepairsWorkshop(const AutoSalonDatabaseManager& dbManager) {
    std::vector<FullRepairInfo> repairs = dbManager.GetTopWorkshopRepairsAndCars();
    for (const auto& repair : repairs) {
        repair.Print();
    }
}

void PrintWorkshopRepairCountStats(const AutoSalonDatabaseManager& dbManager) {
    int workshopId;
    std::string startDate, endDate;
    std::cout << "Enter workshop ID: ";
    std::cin >> workshopId;
    std::cout << "Enter start date (YYYY-MM-DD): ";
    std::cin >> startDate;
    std::cout << "Enter end date (YYYY-MM-DD): ";
    std::cin >> endDate;

    std::vector<WorkshopStats> stats = dbManager.GetTotalRepairsAndRevenuePerWorkshop();
    for (const auto& stat : stats) {
        if (stat.workshopId == workshopId) {
            stat.Print();
        }
    }
}

void PrintWorkshopRepairCountOnDate(const AutoSalonDatabaseManager& dbManager) {
    int workshopId;
    std::string date;
    std::cout << "Enter workshop ID: ";
    std::cin >> workshopId;
    std::cout << "Enter date (YYYY-MM-DD): ";
    std::cin >> date;

    int repairCount = dbManager.GetRepairCountByWorkshopAndDate(workshopId, date);
    std::cout << "Repair count on the specified date: " << repairCount << std::endl;
}
