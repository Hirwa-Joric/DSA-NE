Okay, let's dive deep into this enhanced C++ code. I'll break it down section by section, explaining the purpose and functionality of each part, with special attention to the new additions for UI and data loading.

**1. Header Files and Namespace**

```cpp
#include <iostream>   // For standard input/output operations (cout, cin).
#include <fstream>    // For file stream operations (ifstream for reading, ofstream for writing files).
#include <vector>     // For using the std::vector dynamic array container.
#include <string>     // For using the std::string class for text manipulation.
#include <iomanip>    // For I/O manipulators like setw (set width), setprecision, fixed.
#include <algorithm>  // Provides a collection of functions for various purposes (e.g., sorting, searching), though not heavily used directly in simple forms here.
#include <limits>     // Used for std::numeric_limits, often to clear input buffers effectively.
#include <sstream>    // NEW: For std::stringstream, which allows string parsing and formatting as if it were a stream. Very useful for reading structured data from lines of text.
#include <cstdlib>    // NEW: For general utilities, specifically system() function used here for system("clear") to clear the console screen.

using namespace std;  // Brings all names from the standard (std) namespace into the current scope.
                      // This avoids having to write `std::cout`, `std::vector`, etc.
                      // While convenient, for very large projects, it's sometimes discouraged to prevent potential name collisions.
```

**2. UI Helper Functions**

These are new functions designed to make the console output more organized and visually appealing.

```cpp
// Helper function to print a horizontal line divider
void printDivider(char symbol = '=', int length = 50) {
    cout << string(length, symbol) << endl;
}
```
*   **Purpose**: Prints a line of repeated characters to visually separate sections of output.
*   **`char symbol = '='`**: The character to repeat. It defaults to `=` if no symbol is provided when calling the function.
*   **`int length = 50`**: The length of the line. It defaults to `50` if no length is provided.
*   **`string(length, symbol)`**: This is a C++ string constructor that creates a string by repeating `symbol` for `length` times.
*   **`endl`**: Inserts a newline character and flushes the output buffer.

```cpp
// Helper function to print a centered title
void printTitle(const string& title, char symbol = '=', int length = 50) {
    int padding = (length - title.length()) / 2;
    cout << string(padding, symbol) << " " << title << " " << string(padding - (title.length() % 2 == 0 ? 0 : 1), symbol) << endl;
}
```
*   **Purpose**: Prints a title string, centered within a line of specified characters.
*   **`const string& title`**: The title text to be printed (passed by constant reference for efficiency).
*   **`char symbol = '='`, `int length = 50`**: Similar to `printDivider`, for the surrounding characters and total line length.
*   **`int padding = (length - title.length()) / 2;`**: Calculates the amount of padding (repeated symbols) needed on each side of the title for centering.
*   **`string(padding, symbol)`**: Creates the left padding string.
*   **`title.length() % 2 == 0 ? 0 : 1`**: This is a ternary operator. If the title length is even, the padding on the right is the same as the left. If the title length is odd, one less padding character is used on the right to maintain better centering within the overall `length`.
*   **`string(padding - (adjustment), symbol)`**: Creates the right padding string.

**3. `City` Class**

This class remains the same as in the previous version and serves as a blueprint for city objects.

```cpp
class City {
private:
    int index;    // Stores the 1-based unique identifier for the city (as seen by the user).
    string name;  // Stores the name of the city.

public:
    // Constructor: Initializes a City object with an index and a name.
    City(int idx, string n) : index(idx), name(n) {}

    // Getter for index (read-only access).
    int getIndex() const { return index; }
    // Getter for name (read-only access).
    string getName() const { return name; }
    // Setter for name (allows modification of the city's name).
    void setName(string n) { name = n; }
};
```

**4. `InfrastructureManagement` Class**

This is the core class, and it has seen significant updates, especially regarding data loading and UI integration.

```cpp
class InfrastructureManagement {
private:
    vector<City> cities;                  // Stores all City objects.
    vector<vector<int>> roadsMatrix;      // Adjacency matrix for roads (1 if road exists, 0 otherwise).
    vector<vector<double>> budgetMatrix;  // Adjacency matrix for road budgets.
    bool dataLoaded;                      // NEW: Flag to track if data has been loaded from files.
                                          // This helps prevent redundant loading or issues if loading fails.
```

