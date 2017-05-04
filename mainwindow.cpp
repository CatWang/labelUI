#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <vector>
#include <sys/stat.h>
#include <QFile>
#include <QTextStream>
#include <QImageReader>
#include <QImage>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QBitmap>
#include <sys/stat.h>
#include <QFileDialog>
#include <QTextCursor>
#include <QTextEdit>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

static QString fileDir;

void showAllDirectories (const char* dir_name, vector<char *>& files) {
    if (dir_name == NULL) {
        cout << "dir_name is null!" << endl;
        return;
    }

    struct stat s;
    lstat(dir_name, &s);
    if (!S_ISDIR(s.st_mode)) {
        cout << "dir_name is not a valid directory!" << endl;
        return;
    }

    struct dirent* filename;
    DIR* dir;
    dir = opendir(dir_name);

    if (dir == NULL) {
        cout << "Can not open dir " << dir_name <<endl;
        return;
    }

    cout << "Successfully open the dir!" << endl;

    while ((filename = readdir(dir)) != NULL) {
        if (strcmp(filename->d_name, ".") == 0 || strcmp(filename->d_name, "..") == 0)
            continue;
        if (filename->d_type == DT_DIR) {
            char* dname = filename->d_name;
            char *s = new char[strlen(dir_name) + strlen(dname) + 10];
            strcpy(s, dir_name);
            strcat(s, dname);
            strcat(s, "/");
            files.push_back(s);
        }
    }
}

void getAllPics( const char * dir_name, vector<char *>& pics )
{
    // check the parameter !
    if( NULL == dir_name )
    {
        cout<<" dir_name is null ! "<<endl;
        return;
    }

    // check if dir_name is a valid dir
    struct stat s;
    lstat( dir_name , &s );
    if( ! S_ISDIR( s.st_mode ) )
    {
        cout<<"dir_name is not a valid directory !"<<endl;
        return;
    }

    struct dirent * filename;    // return value for readdir()
    DIR * dir;                   // return value for opendir()
    dir = opendir( dir_name );
    if( NULL == dir )
    {
        cout<<"Can not open dir "<<dir_name<<endl;
        return;
    }
    cout<<"Successfully opened the dir !"<<endl;

    /* read all the files in the dir ~ */
    while( ( filename = readdir(dir) ) != NULL )
    {
        // get rid of "." and ".."
        if( strcmp( filename->d_name , "." ) == 0 ||
            strcmp( filename->d_name , "..") == 0    )
            continue;

//        if (filename ->d_type == DT_DIR) {
//            char * fname = filename ->d_name;
//
//            char *s = new char[strlen(dir_name)+strlen(fname)+20];
//            strcpy(s,dir_name);
//            strcat(s,fname);
//            strcat(s, "/");
//            showAllFiles(s, files);
//        } else {
//            cout << filename->d_name << endl;
            char *fname = filename->d_name;

        string file_name = fname;

        if (file_name.find("result.txt") != string::npos)
            continue;

        char *s = new char[strlen(dir_name) + strlen(fname) + 1];
        strcpy(s, dir_name);
        strcat(s, fname);

        pics.push_back(s);
//        }
    }
}



vector<char*> tempVector;
vector<char*> dirVector;

char* text = "result.txt";

static int dir_i = 0;
static int dir_size = 0;
static int deleted = 0;

char* img = "img_";

