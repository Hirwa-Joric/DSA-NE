#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <set>
#include <algorithm>
#include <cctype>
#include <stack>
#include <cmath>
#include <unordered_map>

using namespace std;

const int MAX = 1000; // Maximum number of health centers

// Health Center structure
struct HealthCenter {
    int id;
    string name;
    string district;
    float lat;
    float lon;
    int capacity;
};

// Connection structure
struct Connection {
    int from;  // Source health center ID
    int to;    // Destination health center ID
    float distance;  // Distance in kilometers
    int time;  // Travel time in minutes
    string description;
};

// Global variables
vector<HealthCenter> centers;
vector<Connection> adjList[MAX];

// Function prototypes for file I/O
bool readHealthCenters();
bool readConnections();
bool saveHealthCenters();
bool saveConnections();

// Function prototypes for CRUD operations - Health Centers
void addHealthCenter();
void editHealthCenter();
void viewHealthCenters();
void removeHealthCenter();

// Function prototypes for CRUD operations - Connections
void addConnection();
void editConnection();
void viewConnections();
void removeConnection();

// Function prototypes for graph algorithms
void dijkstra();
void bfs();
bool detectCycle();
void floydWarshall();
void primMST();
void emergencyRouting();

// Function prototype for additional features
void viewRelationships();

// Helper functions
bool isHealthCenterExists(int id);
int getHealthCenterIndex(int id);
void clearInputBuffer();
bool isInteger(const string& str);
bool isFloat(const string& str);
string trim(const string& str);

// File I/O Functions Implementation
bool readHealthCenters() {
    ifstream file("health_centers.csv");
    
    // If file doesn't exist, create it with headers
    if (!file) {
        ofstream newFile("health_centers.csv");
        if (!newFile) {
            cout << "Error: Could not create health_centers.csv" << endl;
            return false;
        }
        newFile << "ID,Name,District,Latitude,Longitude,Capacity" << endl;
        newFile.close();
        return true; // File created with headers only
    }
    
    string line;
    getline(file, line); // Skip header line
    
    centers.clear(); // Clear existing data
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string token;
        HealthCenter center;
        
        try {
            // Parse ID
            getline(ss, token, ',');
            center.id = stoi(token);
            
            // Parse Name
            getline(ss, token, ',');
            center.name = trim(token);
            
            // Parse District
            getline(ss, token, ',');
            center.district = trim(token);
            
            // Parse Latitude
            getline(ss, token, ',');
            center.lat = stof(token);
            
            // Parse Longitude
            getline(ss, token, ',');
            center.lon = stof(token);
            
            // Parse Capacity
            getline(ss, token, ',');
            center.capacity = stoi(token);
            
            centers.push_back(center);
        } catch (const exception& e) {
            cout << "Error parsing line: " << line << endl;
            cout << "Error message: " << e.what() << endl;
            continue; // Skip this line and continue with the next
        }
    }
    
    file.close();
    return true;
}

bool readConnections() {
    ifstream file("connections.csv");
    
    // If file doesn't exist, create it with headers
    if (!file) {
        ofstream newFile("connections.csv");
        if (!newFile) {
            cout << "Error: Could not create connections.csv" << endl;
            return false;
        }
        newFile << "FromID,ToID,DistanceKM,TimeMinutes,Description" << endl;
        newFile.close();
        return true; // File created with headers only
    }
    
    string line;
    getline(file, line); // Skip header line
    
    // Clear existing adjacency list
    for (int i = 0; i < MAX; i++) {
        adjList[i].clear();
    }
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string token;
        Connection conn;
        int fromId;
        
        try {
            // Parse FromID
            getline(ss, token, ',');
            fromId = stoi(token);
            conn.from = fromId;
            
            // Parse ToID
            getline(ss, token, ',');
            conn.to = stoi(token);
            
            // Parse Distance
            getline(ss, token, ',');
            conn.distance = stof(token);
            
            // Parse Time
            getline(ss, token, ',');
            conn.time = stoi(token);
            
            // Parse Description
            getline(ss, token);
            conn.description = trim(token);
            
            // Add to adjacency list if both health centers exist
            if (isHealthCenterExists(fromId) && isHealthCenterExists(conn.to)) {
                adjList[fromId].push_back(conn);
            } else {
                cout << "Warning: Connection references non-existent health center(s): " 
                     << fromId << " -> " << conn.to << endl;
            }
        } catch (const exception& e) {
            cout << "Error parsing line: " << line << endl;
            cout << "Error message: " << e.what() << endl;
            continue; // Skip this line and continue with the next
        }
    }
    
    file.close();
    return true;
}

bool saveHealthCenters() {
    ofstream file("health_centers.csv");
    if (!file) {
        cout << "Error: Could not open health_centers.csv for writing" << endl;
        return false;
    }
    
    // Write header
    file << "ID,Name,District,Latitude,Longitude,Capacity" << endl;
    
    // Write data
    for (const auto& center : centers) {
        file << center.id << ","
             << center.name << ","
             << center.district << ","
             << fixed << setprecision(4) << center.lat << ","
             << fixed << setprecision(4) << center.lon << ","
             << center.capacity << endl;
    }
    
    file.close();
    return true;
}

bool saveConnections() {
    ofstream file("connections.csv");
    if (!file) {
        cout << "Error: Could not open connections.csv for writing" << endl;
        return false;
    }
    
    // Write header
    file << "FromID,ToID,DistanceKM,TimeMinutes,Description" << endl;
    
    // Write data
    for (int i = 0; i < MAX; i++) {
        for (const auto& conn : adjList[i]) {
            file << conn.from << ","
                 << conn.to << ","
                 << fixed << setprecision(2) << conn.distance << ","
                 << conn.time << ","
                 << conn.description << endl;
        }
    }
    
    file.close();
    return true;
}

