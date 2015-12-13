#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->readFile,SIGNAL(triggered(bool)),SLOT(readJSON()));
    connect(ui->btnOK,SIGNAL(pressed()),SLOT(btnOKPressed()));
    connect(ui->checkCost,SIGNAL(pressed()),SLOT(checkPercent()));
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
}

void MainWindow::fn(int t)
{
    st temp;

    if (t == 1){
        f1();
    }else{
        fn(t-1);
    }

    if (percent != 0){
        s = s * (100 - percent) / 100;
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

QTableWidget *MainWindow::createTable()
{
    QTableWidget *TempTable = new QTableWidget(2,time.size(),this);

    QStringList verticalHeader;
    verticalHeader.append("rt");
    verticalHeader.append("ut");

    TempTable->setVerticalHeaderLabels(verticalHeader);

    return TempTable;
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

        time.clear();
        rt.clear();
        ut.clear();

        t = object["t"].toString().toInt();
        s = object["s"].toString().toInt();
        p = object["p"].toString().toInt();
        n = object["n"].toString().toInt();
        percent = object["percent"].toString().toInt();
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
        connect(ok,SIGNAL(released()),SLOT(viewHTML()));

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
    f.clear();

    fn(n-1);

    addHTML();

    qWarning() << "Good";

}

void MainWindow::addHTML()
{
    //записываем вывод первой таблицы в наглядной форме
    QFile file("output.html");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){

    }else{
        QTextStream out(&file);
        out << QString("<!DOCTYPE html>").toUtf8() << QString("\n").toUtf8();
        out << QString("<html>"
               "<head>"
                 "<meta charset=\"windows-1251\">"
                 "<title>Решение</title>"
               "</head>"
               "<body>").toUtf8();

        out << QString("<table border=1 align=center>").toUtf8()
            << QString("\n").toUtf8();
        out << QString("<tr>"
                    "<th>fn(t)/t</th>").toUtf8();

        for (int i = 0; i < time.size(); i++){
            out << QString("<th>").toUtf8() << i << QString("</th>").toUtf8();
        }

        out << QString("</tr>").toUtf8() << QString("\n").toUtf8();

        for (int i = 0; i < f.size(); i++){
            out << QString("<tr>").toUtf8() << QString("\n").toUtf8();
            out << QString("<td>f_").toUtf8() << i+1
                << QString("(t)</td>").toUtf8();
            for (int j = 0; j < time.size(); j++){

                if (j >= graniza[i]){
                    out << QString("<td bgcolor=\"#D3EDF6\">").toUtf8() << f[i].line[j]
                        << QString("</td>").toUtf8();
                }else{
                    out << QString("<td>").toUtf8() << f[i].line[j]
                        << QString("</td>").toUtf8();
                }
            }

            out << QString("</tr>").toUtf8() << QString("\n").toUtf8();
        }
        out << QString("</table>").toUtf8() << QString("\n").toUtf8();

        out << QString("<p>f_%1(%2) = %3</p>").arg(n)
               .arg(t)
               .arg(f[n-1].line[t]).toUtf8()
            << QString("\n").toUtf8();

        //вывод цепочки
        out << QString("<table border=1 align=center>"
                       "<th>F_i</th>"
                       "<th>Год эксплуатации</th>"
                       "<th>Замена</th>")
               .toUtf8();

        std::vector<int> replace;
        int temp = t;
        int step = 1;
        for (int i = n-1; i >= 0; i--){
            out << QString("<tr><td><center> f_%1 </center></td><td><center>%2</center></td>")
                   .arg(i+1)
                   .arg(temp)
                   .toUtf8();

            if (temp >= graniza[i]){
                out << QString("<td><center>x</center></td>").toUtf8();
                replace.push_back(step);
                temp = 1;
            }else{
                out << QString("<td> </td>").toUtf8();
                temp++;
            }
            step++;
            out << QString("</tr>").toUtf8();
        }

        out << QString("</table>").toUtf8();

        if (replace.size() == 0){
            out << QString("<p>Замена не требуется!</p>").toUtf8();
        }else{
            out << QString("<p>Замена: ").toUtf8();
            for (int i = 0; i < replace.size(); i++){
                out << replace[i] << QString(" ").toUtf8();
            }
            out << QString("</p>").toUtf8();
        }

        //close html
        out << QString("</body>").toUtf8();
        out << QString("</html>").toUtf8();
    }

    file.close();
}

void MainWindow::viewHTML()
{
    QDialog *d = new QDialog();

    QSizePolicy sizePolicy;

    sizePolicy.setVerticalPolicy(QSizePolicy::Fixed);
    sizePolicy.setHorizontalPolicy(QSizePolicy::Fixed);

    d->setSizePolicy(sizePolicy);

    d->setGeometry(100, 100, 800, 600);

    QVBoxLayout *l = new QVBoxLayout();
    d->setLayout(l);

    QWebView *view = new QWebView(d);
    QUrl url("file:///" + QDir::currentPath() + "/output.html");

    view->load(url);
    view->show();

    l->addWidget(view);

    d->exec();

    d->deleteLater();

}

void MainWindow::btnOKPressed()
{
    time.clear();
    rt.clear();
    ut.clear();

    n = ui->editN->text().toInt();
    s = ui->editS->text().toInt();
    p = ui->editP->text().toInt();
    t = ui->editT->text().toInt();
    int tempTime = ui->editTime->text().toInt();
    if (ui->checkCost->isChecked()){
        percent = ui->editPercent->text().toInt();
    }

    //таблица времени
    for (int i = 0; i < tempTime; i++){
        time.push_back(i);
    }

    QDialog *d = new QDialog();

    QSizePolicy sizePolicy;

    sizePolicy.setVerticalPolicy(QSizePolicy::Fixed);
    sizePolicy.setHorizontalPolicy(QSizePolicy::Fixed);

    d->setSizePolicy(sizePolicy);

    d->setGeometry(100, 100, 800, 600);

    QVBoxLayout *l = new QVBoxLayout();
    d->setLayout(l);

    QPushButton *btn = new QPushButton();
    btn->setText("OK");

    connect(btn,SIGNAL(pressed()),SLOT(addTableValues()));
    connect(btn,SIGNAL(released()),d,SLOT(deleteLater()));
    connect(btn,SIGNAL(released()),SLOT(reshenie()));
    connect(btn,SIGNAL(released()),SLOT(viewHTML()));

    table = createTable();

    l->addWidget(table);
    l->addWidget(btn);

    d->exec();
}

void MainWindow::addTableValues()
{
    for(int i = 0; i < time.size(); i++){
        rt.push_back(table->item(0,i)->text().toInt());
        ut.push_back(table->item(1,i)->text().toInt());
    }
}

void MainWindow::checkPercent()
{
    if (!ui->checkCost->isChecked()){
        ui->editPercent->setEnabled(true);
        //ui->edtBeginMaterials->setEnabled(true);
    }else{
        //ui->labelBeginMaterials->setEnabled(false);
        ui->editPercent->setEnabled(false);
    }
}
