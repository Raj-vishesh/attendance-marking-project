
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

const string ADMIN_PASS = "admin123";
const string DATA_FILE = "attendance_data.txt";

struct Attendance {
    string date;
    bool isPresent;
};

struct Student {
    string name;
    vector<Attendance> records;
};

class AttendanceSystem {
private:
    vector<Student> students;

public:
    AttendanceSystem() {
        loadFromFile();
    }

    void saveToFile() {
        ofstream file(DATA_FILE);
        for (auto &student : students) {
            file << student.name << "\n";
            file << student.records.size() << "\n";
            for (auto &rec : student.records) {
                file << rec.date << " " << rec.isPresent << "\n";
            }
        }
        file.close();
    }

    void loadFromFile() {
        ifstream file(DATA_FILE);
        if (!file) return;

        while (!file.eof()) {
            Student student;
            getline(file, student.name);
            if (student.name == "") break;

            int count;
            file >> count;
            file.ignore();

            for (int i = 0; i < count; i++) {
                Attendance rec;
                file >> rec.date >> rec.isPresent;
                file.ignore();
                student.records.push_back(rec);
            }

            students.push_back(student);
        }

        file.close();
    }

    void addStudent() {
        Student student;
        cout << "Enter student name: ";
        getline(cin, student.name);
        students.push_back(student);
        saveToFile();
        cout << "Student added successfully.\n";
    }

    void markAttendance() {
        if (students.empty()) {
            cout << "No students available.\n";
            return;
        }

        string date;
        cout << "Enter date (dd-mm-yyyy): ";
        getline(cin, date);

        for (auto &student : students) {
            cout << "Mark attendance for " << student.name << " (P/A): ";
            char status;
            cin >> status;
            cin.ignore();

            Attendance rec;
            rec.date = date;
            rec.isPresent = (status == 'P' || status == 'p');
            student.records.push_back(rec);
        }

        saveToFile();
        cout << "Attendance marked successfully.\n";
    }

    void displayRecords() {
        if (students.empty()) {
            cout << "No records available.\n";
            return;
        }

        for (auto &student : students) {
            cout << "\nStudent: " << student.name << "\n";

            if (student.records.empty()) {
                cout << "  No attendance records.\n";
                continue;
            }

            for (auto &rec : student.records) {
                cout << "  Date: " << rec.date
                     << ", Status: "
                     << (rec.isPresent ? "Present" : "Absent")
                     << "\n";
            }

            cout << "  Attendance Percentage: "
                 << calculatePercentage(student) << "%\n";
        }
    }

    float calculatePercentage(Student &student) {
        if (student.records.empty()) return 0.0;

        int present = 0;
        for (auto &rec : student.records) {
            if (rec.isPresent) present++;
        }

        return (float)present / student.records.size() * 100;
    }

    int searchStudent(string name) {
        for (int i = 0; i < students.size(); i++) {
            if (students[i].name == name)
                return i;
        }
        return -1;
    }

    void viewOwnAttendance() {
        string name;
        cout << "Enter your name: ";
        getline(cin, name);

        int idx = searchStudent(name);
        if (idx == -1) {
            cout << "Student not found.\n";
            return;
        }

        Student &student = students[idx];

        if (student.records.empty()) {
            cout << "No attendance records.\n";
            return;
        }

        cout << "\nAttendance for " << student.name << ":\n";
        for (auto &rec : student.records) {
            cout << "  Date: " << rec.date
                 << ", Status: "
                 << (rec.isPresent ? "Present" : "Absent")
                 << "\n";
        }

        cout << "Attendance Percentage: "
             << calculatePercentage(student) << "%\n";
    }

    void teacherDashboard() {
        string pass;
        cout << "Enter admin password: ";
        getline(cin, pass);

        if (pass != ADMIN_PASS) {
            cout << "Incorrect password.\n";
            return;
        }

        int choice;
        while (true) {
            cout << "\n--- Teacher Dashboard ---\n";
            cout << "1. Add Student\n";
            cout << "2. Mark Attendance\n";
            cout << "3. Display All Records\n";
            cout << "4. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1: addStudent(); break;
                case 2: markAttendance(); break;
                case 3: displayRecords(); break;
                case 4: return;
                default: cout << "Invalid choice.\n";
            }
        }
    }

    void studentDashboard() {
        viewOwnAttendance();
    }
};

int main() {
    AttendanceSystem system;

    int choice;
    while (true) {
        cout << "\n--- Attendance System Main Menu ---\n";
        cout << "1. Teacher Dashboard\n";
        cout << "2. Student Dashboard\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: system.teacherDashboard(); break;
            case 2: system.studentDashboard(); break;
            case 3:
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice.\n";
        }
    }
}