void MainWindow::on_btnDir_clicked()
{
    tempVector.clear();
    dirVector.clear();
    dir_i = 0;
    dir_size = 0;

    QByteArray byteArray = ui->txtDir->text().toLatin1();
    const char *root = byteArray.constData();
    showAllDirectories(root, tempVector);

    for (int i = 0; i < tempVector.size(); i++) {
        showAllDirectories(tempVector[i], dirVector);
    }

    for (int i = 0; i < dirVector.size(); i++) {
        cout << dirVector[i] << endl;
    }

    dir_size = dirVector.size();

    dir_i = ui->txtCount->text().toInt();
    ui->labelCount->setText(QString::number(dir_i));
    ui->labelDelete->setText(QString::number(deleted));

    if (dir_i >= dir_size) {
        ui->txtDir->setText("ALL DONE!");
        return;
    }

    if (dir_i < 0) {
        ui->txtDir->setText("NO FORMORE DIRECTORIES!");
        return;
    }

    char* currentDir = dirVector[dir_i];
    char* resultText = new char[strlen(currentDir) + strlen(text) + 2];

    strcpy(resultText, currentDir);
    strcat(resultText, text);

    QFile textFile(resultText);


    if(textFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&textFile);

        QString line = in.readLine();
        ui->labelAge->setText(line);

        line = in.readLine();
        ui->labelGender->setText(line);
    }
    else if(!textFile.exists()) {
        cout << "1" << endl;
        if(textFile.open(QIODevice::ReadWrite)) {
            cout<<"file now exists" << endl;
        }
    }
    textFile.close();

    ui->text_age->setFocus();

    vector<char*> picVector;
    getAllPics(currentDir, picVector);

    QPixmap image;

    ui->img_0->clear();
    ui->img_1->clear();
    ui->img_2->clear();
    ui->img_3->clear();
    ui->img_4->clear();
    ui->img_5->clear();
    ui->img_6->clear();
    ui->img_7->clear();

    for (int i = 0; i < picVector.size(); i++) {
        if (i > 7) {
            break;
        }

        image.load(picVector[i]);

        switch (i) {
        case 0:
            ui->img_0->setPixmap(image);
            ui->img_0->setMask(image.mask());
            break;
        case 1:
            ui->img_1->setPixmap(image);
            ui->img_1->setMask(image.mask());
            break;
        case 2:
            ui->img_2->setPixmap(image);
            ui->img_2->setMask(image.mask());
            break;
        case 3:
            ui->img_3->setPixmap(image);
            ui->img_3->setMask(image.mask());
            break;
        case 4:
            ui->img_4->setPixmap(image);
            ui->img_4->setMask(image.mask());
            break;
        case 5:
            ui->img_5->setPixmap(image);
            ui->img_5->setMask(image.mask());
            break;
        case 6:
            ui->img_6->setPixmap(image);
            ui->img_6->setMask(image.mask());
            break;
        case 7:
            ui->img_7->setPixmap(image);
            ui->img_7->setMask(image.mask());
            break;
        default:
            break;
        }
    }
}

void MainWindow::on_btn_nextDir_clicked()
{
    if (ui->text_age->text().isEmpty() || ui->text_gender->text().isEmpty())
        return;

    on_btn_saveResult_clicked();
    dir_i++;

    if (dir_i >= dir_size) {
        ui->txtDir->setText("ALL_DONE!");
        return;
    }
    ui->text_gender->clear();
    ui->text_age->clear();

    char* currentDir = dirVector[dir_i];
    char* resultText = new char[strlen(currentDir) + strlen(text) + 2];

    strcpy(resultText, currentDir);
    strcat(resultText, text);

    QFile textFile(resultText);

    if(textFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&textFile);

        QString line = in.readLine();
        ui->labelAge->setText(line);

        line = in.readLine();
        ui->labelGender->setText(line);
    }

    ui->text_age->setFocus();

    ui->labelCount->setText(QString::number(dir_i));

    vector<char*> picVector;
    getAllPics(currentDir, picVector);

    QPixmap image;

    ui->img_0->clear();
    ui->img_1->clear();
    ui->img_2->clear();
    ui->img_3->clear();
    ui->img_4->clear();
    ui->img_5->clear();
    ui->img_6->clear();
    ui->img_7->clear();

    for (int i = 0; i < picVector.size(); i++) {
        if (i > 7) {
            break;
        }

        image.load(picVector[i]);

        switch (i) {
        case 0:
            ui->img_0->setPixmap(image);
            ui->img_0->setMask(image.mask());
            break;
        case 1:
            ui->img_1->setPixmap(image);
            ui->img_1->setMask(image.mask());
            break;
        case 2:
            ui->img_2->setPixmap(image);
            ui->img_2->setMask(image.mask());
            break;
        case 3:
            ui->img_3->setPixmap(image);
            ui->img_3->setMask(image.mask());
            break;
        case 4:
            ui->img_4->setPixmap(image);
            ui->img_4->setMask(image.mask());
            break;
        case 5:
            ui->img_5->setPixmap(image);
            ui->img_5->setMask(image.mask());
            break;
        case 6:
            ui->img_6->setPixmap(image);
            ui->img_6->setMask(image.mask());
            break;
        case 7:
            ui->img_7->setPixmap(image);
            ui->img_7->setMask(image.mask());
            break;
        default:
            break;
        }
    }
}

