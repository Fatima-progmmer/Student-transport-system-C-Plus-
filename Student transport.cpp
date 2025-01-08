#include<iostream>
#include<iomanip>
#include<conio.h>
#include<string>
#include<fstream>
#include<windows.h>
#include<vector>
#include<sstream>

using namespace std;

class TransportSystem {
private:
    struct Student {
        string id;
        string name;
        string route;
        string contact;
        double fee;
        bool feeStatus;
    };

    struct Route {
        string routeId;
        string driverName;
        string driverContact;
        string busNumber;
        int capacity;
        int currentOccupancy;
    };

    vector<Student> students;
    vector<Route> routes;

public:
    TransportSystem() {
        loadStudents();
        loadRoutes();
    }

    void clearScreen() {
        system("cls");
    }

    void registerStudent() {
        clearScreen();
        Student newStudent;
        cout << "\n\t====================" << endl;
        cout << "\tStudent Registration" <<endl;
        cout << "\t====================" << endl;
        
        do {
            cout << "->Enter Student ID (must be unique): ";
            cin >> newStudent.id;
            if(isStudentExists(newStudent.id)) {
                cout << "->ID already exists! Try another.\n";
            }
        } while(isStudentExists(newStudent.id));
        
        cin.ignore();
        cout << "->Enter Student Name: ";
        getline(cin, newStudent.name);
        
        displayRoutes();
        cout << "->Enter Route ID: ";
        getline(cin, newStudent.route);
        
        cout << "->Enter Contact Number: ";
        getline(cin, newStudent.contact);
        
        cout << "->Enter Monthly Fee: ";
        cin >> newStudent.fee;
        
        newStudent.feeStatus = false;

        students.push_back(newStudent);
        saveStudentToFile(newStudent);
        
        // Update route occupancy
        for(auto& route : routes) {
            if(route.routeId == newStudent.route) {
                route.currentOccupancy++;
                updateRouteFile();
                break;
            }
        }
        
        cout << "\nStudent registered successfully!\n";
    }

    void addRoute() {
        clearScreen();
        Route newRoute;
        cout << "\n\t==============" << endl;
        cout << "\tAdd New Route" << endl;
        cout << "\t==============" << endl;
        
        do {
            cout << "->Enter Route ID (must be unique): ";
            cin >> newRoute.routeId;
            if(isRouteExists(newRoute.routeId)) {
                cout << "->Route ID already exists! Try another.\n";
            }
        } while(isRouteExists(newRoute.routeId));
        
        cin.ignore();
        cout << "->Enter Driver Name: ";
        getline(cin, newRoute.driverName);
        
        cout << "->Enter Driver Contact: ";
        getline(cin, newRoute.driverContact);
        
        cout << "->Enter Bus Number: ";
        getline(cin, newRoute.busNumber);
        
        cout << "->Enter Bus Capacity: ";
        cin >> newRoute.capacity;
        
        newRoute.currentOccupancy = 0;

        routes.push_back(newRoute);
        saveRouteToFile(newRoute);
        cout << "\nRoute added successfully!\n";
    }

    void updateFeeStatus() {
        clearScreen();
        string studentId;
        cout << "\n\t=================="<< endl;
        cout << "\tUpdate Fee Status" << endl;
        cout << "\t=================="<< endl;
        cout << "->Enter Student ID: ";
        cin >> studentId;

        for(auto& student : students) {
            if(student.id == studentId) {
                cout << "\nStudent Details:\n";
                cout << "Name: " << student.name << endl;
                cout << "Current Fee Status: " << (student.feeStatus ? "Paid" : "Unpaid") << endl;
                cout << "Monthly Fee Amount: $" << fixed << setprecision(2) << student.fee << endl;
                
                char choice;
                cout << "\nMark as paid? (Y/N): ";
                cin >> choice;
                
                if(toupper(choice) == 'Y') {
                    student.feeStatus = true;
                    updateStudentFile();
                    cout << "\nFee status updated successfully!\n";
                }
                return;
            }
        }
        cout << "\nStudent not found!\n";
    }

    void markAttendance() {
        clearScreen();
        string routeId;
        cout << "\n\t===============" << endl;
        cout << "\tMark Attendance" << endl;
        cout << "\t===============" << endl;
        
        displayRoutes();
        cout << "->Enter Route ID: ";
        cin >> routeId;

        if(!isRouteExists(routeId)) {
            cout << "\nRoute not found!\n";
            return;
        }

        string date;
        cout << "->Enter Date (DD/MM/YYYY): ";
        cin >> date;

        ofstream attendanceFile("attendance_" + date + ".txt", ios::app);
        
        cout << "\nStudents on Route " << routeId << ":\n";
        bool foundStudents = false;
        
        for(const auto& student : students) {
            if(student.route == routeId) {
                foundStudents = true;
                char present;
                cout << student.name << " (ID: " << student.id << ") Present? (Y/N): ";
                cin >> present;
                attendanceFile << date << "," << student.id << "," << student.name << "," 
                             << (toupper(present) == 'Y' ? "Present" : "Absent") << endl;
            }
        }
        
        if(!foundStudents) {
            cout << "\nNo students found for this route!\n";
        } else {
            cout << "\nAttendance marked successfully!\n";
        }
        
        attendanceFile.close();
    }