*   **`bool dataLoaded;`**: This new member variable is crucial. It's used to ensure that data from files is attempted to be loaded only once, typically when the `InfrastructureManagement` object is created.

```cpp
    // Helper functions (private - internal use only)

    // Finds the 0-based vector index of a city by its name. Case-sensitive.
    int findCityIndexByName(const string& cityName) {
        for (size_t i = 0; i < cities.size(); i++) {
            if (cities[i].getName() == cityName) {
                return i; // Return the 0-based index
            }
        }
        return -1; // Not found
    }

    // Finds the 0-based vector index of a city by its 1-based public index.
    int findCityIndexByIndex(int index) {
        for (size_t i = 0; i < cities.size(); i++) {
            if (cities[i].getIndex() == index) {
                return i; // Return the 0-based index
            }
        }
        return -1; // Not found
    }

    // Resizes adjacency matrices to match the current number of cities.
    // Initializes new elements to 0 or 0.0.
    void resizeMatrices() {
        size_t n = cities.size();
        roadsMatrix.resize(n);
        for (size_t i = 0; i < n; i++) {
            roadsMatrix[i].resize(n, 0);
        }
        budgetMatrix.resize(n);
        for (size_t i = 0; i < n; i++) {
            budgetMatrix[i].resize(n, 0.0);
        }
    }
```
These helper functions are largely the same but are essential for mapping user-facing city names/indices to internal vector indices used by the matrices.

```cpp
public:
    // Constructor
    InfrastructureManagement() : dataLoaded(false) {
        // Try to load data from files on initialization
        loadDataFromFiles(); // NEW: Call to load data when an object is created.
    }
```
*   **Constructor Update**:
    *   `dataLoaded(false)`: Initializes the `dataLoaded` flag to `false`.
    *   `loadDataFromFiles();`: This is a key change. Now, as soon as an `InfrastructureManagement` object is created (e.g., `InfrastructureManagement infra;` in `main`), it immediately attempts to load any existing data from "cities.txt" and "roads.txt".

The `addCity`, `addRoad`, `addBudget`, `editCity` methods remain functionally similar to the previous version in their core logic of modifying the `cities` vector and the adjacency matrices. The main difference in their usage context is that the data might already be partially populated from files.

