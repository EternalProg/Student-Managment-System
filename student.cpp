#include "student.h"

#include <vector>

size_t Student::count = 1;

enum Operation {
  ADD_STUDENT = 1,
  DISPLAY_STUDENTS,
  SEARCH_STUDENT,
  UPDATE_STUDENT,
  DELETE_STUDENT,
  EXIT
};

Student::Student() : _name("Unknown"), _course(0), _age(0), _id(count) {
  ++count;
}

Student::Student(std::string name, size_t course, size_t age, size_t id)
    : _name(name), _course(course), _age(age), _id(id) {
  ++count;
}

Student::~Student() { --count; }

std::string Student::getName() const { return _name; }

size_t Student::getCourse() const { return _course; }

size_t Student::getAge() const { return _age; }

size_t Student::getID() const { return _id; }

void Student::setAge(size_t age) { _age = age; }

void Student::setCourse(size_t course) { _course = course; }

void Student::setName(std::string name) { _name = name; }

Student *search(std::vector<Student> &database) {
  int operation;
  std::cout << "Find by: " << '\n';
  std::cout << "1. name" << '\n';
  std::cout << "2. id" << '\n';
  std::cout << "Enter choice: ";
  std::cin >> operation;
  switch (operation) {
    case 1: {
      std::string name;
      std::cout << "Enter Name: ";
      std::cin >> name;

      for (auto &it : database)
        if (it.getName() == name) return &it;
      break;
    }
    case 2: {
      size_t id;
      std::cout << "Enter ID: ";
      std::cin >> id;

      for (auto &it : database)
        if (it.getID() == id) return &it;
      break;
    }
  }
  return nullptr;
}

void searchStudent(std::vector<Student> &database) {
  auto student = search(database);
  if (student == nullptr) {
    std::cout << "Student doesn't exist" << '\n';
    return;
  }
  std::cout << "------------------------------------" << '\n';
  std::cout << "Name: " << student->getName() << '\n';
  std::cout << "Course: " << student->getCourse() << '\n';
  std::cout << "Age: " << student->getAge() << '\n';
  std::cout << "------------------------------------" << '\n';
}

void addNewStudent(std::vector<Student> &database) {
  size_t age, course;
  std::string name;

  std::cout << "Enter name : ";
  std::cin >> name;

  std::cout << "Enter course : ";
  std::cin >> course;

  std::cout << "Enter age : ";
  std::cin >> age;

  database.push_back(Student(name, course, age));
  std::cout << "Student Add succesfully" << std::endl;
}

void displayAllStudents(const std::vector<Student> &database) {
  std::cout << "Name"
            << "  "
            << "Course"
            << "   "
            << "Age"
            << "  "
            << "Id" << '\n';
  for (auto it : database) {
    std::cout << it.getName() << " " << it.getCourse() << " " << it.getAge()
              << " " << it.getID() << '\n';
  }
}

void updateStudent(std::vector<Student> &database) {
  auto student = search(database);

  if (student == nullptr) {
    std::cout << "Student doesn't exist" << '\n';
    return;
  }

  std::cout << "------------------------------------" << '\n';
  std::cout << "Name: " << student->getName() << '\n';
  std::cout << "Course: " << student->getCourse() << '\n';
  std::cout << "Age: " << student->getAge() << '\n';
  std::cout << "------------------------------------" << '\n';

  int choice = 0;
  std::cout << "What do you want to change: " << '\n';
  std::cout << " 1. Change Name" << '\n';
  std::cout << " 2. Change Course" << '\n';
  std::cout << " 3. Change Age" << '\n';
  std::cout << " 4. Change Everything" << '\n';

  std::cout << "Enter choice: ";
  std::cin >> choice;

  switch (choice) {
    case 1: {
      std::string name;
      std::cout << "Print new Name: ";
      std::cin >> name;
      student->setName(name);
      std::cout << "Name changed succesfully" << '\n';
      break;
    }
    case 2: {
      size_t course;
      std::cout << "Print new Course: ";
      std::cin >> course;
      student->setCourse(course);
      std::cout << "Course changed succesfully" << '\n';
      break;
    }
    case 3: {
      size_t age;
      std::cout << "Print new Age: ";
      std::cin >> age;
      student->setAge(age);
      break;
    }
    case 4: {
      std::string name;
      size_t course, age;

      std::cout << "Print new Name: ";
      std::cin >> name;

      std::cout << "Print new Course: ";
      std::cin >> course;

      std::cout << "Print new Age: ";
      std::cin >> age;

      student->setName(name);
      student->setCourse(course);
      student->setAge(age);

      std::cout << "All information changed succesfully" << '\n';
      break;
    }
  }
}

void deleteStudent(std::vector<Student> &database) {
  auto student = search(database);

  if (student == nullptr) {
    std::cout << "Student doesn't exist" << '\n';
    return;
  }
  std::cout << "Student " << student->getName() << " #" << student->getID()
            << " succesfully deleted" << '\n';
  database.erase(static_cast<std::vector<Student>::iterator>(student));
}

void printMenu() {
  using std::cout;
  cout << " 1. Add New Student" << '\n';
  cout << " 2. Display All Students" << '\n';
  cout << " 3. Search Student" << '\n';
  cout << " 4. Update Student" << '\n';
  cout << " 5. Delete Student" << '\n';
  cout << " 6. Exit" << '\n';
  cout << "Enter your choice : ";
}

int main() {
  using namespace std;
  vector<Student> students;
  students.push_back(Student("Ivan", 1, 17));

  int operation = 0;
  while (operation != Operation::EXIT) {
    printMenu();
    cin >> operation;

    switch (operation) {
      case Operation::ADD_STUDENT:
        addNewStudent(students);
        break;
      case Operation::DISPLAY_STUDENTS:
        displayAllStudents(students);
        break;
      case Operation::SEARCH_STUDENT:
        searchStudent(students);
        break;
      case Operation::UPDATE_STUDENT:
        updateStudent(students);
        break;
      case Operation::DELETE_STUDENT:
        deleteStudent(students);
        break;
      case Operation::EXIT:
        std::cout << "See you next time!" << '\n';
        break;
      default:
        cout << "Invalid operation" << '\n';
    }
  }

  return 0;
}
