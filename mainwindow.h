#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QMouseEvent>
#include "scene.h"
#include "game.h"

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
    void on_pushButton_clicked();

    void on_actionNew_triggered();

    void on_rules_triggered();

    void on_exit_triggered();

    void on_unDo_triggered();

    void on_radioButton_2_pressed();

    void on_radioButton_pressed();

private:
    Ui::MainWindow *ui;
    Scene *scene;
    Game *game_;
};

#endif // MAINWINDOW_H
