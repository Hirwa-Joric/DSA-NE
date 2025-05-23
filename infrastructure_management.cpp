#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <sstream>  // For string stream operations
#include <cstdlib>   // For system("clear") function

using namespace std;

// Helper function to print a horizontal line divider
void printDivider(char symbol = '=', int length = 50) {
    cout << string(length, symbol) << endl;
}

// Helper function to print a centered title
void printTitle(const string& title, char symbol = '=', int length = 50) {
    int padding = (length - title.length()) / 2;
    cout << string(padding, symbol) << " " << title << " " << string(padding - (title.length() % 2 == 0 ? 0 : 1), symbol) << endl;
}

class City {
private:
    int index;
    string name;

public:
    City(int idx, string n) : index(idx), name(n) {}

    int getIndex() const { return index; }
    string getName() const { return name; }
    void setName(string n) { name = n; }
};

class InfrastructureManagement {
private:
    vector<City> cities;
    vector<vector<int>> roadsMatrix;
    vector<vector<double>> budgetMatrix;
    bool dataLoaded;

    // Helper functions
    int findCityIndexByName(const string& cityName) {
        for (size_t i = 0; i < cities.size(); i++) {
            if (cities[i].getName() == cityName) {
                return i;
            }
        }
        return -1;
    }

    int findCityIndexByIndex(int index) {
        for (size_t i = 0; i < cities.size(); i++) {
            if (cities[i].getIndex() == index) {
                return i;
            }
        }
        return -1;
    }

    void resizeMatrices() {
        size_t n = cities.size();
        
        // Resize roads matrix
        roadsMatrix.resize(n);
        for (size_t i = 0; i < n; i++) {
            roadsMatrix[i].resize(n, 0);
        }
        
        // Resize budget matrix
        budgetMatrix.resize(n);
        for (size_t i = 0; i < n; i++) {
            budgetMatrix[i].resize(n, 0.0);
        }
    }

public:
    InfrastructureManagement() : dataLoaded(false) {
        // Try to load data from files on initialization
        loadDataFromFiles();
    }

    void addCity(string name) {
        // Check if city already exists
        if (findCityIndexByName(name) != -1) {
            cout << "City '" << name << "' already exists!" << endl;
            return;
        }
        
        // Assign the next available index
        int nextIndex = cities.empty() ? 1 : cities.back().getIndex() + 1;
        cities.push_back(City(nextIndex, name));
        
        // Resize matrices to accommodate the new city
        resizeMatrices();
        
        cout << "City '" << name << "' added with index " << nextIndex << endl;
    }

    void addRoad(string city1, string city2) {
        int idx1 = findCityIndexByName(city1);
        int idx2 = findCityIndexByName(city2);
        
        if (idx1 == -1) {
            cout << "City '" << city1 << "' does not exist!" << endl;
            return;
        }
        
        if (idx2 == -1) {
            cout << "City '" << city2 << "' does not exist!" << endl;
            return;
        }
        
        if (idx1 == idx2) {
            cout << "Cannot add a road between the same city!" << endl;
            return;
        }
        
        // Add road in both directions (undirected graph)
        roadsMatrix[idx1][idx2] = 1;
        roadsMatrix[idx2][idx1] = 1;
        
        cout << "Road added between " << city1 << " and " << city2 << endl;
    }

    void addBudget(string city1, string city2, double budget) {
        int idx1 = findCityIndexByName(city1);
        int idx2 = findCityIndexByName(city2);
        
        if (idx1 == -1) {
            cout << "City '" << city1 << "' does not exist!" << endl;
            return;
        }
        
        if (idx2 == -1) {
            cout << "City '" << city2 << "' does not exist!" << endl;
            return;
        }
        
        if (roadsMatrix[idx1][idx2] != 1) {
            cout << "No road exists between " << city1 << " and " << city2 << "!" << endl;
            cout << "Please add a road first before assigning a budget." << endl;
            return;
        }
        
        // Add budget in both directions (undirected graph)
        budgetMatrix[idx1][idx2] = budget;
        budgetMatrix[idx2][idx1] = budget;
        
        cout << "Budget added for the road between " << city1 << " and " << city2 << endl;
    }

    void editCity(int index, string newName) {
        int idx = findCityIndexByIndex(index);
        
        if (idx == -1) {
            cout << "No city found with index " << index << endl;
            return;
        }
        
        // Check if the new name already exists
        int existingIdx = findCityIndexByName(newName);
        if (existingIdx != -1 && existingIdx != idx) {
            cout << "City '" << newName << "' already exists!" << endl;
            return;
        }
        
        cities[idx].setName(newName);
        cout << "City updated successfully" << endl;
    }