// Helper functions implementation
bool isHealthCenterExists(int id) {
    for (const auto& center : centers) {
        if (center.id == id) {
            return true;
        }
    }
    return false;
}

int getHealthCenterIndex(int id) {
    for (size_t i = 0; i < centers.size(); i++) {
        if (centers[i].id == id) {
            return i;
        }
    }
    return -1; // Not found
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool isInteger(const string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool isFloat(const string& str) {
    if (str.empty()) return false;
    
    bool hasDecimal = false;
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '.') {
            if (hasDecimal) return false; // More than one decimal point
            hasDecimal = true;
        } else if (!isdigit(str[i]) && !(i == 0 && str[i] == '-')) {
            return false;
        }
    }
    return true;
}

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

// CRUD Operations for Health Centers
void addHealthCenter() {
    HealthCenter newCenter;
    string input;
    
    cout << "\n=== Add New Health Center ===\n";
    
    // Get ID
    while (true) {
        cout << "Enter ID (integer): ";
        getline(cin, input);
        
        if (!isInteger(input)) {
            cout << "Error: ID must be an integer.\n";
            continue;
        }
        
        newCenter.id = stoi(input);
        
        if (isHealthCenterExists(newCenter.id)) {
            cout << "Error: A health center with ID " << newCenter.id << " already exists.\n";
            continue;
        }
        
        break;
    }
    
    // Get Name
    while (true) {
        cout << "Enter Name: ";
        getline(cin, input);
        
        if (input.empty()) {
            cout << "Error: Name cannot be empty.\n";
            continue;
        }
        
        newCenter.name = input;
        break;
    }
    
    // Get District
    while (true) {
        cout << "Enter District: ";
        getline(cin, input);
        
        if (input.empty()) {
            cout << "Error: District cannot be empty.\n";
            continue;
        }
        
        newCenter.district = input;
        break;
    }
    
    // Get Latitude
    while (true) {
        cout << "Enter Latitude (-90 to 90): ";
        getline(cin, input);
        
        if (!isFloat(input)) {
            cout << "Error: Latitude must be a number.\n";
            continue;
        }
        
        newCenter.lat = stof(input);
        
        if (newCenter.lat < -90 || newCenter.lat > 90) {
            cout << "Error: Latitude must be between -90 and 90.\n";
            continue;
        }
        
        break;
    }
    
    // Get Longitude
    while (true) {
        cout << "Enter Longitude (-180 to 180): ";
        getline(cin, input);
        
        if (!isFloat(input)) {
            cout << "Error: Longitude must be a number.\n";
            continue;
        }
        
        newCenter.lon = stof(input);
        
        if (newCenter.lon < -180 || newCenter.lon > 180) {
            cout << "Error: Longitude must be between -180 and 180.\n";
            continue;
        }
        
        break;
    }
    
    // Get Capacity
    while (true) {
        cout << "Enter Capacity (integer): ";
        getline(cin, input);
        
        if (!isInteger(input)) {
            cout << "Error: Capacity must be an integer.\n";
            continue;
        }
        
        newCenter.capacity = stoi(input);
        
        if (newCenter.capacity <= 0) {
            cout << "Error: Capacity must be greater than 0.\n";
            continue;
        }
        
        break;
    }
    
    // Add to centers vector
    centers.push_back(newCenter);
    
    // Save changes to file
    if (saveHealthCenters()) {
        cout << "Health center added successfully.\n";
    } else {
        cout << "Error: Failed to save health center.\n";
    }
}

void editHealthCenter() {
    string input;
    int id;
    
    cout << "\n=== Edit Health Center ===\n";
    
    // Get ID of health center to edit
    while (true) {
        cout << "Enter ID of health center to edit: ";
        getline(cin, input);
        
        if (!isInteger(input)) {
            cout << "Error: ID must be an integer.\n";
            continue;
        }
        
        id = stoi(input);
        
        if (!isHealthCenterExists(id)) {
            cout << "Error: No health center with ID " << id << " exists.\n";
            continue;
        }
        
        break;
    }
    
    int index = getHealthCenterIndex(id);
    HealthCenter& center = centers[index];
    
    // Display current information
    cout << "\nCurrent Information:\n";
    cout << "ID: " << center.id << endl;
    cout << "Name: " << center.name << endl;
    cout << "District: " << center.district << endl;
    cout << "Latitude: " << center.lat << endl;
    cout << "Longitude: " << center.lon << endl;
    cout << "Capacity: " << center.capacity << endl;
    
    cout << "\nEnter new information (leave blank to keep current value):\n";
    
    // Get Name
    cout << "Enter Name: ";
    getline(cin, input);
    if (!input.empty()) {
        center.name = input;
    }
    
    // Get District
    cout << "Enter District: ";
    getline(cin, input);
    if (!input.empty()) {
        center.district = input;
    }
    
    // Get Latitude
    while (true) {
        cout << "Enter Latitude (-90 to 90): ";
        getline(cin, input);
        
        if (input.empty()) {
            break; // Keep current value
        }
        
        if (!isFloat(input)) {
            cout << "Error: Latitude must be a number.\n";
            continue;
        }
        
        float lat = stof(input);
        
        if (lat < -90 || lat > 90) {
            cout << "Error: Latitude must be between -90 and 90.\n";
            continue;
        }
        
        center.lat = lat;
        break;
    }
    
    // Get Longitude
    while (true) {
        cout << "Enter Longitude (-180 to 180): ";
        getline(cin, input);
        
        if (input.empty()) {
            break; // Keep current value
        }
        
        if (!isFloat(input)) {
            cout << "Error: Longitude must be a number.\n";
            continue;
        }
        
        float lon = stof(input);
        
        if (lon < -180 || lon > 180) {
            cout << "Error: Longitude must be between -180 and 180.\n";
            continue;
        }
        
        center.lon = lon;
        break;
    }
    
    // Get Capacity
    while (true) {
        cout << "Enter Capacity (integer): ";
        getline(cin, input);
        
        if (input.empty()) {
            break; // Keep current value
        }
        
        if (!isInteger(input)) {
            cout << "Error: Capacity must be an integer.\n";
            continue;
        }
        
        int capacity = stoi(input);
        
        if (capacity <= 0) {
            cout << "Error: Capacity must be greater than 0.\n";
            continue;
        }
        
        center.capacity = capacity;
        break;
    }
    
    // Save changes to file
    if (saveHealthCenters()) {
        cout << "Health center updated successfully.\n";
    } else {
        cout << "Error: Failed to save changes.\n";
    }
}