    void generateReports() {
        clearScreen();
        int choice;
        cout << "\t================" <<endl;
        cout << "\tGenerate Reports";
        cout << "\t================" <<endl;
        cout << "1. Route-wise Student List\n";
        cout << "2. Fee Defaulters List\n";
        cout << "3. Attendance Report\n";
        cout << "4. Vehicle Status\n";
        cout << "->Enter choice: ";
        cin >> choice;

        switch(choice) {
            case 1:
                routeWiseReport();
                break;
            case 2:
                feeDefaultersReport();
                break;
            case 3:
                attendanceReport();
                break;
            case 4:
                vehicleStatusReport();
                break;
            default:
                cout << "Invalid choice!\n";
        }
    }

private:
    bool isStudentExists(const string& id) {
        for(const auto& student : students) {
            if(student.id == id) return true;
        }
        return false;
    }

    bool isRouteExists(const string& id) {
        for(const auto& route : routes) {
            if(route.routeId == id) return true;
        }
        return false;
    }

    void displayRoutes() {
        cout << "\nAvailable Routes:\n";
        cout << setfill('-') << setw(50) << "-" << endl;
        cout << setfill(' ');
        cout << left << setw(10) << "Route ID" 
             << setw(20) << "Bus Number" 
             << setw(20) << "Driver" << endl;
        cout << setfill('-') << setw(50) << "-" << endl;
        cout << setfill(' ');
        
        for(const auto& route : routes) {
            cout << left << setw(10) << route.routeId 
                 << setw(20) << route.busNumber 
                 << setw(20) << route.driverName << endl;
        }
        cout << endl;
    }

    // File Operations
    void loadStudents() {
        ifstream file("students.txt");
        string line;
        
        while(getline(file, line)) {
            stringstream ss(line);
            Student student;
            string feeStatus;
            
            getline(ss, student.id, ',');
            getline(ss, student.name, ',');
            getline(ss, student.route, ',');
            getline(ss, student.contact, ',');
            ss >> student.fee;
            ss.ignore();
            getline(ss, feeStatus);
            student.feeStatus = (feeStatus == "1");
            
            students.push_back(student);
        }
        file.close();
    }

    void loadRoutes() {
        ifstream file("routes.txt");
        string line;
        
        while(getline(file, line)) {
            stringstream ss(line);
            Route route;
            
            getline(ss, route.routeId, ',');
            getline(ss, route.driverName, ',');
            getline(ss, route.driverContact, ',');
            getline(ss, route.busNumber, ',');
            ss >> route.capacity;
            ss.ignore();
            ss >> route.currentOccupancy;
            
            routes.push_back(route);
        }
        file.close();
    }

    void saveStudentToFile(const Student& student) {
        ofstream file("students.txt", ios::app);
        file << student.id << "," 
             << student.name << "," 
             << student.route << "," 
             << student.contact << "," 
             << student.fee << "," 
             << student.feeStatus << endl;
        file.close();
    }

    void saveRouteToFile(const Route& route) {
        ofstream file("routes.txt", ios::app);
        file << route.routeId << "," 
             << route.driverName << "," 
             << route.driverContact << "," 
             << route.busNumber << "," 
             << route.capacity << "," 
             << route.currentOccupancy << endl;
        file.close();
    }

    void updateStudentFile() {
        ofstream file("students.txt");
        for(const auto& student : students) {
            file << student.id << "," 
                 << student.name << "," 
                 << student.route << "," 
                 << student.contact << "," 
                 << student.fee << "," 
                 << student.feeStatus << endl;
        }
        file.close();
    }

    void updateRouteFile() {
        ofstream file("routes.txt");
        for(const auto& route : routes) {
            file << route.routeId << "," 
                 << route.driverName << "," 
                 << route.driverContact << "," 
                 << route.busNumber << "," 
                 << route.capacity << "," 
                 << route.currentOccupancy << endl;
        }
        file.close();
    }

