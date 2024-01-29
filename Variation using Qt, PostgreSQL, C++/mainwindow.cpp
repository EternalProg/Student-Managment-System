#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Student Managment System");
  setFixedSize(1280, 720);

  QString buttonStyle =
      "QPushButton{ padding:0.7em 1.4em; margin:0 0.3em 0.3em 0; "
      "border-radius:0.15em; text-decoration:none; "
      "font-family:'Roboto',sans-serif; text-transform:uppercase; "
      "font-weight:400; color:#FFFFFF; background-color:#483D8B; "
      "text-align:center; position:relative;} QPushButton:pressed { "
      "border:0.1em solid #443b8f; background-color: qlineargradient(x1: 0, "
      "y1: 0, x2: 0, y2: 1, stop: 0 #480D8F, stop: 1 #480D8F); }";

  QWidget *widget = new QWidget(this);
  QVBoxLayout *mainLayout = new QVBoxLayout(widget);
  QVBoxLayout *vbox = new QVBoxLayout();

  QMovie *background = new QMovie(":/images/space.gif");
  QLabel *movieLabel = new QLabel(widget);
  movieLabel->setGeometry(0, 0, 1280, 720);
  movieLabel->setMovie(background);
  background->start();

  db = QSqlDatabase::addDatabase("QPSQL");
  db.setHostName("localhost");
  db.setDatabaseName("mydb");
  db.setUserName("postgres");
  db.setPassword("admin123");

  if (!db.open()) {
    qDebug() << db.lastError().text();
    QMessageBox::warning(this, "ERROR", "Failed to connect to the database");
  }

  QPushButton *addStudentBtn = new QPushButton("Add New Student");
  addStudentBtn->setFixedSize(250, 70);
  addStudentBtn->setStyleSheet(buttonStyle);
  connect(addStudentBtn, &QPushButton::clicked, this, &MainWindow::addStudent);

  QPushButton *displayStudentsBtn = new QPushButton("Display All Students");
  displayStudentsBtn->setFixedSize(250, 70);
  displayStudentsBtn->setStyleSheet(buttonStyle);
  connect(displayStudentsBtn, &QPushButton::clicked, this,
          &MainWindow::displayStudents);

  QPushButton *searchStudentBtn = new QPushButton("Search Student");
  searchStudentBtn->setFixedSize(250, 70);
  searchStudentBtn->setStyleSheet(buttonStyle);
  connect(searchStudentBtn, &QPushButton::clicked, this,
          &MainWindow::searchStudent);

  QPushButton *updateStudentBtn = new QPushButton("Update Student");
  updateStudentBtn->setFixedSize(250, 70);
  updateStudentBtn->setStyleSheet(buttonStyle);
  connect(updateStudentBtn, &QPushButton::clicked, this,
          &MainWindow::updateStudent);

  QPushButton *deleteStudentBtn = new QPushButton("Delete Student");
  deleteStudentBtn->setFixedSize(250, 70);
  deleteStudentBtn->setStyleSheet(buttonStyle);
  connect(deleteStudentBtn, &QPushButton::clicked, this,
          &MainWindow::deleteStudent);

  vbox->addWidget(addStudentBtn);
  vbox->addWidget(displayStudentsBtn);
  vbox->addWidget(searchStudentBtn);
  vbox->addWidget(updateStudentBtn);
  vbox->addWidget(deleteStudentBtn);

  mainLayout->addLayout(vbox);
  mainLayout->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
  setCentralWidget(widget);
}

MainWindow::~MainWindow() {}

void MainWindow::searchByName(const QString &name) {
  searchModel->setQuery(
      "SELECT name, course, age, id FROM students WHERE name = '" + name + "'");

  if (searchModel->rowCount() > 0) {
    searchTableView->setModel(searchModel);
  } else {
    QMessageBox::warning(this, "Search Result",
                         "No student found with the given Name.");
  }
}

