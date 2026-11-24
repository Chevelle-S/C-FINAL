// ============================================================
//  WEATHER AND ENVIRONMENTAL MONITORING SYSTEM
//  Tracks temperature, humidity, rainfall and air quality (AQI)
//  readings from multiple monitoring stations.
// ============================================================
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cctype>
using namespace std;

const int MAX_STATIONS = 10;   // maximum number of stations
const int MAX_DAYS     = 7;    // maximum readings (days) per station

// ---------- 1D ARRAYS (one value per station) ----------
string stationName[MAX_STATIONS];
int    daysLogged[MAX_STATIONS];

// ---------- 2D ARRAYS [station][day] ----------
double temperature[MAX_STATIONS][MAX_DAYS];   // degrees Celsius
double humidity[MAX_STATIONS][MAX_DAYS];      // percent
double rainfall[MAX_STATIONS][MAX_DAYS];      // millimeters
double aqi[MAX_STATIONS][MAX_DAYS];           // Air Quality Index

int stationCount = 0;

// ---------- Helper functions ----------
string toLower(string s) {
    for (size_t i = 0; i < s.length(); i++)
        s[i] = tolower(s[i]);
    return s;
}

double readNumber(const string &prompt, double minVal, double maxVal) {
    string line;
    double value;
    while (true) {
        cout << prompt;
        getline(cin, line);
        stringstream ss(line);
        char extra;
        if ((ss >> value) && !(ss >> extra) && value >= minVal && value <= maxVal)
            return value;
        cout << "  Invalid input. Enter a number from " << minVal << " to " << maxVal << ".\n";
    }
}

int findStation(const string &name) {
    for (int i = 0; i < stationCount; i++)
        if (toLower(stationName[i]) == toLower(name))
            return i;
    return -1;
}

string aqiCategory(double value) {
    if (value <= 50)  return "Good";
    if (value <= 100) return "Moderate";
    if (value <= 150) return "Unhealthy (Sensitive)";
    if (value <= 200) return "Unhealthy";
    return "Very Unhealthy";
}

void showMenu() {
    cout << "\n==================================\n";
    cout << "   WEATHER & ENVIRONMENTAL MONITORING\n";
    cout << "==================================\n";
    cout << "1. Add Record\n";
    cout << "2. View Records\n";
    cout << "3. Search Record\n";
    cout << "4. Update Record\n";
    cout << "5. Delete Record\n";
    cout << "6. Display Summary\n";
    cout << "7. Exit\n";
    cout << "==================================\n";
}

void inputReading(int s, int d) {
    temperature[s][d] = readNumber("  Temperature (C, -50 to 60): ", -50, 60);
    humidity[s][d]    = readNumber("  Humidity (%, 0 to 100): ", 0, 100);
    rainfall[s][d]    = readNumber("  Rainfall (mm, 0 to 500): ", 0, 500);
    aqi[s][d]         = readNumber("  Air Quality Index (0 to 500): ", 0, 500);
}

void printTableHeader() {
    cout << left << setw(5) << "Day" << setw(10) << "Temp(C)" << setw(12) << "Humidity(%)"
         << setw(14) << "Rainfall(mm)" << setw(7) << "AQI" << "Air Quality\n";
    cout << string(66, '-') << "\n";
}

void printReadingRow(int s, int d) {
    cout << left << setw(5) << d + 1
         << setw(10) << fixed << setprecision(1) << temperature[s][d]
         << setw(12) << humidity[s][d]
         << setw(14) << rainfall[s][d]
         << setw(7)  << setprecision(0) << aqi[s][d]
         << aqiCategory(aqi[s][d]) << "\n";
}

void printStation(int s) {
    cout << "\nStation: " << stationName[s] << "  (" << daysLogged[s] << " reading(s))\n";
    printTableHeader();
    for (int d = 0; d < daysLogged[s]; d++)
        printReadingRow(s, d);
}

// ---------- 1. ADD ----------
void addRecord() {
    string name;
    cout << "\nEnter station name: ";
    getline(cin, name);
    if (name.empty()) { cout << "Name cannot be empty.\n"; return; }

    int s = findStation(name);
    if (s == -1) {                       // new station
        if (stationCount >= MAX_STATIONS) {
            cout << "Station limit reached (" << MAX_STATIONS << ").\n";
            return;
        }
        s = stationCount++;
        stationName[s] = name;
        daysLogged[s] = 0;
        cout << "New station created.\n";
    }
    if (daysLogged[s] >= MAX_DAYS) {
        cout << "This station already has " << MAX_DAYS << " readings.\n";
        return;
    }
    int d = daysLogged[s];
    cout << "Entering reading for Day " << d + 1 << ":\n";
    inputReading(s, d);
    daysLogged[s]++;
    cout << "Record added successfully!\n";
}

// ---------- 2. VIEW ----------
void viewRecords() {
    if (stationCount == 0) { cout << "\nNo records available.\n"; return; }
    for (int i = 0; i < stationCount; i++)
        printStation(i);
}

// ---------- 3. SEARCH ----------
void searchRecord() {
    if (stationCount == 0) { cout << "\nNo records available.\n"; return; }
    string name;
    cout << "\nEnter station name to search: ";
    getline(cin, name);
    int s = findStation(name);
    if (s == -1) cout << "Station not found.\n";
    else printStation(s);
}

