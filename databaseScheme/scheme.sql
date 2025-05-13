CREATE TABLE IF NOT EXISTS Users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    password TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS Workshop (
    workshop_id INTEGER PRIMARY KEY AUTOINCREMENT,
    address TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS Workshop_Brand (
    workshop_id INTEGER NOT NULL,
    brand TEXT NOT NULL,
    PRIMARY KEY (workshop_id, brand),
    FOREIGN KEY (workshop_id) REFERENCES Workshop(workshop_id)
);

CREATE TABLE IF NOT EXISTS Mechanic (
    mechanic_id INTEGER PRIMARY KEY AUTOINCREMENT,
    last_name TEXT NOT NULL,
    first_name TEXT,
    middle_name TEXT
);

CREATE TABLE IF NOT EXISTS Workshop_Mechanic (
    workshop_id INTEGER NOT NULL,
    mechanic_id INTEGER NOT NULL,
    PRIMARY KEY (workshop_id, mechanic_id),
    FOREIGN KEY (workshop_id) REFERENCES Workshop(workshop_id),
    FOREIGN KEY (mechanic_id) REFERENCES Mechanic(mechanic_id)
);

CREATE TABLE IF NOT EXISTS Car (
    license_plate TEXT PRIMARY KEY,
    brand TEXT NOT NULL,
    manufacture_year INTEGER NOT NULL,
    owner_last_name TEXT NOT NULL,
    passport_number TEXT NOT NULL,
    owner_address TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS Repair (
    repair_id INTEGER PRIMARY KEY AUTOINCREMENT,
    workshop_id INTEGER NOT NULL,
    license_plate TEXT NOT NULL,
    start_date DATE NOT NULL,
    end_date DATE,
    repair_type TEXT NOT NULL,
    cost NUMERIC(10,2) NOT NULL,
    mechanic_id INTEGER NOT NULL,
    FOREIGN KEY (workshop_id) REFERENCES Workshop(workshop_id),
    FOREIGN KEY (license_plate) REFERENCES Car(license_plate),
    FOREIGN KEY (mechanic_id) REFERENCES Mechanic(mechanic_id)
);

CREATE TABLE IF NOT EXISTS WorkStats (
    mechanic_id INTEGER,
    repair_type TEXT,
    repair_count INTEGER DEFAULT 0,
    PRIMARY KEY (mechanic_id, repair_type)
);