    void searchCity(int index) {
        int idx = findCityIndexByIndex(index);
        
        printDivider('=', 60);
        printTitle("SEARCH RESULTS", '=', 60);
        printDivider('-', 60);
        
        if (idx == -1) {
            cout << "No city found with index " << index << endl;
            printDivider('-', 60);
            return;
        }
        
        cout << setw(10) << "INDEX" << setw(30) << "CITY NAME" << endl;
        printDivider('-', 60);
        cout << setw(10) << cities[idx].getIndex() << setw(30) << cities[idx].getName() << endl;
        printDivider('=', 60);
    }

    void displayCities() {
        printDivider('=', 60);
        printTitle("CITIES LIST", '=', 60);
        printDivider('-', 60);
        
        cout << setw(10) << "INDEX" << setw(30) << "CITY NAME" << endl;
        printDivider('-', 60);
        
        for (const auto& city : cities) {
            cout << setw(10) << city.getIndex() << setw(30) << city.getName() << endl;
        }
        
        printDivider('=', 60);
    }

    void displayRoads() {
        displayCities();
        
        printDivider('=', 60);
        printTitle("ROADS ADJACENCY MATRIX", '=', 60);
        printDivider('-', 60);
        
        // Display column headers (city indices)
        cout << setw(6) << " ";
        for (size_t j = 0; j < cities.size(); j++) {
            cout << setw(4) << cities[j].getIndex();
        }
        cout << endl;
        
        printDivider('-', 60);
        
        // Display matrix with row headers
        for (size_t i = 0; i < cities.size(); i++) {
            cout << setw(4) << cities[i].getIndex() << " |";
            for (size_t j = 0; j < cities.size(); j++) {
                cout << setw(4) << roadsMatrix[i][j];
            }
            cout << endl;
        }
        
        printDivider('=', 60);
    }

    void displayAllData() {
        displayCities();
        
        // Display Roads Adjacency Matrix
        printDivider('=', 60);
        printTitle("ROADS ADJACENCY MATRIX", '=', 60);
        printDivider('-', 60);
        
        // Display column headers (city indices)
        cout << setw(6) << " ";
        for (size_t j = 0; j < cities.size(); j++) {
            cout << setw(4) << cities[j].getIndex();
        }
        cout << endl;
        
        printDivider('-', 60);
        
        // Display matrix with row headers
        for (size_t i = 0; i < cities.size(); i++) {
            cout << setw(4) << cities[i].getIndex() << " |";
            for (size_t j = 0; j < cities.size(); j++) {
                cout << setw(4) << roadsMatrix[i][j];
            }
            cout << endl;
        }
        
        printDivider('=', 60);
        
        // Display Budgets Adjacency Matrix
        printTitle("BUDGETS ADJACENCY MATRIX (Billion RWF)", '=', 60);
        printDivider('-', 60);
        
        // Display column headers (city indices)
        cout << setw(6) << " ";
        for (size_t j = 0; j < cities.size(); j++) {
            cout << setw(7) << cities[j].getIndex();
        }
        cout << endl;
        
        printDivider('-', 60);
        
        // Display matrix with row headers
        for (size_t i = 0; i < cities.size(); i++) {
            cout << setw(4) << cities[i].getIndex() << " |";
            for (size_t j = 0; j < cities.size(); j++) {
                cout << fixed << setprecision(1) << setw(7) << budgetMatrix[i][j];
            }
            cout << endl;
        }
        
        printDivider('=', 60);
    }

    void saveCitiesToFile() {
        ofstream file("cities.txt");
        
        if (!file.is_open()) {
            cout << "Error opening cities.txt for writing!" << endl;
            return;
        }
        
        file << "Index\tCity_name" << endl;
        for (const auto& city : cities) {
            file << city.getIndex() << "\t" << city.getName() << endl;
        }
        
        file.close();
        cout << "Cities saved to cities.txt" << endl;
    }

    void saveRoadsToFile() {
        ofstream file("roads.txt");
        
        if (!file.is_open()) {
            cout << "Error opening roads.txt for writing!" << endl;
            return;
        }
        
        file << "Nbr\tRoad\t\t\tBudget" << endl;
        
        int roadCount = 0;
        for (size_t i = 0; i < cities.size(); i++) {
            for (size_t j = i + 1; j < cities.size(); j++) {
                if (roadsMatrix[i][j] == 1) {
                    roadCount++;
                    file << roadCount << ".\t" 
                         << cities[i].getName() << "-" << cities[j].getName() 
                         << "\t\t" << fixed << setprecision(2) << budgetMatrix[i][j] << endl;
                }
            }
        }
        
        file.close();
        cout << "Roads saved to roads.txt" << endl;
    }
    