```cpp
    // --- Display Methods ---
    // These methods now incorporate the UI helper functions for better presentation.

    void searchCity(int index) {
        int idx = findCityIndexByIndex(index); // 0-based index

        printDivider('=', 60);
        printTitle("SEARCH RESULTS", '=', 60);
        printDivider('-', 60);

        if (idx == -1) {
            cout << "No city found with index " << index << endl;
            printDivider('-', 60); // Use a lighter divider for content
            return;
        }

        cout << setw(10) << "INDEX" << setw(30) << "CITY NAME" << endl; // Column headers
        printDivider('-', 60);
        cout << setw(10) << cities[idx].getIndex() << setw(30) << cities[idx].getName() << endl; // Formatted output
        printDivider('=', 60);
    }

    void displayCities() {
        printDivider('=', 60);
        printTitle("CITIES LIST", '=', 60);
        printDivider('-', 60);

        cout << setw(10) << "INDEX" << setw(30) << "CITY NAME" << endl; // Column headers
        printDivider('-', 60);

        if (cities.empty()) {
            cout << "No cities recorded yet." << endl;
        } else {
            for (const auto& city : cities) {
                cout << setw(10) << city.getIndex() << setw(30) << city.getName() << endl; // Formatted output
            }
        }
        printDivider('=', 60);
    }

    void displayRoads() {
        displayCities(); // Show cities list first for context

        printDivider('=', 60);
        printTitle("ROADS ADJACENCY MATRIX", '=', 60);
        printDivider('-', 60);

        if (cities.empty()) {
            cout << "No cities to display roads for." << endl;
        } else {
            // Display column headers (city public indices)
            cout << setw(6) << " "; // Indent for row headers
            for (size_t j = 0; j < cities.size(); j++) {
                cout << setw(4) << cities[j].getIndex(); // Use city's actual index
            }
            cout << endl;
            printDivider('-', 60); // Separator line

            // Display matrix with row headers (city public indices)
            for (size_t i = 0; i < cities.size(); i++) {
                cout << setw(4) << cities[i].getIndex() << " |"; // Row header
                for (size_t j = 0; j < cities.size(); j++) {
                    cout << setw(4) << roadsMatrix[i][j];
                }
                cout << endl;
            }
        }
        printDivider('=', 60);
    }

    void displayAllData() {
        displayCities(); // Reuses the enhanced displayCities

        // Display Roads Adjacency Matrix (similar to displayRoads but integrated)
        printDivider('=', 60);
        printTitle("ROADS ADJACENCY MATRIX", '=', 60);
        printDivider('-', 60);
        if (cities.empty()) { /* ... */ } else { /* ... matrix display logic ... */ } // Same as displayRoads
        // ... (Code for displaying roads matrix as in displayRoads) ...

        // Display Budgets Adjacency Matrix
        printDivider('=', 60); // Use a more prominent divider
        printTitle("BUDGETS ADJACENCY MATRIX (Billion RWF)", '=', 60);
        printDivider('-', 60);

        if (cities.empty()) {
            cout << "No cities to display budgets for." << endl;
        } else {
            // Display column headers (city public indices)
            cout << setw(6) << " ";
            for (size_t j = 0; j < cities.size(); j++) {
                cout << setw(7) << cities[j].getIndex(); // Wider for budget values
            }
            cout << endl;
            printDivider('-', 60);

            // Display matrix with row headers (city public indices)
            for (size_t i = 0; i < cities.size(); i++) {
                cout << setw(4) << cities[i].getIndex() << " |";
                for (size_t j = 0; j < cities.size(); j++) {
                    cout << fixed << setprecision(1) << setw(7) << budgetMatrix[i][j]; // Formatted budget
                }
                cout << endl;
            }
        }
        printDivider('=', 60);
    }
```
*   **UI Enhancements in Display Methods**:
    *   `printDivider` and `printTitle` are used extensively to structure the output.
    *   `setw()` is used to align data in columns, making tables more readable.
    *   Matrices now display city *indices* (the 1-based public ones) as row and column headers, which is much more user-friendly than just raw matrix positions.
    *   `fixed` and `setprecision(1)` (or `setprecision(2)` in file saving) are used for consistent formatting of budget values.

```cpp
    // --- File Saving Methods ---
    // These remain largely the same, responsible for writing current data to files.
    void saveCitiesToFile() { /* ... as before ... */ }
    void saveRoadsToFile() { /* ... as before ... */ }
    void saveAllData() { /* ... calls saveCitiesToFile and saveRoadsToFile ... */ }
```