void MainWindow::searchById(int id) {
  searchModel->setQuery(
      "SELECT name, course, age, id FROM students WHERE id = " +
      QString::number(id));

  if (searchModel->rowCount() > 0) {
    searchTableView->setModel(searchModel);
  } else {
    QMessageBox::warning(this, "Search Result",
                         "No student found with the given ID.");
  }
}

void MainWindow::deleteByName(const QString &name) {
  QSqlQuery query(db);
  query.prepare("DELETE FROM students WHERE name = '" + name + "'");
  if (searchModel->rowCount() > 0 && query.exec()) {
    qDebug() << "Student deleted from the database.";
    QMessageBox::information(this, "Info",
                             "Student deleted from the database.");
  } else {
    QMessageBox::warning(this, "ERROR",
                         "Error deleting student from the database.");
    qDebug() << "Error deleting student from the database:"
             << query.lastError().text();
  }
}

void MainWindow::deleteById(int id) {
  QSqlQuery query(db);
  query.prepare("DELETE FROM students WHERE id = " + QString::number(id));
  if (searchModel->rowCount() > 0 && query.exec()) {
    qDebug() << "Student deleted from the database.";
    QMessageBox::information(this, "Info",
                             "Student deleted from the database.");
  } else {
    QMessageBox::warning(this, "ERROR",
                         "Error deleting student from the database.");
    qDebug() << "Error deleting student from the database:"
             << query.lastError().text();
  }
}

void MainWindow::updateName(int id) {
  QSqlQuery query(db);
  QString newName =
      QInputDialog::getText(this, "Update Information", "New Name:");
  query.prepare("UPDATE students SET name = '" + newName +
                "' WHERE id = " + QString::number(id));
  if (searchModel->rowCount() > 0 && query.exec()) {
    qDebug() << "Student set new name successfully";
    QMessageBox::information(this, "Info", "Student set new name successfully");
  } else {
    QMessageBox::warning(this, "ERROR", "Error changing student name");
    qDebug() << "Error changing student name: " << query.lastError().text();
  }
}

void MainWindow::updateAge(int id) {
  QSqlQuery query(db);
  int newAge = QInputDialog::getInt(this, "Update Information", "New Age:", 1,
                                    1, 100, 1);
  query.prepare("UPDATE students SET age = " + QString::number(newAge) +
                " WHERE id = " + QString::number(id));
  if (searchModel->rowCount() > 0 && query.exec()) {
    qDebug() << "Student set new age successfully";
    QMessageBox::information(this, "Info", "Student set new age successfully");
  } else {
    QMessageBox::warning(this, "ERROR", "Error changing student age");
    qDebug() << "Error changing student age: " << query.lastError().text();
  }
}

void MainWindow::updateCourse(int id) {
  QSqlQuery query(db);
  int newCourse = QInputDialog::getInt(this, "Update Information",
                                       "New Course:", 1, 1, 5, 1);
  query.prepare("UPDATE students SET course = " + QString::number(newCourse) +
                " WHERE id = " + QString::number(id));
  if (searchModel->rowCount() > 0 && query.exec()) {
    qDebug() << "Student set new course successfully";
    QMessageBox::information(this, "Info",
                             "Student set new course successfully");
  } else {
    QMessageBox::warning(this, "ERROR", "Error changing student course");
    qDebug() << "Error changing student course: " << query.lastError().text();
  }
}

void MainWindow::updateAllInformation(int id) {
  QSqlQuery query(db);
  QString newName =
      QInputDialog::getText(this, "Update Information", "New Name:");
  int newAge = QInputDialog::getInt(this, "Update Information", "New Age:", 1,
                                    1, 100, 1);
  int newCourse = QInputDialog::getInt(this, "Update Information",
                                       "New Course:", 1, 1, 5, 1);

  query.prepare("Update students SET name = '" + newName +
                "', course = " + QString::number(newCourse) + ", age = " +
                QString::number(newAge) + " WHERE id = " + QString::number(id));
  if (searchModel->rowCount() > 0 && query.exec()) {
    qDebug() << "Student set new information successfully";
    QMessageBox::information(this, "Info",
                             "Student set new information successfully");
  } else {
    QMessageBox::warning(this, "ERROR", "Error changing student information");
    qDebug() << "Error changing student information: "
             << query.lastError().text();
  }
}

