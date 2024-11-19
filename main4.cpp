#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <fstream>

using namespace std;

class User {
protected:
    string username;
    string password;
    string role; 

public:
    User(string u, string p, string r) : username(u), password(p), role(r) {}

    virtual ~User  () {}

    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getRole() const { return role; }

    bool checkLogin(string u, string p) const {
        return (username == u && password == p);
    }

    virtual void menu() = 0;
};

class Admin : public User {
public:
    Admin(string u, string p) : User(u, p, "admin") {}

    void menu() override {
        cout << "Admin Menu\n";
        cout << "1. View All Users\n";
        cout << "2. Add New Student\n";
        cout << "3. Add New Faculty\n";
        cout << "4. Add New Course\n";
        cout << "5. View All Courses\n";
        cout << "6. Manage Notices\n";
        cout << "7. Manage Hostel\n";
        cout << "8. Manage Events\n";
        cout << "9. Logout\n";
    }
};

class Course {
private:
    string courseCode;
    string courseName;

public:
    Course(string code, string name) : courseCode(code), courseName(name) {}

    string getCourseCode() const { return courseCode; }
    string getCourseName() const { return courseName; }

    void displayCourse() const {
        cout << "Course Code: " << courseCode << ", Course Name: " << courseName << "\n";
    }
};

class Fee {
private:
    float totalAmount;
    float paidAmount;

public:
    Fee(float total) : totalAmount(total), paidAmount(0) {}

    void makePayment(float amount) {
        if (paidAmount + amount <= totalAmount) {
            paidAmount += amount;
            cout << "Payment of $" << amount << " successful. Total Paid: $" << paidAmount << "\n";
            displayDueAmount();
        } else {
            cout << "Payment exceeds total amount due!\n";
        }
    }

    void viewFees() const {
        cout << "Total Amount: $" << totalAmount << ", Amount Paid: $" << paidAmount << "\n";
        displayDueAmount(); 
    }

    void displayDueAmount() const {
        float dueAmount = totalAmount - paidAmount;
        cout << "Amount Due: $" << dueAmount << "\n";
    }

    bool isPaidInFull() const {
        return paidAmount >= totalAmount;
    }

    float getTotalAmount() const { return totalAmount; } 
};

class Student : public User {
private:
    int id;
    int age;
    string course;
    float finalGrade;
    vector<Course> enrolledCourses;
    map<string, int> attendance;
    map<string, map<string, float>> examScores;
    Fee* feeInfo; 

public:
    Student(string u, string p, int id, int age, string course, float feeAmount)
        : User(u, p, "student"), id(id), age(age), course(course), finalGrade(0.0) {
        feeInfo = new Fee(feeAmount); 
    }

    ~Student() {
        delete feeInfo; 
    }

    int getId() const { return id; }
    int getAge() const { return age; } 
    string getCourse() const { return course; }
    float getFeeAmount() const { return feeInfo->getTotalAmount(); }
    
    void setFinalGrade(float grade) {
        finalGrade = grade;
    }

    void addCourse(const Course& course) {
        enrolledCourses.push_back(course);
        attendance[course.getCourseCode()] = 0; 
        examScores[course.getCourseCode()] = {}; 
    }

    void updateAttendance(string courseCode, int newAttendance) {
        attendance[courseCode] = newAttendance;
    }

    void recordExamScore(string courseCode, string examType, float score) {
        examScores[courseCode][examType] = score;
    }

    void viewExamScores() const {
        if (examScores.empty()) {
            cout << "No exam scores recorded.\n";
            return;
        }
        cout << "Exam Scores:\n";
        for (const auto& course : examScores) {
            cout << "Course: " << course.first << "\n ";
            for ( const auto& exam : course.second) {
                cout << "  Exam: " << exam.first << " | Score: " << exam.second << "\n";
            }
        }
    }

    void viewAttendance() const {
        if (attendance.empty()) {
            cout << "No attendance recorded.\n";
            return;
        }
        cout << "Attendance Report:\n";
        for (const auto& entry : attendance) {
            cout << "Course: " << entry.first << " | Attendance: " << entry.second << "%\n";
        }
    }

    void viewGrades() const {
        cout << "Final Grade: " << finalGrade << "\n";
    }

    void displayEnrolledCourses() const {
        if (enrolledCourses.empty()) {
            cout << "No courses enrolled.\n";
            return;
        }
        cout << "Enrolled Courses:\n";
        for (const auto& course : enrolledCourses) {
            course.displayCourse();
        }
    }

