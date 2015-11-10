#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->readFile,SIGNAL(triggered(bool)),SLOT(readJSON()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::f1()
{
    st temp;

    for (int i = 0; i < time.size(); i++){
        int a = rt[i] - ut[i];
        int b = s - p + rt[0] - ut[0];
        if (a >= b){
            temp.line.push_back(a);
        }else{
            graniza.push_back(i);
            while (i < time.size()){
                temp.line.push_back(b);
                i++;
            }
        }
    }

    f.push_back(temp);
    //return temp;
}

void MainWindow::fn(int t)
{
    st temp;

    if (t == 1){
        f1();
    }else{
        fn(t-1);
    }

    for (int i = 0; i < time.size(); i++){
        int a = rt[i] - ut[i] + f[t-1].line[i+1];
        int b = s - p + rt[0] - ut[0] + f[t-1].line[1];
        if (a >= b){
            temp.line.push_back(a);
        }else{
            graniza.push_back(i);
            while (i < time.size()){
                temp.line.push_back(b);
                i++;
            }
        }
    }

    f.push_back(temp);
}

void MainWindow::readJSON()
{
    //читаем input.json
    QString settings;
    QFile file;
    file.setFileName("input.json");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        settings = file.readAll();
    }else{
        QMessageBox msgBox;
        msgBox.setText("File \"input.json\" not found!");
        msgBox.exec();
    }
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(settings.toUtf8());
    if (!document.isNull()){
        QJsonObject object = document.object();

        t = object["t"].toString().toInt();
        s = object["s"].toString().toInt();
        p = object["p"].toString().toInt();
        int tempTime = object["time"].toString().toInt();

        //таблица времени
        for (int i = 0; i < tempTime; i++){
            time.push_back(i);
        }

        QJsonArray RT = object.value(QString("rt")).toArray();
        QJsonArray UT = object.value(QString("ut")).toArray();

        for (int i = 0; i < tempTime; i++){
            rt.push_back(RT[i].toString().toInt());
            ut.push_back(UT[i].toString().toInt());
        }

        QMessageBox msgBox;
        QPushButton *ok = msgBox.addButton(tr("OK"),QMessageBox::ActionRole);

        connect(ok,SIGNAL(pressed()),SLOT(reshenie()));
        //connect(ok,SIGNAL(released()),SLOT(viewHTML()));

        msgBox.setText("Reading complete!");
        msgBox.exec();

    }else{
        QMessageBox msgBox;
        msgBox.setText("Reading failed!");
        msgBox.exec();
    }
}

void MainWindow::reshenie()
{
    fn(time.size()-2);

    qWarning() << "Good";

}
