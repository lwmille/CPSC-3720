#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

// Forward declaration
class Session;

class Course {
public:
    std::string courseName;

    Course(const std::string& name) : courseName(name) {}
};

class AvailabilitySlot {
public:
    std::string date;      // YYYY-MM-DD
    std::string startTime; // HH:MM (24h)
    std::string endTime;

    AvailabilitySlot(const std::string& d, const std::string& start, const std::string& end)
        : date(d), startTime(start), endTime(end) {}

    void print() const {
        std::cout << date << " " << startTime << "-" << endTime << std::endl;
    }
};

enum SessionStatus { Proposed, Confirmed, Rejected };

class Session {
public:
        static int nextSessionId;
        int sessionId;
        std::vector<std::string> participants; // usernames
        std::string date;
        std::string startTime;
        std::string endTime;
        SessionStatus status;

        Session() : sessionId(0), status(Proposed) {}

        Session(const std::vector<std::string>& users, const std::string& d,
                const std::string& start, const std::string& end)
            : participants(users), date(d), startTime(start), endTime(end), status(Proposed) {
            sessionId = nextSessionId++;
        }

        void print() const {
            std::cout << "Session ID: " << sessionId << "\nDate: " << date << " " << startTime << "-" << endTime
                    << "\nParticipants: ";
            for (const auto& user : participants)
                std::cout << user << " ";
            std::cout << "\nStatus: " << (status == Proposed ? "Proposed" : status == Confirmed ? "Confirmed" : "Rejected") << std::endl;
        }
};

int Session::nextSessionId = 1;

class Student {
private:
        std::string username;
        std::string password;
        std::vector<Course> courses;
        std::vector<AvailabilitySlot> availability;
        std::vector<int> sessionIds; // IDs of sessions student is part of

public:
    Student() = default;

    Student(const std::string& user, const std::string& pass)
        : username(user), password(pass) {}

    const std::string& getUsername() const { return username; }

    bool checkPassword(const std::string& pass) const { return password == pass; }

    void addCourse(const std::string& courseName) {
        auto it = std::find_if(courses.begin(), courses.end(), [&](const Course& c) {
            return c.courseName == courseName;
        });
        if (it == courses.end()) {
            courses.emplace_back(courseName);
            std::cout << "Course added: " << courseName << std::endl;
        } else {
            std::cout << "Course already exists in profile." << std::endl;
        }
    }

    void removeCourse(const std::string& courseName) {
        auto it = std::remove_if(courses.begin(), courses.end(), [&](const Course& c) {
            return c.courseName == courseName;
        });
        if (it != courses.end()) {
            courses.erase(it, courses.end());
            std::cout << "Course removed: " << courseName << std::endl;
        } else {
            std::cout << "Course not found in profile." << std::endl;
        }
    }

    void listCourses() const {
        std::cout << "Courses for " << username << ":" << std::endl;
        for (const auto& c : courses) {
            std::cout << "- " << c.courseName << std::endl;
        }
    }

    void addAvailability(const std::string& date, const std::string& start, const std::string& end) {
        // Could add validation of date/time format here
        availability.emplace_back(date, start, end);
        std::cout << "Availability added: ";
        availability.back().print();
    }

    void removeAvailability(const std::string& date, const std::string& start) {
        auto it = std::remove_if(availability.begin(), availability.end(), [&](const AvailabilitySlot& a) {
            return a.date == date && a.startTime == start;
        });
        if (it != availability.end()) {
            availability.erase(it, availability.end());
            std::cout << "Availability removed for " << date << " at " << start << std::endl;
        } else {
            std::cout << "Availability slot not found." << std::endl;
        }
    }

    void listAvailability() const {
        std::cout << "Availability for " << username << ":" << std::endl;
        for (const auto& slot : availability) {
            slot.print();
        }
    }

    const std::vector<Course>& getCourses() const { return courses; }

    const std::vector<AvailabilitySlot>& getAvailability() const { return availability; }

    void addSession(int sessionId) {
        sessionIds.push_back(sessionId);
    }

    const std::vector<int>& getSessionIds() const { return sessionIds; }
};

// Database simulation for users and sessions
class SchedulerApp {
private:
        std::map<std::string, Student> users;    // username -> Student
        std::map<int, Session> sessions;         // sessionId -> Session
        Student* loggedInUser = nullptr;

public:
    void registerUser(const std::string& username, const std::string& password) {
        if (users.find(username) != users.end()) {
            std::cout << "Username already exists." << std::endl;
            return;
        }
        users.emplace(username, Student(username, password));
        std::cout << "User registered successfully." << std::endl;
    }