    void viewFees() const {
        feeInfo->viewFees();
    }

    void makePayment(float amount) {
        feeInfo->makePayment(amount);
    }

    void menu() override {
        int choice;
        do {
            cout << "\nStudent Menu\n";
            cout << "1. Add Course\n";
            cout << "2. View Enrolled Courses\n";
            cout << "3. View Grades\n";
            cout << "4. View Attendance\n";
            cout << "5. View Exam Scores\n";
            cout << "6. View Fees\n";
            cout << "7. Make Payment\n";
            cout << "8. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    string courseCode, courseName;
                    cout << "Enter Course Code: ";
                    cin >> courseCode;
                    cin.ignore();
                    cout << "Enter Course Name: ";
                    getline(cin, courseName);
                    addCourse(Course(courseCode, courseName));
                    cout << "Course added successfully!\n";
                    break;
                }
                case 2:
                    displayEnrolledCourses();
                    break;
                case 3:
                    viewGrades();
                    break;
                case 4:
                    viewAttendance();
                    break;
                case 5:
                    viewExamScores();
                    break;
                case 6:
                    viewFees();
                    break;
                case 7: {
                    float paymentAmount;
                    cout << "Enter payment amount: ";
                    cin >> paymentAmount;
                    makePayment(paymentAmount);
                    break;
                }
                case 8:
                    cout << "Logging out...\n";
                    break;
                default:
                    cout << "Invalid choice! Please try again.\n";
            }
        } while (choice != 8);
    }
};

class Faculty : public User {
private:
    vector<Course> assignedCourses;
    map<int, map<string, int>> studentAttendance; 
    map<int, map<string, map<string, float>>> studentExamScores; 
    map<int, map<string, float>> studentFinalGrades; 

public:
    Faculty(string u, string p) : User(u, p, "faculty") {}

    void assignCourse(const Course& course) {
        assignedCourses.push_back(course);
    }

    void recordAttendance(int studentId, string courseCode, int attendance) {
        studentAttendance[studentId][courseCode] = attendance;
        cout << "Attendance recorded for Student ID: " << studentId << " in Course: " << courseCode << ".\n";
    }

    void recordExamScore(int studentId, string courseCode, string examType, float score) {
        studentExamScores[studentId][courseCode][examType] = score;
        cout << "Exam score recorded for Student ID: " << studentId << " in Course: " << courseCode << " for " << examType << " exam.\n";
    }

    void assignFinalGrade(int studentId, string courseCode, float grade) {
        studentFinalGrades[studentId][courseCode] = grade;
        cout << "Final grade assigned for Student ID: " << studentId << " in Course: " << courseCode << ".\n";
    }

    void viewStudentAttendance() const {
        if (studentAttendance.empty()) {
            cout << "No attendance records available.\n";
            return;
        }
        cout << "Attendance Records for Assigned Courses:\n";
        for (const auto& studentEntry : studentAttendance) {
            cout << "Student ID: " << studentEntry.first << "\n";
            for (const auto& courseEntry : studentEntry.second) {
                cout << "  Course: " << courseEntry.first << " | Attendance: " << courseEntry.second << "%\n";
            }
        }
    }

    void viewStudentExamScores() const {
        if (studentExamScores.empty()) {
            cout << "No exam scores recorded.\n";
            return;
        }
        cout << "Exam Scores for Assigned Courses:\n";
        for (const auto& studentEntry : studentExamScores) {
            cout << "Student ID: " << studentEntry.first << "\n";
            for (const auto& courseEntry : studentEntry.second) {
                cout << "  Course: " << courseEntry.first << "\n";
                for (const auto& examEntry : courseEntry.second) {
                    cout << "    Exam: " << examEntry.first << " | Score: " << examEntry.second << "\n";
                }
            }
        }
    }

    void viewStudentFinalGrades() const {
        if (studentFinalGrades.empty()) {
            cout << "No final grades assigned.\n";
            return;
        }
        cout << "Final Grades for Assigned Courses:\n";
        for (const auto& studentEntry : studentFinalGrades) {
            cout << "Student ID: " << studentEntry.first << "\n";
            for (const auto& courseEntry : studentEntry.second) {
                cout << "  Course: " << courseEntry.first << " | Final Grade: " << courseEntry.second << "\n";
            }
        }
    }