void MainWindow::addStudent() {
  // Зробити, щоб можна було вводити лише додатні значення
  QString name =
      QInputDialog::getText(this, "Enter Student Information", "Name:");

  int age = QInputDialog::getInt(this, "Enter Student Information", "Age:", 1,
                                 1, 100, 1);
  int course = QInputDialog::getInt(this, "Enter Student Information",
                                    "Course:", 1, 1, 5, 1);

  QSqlQuery query(db);
  query.prepare(
      "INSERT INTO students (name, age, course) VALUES (:name, :age, :course)");
  query.bindValue(":name", name);
  query.bindValue(":age", age);
  query.bindValue(":course", course);

  if (query.exec()) {
    qDebug() << "Student added to the database.";
    QMessageBox::information(this, "Info", "Student added to the database.");
  } else {
    QMessageBox::warning(this, "ERROR",
                         "Error adding student to the database.");
    qDebug() << "Error adding student to the database:"
             << query.lastError().text();
  }
}

void MainWindow::displayStudents() {
  QSqlQueryModel *model = new QSqlQueryModel();
  model->setQuery("SELECT name, course, age, id FROM students");

  QTableView *tableView = new QTableView();
  tableView->setModel(model);

  searchTableView = tableView;
  QDialog *dialog = new QDialog(this);
  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(tableView);
  dialog->setFixedSize(437, 400);
  dialog->setLayout(layout);

  searchDialog = dialog;

  dialog->exec();
}
void MainWindow::searchStudent() {
  QVBoxLayout *vboxLayout = new QVBoxLayout();
  QHBoxLayout *hboxLayout = new QHBoxLayout();
  QDialog *dialog = new QDialog(this);
  dialog->setFixedSize(600, 150);
  dialog->setWindowTitle("Search Student Panel");

  QTableView *tableView = new QTableView();
  tableView->setFixedSize(400, 120);

  searchTableView = tableView;

  comboBox = new QComboBox();
  comboBox->addItem("Find by name");
  comboBox->addItem("Find by id");
  connect(comboBox, &QComboBox::currentTextChanged, this,
          &MainWindow::changeMode);

  QPushButton *searchBtn = new QPushButton("Search");
  connect(searchBtn, &QPushButton::clicked, this, &MainWindow::searchButton);

  vboxLayout->addWidget(comboBox);
  vboxLayout->addWidget(searchBtn);
  hboxLayout->addLayout(vboxLayout);
  hboxLayout->addWidget(tableView);

  dialog->setLayout(hboxLayout);
  searchDialog = dialog;

  searchModel = new QSqlQueryModel(this);

  dialog->exec();
}

void MainWindow::updateStudent() {
  QVBoxLayout *vboxLayout = new QVBoxLayout();
  QHBoxLayout *hboxLayout = new QHBoxLayout();
  QDialog *dialog = new QDialog(this);
  dialog->setFixedSize(600, 150);
  dialog->setWindowTitle("Update Student Panel");

  QTableView *tableView = new QTableView();
  tableView->setFixedSize(400, 120);

  searchTableView = tableView;

  comboBox = new QComboBox();
  comboBox->addItem("Update name");
  comboBox->addItem("Update age");
  comboBox->addItem("Update course");
  comboBox->addItem("Update all information");
  connect(comboBox, &QComboBox::currentTextChanged, this,
          &MainWindow::changeUpdateMode);

  QPushButton *searchBtn = new QPushButton("Search");
  connect(searchBtn, &QPushButton::clicked, this,
          &MainWindow::updateSearchButton);

  vboxLayout->addWidget(comboBox);
  vboxLayout->addWidget(searchBtn);
  hboxLayout->addLayout(vboxLayout);
  hboxLayout->addWidget(tableView);

  dialog->setLayout(hboxLayout);
  searchDialog = dialog;

  searchModel = new QSqlQueryModel(this);

  dialog->exec();
}