    // Report Generation Functions
    void routeWiseReport() {
        clearScreen();
        cout << "\n=== Route-wise Student List ===\n";
        
        for(const auto& route : routes) {
            cout << "\nRoute: " << route.routeId << " (Bus: " << route.busNumber << ")\n";
            cout << "Driver: " << route.driverName << "\n";
            cout << setfill('-') << setw(60) << "-" << endl;
            cout << setfill(' ');
            cout << left << setw(10) << "ID" 
                 << setw(30) << "Name" 
                 << setw(20) << "Contact" << endl;
            cout << setfill('-') << setw(60) << "-" << endl;
            cout << setfill(' ');
            
            bool foundStudents = false;
            for(const auto& student : students) {
                if(student.route == route.routeId) {
                    foundStudents = true;
                    cout << left << setw(10) << student.id 
                         << setw(30) << student.name 
                         << setw(20) << student.contact << endl;
                }
            }
            
            if(!foundStudents) {
                cout << "No students assigned to this route.\n";
            }
            cout << endl;
        }
    }

    void feeDefaultersReport() {
        clearScreen();
        cout << "\n=== Fee Defaulters List ===\n";
        cout << setfill('-') << setw(70) << "-" << endl;
        cout << setfill(' ');
        cout << left << setw(10) << "ID" 
             << setw(30) << "Name" 
             << setw(15) << "Route" 
             << setw(15) << "Fee Amount" << endl;
        cout << setfill('-') << setw(70) << "-" << endl;
        cout << setfill(' ');
        
        bool foundDefaulters = false;
        for(const auto& student : students) {
            if(!student.feeStatus) {
                foundDefaulters = true;
                cout << left << setw(10) << student.id 
                     << setw(30) << student.name 
                     << setw(15) << student.route 
                     << "$" << fixed << setprecision(2) << student.fee << endl;
            }
        }
        
        if(!foundDefaulters) {
            cout << "\nNo fee defaulters found.\n";
        }
    }

    void attendanceReport() {
        clearScreen();
        string date;
        cout << "\n=== Attendance Report ===\n";
        cout << "Enter Date (DD/MM/YYYY): ";
        cin >> date;
        
        ifstream file("attendance_" + date + ".txt");
        if(!file) {
            cout << "\nNo attendance records found for " << date << endl;
            return;
        }
        
        cout << "\nAttendance for " << date << ":\n";
        cout << setfill('-') << setw(60) << "-" << endl;
        cout << setfill(' ');
        cout << left << setw(10) << "ID" 
             << setw(30) << "Name" 
             << setw(20) << "Status" << endl;
        cout << setfill('-') << setw(60) << "-" << endl;
        cout << setfill(' ');
        
        string line;
        while(getline(file, line)) {
            stringstream ss(line);
            string dateStr, id, name, status;
            
            getline(ss, dateStr, ',');
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, status);
            
            cout << left << setw(10) << id 
                 << setw(30) << name 
                 << status << endl;
        }
        file.close();
    }

    void vehicleStatusReport() {
        clearScreen();
        cout << "\n=== Vehicle Status Report ===\n";
        cout << setfill('-') << setw(80) << "-" << endl;
        cout << setfill(' ');
        cout << left << setw(15) << "Bus Number" 
             << setw(15) << "Route" 
             << setw(20) << "Driver" 
             << setw(15) << "Capacity" 
             << "Occupancy" << endl;
        cout << setfill('-') << setw(80) << "-" << endl;
        cout << setfill(' ');
        
        for(const auto& route : routes) {
            cout << left << setw(15) << route.busNumber 
                 << setw(15) << route.routeId 
                 << setw(20) << route.driverName 
                 << setw(15) << route.capacity 
                 << route.currentOccupancy;
            
            // Show capacity warning if nearly full
            if(route.currentOccupancy >= route.capacity * 0.9) {
                cout << " (Nearly Full!)";
            }
            cout << endl;
        }
    }
};

void displayMenu() {
	cout << "\n\t====================================" << endl;
    cout << "\tStudent Transport Management System";
    cout << "\n\t====================================" << endl;
    cout << "\n\t\t1. Register New Student\n";
    cout << "\t\t2. Add New Route\n";
    cout << "\t\t3. Update Fee Status\n";
    cout << "\t\t4. Mark Attendance\n";
    cout << "\t\t5. Generate Reports\n";
    cout << "\t\t6. Exit\n" << endl;
    cout << "\tEnter your choice: ";
}

int main() {
    SetConsoleTitle("Student Transport Management System");
    
    // Hide cursor
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
    
    TransportSystem transportSystem;
    int choice;

    do {
        system("cls");
        displayMenu();
        cin >> choice;

        switch(choice) {
            case 1:
                transportSystem.registerStudent();
                break;
            case 2:
                transportSystem.addRoute();
                break;
            case 3:
                transportSystem.updateFeeStatus();
                break;
            case 4:
                transportSystem.markAttendance();
                break;
            case 5:
                transportSystem.generateReports();
                break;
            case 6:
                cout << "\nExiting...\n";
                break;
            default:
                cout << "\nInvalid choice!\n";
        }
        
        if(choice != 6) {
            cout << "\nPress any key to continue...";
            _getch();
        }
    } while(choice != 6);

    return 0;
}