    void menu() override {
        int choice;
        do {
            cout << "\nFaculty Menu\n";
            cout << "1. Assign Course\n";
            cout << "2. Record Attendance\n";
            cout << "3. Record Exam Score\n";
            cout << "4. Assign Final Grade\n";
            cout << "5. View Student Attendance\n";
            cout << "6. View Student Exam Scores\n";
            cout << "7. View Student Final Grades\n";
            cout << "8. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    string courseCode, courseName;
                    cout << "Enter Course Code: ";
                    cin >> courseCode;
                    cin.ignore();
                    cout << "Enter Course Name: ";
                    getline(cin, courseName);
                    assignCourse(Course(courseCode, courseName));
                    cout << "Course assigned successfully!\n";
                    break;
                }
                case 2: {
                    int studentId;
                    string courseCode;
                    int attendance;
                    cout << "Enter Student ID: ";
                    cin >> studentId;
                    cout << "Enter Course Code: ";
                    cin >> courseCode;
                    cout << "Enter Attendance: ";
                    cin >> attendance;
                    recordAttendance(studentId, courseCode, attendance);
                    break;
                }
                case 3: {
                    int studentId;
                    string courseCode, examType;
                    float score;
                    cout << "Enter Student ID: ";
                    cin >> studentId;
                    cout << "Enter Course Code: ";
                    cin >> courseCode;
                    cout << "Enter Exam Type: ";
                    cin >> examType;
                    cout << "Enter Score: ";
                    cin >> score;
                    recordExamScore(studentId, courseCode, examType, score);
                    break;
                }
                case 4: {
                    int studentId;
                    string courseCode;
                    float grade;
                    cout << "Enter Student ID: ";
                    cin >> studentId;
                    cout << "Enter Course Code: ";
                    cin >> courseCode;
                    cout << "Enter Final Grade: ";
                    cin >> grade;
                    assignFinalGrade(studentId, courseCode, grade);
                    break;
                }
                case 5:
                    viewStudentAttendance();
                    break;
                case 6:
                    viewStudentExamScores();
                    break;
                case 7:
                    viewStudentFinalGrades();
                    break;
                case 8:
                    cout << "Logging out...\n";
                    break;
                default:
                    cout << "Invalid choice! Please try again.\n";
            }
        } while (choice != 8);
    }
};

class Notice {
public:
    int id;
    string message;
    string category;
    time_t timestamp;

    Notice(int noticeId, const string& msg, const string& cat)
        : id(noticeId), message(msg), category(cat), timestamp(time(0)) {}

    string getTimestamp() const {
        char buffer[80];
        struct tm* timeinfo = localtime(&timestamp);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
        return string(buffer);
    }
};

class NoticeBoard {
private:
    vector<Notice> notices;
    int nextNoticeId;

public:
    NoticeBoard() : nextNoticeId(1) {}

    void addNotice(const string& message, const string& category) {
        notices.push_back(Notice (nextNoticeId++, message, category));
        cout << "Notice added successfully!\n";
    }

    void editNotice(int id, const string& newMessage) {
        for (auto& notice : notices) {
            if (notice.id == id) {
                notice.message = newMessage;
                cout << "Notice updated successfully!\n";
                return;
            }
        }
        cout << "Notice not found!\n";
    }

    void deleteNotice(int id) {
        for (auto it = notices.begin(); it != notices.end(); ++it) {
            if (it->id == id) {
                notices.erase(it);
                cout << "Notice deleted successfully!\n";
                return;
            }
        }
        cout << "Notice not found!\n";
    }

    void viewNotices() const {
        if (notices.empty()) {
            cout << "No notices available.\n";
            return;
        }
        cout << "Notice Board:\n";
        for (const auto& notice : notices) {
            cout << "Notice ID: " << notice.id << "\n";
            cout << "Message: " << notice.message << "\n";
            cout << "Category: " << notice.category << "\n";
            cout << "Timestamp: " << notice.getTimestamp() << "\n";
            cout << "-------------------------\n";
        }
    }
};

class Library {
private:
    vector<string> books;

public:
    void addBook(const string& book) {
        books.push_back(book);
        cout << "Book added successfully!\n";
    }

    void removeBook(const string& book) {
        for (auto it = books.begin(); it != books.end(); ++it) {
            if (*it == book) {
                books.erase(it);
                cout << "Book removed successfully!\n";
                return;
            }
        }
        cout << "Book not found!\n";
    }

