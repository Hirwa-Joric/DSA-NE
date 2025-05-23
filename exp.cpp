#include <iostream>   // For input/output operations (like cout, cin)
#include <fstream>    // For file input/output operations (like ofstream to write to files)
#include <vector>     // For using the vector container (dynamic arrays)
#include <string>     // For using the string class
#include <iomanip>    // For formatting output (like setw, setprecision, fixed)
#include <algorithm>  // For algorithms like std::find (though not explicitly used here for vector find, it's a common include for vector operations)
#include <limits>     // For numeric_limits, used here to clear the input buffer

using namespace std;  // This line brings all the names from the std (standard) namespace
                      // into the current scope. This means you can write `cout` instead of
                      // `std::cout`. While convenient for small programs, in larger projects,
                      // it's often recommended to use `std::` prefix to avoid name collisions.

class City {
private: // Access specifier: these members can only be accessed from within the City class
    int index;    // Stores the unique numerical identifier for the city
    string name;  // Stores the name of the city

public:  // Access specifier: these members can be accessed from outside the City class
    // Constructor: Initializes a new City object
    // It takes an integer 'idx' and a string 'n' as arguments.
    // The ': index(idx), name(n)' part is an initializer list, which is an
    // efficient way to initialize member variables.
    City(int idx, string n) : index(idx), name(n) {}

    // Getter method for 'index': Returns the city's index
    // 'const' means this method doesn't modify the object's state.
    int getIndex() const { return index; }

    // Getter method for 'name': Returns the city's name
    string getName() const { return name; }

    // Setter method for 'name': Allows changing the city's name
    void setName(string n) { name = n; }
};

class InfrastructureManagement {
    private: // Private members, internal to the class's operation
        vector<City> cities;                  // A dynamic array (vector) to store all City objects
        vector<vector<int>> roadsMatrix;      // A 2D vector representing the adjacency matrix for roads.
                                              // roadsMatrix[i][j] = 1 if there's a road between city i and city j, 0 otherwise.
        vector<vector<double>> budgetMatrix;  // A 2D vector representing the adjacency matrix for budgets.
                                              // budgetMatrix[i][j] stores the budget for the road between city i and city j.
    
        // --- Helper functions (private) ---
        // These functions are used internally by other methods of this class.
    
        // Finds the 0-based index of a city in the 'cities' vector by its name.
        // Returns -1 if the city is not found.
        int findCityIndexByName(const string& cityName) {
            for (size_t i = 0; i < cities.size(); i++) { // Loop through each city in the vector
                if (cities[i].getName() == cityName) {   // Compare names
                    return i; // Return the 0-based index if found
                }
            }
            return -1; // City not found
        }
    
        // Finds the 0-based index of a city in the 'cities' vector by its 1-based public index.
        // Returns -1 if the city is not found.
        int findCityIndexByIndex(int index) {
            for (size_t i = 0; i < cities.size(); i++) { // Loop through each city
                if (cities[i].getIndex() == index) {     // Compare public indices
                    return i; // Return the 0-based vector index
                }
            }
            return -1; // City not found
        }
    
        // Resizes the 'roadsMatrix' and 'budgetMatrix' to match the current number of cities.
        // Initializes new entries to 0 or 0.0.
        void resizeMatrices() {
            size_t n = cities.size(); // Get the current number of cities
    
            // Resize roads matrix
            roadsMatrix.resize(n); // Adjust the number of rows
            for (size_t i = 0; i < n; i++) {
                roadsMatrix[i].resize(n, 0); // Adjust columns for each row, initialize new cells to 0
            }
    
            // Resize budget matrix
            budgetMatrix.resize(n); // Adjust the number of rows
            for (size_t i = 0; i < n; i++) {
                budgetMatrix[i].resize(n, 0.0); // Adjust columns, initialize new cells to 0.0
            }
        }
    
    public: // Public interface of the class
        // Constructor: Initializes an InfrastructureManagement object.
        // It's empty because the vectors are initialized automatically.
        InfrastructureManagement() {}
    
