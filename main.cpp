#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace chrono;

class Subject {
private:
    string name;
    int hoursRequired;
    int hoursCompleted;
    steady_clock::time_point startTime;

public:
    Subject(string name, int hoursRequired)
        : name(name), hoursRequired(hoursRequired), hoursCompleted(0) {}

    // Start tracking time
    void startStudySession() {
        startTime = steady_clock::now();
        cout << "Started studying " << name << " at " << getCurrentTime() << endl;
    }

    // Stop tracking time and calculate hours
    void stopStudySession() {
        if (startTime.time_since_epoch().count() == 0) {
            cout << "Study session not started yet for " << name << ".\n";
            return;
        }

        auto endTime = steady_clock::now();
        int duration = duration_cast<minutes>(endTime - startTime).count();
        int hours = duration / 60;
        int minutes = duration % 60;

        hoursCompleted += hours;
        cout << "Stopped studying " << name << " at " << getCurrentTime() << "\n";
        cout << "Duration: " << hours << " hours and " << minutes << " minutes\n";
        startTime = steady_clock::time_point(); // Reset start time
    }

    bool isCompleted() const {
        return hoursCompleted >= hoursRequired;
    }

    void displayProgress() const {
        cout << "Subject: " << name
             << " | Required Hours: " << hoursRequired
             << " | Completed Hours: " << hoursCompleted << "\n";
    }

    string getName() const {
        return name;
    }

    static string getCurrentTime() {
        auto now = system_clock::now();
        time_t now_c = system_clock::to_time_t(now);
        tm* parts = localtime(&now_c);
        stringstream ss;
        ss << put_time(parts, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
};

class StudyScheduler {
private:
    vector<Subject> subjects;

public:
    void addSubject(const string &name, int hoursRequired) {
        subjects.emplace_back(name, hoursRequired);
        cout << "Subject " << name << " added with "
             << hoursRequired << " hours required.\n";
    }

    void startStudySession(const string &subjectName) {
        for (auto &subject : subjects) {
            if (subject.getName() == subjectName) {
                subject.startStudySession();
                return;
            }
        }
        cout << "Subject not found!\n";
    }

    void stopStudySession(const string &subjectName) {
        for (auto &subject : subjects) {
            if (subject.getName() == subjectName) {
                subject.stopStudySession();
                return;
            }
        }
        cout << "Subject not found!\n";
    }

    void displaySchedule() const {
        cout << "\nStudy Schedule:\n";
        for (const auto &subject : subjects) {
            subject.displayProgress();
        }
    }

    void displayPendingSubjects() const {
        cout << "\nPending Subjects:\n";
        for (const auto &subject : subjects) {
            if (!subject.isCompleted()) {
                cout << "Subject: " << subject.getName() << " is pending.\n";
            }
        }
    }
};

int main() {
    StudyScheduler scheduler;
    int choice;
    string name;
    int hoursRequired;

    cout << "Smart Study Scheduler with Real-Time Performance\n";

    do {
        cout << "\nMenu:\n";
        cout << "1. Add a Subject\n";
        cout << "2. Start Study Session\n";
        cout << "3. Stop Study Session\n";
        cout << "4. Display Study Schedule\n";
        cout << "5. Display Pending Subjects\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter subject name: ";
                cin >> name;
                cout << "Enter required hours: ";
                cin >> hoursRequired;
                scheduler.addSubject(name, hoursRequired);
                break;
            case 2:
                cout << "Enter subject name to start study session: ";
                cin >> name;
                scheduler.startStudySession(name);
                break;
            case 3:
                cout << "Enter subject name to stop study session: ";
                cin >> name;
                scheduler.stopStudySession(name);
                break;
            case 4:
                scheduler.displaySchedule();
                break;
            case 5:
                scheduler.displayPendingSubjects();
                break;
            case 6:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}