    bool loginUser(const std::string& username, const std::string& password) {
        auto it = users.find(username);
        if (it == users.end()) {
            std::cout << "User not found." << std::endl;
            return false;
        }
        if (!it->second.checkPassword(password)) {
            std::cout << "Incorrect password." << std::endl;
            return false;
        }
        loggedInUser = &(it->second);
        std::cout << "Logged in as " << username << std::endl;
        return true;
    }

    void logout() {
        loggedInUser = nullptr;
        std::cout << "Logged out." << std::endl;
    }

    void addCourse(const std::string& course) {
        if (!loggedInUser) {
            std::cout << "Please login first." << std::endl;
            return;
        }
        loggedInUser->addCourse(course);
    }

    void removeCourse(const std::string& course) {
        if (!loggedInUser) {
            std::cout << "Please login first." << std::endl;
            return;
        }
        loggedInUser->removeCourse(course);
    }

    void listCourses() {
        if (!loggedInUser) {
            std::cout << "Please login first." << std::endl;
            return;
        }
        loggedInUser->listCourses();
    }

    void addAvailability(const std::string& date, const std::string& start, const std::string& end) {
        if (!loggedInUser) {
            std::cout << "Please login first." << std::endl;
            return;
        }
        loggedInUser->addAvailability(date, start, end);
    }

    void removeAvailability(const std::string& date, const std::string& start) {
        if (!loggedInUser) {
            std::cout << "Please login first." << std::endl;
            return;
        }
        loggedInUser->removeAvailability(date, start);
    }

    void listAvailability() {
        if (!loggedInUser) {
            std::cout << "Please login first." << std::endl;
            return;
        }
        loggedInUser->listAvailability();
    }

    // Search users by course
    void searchUsersByCourse(const std::string& courseName) {
        std::cout << "Students enrolled in " << courseName << ":" << std::endl;
        for (const auto& [username, student] : users) {
            for (const auto& course : student.getCourses()) {
                if (course.courseName == courseName) {
                    std::cout << "- " << username << std::endl;
                    break;
                }
            }
        }
    }

    // Simplified match suggestion based on shared courses and any overlapping availability date/time (basic check)
    void suggestMatches() {
        if (!loggedInUser) {
            std::cout << "Please login first." << std::endl;
            return;
        }

        std::cout << "Suggested matches based on your courses and availability:" << std::endl;

        for (const auto& [username, student] : users) {
            if (username == loggedInUser->getUsername())
                continue;

            // Check shared courses
            bool sharedCourse = false;
            for (const auto& myCourse : loggedInUser->getCourses()) {
                for (const auto& theirCourse : student.getCourses()) {
                    if (myCourse.courseName == theirCourse.courseName) {
                        sharedCourse = true;
                        break;
                    }
                }
                if (sharedCourse) break;
            }
            if (!sharedCourse) continue;

            // Check overlapping availability - naive check only by date (could be improved)
            bool availabilityOverlap = false;
            for (const auto& myAvail : loggedInUser->getAvailability()) {
                for (const auto& theirAvail : student.getAvailability()) {
                    if (myAvail.date == theirAvail.date) {
                        availabilityOverlap = true;
                        break;
                    }
                }
                if (availabilityOverlap) break;
            }
            if (availabilityOverlap) {
                std::cout << "- " << username << std::endl;
            }
        }
    }

    // Propose a session with another user at a specific time
    void proposeSession(const std::string& otherUser, const std::string& date,
                        const std::string& start, const std::string& end) {
        if (!loggedInUser) {
            std::cout << "Please login first." << std::endl;
            return;
        }
        if (users.find(otherUser) == users.end()) {
            std::cout << "User " << otherUser << " does not exist." << std::endl;
            return;
        }
        if (otherUser == loggedInUser->getUsername()) {
            std::cout << "Cannot propose session with yourself." << std::endl;
            return;
        }

        // Create session
        std::vector<std::string> participants = {loggedInUser->getUsername(), otherUser};
        Session newSession(participants, date, start, end);
        int sessionId = newSession.sessionId;
        sessions[sessionId] = newSession;

        // Add session to users
        loggedInUser->addSession(sessionId);
        users[otherUser].addSession(sessionId);

        std::cout << "Session proposed with " << otherUser << ". Session ID: " << sessionId << std::endl;
    }