void MainWindow::on_btn_saveResult_clicked()
{
    if (ui->text_age->text().isEmpty() || ui->text_gender->text().isEmpty())
        return;
    char* currentDir = dirVector[dir_i];
    char* resultText = new char[strlen(currentDir) + strlen(text) + 2];

    strcpy(resultText, currentDir);
    strcat(resultText, text);

    QFile textFile(resultText);

    if(textFile.open(QIODevice::WriteOnly|QFile::Truncate)) {
        QTextStream out(&textFile);

        out << ui->text_age->text() <<endl;
        out << ui->text_gender->text() << endl;
    }
}

void MainWindow::on_btn_deleteDir_clicked()
{
    char* currentDir = dirVector[dir_i];
    string rm = "rm -r ";
    string current = currentDir;
    string command = rm + current;

    system(command.c_str());
    deleted += 1;
    ui->labelDelete->setText(QString::number(deleted));
    ui->text_age->setText("0");
    ui->text_gender->setText("0");
}

void MainWindow::on_btn_openDir_clicked()
{
    char* currentDir = dirVector[dir_i];
    char* open = "xdg-open ";
    char* openDir = new char[strlen(currentDir) + strlen(open) + 2];

    strcpy(openDir, open);
    strcat(openDir, currentDir);

    system(openDir);
}

void MainWindow::on_btn_preDir_clicked()
{
    dir_i--;

    if (dir_i >= dir_size) {
        ui->txtDir->setText("ALL DOWN!");
        return;
    }

    if (dir_i < 0) {
        ui->txtDir->setText("NO FORMER DIRECTORIES!");
        return;
    }

    char* currentDir = dirVector[dir_i];
    char* resultText = new char[strlen(currentDir) + strlen(text) + 2];

    strcpy(resultText, currentDir);
    strcat(resultText, text);

    ui->labelCount->setText(QString::number(dir_i));

    QFile textFile(resultText);

    if(textFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&textFile);

        QString line = in.readLine();
        ui->text_age->setText(line);
        ui->labelAge->setText(line);

        line = in.readLine();
        ui->labelGender->setText(line);
        ui->text_gender->setText(line);
    }

    ui->text_age->setFocus();

    vector<char*> picVector;
    getAllPics(currentDir, picVector);

    QPixmap image;

    ui->img_0->clear();
    ui->img_1->clear();
    ui->img_2->clear();
    ui->img_3->clear();
    ui->img_4->clear();
    ui->img_5->clear();
    ui->img_6->clear();
    ui->img_7->clear();

    for (int i = 0; i < picVector.size(); i++) {
        if (i > 7) {
            break;
        }

        image.load(picVector[i]);

        switch (i) {
        case 0:
            ui->img_0->setPixmap(image);
            ui->img_0->setMask(image.mask());
            break;
        case 1:
            ui->img_1->setPixmap(image);
            ui->img_1->setMask(image.mask());
            break;
        case 2:
            ui->img_2->setPixmap(image);
            ui->img_2->setMask(image.mask());
            break;
        case 3:
            ui->img_3->setPixmap(image);
            ui->img_3->setMask(image.mask());
            break;
        case 4:
            ui->img_4->setPixmap(image);
            ui->img_4->setMask(image.mask());
            break;
        case 5:
            ui->img_5->setPixmap(image);
            ui->img_5->setMask(image.mask());
            break;
        case 6:
            ui->img_6->setPixmap(image);
            ui->img_6->setMask(image.mask());
            break;
        case 7:
            ui->img_7->setPixmap(image);
            ui->img_7->setMask(image.mask());
            break;
        default:
            break;
        }
    }


}
