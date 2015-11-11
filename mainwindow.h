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
    std::vector<int> rt;
    std::vector<int> ut;
    std::vector<int> time;
    std::vector<int> graniza;
    std::vector<st> f;

private:
    Ui::MainWindow *ui;

    void f1();
    void fn(int t);

private slots:
    void readJSON();
    void reshenie();
    void addHTML();
    void viewHTML();

};

#endif // MAINWINDOW_H