void viewHealthCenters() {
    cout << "\n=== Health Centers ===\n";
    
    if (centers.empty()) {
        cout << "No health centers found.\n";
        return;
    }
    
    // Print header
    cout << left << setw(5) << "ID" << " | "
         << setw(25) << "Name" << " | "
         << setw(15) << "District" << " | "
         << setw(10) << "Latitude" << " | "
         << setw(10) << "Longitude" << " | "
         << setw(10) << "Capacity" << endl;
    
    cout << string(85, '-') << endl;
    
    // Print data
    for (const auto& center : centers) {
        cout << left << setw(5) << center.id << " | "
             << setw(25) << center.name << " | "
             << setw(15) << center.district << " | "
             << fixed << setprecision(4) << setw(10) << center.lat << " | "
             << fixed << setprecision(4) << setw(10) << center.lon << " | "
             << setw(10) << center.capacity << endl;
    }
}

void removeHealthCenter() {
    string input;
    int id;
    
    cout << "\n=== Remove Health Center ===\n";
    
    // Get ID of health center to remove
    while (true) {
        cout << "Enter ID of health center to remove: ";
        getline(cin, input);
        
        if (!isInteger(input)) {
            cout << "Error: ID must be an integer.\n";
            continue;
        }
        
        id = stoi(input);
        
        if (!isHealthCenterExists(id)) {
            cout << "Error: No health center with ID " << id << " exists.\n";
            continue;
        }
        
        break;
    }
    
    // Confirm deletion
    cout << "Are you sure you want to remove this health center? (y/n): ";
    getline(cin, input);
    
    if (input != "y" && input != "Y") {
        cout << "Operation canceled.\n";
        return;
    }
    
    // Remove health center from centers vector
    int index = getHealthCenterIndex(id);
    centers.erase(centers.begin() + index);
    
    // Remove all connections involving this health center
    adjList[id].clear(); // Remove outgoing connections
    
    // Remove incoming connections
    for (int i = 0; i < MAX; i++) {
        auto& connections = adjList[i];
        connections.erase(
            remove_if(connections.begin(), connections.end(),
                [id](const Connection& conn) { return conn.to == id; }),
            connections.end());
    }
    
    // Save changes to files
    bool saveSuccess = saveHealthCenters() && saveConnections();
    
    if (saveSuccess) {
        cout << "Health center and its connections removed successfully.\n";
    } else {
        cout << "Error: Failed to save changes.\n";
    }
}

// CRUD Operations for Connections
void addConnection() {
    string input;
    int fromId, toId;
    Connection newConn;
    
    cout << "\n=== Add New Connection ===\n";
    
    // Get FromID
    while (true) {
        cout << "Enter source health center ID: ";
        getline(cin, input);
        
        if (!isInteger(input)) {
            cout << "Error: ID must be an integer.\n";
            continue;
        }
        
        fromId = stoi(input);
        
        if (!isHealthCenterExists(fromId)) {
            cout << "Error: No health center with ID " << fromId << " exists.\n";
            continue;
        }
        
        break;
    }
    
    // Get ToID
    while (true) {
        cout << "Enter destination health center ID: ";
        getline(cin, input);
        
        if (!isInteger(input)) {
            cout << "Error: ID must be an integer.\n";
            continue;
        }
        
        toId = stoi(input);
        
        if (!isHealthCenterExists(toId)) {
            cout << "Error: No health center with ID " << toId << " exists.\n";
            continue;
        }
        
        if (fromId == toId) {
            cout << "Error: Source and destination cannot be the same.\n";
            continue;
        }
        
        // Check if connection already exists
        bool connectionExists = false;
        for (const auto& conn : adjList[fromId]) {
            if (conn.to == toId) {
                connectionExists = true;
                break;
            }
        }
        
        if (connectionExists) {
            cout << "Error: A connection from " << fromId << " to " << toId << " already exists.\n";
            continue;
        }
        
        break;
    }
    
    newConn.from = fromId;
    newConn.to = toId;
    
    // Get Distance
    while (true) {
        cout << "Enter distance in kilometers: ";
        getline(cin, input);
        
        if (!isFloat(input)) {
            cout << "Error: Distance must be a number.\n";
            continue;
        }
        
        newConn.distance = stof(input);
        
        if (newConn.distance <= 0) {
            cout << "Error: Distance must be greater than 0.\n";
            continue;
        }
        
        break;
    }
    
    // Get Time
    while (true) {
        cout << "Enter travel time in minutes: ";
        getline(cin, input);
        
        if (!isInteger(input)) {
            cout << "Error: Time must be an integer.\n";
            continue;
        }
        
        newConn.time = stoi(input);
        
        if (newConn.time <= 0) {
            cout << "Error: Time must be greater than 0.\n";
            continue;
        }
        
        break;
    }
    
    // Get Description
    cout << "Enter description: ";
    getline(cin, newConn.description);
    
    // Add to adjacency list
    adjList[fromId].push_back(newConn);
    
    // Save changes to file
    if (saveConnections()) {
        cout << "Connection added successfully.\n";
    } else {
        cout << "Error: Failed to save connection.\n";
    }
}