    void loadCitiesFromFile() {
        ifstream file("cities.txt");
        
        if (!file.is_open()) {
            cout << "No previous cities data found." << endl;
            return;
        }
        
        string line;
        // Skip header line
        getline(file, line);
        
        cities.clear(); // Clear existing cities
        
        while (getline(file, line)) {
            stringstream ss(line);
            int index;
            string name;
            
            ss >> index;
            // Skip the tab character
            ss.ignore(1);
            // Read the rest of the line as the city name
            getline(ss, name);
            
            cities.push_back(City(index, name));
        }
        
        file.close();
        
        // Resize matrices to match loaded cities
        if (!cities.empty()) {
            resizeMatrices();
            cout << cities.size() << " cities loaded from cities.txt" << endl;
        }
    }
    
    void loadRoadsFromFile() {
        ifstream file("roads.txt");
        
        if (!file.is_open()) {
            cout << "No previous roads data found." << endl;
            return;
        }
        
        string line;
        // Skip header line
        getline(file, line);
        
        while (getline(file, line)) {
            stringstream ss(line);
            int roadNum;
            string roadInfo;
            double budget;
            string city1, city2;
            
            // Read road number (with the dot)
            ss >> roadNum;
            ss.ignore(2); // Skip the dot and tab
            
            // Read the road info (city1-city2)
            getline(ss, roadInfo, '\t');
            
            // Find the position of the hyphen
            size_t hyphenPos = roadInfo.find('-');
            if (hyphenPos != string::npos) {
                city1 = roadInfo.substr(0, hyphenPos);
                city2 = roadInfo.substr(hyphenPos + 1);
                
                // Skip tabs
                ss >> budget;
                
                // Add the road and budget
                int idx1 = findCityIndexByName(city1);
                int idx2 = findCityIndexByName(city2);
                
                if (idx1 != -1 && idx2 != -1) {
                    roadsMatrix[idx1][idx2] = 1;
                    roadsMatrix[idx2][idx1] = 1;
                    budgetMatrix[idx1][idx2] = budget;
                    budgetMatrix[idx2][idx1] = budget;
                }
            }
        }
        
        file.close();
        cout << "Roads and budgets loaded from roads.txt" << endl;
    }
    
    void loadDataFromFiles() {
        loadCitiesFromFile();
        loadRoadsFromFile();
        dataLoaded = true;
    }

    void saveAllData() {
        saveCitiesToFile();
        saveRoadsToFile();
    }
};



void displayMenu() {
    system("clear"); // Clear the console on Linux/macOS
    
    printDivider('=', 60);
    printTitle("RWANDA INFRASTRUCTURE MANAGEMENT SYSTEM", '=', 60);
    printDivider('=', 60);
    
    cout << "\n" << setw(25) << "MAIN MENU" << "\n" << endl;
    printDivider('-', 60);
    
    cout << "  1. Add new city(ies)" << endl;
    cout << "  2. Add roads between cities" << endl;
    cout << "  3. Add the budget for roads" << endl;
    cout << "  4. Edit city" << endl;
    cout << "  5. Search for a city using its index" << endl;
    cout << "  6. Display cities" << endl;
    cout << "  7. Display roads" << endl;
    cout << "  8. Display recorded data on console" << endl;
    cout << "  9. Exit" << endl;
    
    printDivider('-', 60);
    cout << "Enter your choice: ";
}