// ---------- 4. UPDATE ----------
void updateRecord() {
    if (stationCount == 0) { cout << "\nNo records available.\n"; return; }
    string name;
    cout << "\nEnter station name to update: ";
    getline(cin, name);
    int s = findStation(name);
    if (s == -1) { cout << "Station not found.\n"; return; }
    if (daysLogged[s] == 0) { cout << "No readings to update.\n"; return; }

    printStation(s);
    int day = (int)readNumber("Enter day number to update: ", 1, daysLogged[s]);
    cout << "Enter new values for Day " << day << ":\n";
    inputReading(s, day - 1);
    cout << "Record updated successfully!\n";
}

// ---------- 5. DELETE ----------
void deleteRecord() {
    if (stationCount == 0) { cout << "\nNo records available.\n"; return; }
    string name;
    cout << "\nEnter station name: ";
    getline(cin, name);
    int s = findStation(name);
    if (s == -1) { cout << "Station not found.\n"; return; }

    int choice = (int)readNumber("1 = Delete one day's reading, 2 = Delete entire station: ", 1, 2);
    if (choice == 1) {
        if (daysLogged[s] == 0) { cout << "No readings to delete.\n"; return; }
        printStation(s);
        int day = (int)readNumber("Enter day number to delete: ", 1, daysLogged[s]);
        for (int d = day - 1; d < daysLogged[s] - 1; d++) {   // shift left
            temperature[s][d] = temperature[s][d + 1];
            humidity[s][d]    = humidity[s][d + 1];
            rainfall[s][d]    = rainfall[s][d + 1];
            aqi[s][d]         = aqi[s][d + 1];
        }
        daysLogged[s]--;
        cout << "Reading deleted.\n";
    } else {
        for (int i = s; i < stationCount - 1; i++) {          // shift stations up
            stationName[i] = stationName[i + 1];
            daysLogged[i]  = daysLogged[i + 1];
            for (int d = 0; d < MAX_DAYS; d++) {
                temperature[i][d] = temperature[i + 1][d];
                humidity[i][d]    = humidity[i + 1][d];
                rainfall[i][d]    = rainfall[i + 1][d];
                aqi[i][d]         = aqi[i + 1][d];
            }
        }
        stationCount--;
        cout << "Station deleted.\n";
    }
}

// ---------- 6. SUMMARY ----------
void displaySummary() {
    if (stationCount == 0) { cout << "\nNo records available.\n"; return; }

    cout << "\n=========== SUMMARY PER STATION ===========\n";
    cout << left << setw(14) << "Station" << setw(10) << "AvgTemp" << setw(10) << "AvgHum"
         << setw(12) << "TotalRain" << setw(8) << "AvgAQI" << "Status\n";
    cout << string(70, '-') << "\n";

    double hottest = -1000, coldest = 1000, wettest = -1, worstAqi = -1;
    string hottestName, coldestName, wettestName, worstAqiName;

    for (int s = 0; s < stationCount; s++) {
        if (daysLogged[s] == 0) continue;
        double sumT = 0, sumH = 0, sumR = 0, sumA = 0;
        for (int d = 0; d < daysLogged[s]; d++) {
            sumT += temperature[s][d];
            sumH += humidity[s][d];
            sumR += rainfall[s][d];
            sumA += aqi[s][d];
        }
        double n = daysLogged[s];
        double avgT = sumT / n, avgH = sumH / n, avgA = sumA / n;

        string status = "Normal";
        if (avgA > 100)      status = "AIR QUALITY ALERT";
        else if (avgT >= 35) status = "HEAT ALERT";
        else if (sumR >= 100) status = "HEAVY RAIN ALERT";

        cout << left << setw(14) << stationName[s].substr(0, 13)
             << setw(10) << fixed << setprecision(1) << avgT
             << setw(10) << avgH
             << setw(12) << sumR
             << setw(8)  << avgA
             << status << "\n";

        if (avgT > hottest)  { hottest = avgT;  hottestName = stationName[s]; }
        if (avgT < coldest)  { coldest = avgT;  coldestName = stationName[s]; }
        if (sumR > wettest)  { wettest = sumR;  wettestName = stationName[s]; }
        if (avgA > worstAqi) { worstAqi = avgA; worstAqiName = stationName[s]; }
    }

    // Per-day network averages using the 2D arrays (column-wise processing)
    cout << "\n--- Network average temperature per day ---\n";
    for (int d = 0; d < MAX_DAYS; d++) {
        double sum = 0; int count = 0;
        for (int s = 0; s < stationCount; s++)
            if (d < daysLogged[s]) { sum += temperature[s][d]; count++; }
        if (count > 0)
            cout << "Day " << d + 1 << ": " << fixed << setprecision(1) << sum / count << " C\n";
    }

    cout << "\n--- Highlights ---\n";
    cout << "Hottest station (avg) : " << hottestName << " (" << hottest << " C)\n";
    cout << "Coldest station (avg) : " << coldestName << " (" << coldest << " C)\n";
    cout << "Wettest station       : " << wettestName << " (" << wettest << " mm)\n";
    cout << "Worst air quality     : " << worstAqiName << " (AQI " << setprecision(0) << worstAqi
         << ", " << aqiCategory(worstAqi) << ")\n";
}

// ---------- MAIN ----------
int main() {
    int choice;
    do {
        showMenu();
        choice = (int)readNumber("Enter your choice: ", 1, 7);
        switch (choice) {
            case 1: addRecord();      break;
            case 2: viewRecords();    break;
            case 3: searchRecord();   break;
            case 4: updateRecord();   break;
            case 5: deleteRecord();   break;
            case 6: displaySummary(); break;
            case 7: cout << "\nThank you for using the system. Goodbye!\n"; break;
        }
    } while (choice != 7);
    return 0;
}