void editConnection() {
    string input;
    int fromId, toId;
    
    cout << "\n=== Edit Connection ===\n";
    
    // Get FromID
    while (true) {
        cout << "Enter source health center ID: ";
        getline(cin, input);
        
        if (!isInteger(input)) {
            cout << "Error: ID must be an integer.\n";
            continue;
        }
        
        fromId = stoi(input);
        
        if (!isHealthCenterExists(fromId)) {
            cout << "Error: No health center with ID " << fromId << " exists.\n";
            continue;
        }
        
        break;
    }
    
    // Get ToID
    while (true) {
        cout << "Enter destination health center ID: ";
        getline(cin, input);
        
        if (!isInteger(input)) {
            cout << "Error: ID must be an integer.\n";
            continue;
        }
        
        toId = stoi(input);
        
        if (!isHealthCenterExists(toId)) {
            cout << "Error: No health center with ID " << toId << " exists.\n";
            continue;
        }
        
        // Check if connection exists
        bool connectionExists = false;
        for (const auto& conn : adjList[fromId]) {
            if (conn.to == toId) {
                connectionExists = true;
                break;
            }
        }
        
        if (!connectionExists) {
            cout << "Error: No connection from " << fromId << " to " << toId << " exists.\n";
            continue;
        }
        
        break;
    }
    
    // Find the connection to edit
    Connection* connToEdit = nullptr;
    for (auto& conn : adjList[fromId]) {
        if (conn.to == toId) {
            connToEdit = &conn;
            break;
        }
    }
    
    if (!connToEdit) {
        cout << "Error: Connection not found.\n";
        return;
    }
    
    // Display current information
    cout << "\nCurrent Information:\n";
    cout << "From: " << connToEdit->from << endl;
    cout << "To: " << connToEdit->to << endl;
    cout << "Distance: " << connToEdit->distance << " km" << endl;
    cout << "Time: " << connToEdit->time << " minutes" << endl;
    cout << "Description: " << connToEdit->description << endl;
    
    cout << "\nEnter new information (leave blank to keep current value):\n";
    
    // Get Distance
    while (true) {
        cout << "Enter distance in kilometers: ";
        getline(cin, input);
        
        if (input.empty()) {
            break; // Keep current value
        }
        
        if (!isFloat(input)) {
            cout << "Error: Distance must be a number.\n";
            continue;
        }
        
        float distance = stof(input);
        
        if (distance <= 0) {
            cout << "Error: Distance must be greater than 0.\n";
            continue;
        }
        
        connToEdit->distance = distance;
        break;
    }
    
    // Get Time
    while (true) {
        cout << "Enter travel time in minutes: ";
        getline(cin, input);
        
        if (input.empty()) {
            break; // Keep current value
        }
        
        if (!isInteger(input)) {
            cout << "Error: Time must be an integer.\n";
            continue;
        }
        
        int time = stoi(input);
        
        if (time <= 0) {
            cout << "Error: Time must be greater than 0.\n";
            continue;
        }
        
        connToEdit->time = time;
        break;
    }
    
    // Get Description
    cout << "Enter description: ";
    getline(cin, input);
    if (!input.empty()) {
        connToEdit->description = input;
    }
    
    // Save changes to file
    if (saveConnections()) {
        cout << "Connection updated successfully.\n";
    } else {
        cout << "Error: Failed to save changes.\n";
    }
}

void viewConnections() {
    cout << "\n=== Connections ===\n";
    
    bool hasConnections = false;
    for (int i = 0; i < MAX; i++) {
        if (!adjList[i].empty()) {
            hasConnections = true;
            break;
        }
    }
    
    if (!hasConnections) {
        cout << "No connections found.\n";
        return;
    }
    
    // Print header
    cout << left << setw(10) << "From ID" << " | "
         << setw(10) << "To ID" << " | "
         << setw(15) << "Distance (km)" << " | "
         << setw(15) << "Time (min)" << " | "
         << "Description" << endl;
    
    cout << string(100, '-') << endl;
    
    // Print data
    for (int i = 0; i < MAX; i++) {
        for (const auto& conn : adjList[i]) {
            cout << left << setw(10) << conn.from << " | "
                 << setw(10) << conn.to << " | "
                 << fixed << setprecision(2) << setw(15) << conn.distance << " | "
                 << setw(15) << conn.time << " | "
                 << conn.description << endl;
        }
    }
}

void removeConnection() {
    string input;
    int fromId, toId;
    
    cout << "\n=== Remove Connection ===\n";
    
    // Get FromID
    while (true) {
        cout << "Enter source health center ID: ";
        getline(cin, input);
        
        if (!isInteger(input)) {
            cout << "Error: ID must be an integer.\n";
            continue;
        }
        
        fromId = stoi(input);
        
        if (!isHealthCenterExists(fromId)) {
            cout << "Error: No health center with ID " << fromId << " exists.\n";
            continue;
        }
        
        break;
    }
    
    // Get ToID
    while (true) {
        cout << "Enter destination health center ID: ";
        getline(cin, input);
        
        if (!isInteger(input)) {
            cout << "Error: ID must be an integer.\n";
            continue;
        }
        
        toId = stoi(input);
        
        if (!isHealthCenterExists(toId)) {
            cout << "Error: No health center with ID " << toId << " exists.\n";
            continue;
        }
        
        // Check if connection exists
        bool connectionExists = false;
        for (const auto& conn : adjList[fromId]) {
            if (conn.to == toId) {
                connectionExists = true;
                break;
            }
        }
        
        if (!connectionExists) {
            cout << "Error: No connection from " << fromId << " to " << toId << " exists.\n";
            continue;
        }
        
        break;
    }
    
    // Confirm deletion
    cout << "Are you sure you want to remove this connection? (y/n): ";
    getline(cin, input);
    
    if (input != "y" && input != "Y") {
        cout << "Operation canceled.\n";
        return;
    }
    
    // Remove connection from adjacency list
    auto& connections = adjList[fromId];
    connections.erase(
        remove_if(connections.begin(), connections.end(),
            [toId](const Connection& conn) { return conn.to == toId; }),
        connections.end());
    
    // Save changes to file
    if (saveConnections()) {
        cout << "Connection removed successfully.\n";
    } else {
        cout << "Error: Failed to save changes.\n";
    }
}

