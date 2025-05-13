#pragma once

struct MasterRepairStats {
    int mechanicId;
    std::string fullName;
    std::string repairType;
    int count;

    void Print() const {
        std::cout << "Mechanic ID: " << mechanicId << std::endl;
        std::cout << "Mechanic Name: " << fullName << std::endl;
        std::cout << "Repair Type: " << repairType << std::endl;
        std::cout << "Repair Count: " << count << std::endl;
        std::cout << "-------------------------------" << std::endl;
    }
};


struct RepairWithCarInfo {
    int repairId;
    int workshopId;
    std::string startDate;
    std::string endDate;
    std::string repairType;
    double cost;
    std::string licensePlate;
    std::string brand;
    int manufactureYear;
    std::string ownerLastName;

    void Print() const {
        std::cout << "Repair ID: " << repairId << std::endl;
        std::cout << "Workshop ID: " << workshopId << std::endl;
        std::cout << "Start Date: " << startDate << std::endl;
        std::cout << "End Date: " << endDate << std::endl;
        std::cout << "Repair Type: " << repairType << std::endl;
        std::cout << "Cost: " << cost << std::endl;
        std::cout << "License Plate: " << licensePlate << std::endl;
        std::cout << "Brand: " << brand << std::endl;
        std::cout << "Manufacture Year: " << manufactureYear << std::endl;
        std::cout << "Owner Last Name: " << ownerLastName << std::endl;
        std::cout << "-------------------------------" << std::endl;
    }
};

struct BrandRepairInfo {
    std::string brand;
    int workshopId;
    std::string startDate;
    std::string endDate;
    std::string repairType;
    std::string mechanicLastName;

    void Print() const {
        std::cout << "Brand: " << brand << std::endl;
        std::cout << "Workshop ID: " << workshopId << std::endl;
        std::cout << "Start Date: " << startDate << std::endl;
        std::cout << "End Date: " << endDate << std::endl;
        std::cout << "Repair Type: " << repairType << std::endl;
        std::cout << "Mechanic Last Name: " << mechanicLastName << std::endl;
        std::cout << "-------------------------------" << std::endl;
    }
};


struct WorkshopStats {
    int workshopId;
    int totalRepairs;
    double totalRevenue;

    void Print() const {
        std::cout << "Workshop ID: " << workshopId << std::endl;
        std::cout << "Total Repairs: " << totalRepairs << std::endl;
        std::cout << "Total Revenue: " << totalRevenue << std::endl;
        std::cout << "-------------------------------" << std::endl;
    }
};

struct FullRepairInfo {
    int repairId;
    int workshopId;
    std::string licensePlate;
    std::string startDate;
    std::string endDate;
    std::string repairType;
    double cost;
    int mechanicId;
    std::string carBrand;
    std::string ownerLastName;

    void Print() const {
        std::cout << "Repair ID: " << repairId << std::endl;
        std::cout << "Workshop ID: " << workshopId << std::endl;
        std::cout << "License Plate: " << licensePlate << std::endl;
        std::cout << "Start Date: " << startDate << std::endl;
        std::cout << "End Date: " << endDate << std::endl;
        std::cout << "Repair Type: " << repairType << std::endl;
        std::cout << "Cost: " << cost << std::endl;
        std::cout << "Mechanic ID: " << mechanicId << std::endl;
        std::cout << "Car Brand: " << carBrand << std::endl;
        std::cout << "Owner Last Name: " << ownerLastName << std::endl;
        std::cout << "-------------------------------" << std::endl;
    }
};