void MainWindow::changeUpdateMode() {
  if (comboBox->currentText() == "Update name") {
    updateMode = UpdateName;
  }
  if (comboBox->currentText() == "Update age") {
    updateMode = UpdateAge;
  }
  if (comboBox->currentText() == "Update course") {
    updateMode = UpdateCourse;
  }
  if (comboBox->currentText() == "Update all information") {
    updateMode = UpdateAllInformation;
  }
}

void MainWindow::updateSearchButton() {
  bool ok;
  int id = QInputDialog::getInt(this, "Enter student ID", "Enter ID:", 1, 1,
                                1000, 1, &ok);

  if (!ok) {
    return;
  }

  searchById(id);

  switch (updateMode) {
    case UpdateName:
      updateName(id);
      break;
    case UpdateCourse:
      updateCourse(id);
      break;
    case UpdateAge:
      updateAge(id);
      break;
    case UpdateAllInformation:
      updateAllInformation(id);
      break;
  }
}

void MainWindow::deleteStudent() {
  QVBoxLayout *mainLayout = new QVBoxLayout();
  QHBoxLayout *hboxLayout = new QHBoxLayout();
  QDialog *deleteDialog = new QDialog(this);
  deleteDialog->setFixedSize(600, 150);
  deleteDialog->setWindowTitle("Delete Student Panel");

  QTableView *tableView = new QTableView();
  tableView->setFixedSize(400, 120);

  searchTableView = tableView;

  comboBox = new QComboBox();
  comboBox->addItem("Delete by name");
  comboBox->addItem("Delete by id");
  connect(comboBox, &QComboBox::currentTextChanged, this,
          &MainWindow::changeMode);

  QPushButton *searchBtn = new QPushButton("Search");
  connect(searchBtn, &QPushButton::clicked, this, &MainWindow::searchButton);

  mainLayout->addWidget(comboBox);
  mainLayout->addWidget(searchBtn);
  hboxLayout->addLayout(mainLayout);
  hboxLayout->addWidget(tableView);

  deleteDialog->setLayout(hboxLayout);
  searchDialog = deleteDialog;

  searchModel = new QSqlQueryModel(this);

  deleteDialog->exec();
}

void MainWindow::searchButton() {
  switch (searchMode) {
    case SearchByName: {
      QString name = QInputDialog::getText(this, "Find by Name", "Enter name:");
      searchByName(name);
      break;
    }
    case SearchById: {
      bool ok;
      int id = QInputDialog::getInt(this, "Find by ID", "Enter ID:", 1, 1, 1000,
                                    1, &ok);
      if (ok) {
        searchById(id);
      }
      break;
    }
    case DeleteByName: {
      QString name =
          QInputDialog::getText(this, "Delete by Name", "Enter name:");
      searchByName(name);
      deleteByName(name);
      break;
    }
    case DeleteById: {
      bool ok;
      int id = QInputDialog::getInt(this, "Delete by ID", "Enter ID:", 1, 1,
                                    1000, 1, &ok);
      if (ok) {
        searchById(id);
        deleteById(id);
      }
      break;
    }
  }
}

void MainWindow::changeMode() {
  if (comboBox->currentText() == "Find by name") {
    searchMode = SearchByName;
  } else if (comboBox->currentText() == "Find by id") {
    searchMode = SearchById;
  } else if (comboBox->currentText() == "Delete by name") {
    searchMode = DeleteByName;
  } else if (comboBox->currentText() == "Delete by id") {
    searchMode = DeleteById;
  }
}
