/*
Student Managment System

1. Add new Student
2. Display All Student
3. Search Student
4. Update Student
5. Delete Student
6. Exit
Enter your Choice:
*/
#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>

class Student {
 private:
  size_t _id;
  size_t _age;
  size_t _course;
  std::string _name;
  static size_t count;

 public:
  Student();
  Student(std::string name, size_t course = 0, size_t age = 0,
          size_t id = count);

  ~Student();

  size_t getID() const;
  size_t getAge() const;
  size_t getCourse() const;
  std::string getName() const;

  void setAge(size_t age);
  void setCourse(size_t coruse);
  void setName(std::string name);
};

#endif
