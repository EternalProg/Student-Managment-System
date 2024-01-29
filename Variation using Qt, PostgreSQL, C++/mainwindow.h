#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QDialog>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QMovie>
#include <QPushButton>
#include <QSqlQueryModel>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

enum SearchMode { SearchByName, SearchById, DeleteByName, DeleteById };

enum UpdateMode { UpdateName, UpdateAge, UpdateCourse, UpdateAllInformation };

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void searchByName(const QString &name);
  void searchById(int id);
  void deleteByName(const QString &name);
  void deleteById(int id);

  void updateName(int id);
  void updateAge(int id);
  void updateCourse(int id);
  void updateAllInformation(int id);

 private:
  QSqlDatabase db;
  QTableView *searchTableView;
  QDialog *searchDialog;
  QSqlQueryModel *searchModel;
  SearchMode searchMode;
  QComboBox *comboBox;
  UpdateMode updateMode;
 public slots:
  void addStudent();
  void displayStudents();
  void searchStudent();
  void updateStudent();
  void changeUpdateMode();
  void updateSearchButton();
  void deleteStudent();
  void searchButton();
  void changeMode();
};
#endif  // MAINWINDOW_H