    void checkOutBook(const string& book, const string& borrower) {
        for (auto it = books.begin(); it != books.end(); ++it) {
            if (*it == book) {
                cout << "Book checked out to " << borrower << " successfully!\n";
                books.erase(it);
                return;
            }
        }
        cout << "Book not found!\n";
    }

    void returnBook(const string& book, const string& borrower) {
        books.push_back(book);
        cout << "Book returned by " << borrower << " successfully!\n";
    }

    void viewBooks() const {
        if (books.empty()) {
            cout << "No books available.\n";
            return;
        }
        cout << "Available Books:\n";
        for (const auto& book : books) {
            cout << book << "\n";
        }
    }
};

class Hostel {
private:
    vector<string> rooms;
    map<string, string> roomAssignments;

public:
    void addRoom(const string& room) {
        rooms.push_back(room);
        cout << "Room added successfully!\n";
    }

    void assignRoom(const string& room, const string& student) {
        if (find(rooms.begin(), rooms.end(), room) != rooms.end()) {
            roomAssignments[room] = student;
            cout << "Room assigned to " << student << " successfully!\n";
        } else {
            cout << "Room not found!\n";
        }
    }

    void viewRooms() const {
        if (rooms.empty()) {
            cout << "No rooms available.\n";
            return;
        }
        cout << "Available Rooms:\n";
        for (const auto& room : rooms) {
            cout << room << "\n";
        }
    }

    void viewRoomAssignments() const {
        if (roomAssignments.empty()) {
            cout << "No room assignments available.\n";
            return;
        }
        cout << "Room Assignments:\n";
        for (const auto& assignment : roomAssignments) {
            cout << "Room: " << assignment.first << ", Student: " << assignment.second << "\n";
        }
    }
};

class Event {
public:
    int id;
    string name;
    string description;
    string date;
    string time;
    string location;

    Event(int eventId, const string& name, const string& description, const string& date, const string& time, const string& location) 
        : id(eventId), name(name), description(description), date(date), time(time), location(location) {}

    void displayEvent() const {
        cout << "Event ID: " << id << "\n";
        cout << "Event Name: " << name << "\n";
        cout << "Event Description: " << description << "\n";
        cout << "Event Date: " << date << "\n";
        cout << "Event Time: " << time << "\n";
        cout << "Event Location: " << location << "\n";
    }
};

class EventManagementSystem {
private:
    vector<Event> events;
    int nextEventId;

public:
    EventManagementSystem() : nextEventId(1) {}

    void addEvent(const string& name, const string& description, const string& date, const string& time, const string& location) {
        events.push_back(Event(nextEventId++, name, description, date, time, location));
        cout << "Event added successfully!\n";
    }

    void editEvent(int id, const string& newName, const string& newDescription, const string& newDate, const string& newTime, const string& newLocation) {
        for (auto& event : events) {
            if (event.id == id) {
                event.name = newName;
                event.description = newDescription;
                event.date = newDate;
                event.time = newTime;
                event.location = newLocation;
                cout << "Event updated successfully!\n";
                return;
            }
        }
        cout << "Event not found!\n";
    }

    void deleteEvent(int id) {
        for (auto it = events.begin(); it != events.end(); ++it) {
            if (it->id == id) {
                events.erase(it);
                cout << "Event deleted successfully!\n";
                return;
            }
        }
        cout << "Event not found!\n";
    }

    void viewEvents() const {
        if (events.empty()) {
            cout << "No events available.\n";
            return;
        }
        cout << "Events:\n";
        for (const auto& event : events) {
            event.displayEvent();
            cout << "-------------------------\n";
        }
    }
};

class StudentInformationSystem {
private:
    vector<User*> users;
    vector<Course> courses;
    NoticeBoard noticeBoard;
    Library library;
    Hostel hostel;
    EventManagementSystem eventManagementSystem;

    const string studentFile = "students.txt"; 
    const string facultyFile = "faculty.txt";   

    void loadStudents() {
        ifstream inFile(studentFile);
        if (!inFile) {
            cout << "No student data file found. Starting fresh.\n";
            return;
        }
        string username, password, course;
        int id, age;
        float feeAmount;
        while (inFile >> username >> password >> id >> age >> course >> feeAmount) {
            User* student = new Student(username, password, id, age, course, feeAmount);
            users.push_back(student);
        }
        inFile.close();
        cout << "Student data loaded successfully.\n";
    }