```cpp
    // --- NEW File Loading Methods ---

    void loadCitiesFromFile() {
        ifstream file("cities.txt"); // Open "cities.txt" for reading.

        if (!file.is_open()) {
            cout << "No previous cities data found (cities.txt not found or unreadable)." << endl;
            return; // Exit if file can't be opened.
        }

        string line;
        getline(file, line); // Read and discard the header line ("Index City_name").

        cities.clear(); // IMPORTANT: Clear any existing cities in memory before loading from file
                        // to avoid duplicates if this function were called multiple times (though it's usually called once).

        while (getline(file, line)) { // Read file line by line.
            stringstream ss(line); // Create a stringstream object from the current line.
                                   // Stringstream allows you to treat a string like an input stream (cin).
            int indexFromFile;
            string name;
            char tab_char; // To consume the tab character

            // Try to parse the line: index <tab> name
            if (ss >> indexFromFile && ss.get(tab_char) && tab_char == '\t' && getline(ss, name)) {
                // Trim leading/trailing whitespace from name (good practice)
                name.erase(0, name.find_first_not_of(" \t\n\r\f\v"));
                name.erase(name.find_last_not_of(" \t\n\r\f\v") + 1);
                if (!name.empty()) {
                     cities.push_back(City(indexFromFile, name)); // Add the loaded city.
                }
            } else {
                cerr << "Warning: Malformed line in cities.txt: " << line << endl;
            }
        }
        file.close(); // Close the file.

        if (!cities.empty()) {
            resizeMatrices(); // CRUCIAL: After loading cities, resize adjacency matrices
                              // to the correct dimensions based on the number of loaded cities.
            cout << cities.size() << " cities loaded successfully from cities.txt" << endl;
        }
    }

    void loadRoadsFromFile() {
        if (cities.empty()) { // Cannot load roads if no cities are loaded (needed for name-to-index mapping)
            // cout << "Cannot load roads: No cities loaded yet." << endl; // Optional debug message
            return;
        }

        ifstream file("roads.txt");
        if (!file.is_open()) {
            cout << "No previous roads data found (roads.txt not found or unreadable)." << endl;
            return;
        }

        string line;
        getline(file, line); // Read and discard the header line.

        // Matrices should already be sized correctly by resizeMatrices() in loadCitiesFromFile
        // or if cities were added manually before a hypothetical separate load.

        while (getline(file, line)) {
            stringstream ss(line);
            int roadNum_dummy; // "Nbr." column, not directly used for logic
            char dot_dummy;
            string roadInfoCombined; // "City1-City2" part
            string city1Name, city2Name;
            double budget;

            // Attempt to parse: Nbr. <tab> City1-City2 <tab(s)> Budget
            // Example line: "1. Kigali-Muhanga 28.6" (tab might be between Muhanga and 28.6)
            if (ss >> roadNum_dummy && ss.get(dot_dummy) && dot_dummy == '.') {
                // Successfully read "Nbr."
                // Now, get the roadInfoCombined and budget
                // This parsing is a bit tricky because of variable tabs
                string temp;
                vector<string> parts;
                // Read the rest of the line, then split by tab. This is more robust to multiple tabs.
                getline(ss, temp);
                stringstream lineParser(temp);
                string segment;
                while(getline(lineParser, segment, '\t')){
                    // Remove empty segments that might result from multiple tabs
                    if (!segment.empty()){
                        parts.push_back(segment);
                    }
                }

                if (parts.size() >= 2) { // Expect at least roadInfo and budget
                    roadInfoCombined = parts[0];
                    try {
                        budget = stod(parts.back()); // Budget is the last part
                        // If there are more than 2 parts, middle parts are ignored if file has extra tabs.
                    } catch (const std::invalid_argument& ia) {
                        cerr << "Warning: Invalid budget format in roads.txt: " << line << endl;
                        continue;
                    } catch (const std::out_of_range& oor) {
                        cerr << "Warning: Budget value out of range in roads.txt: " << line << endl;
                        continue;
                    }

                    size_t hyphenPos = roadInfoCombined.find('-');
                    if (hyphenPos != string::npos && hyphenPos > 0 && hyphenPos < roadInfoCombined.length() - 1) {
                        city1Name = roadInfoCombined.substr(0, hyphenPos);
                        city2Name = roadInfoCombined.substr(hyphenPos + 1);

                        // Trim whitespace from parsed city names
                        city1Name.erase(0, city1Name.find_first_not_of(" \t")); city1Name.erase(city1Name.find_last_not_of(" \t") + 1);
                        city2Name.erase(0, city2Name.find_first_not_of(" \t")); city2Name.erase(city2Name.find_last_not_of(" \t") + 1);


                        int idx1 = findCityIndexByName(city1Name);
                        int idx2 = findCityIndexByName(city2Name);

                        if (idx1 != -1 && idx2 != -1 && idx1 != idx2) {
                            // Ensure indices are within matrix bounds (should be true if resizeMatrices was called)
                            if (static_cast<size_t>(idx1) < roadsMatrix.size() && static_cast<size_t>(idx2) < roadsMatrix.size()) {
                                roadsMatrix[idx1][idx2] = 1;
                                roadsMatrix[idx2][idx1] = 1;
                                budgetMatrix[idx1][idx2] = budget;
                                budgetMatrix[idx2][idx1] = budget;
                            }
                        } else {
                            cerr << "Warning: Cities for road '" << roadInfoCombined << "' not found in loaded city list. Skipping road." << endl;
                        }
                    } else {
                         cerr << "Warning: Malformed road format in roads.txt: " << roadInfoCombined << endl;
                    }
                } else {
                    cerr << "Warning: Malformed line structure in roads.txt: " << line << endl;
                }
            } else {
                // cerr << "Warning: Skipping malformed line start in roads.txt: " << line << endl;
            }
        }
        file.close();
        cout << "Roads and budgets (if any) processed from roads.txt" << endl;
    }

    // Orchestrates the loading of all data.
    void loadDataFromFiles() {
        if (dataLoaded) { // Prevent re-loading if already done
            cout << "Data has already been loaded." << endl;
            return;
        }
        cout << "Attempting to load data from files..." << endl;
        loadCitiesFromFile(); // Load cities first, as roads depend on them for indexing.
        if (!cities.empty()) { // Only attempt to load roads if cities were successfully loaded/exist
            loadRoadsFromFile();
        }
        dataLoaded = true; // Set the flag indicating data loading has been attempted.
        cout << "Data loading process complete." << endl;
        printDivider();
    }
```
*   **`loadCitiesFromFile()`**:
    *   Opens "cities.txt".
    *   Skips the header.
    *   Clears the existing `cities` vector in memory.
    *   Reads each line, uses `stringstream` to parse the `index` and `name` (separated by a tab).
    *   Adds new `City` objects to the `cities` vector.
    *   **Crucially, calls `resizeMatrices()` after loading all cities** to ensure the adjacency matrices are correctly sized for the loaded data.