        // Adds a new city.
        void addCity(string name) {
            // Check if city already exists by name to ensure uniqueness.
            if (findCityIndexByName(name) != -1) {
                cout << "City '" << name << "' already exists!" << endl;
                return; // Exit the function if city exists
            }
    
            // Assign the next available 1-based index.
            // If 'cities' is empty, the first index is 1.
            // Otherwise, it's the last city's index + 1.
            int nextIndex = cities.empty() ? 1 : cities.back().getIndex() + 1;
            cities.push_back(City(nextIndex, name)); // Create a new City object and add it to the vector
    
            // Resize matrices to accommodate the new city.
            // This is crucial: if you add a city, the matrices need to be larger.
            resizeMatrices();
    
            cout << "City '" << name << "' added with index " << nextIndex << endl;
        }
    
        // Adds a road between two cities.
        void addRoad(string city1Name, string city2Name) {
            int idx1 = findCityIndexByName(city1Name); // Get 0-based index of the first city
            int idx2 = findCityIndexByName(city2Name); // Get 0-based index of the second city
    
            // Validations
            if (idx1 == -1) {
                cout << "City '" << city1Name << "' does not exist!" << endl;
                return;
            }
            if (idx2 == -1) {
                cout << "City '" << city2Name << "' does not exist!" << endl;
                return;
            }
            if (idx1 == idx2) {
                cout << "Cannot add a road between the same city!" << endl;
                return;
            }
    
            // Add road in both directions (undirected graph) in the roadsMatrix.
            // The indices idx1 and idx2 are 0-based, directly usable for vector<vector>
            roadsMatrix[idx1][idx2] = 1;
            roadsMatrix[idx2][idx1] = 1;
    
            cout << "Road added between " << city1Name << " and " << city2Name << endl;
        }
    
        // Adds a budget to an existing road.
        void addBudget(string city1Name, string city2Name, double budget) {
            int idx1 = findCityIndexByName(city1Name);
            int idx2 = findCityIndexByName(city2Name);
    
            // Validations
            if (idx1 == -1) {
                cout << "City '" << city1Name << "' does not exist!" << endl;
                return;
            }
            if (idx2 == -1) {
                cout << "City '" << city2Name << "' does not exist!" << endl;
                return;
            }
            // Crucial check: A road must exist before a budget can be added.
            if (roadsMatrix[idx1][idx2] != 1) { // Or check roadsMatrix[idx2][idx1]
                cout << "No road exists between " << city1Name << " and " << city2Name << "!" << endl;
                cout << "Please add a road first before assigning a budget." << endl;
                return;
            }
    
            // Add budget in both directions in the budgetMatrix.
            budgetMatrix[idx1][idx2] = budget;
            budgetMatrix[idx2][idx1] = budget;
    
            cout << "Budget added for the road between " << city1Name << " and " << city2Name << endl;
        }
    
        // Edits the name of an existing city.
        void editCity(int index, string newName) {
            int vectorIdx = findCityIndexByIndex(index); // Find the 0-based vector index using the city's public 1-based index
    
            if (vectorIdx == -1) {
                cout << "No city found with index " << index << endl;
                return;
            }
    
            // Check if the new name already exists for another city.
            int existingNameIdx = findCityIndexByName(newName);
            // If newName is found (existingNameIdx != -1) AND it's not the current city being edited (existingNameIdx != vectorIdx)
            if (existingNameIdx != -1 && existingNameIdx != vectorIdx) {
                cout << "City '" << newName << "' already exists for another city!" << endl;
                return;
            }
    
            cities[vectorIdx].setName(newName); // Update the name of the city in the vector
            cout << "City updated successfully" << endl;
        }
    
        // Searches for a city by its 1-based public index and displays it.
        void searchCity(int index) {
            int vectorIdx = findCityIndexByIndex(index);
    
            if (vectorIdx == -1) {
                cout << "No city found with index " << index << endl;
                return;
            }
    
            cout << "City found: " << cities[vectorIdx].getIndex() << ": " << cities[vectorIdx].getName() << endl;
        }
    
