//mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "form.h"

#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Таблица");

    ui->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table->setAlternatingRowColors(true);
}

MainWindow::~MainWindow()
{
    for (int i = 0; i < list.size(); i++)
        delete list[i];
    delete ui;
}

void MainWindow::on_loadBtn_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Открыть файл", "", "*.txt");
    if (path.isEmpty()) return;

    readFile(path);
    showTable();
}

void MainWindow::readFile(const QString& path)
{
    for (int i = 0; i < list.size(); i++)
        delete list[i];
    list.clear();

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        int code;
        in >> code;

        if (code == 1) {
            // фамилия имя отчество д м г
            RuP* p = new RuP();
            in >> p->surname >> p->name >> p->patr
                >> p->dob.d >> p->dob.m >> p->dob.y;
            list.append(p);
        }
        else if (code == 2) {
            // имя имя2 фамилия м д г
            AmP* p = new AmP();
            in >> p->name >> p->name2 >> p->surname
                >> p->dob.m >> p->dob.d >> p->dob.y;
            list.append(p);
        }
    }
    file.close();
}

void MainWindow::showTable()
{
    ui->table->setRowCount(0);
    for (int i = 0; i < list.size(); i++) {
        int row = ui->table->rowCount();
        ui->table->insertRow(row);
        ui->table->setItem(row, 0, new QTableWidgetItem(list[i]->fullName()));
        ui->table->setItem(row, 1, new QTableWidgetItem(list[i]->dateDMY()));
    }
}

void MainWindow::on_table_cellDoubleClicked(int row, int col)
{
    Q_UNUSED(col);
    if (row < 0 || row >= list.size()) return;
    list[row]->Craft(row, this); // полиморфизм
}

void MainWindow::deleteRow(int i)
{
    if (i < 0 || i >= list.size()) return;
    delete list[i];
    list.remove(i);
    ui->table->removeRow(i);
}