    void loadFaculty() {
        ifstream inFile(facultyFile);
        if (!inFile) {
            cout << "No faculty data file found. Starting fresh.\n";
            return;
        }
        string username, password;
        while (inFile >> username >> password) {
            User* faculty = new Faculty(username, password);
            users.push_back(faculty);
        }
        inFile.close();
        cout << "Faculty data loaded successfully.\n";
    }

    void saveStudents() {
        ofstream outFile(studentFile);
        for (const auto& user : users) {
            if (user->getRole() == "student") {
                Student* student = dynamic_cast<Student*>(user);
                if (student) {
                    outFile << student->getUsername() << " "
                            << student->getPassword() << " "
                            << student->getId() << " "
                            << student->getAge() << " "
                            << student->getCourse() << " "
                            << student->getFeeAmount() << "\n";
                }
            }
        }
        outFile.close();
    }

    void saveFaculty() {
        ofstream outFile(facultyFile);
        for (const auto& user : users) {
            if (user->getRole() == "faculty") {
                Faculty* faculty = dynamic_cast<Faculty*>(user);
                if (faculty) {
                    outFile << faculty->getUsername() << " "
                            << faculty->getPassword() << " \n";
                }
            }
        }
        outFile.close();
    }

public:
    StudentInformationSystem() {
        loadStudents(); 
        loadFaculty();  
    }

    ~StudentInformationSystem() {
        saveStudents(); 
        saveFaculty();  
    }

    void registerStudent() {
        string username, password, name;
        int id, age;
        float feeAmount;
        cout << "Enter Student Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Student ID: ";
        cin >> id;
        cout << "Enter Age: ";
        cin >> age;
        cout << "Enter Fee Amount: ";
        cin >> feeAmount;
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;
        User* student = new Student(username, password, id, age, "N/A", feeAmount);
        users.push_back(student);
        cout << "Student registered successfully!\n";
    }

    void registerFaculty() {
        string username, password, name;
        cout << "Enter Faculty Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;
        User* faculty = new Faculty(username, password);
        users.push_back(faculty);
        cout << "Faculty registered successfully!\n";
    }

    User* login(string username, string password) {
        for (auto& user : users) {
            if (user->checkLogin(username, password)) {
                return user;
            }
        }
        return nullptr;
    }