// Additional feature - View Relationships
void viewRelationships() {
    cout << "\n=== Health Center Relationships ===\n";
    
    if (centers.empty()) {
        cout << "No health centers found.\n";
        return;
    }
    
    // Open file for writing
    ofstream file("relationship_table.csv");
    if (!file) {
        cout << "Error: Could not create relationship_table.csv" << endl;
        return;
    }
    
    // Write header to file
    file << "Health Center ID,Health Center Name,Connected To,Distance (km),Time (min),Description" << endl;
    
    // Print header
    cout << left << setw(5) << "ID" << " | "
         << setw(25) << "Name" << " | "
         << setw(10) << "Connected To" << " | "
         << setw(15) << "Distance (km)" << " | "
         << setw(15) << "Time (min)" << " | "
         << "Description" << endl;
    
    cout << string(100, '-') << endl;
    
    // Print data and write to file
    for (const auto& center : centers) {
        bool hasConnections = false;
        
        for (const auto& conn : adjList[center.id]) {
            hasConnections = true;
            int toIndex = getHealthCenterIndex(conn.to);
            string toName = (toIndex != -1) ? centers[toIndex].name : "Unknown";
            
            cout << left << setw(5) << center.id << " | "
                 << setw(25) << center.name << " | "
                 << setw(10) << conn.to << " | "
                 << fixed << setprecision(2) << setw(15) << conn.distance << " | "
                 << setw(15) << conn.time << " | "
                 << conn.description << endl;
            
            // Write to file
            file << center.id << ","
                 << center.name << ","
                 << conn.to << ","
                 << fixed << setprecision(2) << conn.distance << ","
                 << conn.time << ","
                 << conn.description << endl;
        }
        
        if (!hasConnections) {
            cout << left << setw(5) << center.id << " | "
                 << setw(25) << center.name << " | "
                 << setw(10) << "None" << " | "
                 << setw(15) << "-" << " | "
                 << setw(15) << "-" << " | "
                 << "-" << endl;
            
            // Write to file
            file << center.id << ","
                 << center.name << ","
                 << "None,0,0,-" << endl;
        }
    }
    
    file.close();
    cout << "\nRelationship table has been saved to relationship_table.csv" << endl;
}

// Graph Algorithms Implementation

// Dijkstra's Algorithm for shortest path
void dijkstra() {
    string input;
    int sourceId, destId;
    
    cout << "\n=== Dijkstra's Shortest Path ===\n";
    
    if (centers.empty()) {
        cout << "No health centers found.\n";
        return;
    }
    
    // Get source health center ID
    while (true) {
        cout << "Enter source health center ID: ";
        getline(cin, input);
        
        if (!isInteger(input)) {
            cout << "Error: ID must be an integer.\n";
            continue;
        }
        
        sourceId = stoi(input);
        
        if (!isHealthCenterExists(sourceId)) {
            cout << "Error: No health center with ID " << sourceId << " exists.\n";
            continue;
        }
        
        break;
    }
    
    // Get destination health center ID
    while (true) {
        cout << "Enter destination health center ID: ";
        getline(cin, input);
        
        if (!isInteger(input)) {
            cout << "Error: ID must be an integer.\n";
            continue;
        }
        
        destId = stoi(input);
        
        if (!isHealthCenterExists(destId)) {
            cout << "Error: No health center with ID " << destId << " exists.\n";
            continue;
        }
        
        if (sourceId == destId) {
            cout << "Source and destination are the same.\n";
            return;
        }
        
        break;
    }
    
    // Initialize distance array with infinity
    vector<float> dist(MAX, numeric_limits<float>::infinity());
    vector<int> prev(MAX, -1); // To reconstruct the path
    vector<bool> visited(MAX, false);
    
    // Distance from source to itself is 0
    dist[sourceId] = 0;
    
    // Priority queue to get the vertex with minimum distance
    priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> pq;
    pq.push({0, sourceId});
    
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        
        if (visited[u]) continue;
        visited[u] = true;
        
        // If we've reached the destination, we can stop
        if (u == destId) break;
        
        // Relax all adjacent vertices
        for (const auto& conn : adjList[u]) {
            int v = conn.to;
            float weight = conn.distance;
            
            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    
    // Check if destination is reachable
    if (dist[destId] == numeric_limits<float>::infinity()) {
        cout << "No path exists from health center " << sourceId << " to " << destId << ".\n";
        return;
    }
    
    // Reconstruct the path
    vector<int> path;
    for (int at = destId; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    
    // Print the path
    cout << "\nShortest path from " << sourceId << " to " << destId << ":" << endl;
    cout << "Total distance: " << fixed << setprecision(2) << dist[destId] << " km" << endl;
    cout << "Path: ";
    
    for (size_t i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i < path.size() - 1) {
            cout << " -> ";
        }
    }
    cout << endl;
    
    // Print detailed path information
    cout << "\nDetailed path information:" << endl;
    cout << left << setw(10) << "From" << " | "
         << setw(10) << "To" << " | "
         << setw(15) << "Distance (km)" << " | "
         << "Description" << endl;
    cout << string(80, '-') << endl;
    
    for (size_t i = 0; i < path.size() - 1; i++) {
        int from = path[i];
        int to = path[i + 1];
        
        // Find the connection details
        for (const auto& conn : adjList[from]) {
            if (conn.to == to) {
                cout << left << setw(10) << from << " | "
                     << setw(10) << to << " | "
                     << fixed << setprecision(2) << setw(15) << conn.distance << " | "
                     << conn.description << endl;
                break;
            }
        }
    }
}

// BFS Traversal
void bfs() {
    string input;
    int startId;
    
    cout << "\n=== BFS Traversal ===\n";
    
    if (centers.empty()) {
        cout << "No health centers found.\n";
        return;
    }
    
    // Get starting health center ID
    while (true) {
        cout << "Enter starting health center ID: ";
        getline(cin, input);
        
        if (!isInteger(input)) {
            cout << "Error: ID must be an integer.\n";
            continue;
        }
        
        startId = stoi(input);
        
        if (!isHealthCenterExists(startId)) {
            cout << "Error: No health center with ID " << startId << " exists.\n";
            continue;
        }
        
        break;
    }
    
    // Initialize visited array
    vector<bool> visited(MAX, false);
    queue<int> q;
    
    // Start BFS from the given health center
    q.push(startId);
    visited[startId] = true;
    
    cout << "\nBFS Traversal starting from health center " << startId << ":" << endl;
    
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        
        // Print current health center
        int index = getHealthCenterIndex(current);
        if (index != -1) {
            cout << "Health Center " << current << ": " << centers[index].name << " (" << centers[index].district << ")" << endl;
        }
        
        // Visit all adjacent health centers
        for (const auto& conn : adjList[current]) {
            if (!visited[conn.to]) {
                q.push(conn.to);
                visited[conn.to] = true;
            }
        }
    }
}

