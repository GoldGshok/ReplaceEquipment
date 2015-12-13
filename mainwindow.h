#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QPushButton>
#include <QDebug>
#include <QWebView>
#include <QUrl>
#include <QLayout>
#include <QSizePolicy>
#include <QDir>
#include <QTableWidget>

namespace Ui {
class MainWindow;
}

struct st{
    std::vector<int> line;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int t;
    int p;
    int s;
    int n;
    int percent = 0;
    std::vector<int> rt;
    std::vector<int> ut;
    std::vector<int> time;
    std::vector<int> graniza;
    std::vector<st> f;

    QTableWidget *table;

private:
    Ui::MainWindow *ui;

    void f1();
    void fn(int t);

    QTableWidget *createTable();

private slots:
    void readJSON();
    void reshenie();
    void addHTML();
    void viewHTML();
    void btnOKPressed();
    void addTableValues();
    void checkPercent();

};

#endif // MAINWINDOW_H