    void manageNotices() {
        int choice;
        do {
            cout << "\nNotice Management Menu\n";
            cout << "1. Add Notice\n";
            cout << "2. Edit Notice\n";
            cout << "3. Delete Notice\n";
            cout << "4. View Notices\n";
            cout << "5. Back to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    string message, category;
                    cout << "Enter Notice Message: ";
                    cin.ignore();
                    getline(cin, message);
                    cout << "Enter Notice Category: ";
                    cin >> category;
                    noticeBoard.addNotice(message, category);
                    break;
                }
                case 2: {
                    int id;
                    string newMessage;
                    cout << "Enter Notice ID: ";
                    cin >> id;
                    cout << "Enter New Message: ";
                    cin.ignore();
                    getline(cin, newMessage);
                    noticeBoard.editNotice(id, newMessage);
                    break;
                }
                case 3: {
                    int id;
                    cout << "Enter Notice ID: ";
                    cin >> id;
                    noticeBoard.deleteNotice(id);
                    break;
                }
                case 4:
                    noticeBoard.viewNotices();
                    break;
                case 5:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice! Please try again.\n";
            }
        } while (choice != 5);
    }

    void manageLibrary() {
        int choice;
        do {
            cout << "\nLibrary Management Menu\n";
            cout << "1. Add Book\n";
            cout << "2. Remove Book\n";
            cout << "3. Check Out Book\n";
            cout << "4. Return Book\n";
            cout << "5. View Books\n";
            cout << "6. Back to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    string book;
                    cout << "Enter Book Name: ";
                    cin >> book;
                    library.addBook(book);
                    break;
                }
                case 2: {
                    string book;
                    cout << "Enter Book Name: ";
                    cin >> book;
                    library.removeBook(book);
                    break;
                }
                case 3: {
                    string book, borrower;
                    cout << "Enter Book Name: ";
                    cin >> book;
                    cout << "Enter Borrower Name: ";
                    cin >> borrower;
                    library.checkOutBook(book, borrower);
                    break;
                }
                case 4: {
                    string book, borrower;
                    cout << "Enter Book Name: ";
                    cin >> book;
                    cout << "Enter Borrower Name: ";
                    cin >> borrower;
                    library.returnBook(book, borrower);
                    break;
                }
                case 5:
                    library.viewBooks();
                    break;
                case 6:
                    cout << "Returning to main menu ...\n";
                    break;
                default:
                    cout << "Invalid choice! Please try again.\n";
            }
        } while (choice != 6);
    }

    void manageHostel() {
        int choice;
        do {
            cout << "\nHostel Management Menu\n";
            cout << "1. Add Room\n";
            cout << "2. Assign Room\n";
            cout << "3. View Rooms\n";
            cout << "4. View Room Assignments\n";
            cout << "5. Back to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    string room;
                    cout << "Enter Room Number: ";
                    cin >> room;
                    hostel.addRoom(room);
                    break;
                }
                case 2: {
                    string room, student;
                    cout << "Enter Room Number: ";
                    cin >> room;
                    cout << "Enter Student Name: ";
                    cin >> student;
                    hostel.assignRoom(room, student);
                    break;
                }
                case 3:
                    hostel.viewRooms();
                    break;
                case 4:
                    hostel.viewRoomAssignments();
                    break;
                case 5:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice! Please try again.\n";
            }
        } while (choice != 5);
    }

    void manageEvents() {
        int choice;
        do {
            cout << "\nEvent Management Menu\n";
            cout << "1. Add Event\n";
            cout << "2. Edit Event\n";
            cout << "3. Delete Event\n";
            cout << "4. View Events\n";
            cout << "5. Back to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    string name, description, date, time, location;
                    cout << "Enter Event Name: ";
                    cin.ignore();
                    getline(cin, name);
                    cout << "Enter Event Description: ";
                    getline(cin, description);
                    cout << "Enter Event Date: ";
                    cin >> date;
                    cout << "Enter Event Time: ";
                    cin >> time;
                    cout << "Enter Event Location: ";
                    cin >> location;
                    eventManagementSystem.addEvent(name, description, date, time, location);
                    break;
                }
                case 2: {
                    int id;
                    string newName, newDescription, newDate, newTime, newLocation;
                    cout << "Enter Event ID: ";
                    cin >> id;
                    cout << "Enter New Event Name: ";
                    cin.ignore();
                    getline(cin, newName);
                    cout << "Enter New Event Description: ";
                    getline(cin, newDescription);
                    cout << "Enter New Event Date: ";
                    cin >> newDate;
                    cout << "Enter New Event Time: ";
                    cin >> newTime;
                    cout << "Enter New Event Location: ";
                    cin >> newLocation;
                    eventManagementSystem.editEvent(id, newName, newDescription, newDate, newTime, newLocation);
                    break;
                }
                case 3: {
                    int id;
                    cout << "Enter Event ID: ";
                    cin >> id;
                    eventManagementSystem.deleteEvent(id);
                    break;
                }
                case 4:
                    eventManagementSystem.viewEvents();
                    break;
                case 5:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice! Please try again.\n";
            }
        } while (choice != 5);
    }

    void run() {
        int choice;
        do {
            cout << "\nMain Menu\n";
            cout << "1. Register Student\n";
            cout << "2. Register Faculty\n";
            cout << "3. Login\n";
            cout << "4. Manage Notices\n";
            cout << "5. Manage Library\n";
            cout << "6. Manage Hostel\n";
            cout << "7. Manage Events\n";
            cout << "8. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    registerStudent();
                    break;
                case 2:
                    registerFaculty();
                    break;
                case 3: {
                    string username, password;
                    cout << "Enter Username: ";
                    cin >> username;
                    cout << "Enter Password: ";
                    cin >> password;
                    User* user = login(username, password);
                    if (user) {
                        user->menu();
                    } else {
                        cout << "Invalid login credentials.\n";
                    }
                    break;
                }
                case 4:
                    manageNotices();
                    break;
                case 5:
                    manageLibrary();
                    break;
                case 6:
                    manageHostel();
                    break;
                case 7:
                    manageEvents();
                    break;
                case 8:
                    cout << "Exiting...\n";
                    return;
                default:
                    cout << "Invalid choice! Please try again.\n";
            }
        } while (true);
    }
};

int main() {
    StudentInformationSystem sis;
    sis.run();
    return 0;
}