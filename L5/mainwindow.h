//mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "person.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    QVector<Person*> list; // список людей

    void deleteRow(int i);

private slots:
    void on_loadBtn_clicked();
    void on_table_cellDoubleClicked(int row, int col);

private:
    Ui::MainWindow* ui;
    void readFile(const QString& path);
    void showTable();
};

#endif
