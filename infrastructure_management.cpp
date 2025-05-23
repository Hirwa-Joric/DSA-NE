#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <sstream>  // For string stream operations

using namespace std;

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
        
        if (idx == -1) {
            cout << "No city found with index " << index << endl;
            return;
        }
        
        cout << "City found: " << cities[idx].getIndex() << ": " << cities[idx].getName() << endl;
    }

    void displayCities() {
        cout << "\nCities" << endl;
        for (const auto& city : cities) {
            cout << city.getIndex() << ": " << city.getName() << endl;
        }
    }

    void displayRoads() {
        displayCities();
        
        cout << "\nRoads Adjacency Matrix:" << endl;
        for (size_t i = 0; i < cities.size(); i++) {
            for (size_t j = 0; j < cities.size(); j++) {
                cout << roadsMatrix[i][j];
            }
            cout << endl;
        }
    }

    void displayAllData() {
        displayCities();
        
        cout << "\nRoads Adjacency Matrix:" << endl;
        for (size_t i = 0; i < cities.size(); i++) {
            for (size_t j = 0; j < cities.size(); j++) {
                cout << roadsMatrix[i][j];
            }
            cout << endl;
        }
        
        cout << "\nBudgets Adjacency Matrix:" << endl;
        for (size_t i = 0; i < cities.size(); i++) {
            for (size_t j = 0; j < cities.size(); j++) {
                cout << fixed << setprecision(1) << setw(5) << budgetMatrix[i][j] << " ";
            }
            cout << endl;
        }
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
    cout << "\nMenu" << endl;
    cout << "1. Add new city(ies)" << endl;
    cout << "2. Add roads between cities" << endl;
    cout << "3. Add the budget for roads" << endl;
    cout << "4. Edit city" << endl;
    cout << "5. Search for a city using its index" << endl;
    cout << "6. Display cities" << endl;
    cout << "7. Display roads" << endl;
    cout << "8. Display recorded data on console" << endl;
    cout << "9. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
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
                break;
            }
            
            case 2: {
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
                break;
            }
            
            case 3: {
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
                break;
            }
            
            case 4: {
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
                break;
            }
            
            case 5: {
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
                break;
            }
            
            case 6:
                infra.displayCities();
                break;
                
            case 7:
                infra.displayRoads();
                break;
                
            case 8:
                infra.displayAllData();
                break;
                
            case 9:
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