int main() {
    // Display welcome message
    system("clear");
    printDivider('*', 70);
    printTitle("WELCOME TO RWANDA INFRASTRUCTURE MANAGEMENT SYSTEM", '*', 70);
    printTitle("Ministry of Infrastructure", '-', 70);
    printDivider('*', 70);
    cout << "\nThis system helps manage cities, roads, and budgets for Rwanda's infrastructure." << endl;
    cout << "Press Enter to continue..." << endl;
    cin.get();
    
    InfrastructureManagement infra;
    int choice;
    
    do {
        displayMenu();
        
        // Check if the input is a valid integer
        if (!(cin >> choice)) {
            // Clear the error state
            cin.clear();
            // Discard invalid input
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }
        
        // Clear the input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                printDivider('=', 60);
                printTitle("ADD NEW CITIES", '=', 60);
                printDivider('-', 60);
                int numCities;
                cout << "Enter the number of cities to add: ";
                
                // Check if the input is a valid integer
                if (!(cin >> numCities)) {
                    // Clear the error state
                    cin.clear();
                    // Discard invalid input
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a number." << endl;
                    break;
                }
                
                // Validate the number of cities
                if (numCities <= 0) {
                    cout << "Please enter a positive number of cities." << endl;
                    break;
                }
                
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                for (int i = 0; i < numCities; i++) {
                    string cityName;
                    cout << "Enter name for city " << (i + 1) << ": ";
                    getline(cin, cityName);
                    infra.addCity(cityName);
                }
                
                // Save data after adding cities
                infra.saveAllData();
                cout << "\nPress Enter to continue..." << endl;
                cin.get();
                break;
            }
            
            case 2: {
                printDivider('=', 60);
                printTitle("ADD ROADS BETWEEN CITIES", '=', 60);
                printDivider('-', 60);
                string city1, city2;
                cout << "Enter the name of the first city: ";
                getline(cin, city1);
                if (city1.empty()) {
                    cout << "City name cannot be empty. Please try again." << endl;
                    break;
                }
                
                cout << "Enter the name of the second city: ";
                getline(cin, city2);
                if (city2.empty()) {
                    cout << "City name cannot be empty. Please try again." << endl;
                    break;
                }
                
                infra.addRoad(city1, city2);
                
                // Save data after adding a road
                infra.saveAllData();
                cout << "\nPress Enter to continue..." << endl;
                cin.get();
                break;
            }
            
            case 3: {
                printDivider('=', 60);
                printTitle("ADD BUDGET FOR ROADS", '=', 60);
                printDivider('-', 60);
                string city1, city2;
                double budget;
                
                cout << "Enter the name of the first city: ";
                getline(cin, city1);
                if (city1.empty()) {
                    cout << "City name cannot be empty. Please try again." << endl;
                    break;
                }
                
                cout << "Enter the name of the second city: ";
                getline(cin, city2);
                if (city2.empty()) {
                    cout << "City name cannot be empty. Please try again." << endl;
                    break;
                }
                
                cout << "Enter the budget for that road: ";
                if (!(cin >> budget)) {
                    // Clear the error state
                    cin.clear();
                    // Discard invalid input
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid budget value. Please enter a number." << endl;
                    break;
                }
                
                if (budget <= 0) {
                    cout << "Budget must be a positive number." << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                infra.addBudget(city1, city2, budget);
                
                // Save data after adding a budget
                infra.saveAllData();
                cout << "\nPress Enter to continue..." << endl;
                cin.get();
                break;
            }
            
            case 4: {
                printDivider('=', 60);
                printTitle("EDIT CITY", '=', 60);
                printDivider('-', 60);
                int index;
                string newName;
                
                cout << "Enter the index of the city to edit: ";
                if (!(cin >> index)) {
                    // Clear the error state
                    cin.clear();
                    // Discard invalid input
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid index. Please enter a number." << endl;
                    break;
                }
                
                if (index <= 0) {
                    cout << "Index must be a positive number." << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                cout << "Enter new name for city: ";
                getline(cin, newName);
                if (newName.empty()) {
                    cout << "City name cannot be empty. Please try again." << endl;
                    break;
                }
                
                infra.editCity(index, newName);
                
                // Save data after editing a city
                infra.saveAllData();
                cout << "\nPress Enter to continue..." << endl;
                cin.get();
                break;
            }
            
            case 5: {
                printDivider('=', 60);
                printTitle("SEARCH FOR A CITY", '=', 60);
                printDivider('-', 60);
                int index;
                
                cout << "Enter the index of the city to search: ";
                if (!(cin >> index)) {
                    // Clear the error state
                    cin.clear();
                    // Discard invalid input
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid index. Please enter a number." << endl;
                    break;
                }
                
                if (index <= 0) {
                    cout << "Index must be a positive number." << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                infra.searchCity(index);
                cout << "\nPress Enter to continue..." << endl;
                cin.get();
                break;
            }
            
            case 6:
                printDivider('=', 60);
                printTitle("DISPLAY CITIES", '=', 60);
                printDivider('-', 60);
                infra.displayCities();
                cout << "\nPress Enter to continue..." << endl;
                cin.get();
                break;
                
            case 7:
                printDivider('=', 60);
                printTitle("DISPLAY ROADS", '=', 60);
                printDivider('-', 60);
                infra.displayRoads();
                cout << "\nPress Enter to continue..." << endl;
                cin.get();
                break;
                
            case 8:
                printDivider('=', 60);
                printTitle("DISPLAY ALL DATA", '=', 60);
                printDivider('-', 60);
                infra.displayAllData();
                cout << "\nPress Enter to continue..." << endl;
                cin.get();
                break;
                
            case 9:
                printDivider('=', 60);
                printTitle("EXITING PROGRAM", '=', 60);
                printDivider('-', 60);
                cout << "Exiting program. Goodbye!" << endl;
                // Save all data before exiting
                infra.saveAllData();
                break;
                
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
    } while (choice != 9);
    
    return 0;
}