    void confirmSession(int sessionId) {
        if (!loggedInUser) {
            std::cout << "Please login first." << std::endl;
            return;
        }
        auto it = sessions.find(sessionId);
        if (it == sessions.end()) {
            std::cout << "Session not found." << std::endl;
            return;
        }
        Session& sess = it->second;

        // Only participants can confirm
        if (std::find(sess.participants.begin(), sess.participants.end(), loggedInUser->getUsername()) == sess.participants.end()) {
            std::cout << "You are not a participant of this session." << std::endl;
            return;
        }
        if (sess.status == Confirmed) {
            std::cout << "Session already confirmed." << std::endl;
            return;
        }

        //Check for overlap
        for (const auto& participant : sess.participants) {
            const Student& user = users[participant];
            for (int sid : user.getSessionIds()) {
                if (sid == sessionId) continue;
                const Session& other = sessions[sid];
                if (other.status != Confirmed) continue;

                if (sess.date == other.date) {
                    if (!(sess.endTime <= other.startTime || sess.startTime >= other.endTime)) {
                        std::cout << "Cannot confirm session. It overlaps with session "
                                << other.sessionId << " for user " << participant << "." << std::endl;
                        return;
                    }
                }
            }
        }
        sess.status = Confirmed;
        std::cout << "Session " << sessionId << " confirmed." << std::endl;
    }

    void viewSessions() {
        if (!loggedInUser) {
            std::cout << "Please login first." << std::endl;
            return;
        }
        std::cout << "Your sessions:" << std::endl;
        for (int sid : loggedInUser->getSessionIds()) {
            sessions[sid].print();
            std::cout << "-----" << std::endl;
        }
    }
};

// Simple CLI loop
void printHelp() {
    std::cout << "Available commands:\n"
              << "register [username] [password]\n"
              << "login [username] [password]\n"
              << "logout\n"
              << "add_course [course_name]\n"
              << "remove_course [course_name]\n"
              << "list_courses\n"
              << "add_availability [date] [start] [end]\n"
              << "remove_availability [date] [start]\n"
              << "list_availability\n"
              << "search [course_name]\n"
              << "suggest_matches\n"
              << "propose_session [username] [date] [start] [end]\n"
              << "confirm_session [session_id]\n"
              << "view_sessions\n"
              << "help\n"
              << "clear\n"
              << "exit\n";
}

void clearScreen() {
	std::cout << "\033[H\033[2J"; 
}

int main() {
    SchedulerApp app;
    std::string line;

    std::cout << "Welcome to the Study Scheduler CLI App!" << std::endl;
    printHelp();

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line.empty()) continue;

        std::vector<std::string> tokens;
        size_t pos = 0, found;
        while ((found = line.find_first_of(' ', pos)) != std::string::npos) {
            tokens.push_back(line.substr(pos, found - pos));
            pos = found + 1;
        }
        tokens.push_back(line.substr(pos));

        const std::string& cmd = tokens[0];

        if (cmd == "register" && tokens.size() == 3) {
            app.registerUser(tokens[1], tokens[2]);
        } else if (cmd == "login" && tokens.size() == 3) {
            app.loginUser(tokens[1], tokens[2]);
        } else if (cmd == "logout") {
            app.logout();
        } else if (cmd == "add_course" && tokens.size() == 2) {
            app.addCourse(tokens[1]);
        } else if (cmd == "remove_course" && tokens.size() == 2) {
            app.removeCourse(tokens[1]);
        } else if (cmd == "list_courses") {
            app.listCourses();
        } else if (cmd == "add_availability" && tokens.size() == 4) {
            app.addAvailability(tokens[1], tokens[2], tokens[3]);
        } else if (cmd == "remove_availability" && tokens.size() == 4) {
            app.removeAvailability(tokens[1], tokens[2]);
        } else if (cmd == "list_availability") {
            app.listAvailability();
        } else if (cmd == "search" && tokens.size() == 2) {
            app.searchUsersByCourse(tokens[1]);
        } else if (cmd == "suggest_matches") {
            app.suggestMatches();
        } else if (cmd == "propose_session" && tokens.size() == 5) {
            app.proposeSession(tokens[1], tokens[2], tokens[3], tokens[4]);
        } else if (cmd == "confirm_session" && tokens.size() == 2) {
            int sid = std::stoi(tokens[1]);
            app.confirmSession(sid);
        } else if (cmd == "view_sessions") {
            app.viewSessions();
        } else if (cmd == "help") {
            printHelp();
        } else if (cmd == "exit") {
            std::cout << "Goodbye!" << std::endl;
            break;
	    } else if (cmd == "clear") {
            // Note: this only works with Unix terminals, not windows
            std::cout << "\033[H\033[2J";
        } else {
            std::cout << "Unknown command or wrong arguments. Type 'help' for commands." << std::endl;
        }
    }

    return 0;
}