// DFS helper function for cycle detection
bool dfsForCycle(int node, vector<bool>& visited, vector<bool>& recStack, vector<int>& cycle) {
    // Mark the current node as visited and part of recursion stack
    visited[node] = true;
    recStack[node] = true;
    cycle.push_back(node);
    
    // Recur for all the vertices adjacent to this vertex
    for (const auto& conn : adjList[node]) {
        // If the node is not visited, then check its subtree
        if (!visited[conn.to]) {
            if (dfsForCycle(conn.to, visited, recStack, cycle)) {
                return true;
            }
        }
        // If the node is visited and in recStack, then there is a cycle
        else if (recStack[conn.to]) {
            // Find the position of conn.to in the cycle
            auto it = find(cycle.begin(), cycle.end(), conn.to);
            if (it != cycle.end()) {
                // Erase everything before the cycle start
                cycle.erase(cycle.begin(), it);
            }
            cycle.push_back(conn.to); // Add the node again to complete the cycle
            return true;
        }
    }
    
    // Remove the vertex from recursion stack and cycle path
    recStack[node] = false;
    cycle.pop_back();
    return false;
}

// Cycle Detection using DFS
bool detectCycle() {
    cout << "\n=== Cycle Detection ===\n";
    
    if (centers.empty()) {
        cout << "No health centers found.\n";
        return false;
    }
    
    vector<bool> visited(MAX, false);
    vector<bool> recStack(MAX, false);
    vector<int> cycle;
    
    // Check for cycle in different DFS trees
    for (const auto& center : centers) {
        if (!visited[center.id]) {
            cycle.clear();
            if (dfsForCycle(center.id, visited, recStack, cycle)) {
                cout << "Cycle detected: ";
                for (size_t i = 0; i < cycle.size(); i++) {
                    cout << cycle[i];
                    if (i < cycle.size() - 1) {
                        cout << " -> ";
                    }
                }
                cout << endl;
                return true;
            }
        }
    }
    
    return false;
}