*   **`loadRoadsFromFile()`**:
    *   Checks if cities are loaded; if not, it cannot proceed (as it needs city names to find indices).
    *   Opens "roads.txt".
    *   Skips the header.
    *   Reads each line, using `stringstream` for parsing. This part is more complex due to the "City1-City2" format and potential variable spacing/tabs.
        *   It extracts the "Nbr." (dummy).
        *   It tries to extract the `roadInfoCombined` string (e.g., "Kigali-Muhanga").
        *   It extracts the `budget`.
        *   It splits `roadInfoCombined` by the hyphen '-' to get `city1Name` and `city2Name`.
        *   It finds the 0-based indices (`idx1`, `idx2`) for these city names using `findCityIndexByName`.
        *   If both cities are found, it updates `roadsMatrix[idx1][idx2]` and `budgetMatrix[idx1][idx2]` (and their symmetric counterparts).
*   **`loadDataFromFiles()`**:
    *   This public method is called by the constructor.
    *   It checks the `dataLoaded` flag to prevent multiple load attempts.
    *   It calls `loadCitiesFromFile()` and then `loadRoadsFromFile()`.
    *   Sets `dataLoaded = true;` after the process.

**5. `displayMenu()` Function**

```cpp
void displayMenu() {
    // system("clear"); // Clears the console (Linux/macOS). For Windows, it's system("cls").
                        // This is platform-dependent.
    // The code you provided has "clear", so it's targeted for Linux/macOS.
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif


    printDivider('=', 60);
    printTitle("RWANDA INFRASTRUCTURE MANAGEMENT SYSTEM", '=', 60);
    printDivider('=', 60);

    cout << "\n" << setw(35) << "MAIN MENU" << "\n" << endl; // Centered "MAIN MENU"
    printDivider('-', 60);

    cout << "  1. Add new city(ies)" << endl;
    // ... other menu items ...
    cout << "  9. Exit" << endl;

    printDivider('-', 60);
    cout << "Enter your choice: ";
}
```
*   **`system("clear")` / `system("cls")`**: This command clears the terminal screen before displaying the menu, providing a cleaner interface. The `#ifdef _WIN32` is a preprocessor directive for cross-platform compatibility (clears differently on Windows vs. Linux/macOS).
*   Uses `printDivider` and `printTitle` to make the menu look more professional.
*   `setw(35)` is used to roughly center "MAIN MENU".

**6. `main()` Function**

This is where the program execution begins and the main application loop runs.

