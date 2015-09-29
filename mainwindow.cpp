#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpwindow.h"
#include <QGraphicsScene>
#include <QPixMap>
#include <QtMath>
#include <QGraphicsItem>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    game_ = new Game();
    scene = new Scene(game_);

    scene->setSceneRect(0, 0, ui->graphicsView->width()- 2, ui->graphicsView->height()-2);

    ui->graphicsView->setScene(scene);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (ui->radioButton->isChecked())
        game_->startNew(ui->spinBox->value(), Game::EN_COMPUTER, ui->spinBox_2->value());
    else
        game_->startNew(ui->spinBox->value(), Game::EN_HUMAN, ui->spinBox_2->value());
    scene->reset();

    ui->pushButton->setText("Перезапуск");
}

void MainWindow::on_actionNew_triggered()
{
    on_pushButton_clicked();
}

void MainWindow::on_rules_triggered()
{
    HelpWindow *helpWind = new HelpWindow();
    helpWind->setModal(true);
    helpWind->show();
}

void MainWindow::on_exit_triggered()
{
    exit(0);
}

void MainWindow::on_unDo_triggered()
{
    scene->undoMove();
}

void MainWindow::on_radioButton_2_pressed()
{
    ui->spinBox_2->setEnabled(false);
}

void MainWindow::on_radioButton_pressed()
{
    ui->spinBox_2->setEnabled(true);
}