// Floyd-Warshall Algorithm for all-pairs shortest paths
void floydWarshall() {
    cout << "\n=== Floyd-Warshall All-Pairs Shortest Paths ===\n";
    
    if (centers.empty()) {
        cout << "No health centers found.\n";
        return;
    }
    
    // Create a list of all health center IDs
    vector<int> centerIds;
    for (const auto& center : centers) {
        centerIds.push_back(center.id);
    }
    
    int n = centerIds.size();
    
    // Initialize distance matrix with infinity
    vector<vector<float>> dist(n, vector<float>(n, numeric_limits<float>::infinity()));
    vector<vector<int>> next(n, vector<int>(n, -1)); // For path reconstruction
    
    // Map health center IDs to matrix indices
    unordered_map<int, int> idToIndex;
    for (int i = 0; i < n; i++) {
        idToIndex[centerIds[i]] = i;
        dist[i][i] = 0; // Distance to self is 0
    }
    
    // Fill the distance matrix with direct connections
    for (int i = 0; i < n; i++) {
        int fromId = centerIds[i];
        for (const auto& conn : adjList[fromId]) {
            if (idToIndex.find(conn.to) != idToIndex.end()) {
                int j = idToIndex[conn.to];
                dist[i][j] = conn.distance;
                next[i][j] = j; // Direct connection
            }
        }
    }
    
    // Floyd-Warshall algorithm
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != numeric_limits<float>::infinity() && 
                    dist[k][j] != numeric_limits<float>::infinity() && 
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
    
    // Print the all-pairs shortest paths
    cout << "\nAll-Pairs Shortest Paths:\n";
    cout << left << setw(15) << "From->To" << " | ";
    
    for (int j = 0; j < n; j++) {
        cout << setw(8) << centerIds[j] << " | ";
    }
    cout << endl;
    
    cout << string(15 + (n * 12), '-') << endl;
    
    for (int i = 0; i < n; i++) {
        cout << left << setw(15) << centerIds[i] << " | ";
        
        for (int j = 0; j < n; j++) {
            if (i == j) {
                cout << setw(8) << "0" << " | ";
            } else if (dist[i][j] == numeric_limits<float>::infinity()) {
                cout << setw(8) << "INF" << " | ";
            } else {
                cout << fixed << setprecision(1) << setw(8) << dist[i][j] << " | ";
            }
        }
        cout << endl;
    }
    
    // Ask if the user wants to see a specific path
    string input;
    cout << "\nDo you want to see a specific path? (y/n): ";
    getline(cin, input);
    
    if (input == "y" || input == "Y") {
        int fromId, toId;
        
        // Get source health center ID
        while (true) {
            cout << "Enter source health center ID: ";
            getline(cin, input);
            
            if (!isInteger(input)) {
                cout << "Error: ID must be an integer.\n";
                continue;
            }
            
            fromId = stoi(input);
            
            if (!isHealthCenterExists(fromId)) {
                cout << "Error: No health center with ID " << fromId << " exists.\n";
                continue;
            }
            
            break;
        }
        
        // Get destination health center ID
        while (true) {
            cout << "Enter destination health center ID: ";
            getline(cin, input);
            
            if (!isInteger(input)) {
                cout << "Error: ID must be an integer.\n";
                continue;
            }
            
            toId = stoi(input);
            
            if (!isHealthCenterExists(toId)) {
                cout << "Error: No health center with ID " << toId << " exists.\n";
                continue;
            }
            
            if (fromId == toId) {
                cout << "Source and destination are the same.\n";
                return;
            }
            
            break;
        }
        
        // Get matrix indices
        int i = idToIndex[fromId];
        int j = idToIndex[toId];
        
        // Check if path exists
        if (dist[i][j] == numeric_limits<float>::infinity()) {
            cout << "No path exists from health center " << fromId << " to " << toId << ".\n";
            return;
        }
        
        // Reconstruct the path
        vector<int> path;
        path.push_back(fromId);
        
        int current = i;
        while (current != j) {
            current = next[current][j];
            path.push_back(centerIds[current]);
        }
        
        // Print the path
        cout << "\nShortest path from " << fromId << " to " << toId << ":" << endl;
        cout << "Total distance: " << fixed << setprecision(2) << dist[i][j] << " km" << endl;
        cout << "Path: ";
        
        for (size_t k = 0; k < path.size(); k++) {
            cout << path[k];
            if (k < path.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
    }
}

// Prim's Algorithm for Minimum Spanning Tree
void primMST() {
    cout << "\n=== Prim's Minimum Spanning Tree ===\n";
    
    if (centers.empty()) {
        cout << "No health centers found.\n";
        return;
    }
    
    string input;
    int startId;
    
    // Get starting health center ID
    while (true) {
        cout << "Enter starting health center ID: ";
        getline(cin, input);
        
        if (!isInteger(input)) {
            cout << "Error: ID must be an integer.\n";
            continue;
        }
        
        startId = stoi(input);
        
        if (!isHealthCenterExists(startId)) {
            cout << "Error: No health center with ID " << startId << " exists.\n";
            continue;
        }
        
        break;
    }
    
    // Initialize arrays
    vector<float> key(MAX, numeric_limits<float>::infinity());
    vector<bool> mstSet(MAX, false);
    vector<int> parent(MAX, -1);
    
    // Start with the given health center
    key[startId] = 0;
    
    // Priority queue to get the vertex with minimum key value
    priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> pq;
    pq.push({0, startId});
    
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        
        if (mstSet[u]) continue;
        mstSet[u] = true;
        
        // Update key value of all adjacent vertices
        for (const auto& conn : adjList[u]) {
            int v = conn.to;
            float weight = conn.distance;
            
            if (!mstSet[v] && weight < key[v]) {
                parent[v] = u;
                key[v] = weight;
                pq.push({key[v], v});
            }
        }
    }
    
    // Print the MST
    cout << "\nMinimum Spanning Tree starting from health center " << startId << ":" << endl;
    cout << left << setw(10) << "From" << " | "
         << setw(10) << "To" << " | "
         << setw(15) << "Distance (km)" << endl;
    cout << string(40, '-') << endl;
    
    float totalDistance = 0;
    int edgeCount = 0;
    
    for (const auto& center : centers) {
        int v = center.id;
        if (v != startId && parent[v] != -1) {
            int u = parent[v];
            
            // Find the connection details
            for (const auto& conn : adjList[u]) {
                if (conn.to == v) {
                    cout << left << setw(10) << u << " | "
                         << setw(10) << v << " | "
                         << fixed << setprecision(2) << setw(15) << conn.distance << endl;
                    
                    totalDistance += conn.distance;
                    edgeCount++;
                    break;
                }
            }
        }
    }
    
    // Check if MST is complete (connects all health centers)
    if (edgeCount < centers.size() - 1) {
        cout << "\nWarning: The network is not fully connected." << endl;
        cout << "Only " << edgeCount + 1 << " out of " << centers.size() << " health centers are in the MST." << endl;
    }
    
    cout << "\nTotal MST distance: " << fixed << setprecision(2) << totalDistance << " km" << endl;
}

// Emergency Routing - Find nearest health center with sufficient capacity
void emergencyRouting() {
    string input;
    int startId, minCapacity;
    
    cout << "\n=== Emergency Routing ===\n";
    
    if (centers.empty()) {
        cout << "No health centers found.\n";
        return;
    }
    
    // Get starting health center ID
    while (true) {
        cout << "Enter current location (health center ID): ";
        getline(cin, input);
        
        if (!isInteger(input)) {
            cout << "Error: ID must be an integer.\n";
            continue;
        }
        
        startId = stoi(input);
        
        if (!isHealthCenterExists(startId)) {
            cout << "Error: No health center with ID " << startId << " exists.\n";
            continue;
        }
        
        break;
    }
    
    // Get minimum capacity required
    while (true) {
        cout << "Enter minimum capacity required: ";
        getline(cin, input);
        
        if (!isInteger(input)) {
            cout << "Error: Capacity must be an integer.\n";
            continue;
        }
        
        minCapacity = stoi(input);
        
        if (minCapacity <= 0) {
            cout << "Error: Capacity must be greater than 0.\n";
            continue;
        }
        
        break;
    }
    
    // Check if the starting health center already has sufficient capacity
    int startIndex = getHealthCenterIndex(startId);
    if (centers[startIndex].capacity >= minCapacity) {
        cout << "\nThe current health center (ID: " << startId << ") already has sufficient capacity (" 
             << centers[startIndex].capacity << ").\n";
        return;
    }
    
    // Initialize distance array with infinity
    vector<float> dist(MAX, numeric_limits<float>::infinity());
    vector<int> prev(MAX, -1); // To reconstruct the path
    vector<bool> visited(MAX, false);
    
    // Distance from source to itself is 0
    dist[startId] = 0;
    
    // Priority queue to get the vertex with minimum distance
    priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> pq;
    pq.push({0, startId});
    
    int nearestId = -1;
    float nearestDist = numeric_limits<float>::infinity();
    
    while (!pq.empty() && nearestId == -1) {
        int u = pq.top().second;
        float d = pq.top().first;
        pq.pop();
        
        if (visited[u]) continue;
        visited[u] = true;
        
        // Check if this health center has sufficient capacity
        int index = getHealthCenterIndex(u);
        if (index != -1 && u != startId && centers[index].capacity >= minCapacity) {
            nearestId = u;
            nearestDist = d;
            break;
        }
        
        // Relax all adjacent vertices
        for (const auto& conn : adjList[u]) {
            int v = conn.to;
            float weight = conn.distance;
            
            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    
    // Check if a suitable health center was found
    if (nearestId == -1) {
        cout << "\nNo health center with capacity >= " << minCapacity << " found.\n";
        return;
    }
    
    // Reconstruct the path
    vector<int> path;
    for (int at = nearestId; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    
    // Get the details of the nearest health center
    int nearestIndex = getHealthCenterIndex(nearestId);
    
    // Print the result
    cout << "\nNearest health center with capacity >= " << minCapacity << ":" << endl;
    cout << "ID: " << nearestId << endl;
    cout << "Name: " << centers[nearestIndex].name << endl;
    cout << "District: " << centers[nearestIndex].district << endl;
    cout << "Capacity: " << centers[nearestIndex].capacity << endl;
    cout << "Distance: " << fixed << setprecision(2) << nearestDist << " km" << endl;
    
    // Print the path
    cout << "\nRoute from " << startId << " to " << nearestId << ":" << endl;
    cout << "Path: ";
    
    for (size_t i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i < path.size() - 1) {
            cout << " -> ";
        }
    }
    cout << endl;
    
    // Print detailed path information
    cout << "\nDetailed route information:" << endl;
    cout << left << setw(10) << "From" << " | "
         << setw(10) << "To" << " | "
         << setw(15) << "Distance (km)" << " | "
         << "Description" << endl;
    cout << string(80, '-') << endl;
    
    for (size_t i = 0; i < path.size() - 1; i++) {
        int from = path[i];
        int to = path[i + 1];
        
        // Find the connection details
        for (const auto& conn : adjList[from]) {
            if (conn.to == to) {
                cout << left << setw(10) << from << " | "
                     << setw(10) << to << " | "
                     << fixed << setprecision(2) << setw(15) << conn.distance << " | "
                     << conn.description << endl;
                break;
            }
        }
    }
}

int main() {
    int choice;
    
    // Read data from CSV files
    readHealthCenters();
    readConnections();
    
    do {
        cout << "\n==== Health Center Network System ====\n";
        cout << "1. Add Health Center\n";
        cout << "2. Edit Health Center\n";
        cout << "3. View Health Centers\n";
        cout << "4. Remove Health Center\n";
        cout << "5. Add Connection\n";
        cout << "6. Edit Connection\n";
        cout << "7. View Connections\n";
        cout << "8. Remove Connection\n";
        cout << "9. View Relationships\n";
        cout << "10. Dijkstra's Shortest Path\n";
        cout << "11. BFS Traversal\n";
        cout << "12. Detect Cycle\n";
        cout << "13. Floyd-Warshall All-Pairs\n";
        cout << "14. Prim's MST\n";
        cout << "15. Emergency Routing\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        
        string input;
        getline(cin, input);
        
        if (isInteger(input)) {
            choice = stoi(input);
        } else {
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch (choice) {
            case 1:
                addHealthCenter();
                break;
            case 2:
                editHealthCenter();
                break;
            case 3:
                viewHealthCenters();
                break;
            case 4:
                removeHealthCenter();
                break;
            case 5:
                addConnection();
                break;
            case 6:
                editConnection();
                break;
            case 7:
                viewConnections();
                break;
            case 8:
                removeConnection();
                break;
            case 9:
                viewRelationships();
                break;
            case 10:
                dijkstra();
                break;
            case 11:
                bfs();
                break;
            case 12:
                if (detectCycle()) {
                    cout << "Cycle detected in the network.\n";
                } else {
                    cout << "No cycle detected in the network.\n";
                }
                break;
            case 13:
                floydWarshall();
                break;
            case 14:
                primMST();
                break;
            case 15:
                emergencyRouting();
                break;
            case 0:
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);
    
    return 0;
}