```cpp
int main() {
    // Display welcome message
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    printDivider('*', 70);
    printTitle("WELCOME TO RWANDA INFRASTRUCTURE MANAGEMENT SYSTEM", '*', 70);
    printTitle("Ministry of Infrastructure", '-', 70);
    printDivider('*', 70);
    cout << "\nThis system helps manage cities, roads, and budgets for Rwanda's infrastructure." << endl;
    cout << "Press Enter to continue..." << endl;
    cin.get(); // Waits for the user to press Enter.

    InfrastructureManagement infra; // Create an instance. THIS TRIGGERS DATA LOADING.
    int choice;

    do {
        displayMenu(); // Display the enhanced menu.

        // Improved input validation for menu choice
        if (!(cin >> choice)) { // If input is not a valid integer (e.g., user types 'abc')
            cout << "Invalid input. Please enter a number (1-9)." << endl;
            cin.clear(); // Clear error flags on cin.
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the bad input.
            cout << "\nPress Enter to continue..." << endl;
            cin.get(); // Pause
            continue; // Skip the rest of the loop and re-display menu.
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear newline after valid integer input.

        // Switch statement to handle user's choice
        switch (choice) {
            case 1: {
                // UI enhancements for this option
                printDivider('=', 60);
                printTitle("ADD NEW CITIES", '=', 60);
                printDivider('-', 60);
                int numCities;
                cout << "Enter the number of cities to add: ";
                if (!(cin >> numCities) || numCities <= 0) { // Validate input for numCities
                    cout << "Invalid input. Please enter a positive number." << endl;
                    if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');}
                    else { cin.ignore(numeric_limits<streamsize>::max(), '\n');}
                    // ... (pause and continue logic)
                    break;
                }
                // ... (rest of add city logic) ...
                infra.saveAllData(); // Data saved after operation.
                cout << "\nPress Enter to continue..." << endl;
                cin.get(); // Pause
                break;
            }
            // Other cases (2, 3, 4, 5, 6, 7, 8) follow a similar pattern:
            // 1. Print option-specific title using UI helpers.
            // 2. Get necessary input with improved validation (checking for empty strings, valid numbers).
            // 3. Call the appropriate `infra` method.
            // 4. Call `infra.saveAllData()` if data was modified.
            // 5. Pause with "Press Enter to continue...".

            case 9: {
                printDivider('=', 60);
                printTitle("EXITING PROGRAM", '=', 60);
                printDivider('-', 60);
                cout << "Saving data before exiting..." << endl;
                infra.saveAllData(); // Crucial: Save all data before terminating.
                cout << "Exiting program. Goodbye!" << endl;
                printDivider('=', 60);
                break;
            }
            default:
                cout << "Invalid choice. Please try again." << endl;
                cout << "\nPress Enter to continue..." << endl;
                cin.get(); // Pause
        }
    } while (choice != 9);

    return 0; // Indicate successful program termination.
}
```
*   **Welcome Message**: A new, more engaging welcome screen is displayed at the start.
*   **`InfrastructureManagement infra;`**: When this object is created, its constructor is called, which in turn calls `loadDataFromFiles()`. So, data loading happens right at the beginning.
*   **Robust Menu Input Validation**: The `if (!(cin >> choice))` block is a good improvement. It checks if the extraction of an integer from `cin` failed (e.g., user typed letters). If so, it clears `cin`'s error state, discards the bad input, and prompts the user again.
*   **Input Validation within Cases**: Many cases now have more specific input validation:
    *   Checking if the number of cities is positive.
    *   Checking if city names are empty.
    *   Checking if budget/index is a positive number.
*   **Pausing**: `cout << "\nPress Enter to continue..." << endl; cin.get();` is used after most operations to pause the screen, allowing the user to read messages before the menu is redisplayed. This significantly improves user experience.
*   **Frequent Saving**: `infra.saveAllData()` is still called after most data-modifying operations. This ensures data integrity but can be I/O intensive. For very large datasets or frequent operations, a less frequent save strategy (e.g., explicit save option, save only on exit) might be considered.
*   **Clearer Exit**: The exit case also has a more formatted message.

**In Summary of Changes and Improvements:**

1.  **Automatic Data Loading**: Data is now attempted to be loaded from `cities.txt` and `roads.txt` when the program starts.
2.  **Enhanced Console UI**:
    *   Helper functions (`printDivider`, `printTitle`) create a more structured and visually appealing interface.
    *   `system("clear")` / `system("cls")` clears the screen for better readability.
    *   Formatted output using `setw` for tables (cities list, adjacency matrices).
    *   Adjacency matrices display city indices as headers.
    *   "Press Enter to continue" pauses improve usability.
3.  **Improved Input Validation**:
    *   Better handling of non-numeric input for menu choices.
    *   More checks for valid inputs within specific operations (e.g., positive numbers, non-empty strings).
4.  **Code Structure**:
    *   `sstream` is used for more robust parsing of file lines.
    *   The `dataLoaded` flag helps manage the loading process.

This version of the code is significantly more user-friendly and robust due to these enhancements. The automatic data loading makes it persistent across sessions, and the UI improvements make it easier to interact with.