        // Displays the list of all cities.
        void displayCities() {
            cout << "\nCities" << endl;
            if (cities.empty()) {
                cout << "No cities recorded yet." << endl;
                return;
            }
            for (const auto& city : cities) { // Range-based for loop to iterate through cities
                cout << city.getIndex() << ": " << city.getName() << endl;
            }
        }
    
        // Displays the list of cities and then the roads adjacency matrix.
        void displayRoads() {
            displayCities(); // First, show the list of cities for reference
    
            cout << "\nRoads Adjacency Matrix:" << endl;
            if (cities.empty()) {
                cout << "No cities to display roads for." << endl;
                return;
            }
            // The matrix dimensions should match cities.size() due to resizeMatrices()
            for (size_t i = 0; i < cities.size(); i++) {
                for (size_t j = 0; j < cities.size(); j++) {
                    cout << roadsMatrix[i][j]; // Print 0 or 1
                    if (j < cities.size() - 1) cout << " "; // Add a space between numbers for readability
                }
                cout << endl; // Newline after each row
            }
        }
    
        // Displays all recorded data: cities, roads matrix, and budget matrix.
        void displayAllData() {
            displayCities(); // Show cities
    
            cout << "\nRoads Adjacency Matrix:" << endl;
            if (cities.empty()) {
                cout << "No cities to display roads for." << endl;
            } else {
                for (size_t i = 0; i < cities.size(); i++) {
                    for (size_t j = 0; j < cities.size(); j++) {
                        cout << roadsMatrix[i][j];
                        if (j < cities.size() - 1) cout << " ";
                    }
                    cout << endl;
                }
            }
    
            cout << "\nBudgets Adjacency Matrix:" << endl;
            if (cities.empty()) {
                cout << "No cities to display budgets for." << endl;
            } else {
                for (size_t i = 0; i < cities.size(); i++) {
                    for (size_t j = 0; j < cities.size(); j++) {
                        // setw(5) allocates 5 characters for the number.
                        // fixed and setprecision(1) ensure one digit after the decimal point.
                        cout << fixed << setprecision(1) << setw(5) << budgetMatrix[i][j] << " ";
                    }
                    cout << endl;
                }
            }
        }
    
        // Saves the list of cities to "cities.txt".
        void saveCitiesToFile() {
            ofstream file("cities.txt"); // Open file for writing (creates if not exists, overwrites if exists)
    
            if (!file.is_open()) { // Check if the file was opened successfully
                cout << "Error opening cities.txt for writing!" << endl;
                return;
            }
    
            file << "Index\tCity_name" << endl; // Write header
            for (const auto& city : cities) {
                file << city.getIndex() << "\t" << city.getName() << endl; // Write data, tab-separated
            }
    
            file.close(); // Close the file
            cout << "Cities saved to cities.txt" << endl;
        }
    
        // Saves the roads and their budgets to "roads.txt".
        void saveRoadsToFile() {
            ofstream file("roads.txt");
    
            if (!file.is_open()) {
                cout << "Error opening roads.txt for writing!" << endl;
                return;
            }
    
            file << "Nbr\tRoad\t\t\tBudget" << endl; // Write header
    
            int roadCount = 0;
            // Iterate through the upper triangle of the matrix (j = i + 1) to avoid duplicates
            // and self-loops for roads in an undirected graph.
            for (size_t i = 0; i < cities.size(); i++) {
                for (size_t j = i + 1; j < cities.size(); j++) {
                    if (roadsMatrix[i][j] == 1) { // If a road exists
                        roadCount++;
                        file << roadCount << ".\t"
                             << cities[i].getName() << "-" << cities[j].getName() // Road name format
                             << "\t\t" << fixed << setprecision(2) << budgetMatrix[i][j] << endl; // Budget, 2 decimal places
                    }
                }
            }
    
            file.close();
            cout << "Roads saved to roads.txt" << endl;
        }
    
        // Calls both save functions to save all data.
        void saveAllData() {
            saveCitiesToFile();
            saveRoadsToFile();
        }
    };