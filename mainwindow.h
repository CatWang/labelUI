#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_btnDir_clicked();

    void on_btn_nextDir_clicked();

    void on_btn_saveResult_clicked();

    void on_btn_deleteDir_clicked();

    void on_btn_openDir_clicked();

    void on_btn_preDir_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
