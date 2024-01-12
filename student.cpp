#include <iostream>
#include <pqxx/pqxx>
#include <string>

enum Operation {
  ADD_STUDENT = 1,
  DISPLAY_STUDENTS,
  SEARCH_STUDENT,
  UPDATE_STUDENT,
  DELETE_STUDENT,
  EXIT
};

pqxx::result search(pqxx::work &database) {
  int operation;
  std::cout << "Find by: " << '\n';
  std::cout << "1. name" << '\n';
  std::cout << "2. id" << '\n';
  std::cout << "Enter choice: ";
  std::cin >> operation;

  std::string sql;

  switch (operation) {
    case 1: {
      std::string name;
      std::cout << "Enter Name: ";
      std::cin >> name;
      sql = "SELECT * FROM students WHERE name = '" + name + "'";
      break;
    }
    case 2: {
      std::string id;
      std::cout << "Enter ID: ";
      std::cin >> id;
      sql = "SELECT * FROM students WHERE id = " + id;
      break;
    }
  }

  return database.exec(sql);
}

void searchStudent(pqxx::work &database) {
  auto studentInfo = search(database);
  if (studentInfo.empty()) {
    std::cout << "Student doesn't exist" << '\n';
    return;
  }
  std::cout << "------------------------------------" << '\n';
  std::cout << "Name: " << studentInfo[0]["name"].c_str() << '\n';
  std::cout << "Course: " << studentInfo[0]["course"].as<size_t>() << '\n';
  std::cout << "Age: " << studentInfo[0]["age"].as<size_t>() << '\n';
  std::cout << "------------------------------------\n";
}

void addNewStudent(pqxx::work &database) {
  size_t age, course;
  std::string name;

  std::cout << "Enter name : ";
  std::cin >> name;

  std::cout << "Enter course : ";
  std::cin >> course;

  std::cout << "Enter age : ";
  std::cin >> age;

  std::string sql =
      "INSERT INTO students(name, course, age) VALUES ($1, $2, $3)";
  database.exec_params(sql, name, course, age);

  std::cout << "Student Add succesfully\n";
}

void updateStudent(pqxx::work &database) {
  auto studentInfo = search(database);

  if (studentInfo.empty()) {
    std::cout << "Student doesn't exist" << '\n';
    return;
  }

  std::string name = studentInfo[0]["name"].c_str();
  std::string course = studentInfo[0]["course"].c_str();
  std::string age = studentInfo[0]["age"].c_str();
  std::string id = studentInfo[0]["id"].c_str();

  std::cout << "------------------------------------\n";
  std::cout << "Name: " << name << '\n';
  std::cout << "Course: " << course << '\n';
  std::cout << "Age: " << age << '\n';
  std::cout << "------------------------------------\n";

  int choice = 0;
  std::cout << "What do you want to change: " << '\n';
  std::cout << " 1. Change Name" << '\n';
  std::cout << " 2. Change Course" << '\n';
  std::cout << " 3. Change Age" << '\n';
  std::cout << " 4. Change Everything" << '\n';

  std::cout << "Enter choice: ";
  std::cin >> choice;

  std::string sql;

  switch (choice) {
    case 1: {
      std::string newName;
      std::cout << "Print new Name: ";
      std::cin >> newName;

      sql = "UPDATE students SET name = '" + newName + "' WHERE id = " + id;
      break;
    }
    case 2: {
      std::string newCourse;
      std::cout << "Print new Course: ";
      std::cin >> newCourse;

      sql = "UPDATE students SET course = " + newCourse + " WHERE id = " + id;
      break;
    }
    case 3: {
      std::string newAge;
      std::cout << "Print new Age: ";
      std::cin >> newAge;

      sql = "UPDATE students SET age = " + newAge + " WHERE id = " + id;
      break;
    }
    case 4: {
      std::string newName, newCourse, newAge;

      std::cout << "Print new Name: ";
      std::cin >> newName;

      std::cout << "Print new Course: ";
      std::cin >> newCourse;

      std::cout << "Print new Age: ";
      std::cin >> newAge;

      sql = "UPDATE students SET name = '" + newName +
            "', course = " + newCourse + ", age = " + newAge +
            " WHERE id = " + id;
      break;
    }
  }
  try {
    database.exec(sql);
    std::cout << "Student information updated successfully" << '\n';
  } catch (const std::exception &e) {
    std::cerr << "Error executing SQL query: " << e.what() << std::endl;
  }
}

void deleteStudent(pqxx::work &database) {
  int operation = 0;
  std::cout << "------------------------" << '\n';
  std::cout << " 1. Delete by name\n";
  std::cout << " 2. Delete by id\n";
  std::cout << "Enter your choice: ";
  std::cin >> operation;

  std::string sql;
  std::string deletedStudentInfo;

  switch (operation) {
    case 1: {
      std::string name;
      std::cout << "Enter name: ";
      std::cin >> name;
      sql = "DELETE FROM students WHERE name = '" + name + "'";
      deletedStudentInfo = "Student with Name " + name;
      break;
    }
    case 2: {
      std::string id;
      std::cout << "Enter id: ";
      std::cin >> id;
      sql = "DELETE FROM students WHERE id = " + id;
      deletedStudentInfo = "Student with id " + id;
      break;
    }
  }

  try {
    database.exec(sql);

    std::cout << deletedStudentInfo << " deleted successfully.\n";
  } catch (const std::exception &e) {
    std::cerr << "Error executing SQL query: " << e.what() << std::endl;
  }
}

void printMenu() {
  using std::cout;
  cout << " 1. Add New Student\n";
  cout << " 2. Display All Students\n";
  cout << " 3. Search Student\n";
  cout << " 4. Update Student\n";
  cout << " 5. Delete Student\n";
  cout << " 6. Exit\n";
  cout << "Enter your choice : ";
}

void displayAllStudents(pqxx::work &database) {
  try {
    std::string sql = "SELECT name, course, age, id FROM students";
    pqxx::result result = database.exec(sql);

    std::cout << "Name Course Age   ID\n";

    for (const auto &row : result) {
      std::cout << row.at("name").c_str() << " " << row.at("course").as<int>()
                << " " << row.at("age").as<int>() << "  "
                << row.at("id").as<int>() << '\n';
    }
  } catch (const std::exception &e) {
    std::cerr << "Error executing SQL query: " << e.what() << std::endl;
  }
}

int main() {
  using namespace std;

  try {
    pqxx::connection conn(
        "user=postgres password=admin123 host=localhost port=5432 dbname=mydb "
        "target_session_attrs=read-write");
    if (conn.is_open()) {
      std::cout << "Connected to database successfully: " << conn.dbname()
                << std::endl;
      pqxx::work database(conn);

      int operation = 0;
      while (operation != Operation::EXIT) {
        printMenu();
        cin >> operation;

        switch (operation) {
          case Operation::ADD_STUDENT:
            addNewStudent(database);
            break;
          case Operation::DISPLAY_STUDENTS:
            displayAllStudents(database);
            break;
          case Operation::SEARCH_STUDENT:
            searchStudent(database);
            break;
          case Operation::UPDATE_STUDENT:
            updateStudent(database);
            break;
          case Operation::DELETE_STUDENT:
            deleteStudent(database);
            break;
          case Operation::EXIT:
            std::cout << "See you next time!" << '\n';
            break;
          default:
            cout << "Invalid operation" << '\n';
        }
      }
      database.commit();
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
