#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QColorDialog>
#include <QColor>
#include <QFontDialog>
#include <QFont>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
     ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer *timer= new QTimer (this);
    connect(timer,SIGNAL(timeout()),this,SLOT(showTime()));
    timer->start();


}

MainWindow::~MainWindow()
{
    QMessageBox:: StandardButton reply=QMessageBox::question(this," ","Do you want to save changes?",
                                                             QMessageBox::Yes | QMessageBox::No);
    if (reply==QMessageBox::Yes){
    on_actionSave_triggered();
    delete ui;
    }
    else  if (reply==QMessageBox::No) {
        delete ui;
    };

}


void MainWindow:: showTime ()
{
    QTime time= QTime ::currentTime();
    QString time_text=time.toString("hh : mm : ss");
    ui->time->setText(time_text);

}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}


void MainWindow::on_actionNew_triggered()
{

    QMessageBox:: StandardButton reply=QMessageBox::question(this," ","Do you really want to create a new file without saving a current file?",
                                                             QMessageBox::Yes | QMessageBox::No);
    if (reply==QMessageBox::Yes){
   file_path_ ="";
    ui->textEdit->setText("");
    }




}


void MainWindow::on_actionOpen_triggered()
{
    QMessageBox:: StandardButton reply=QMessageBox::question(this," ","Do you really want to open a file without saving a current file?",
                                                             QMessageBox::Yes | QMessageBox::No);
    if (reply==QMessageBox::Yes){
  QString file_name = QFileDialog::getOpenFileName(this,"Open the file");
  QFile file(file_name);
  file_path_= file_name;
  if(!file.open(QFile::ReadOnly| QFile ::Text)){
      QMessageBox::warning(this,"..","file not open");
      return;
  }
  QTextStream in(&file);
  QString text = in.readAll();
  ui->textEdit->setText(text);
  file.close();
}
}


void MainWindow::on_actionSave_triggered()
{
    QFile file(file_path_);
    if(!file.open(QFile::WriteOnly| QFile ::Text)){
        on_actionSave_as_triggered() ;
        return;
    }


    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out<<text;
    file.flush();
    file.close();

}


void MainWindow::on_actionSave_as_triggered()
{
    QString file_name = QFileDialog::getSaveFileName(this,"Open the file");
QFile file(file_name);
file_path_=file_name;
if(!file.open(QFile::WriteOnly| QFile ::Text)){
    QMessageBox::warning(this,"..","file not open");
    return;
}

QTextStream out(&file);
QString text = ui->textEdit->toPlainText();
out<<text;
file.flush();
file.close();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionAbout_note_triggered()
{
    QString about_text="Informatiom about this note: \n";
    about_text+="Time:  ";
    QTime time= QTime ::currentTime();
    QString time_text=time.toString("hh : mm : ss \n");
    about_text+=time_text;
    about_text+="File path:  ";
    about_text+=file_path_;
    QMessageBox:: about(this, "About Note", about_text);


}


void MainWindow::on_actionFont_triggered()
{
   bool ok;
   QFont font =QFontDialog::getFont(&ok,this);
   if (ok){
       ui->textEdit->setFont(font);
   }
   else return;
}


void MainWindow::on_actionColor_triggered()
{
    QColor color =QColorDialog:: getColor(Qt::white,this,"Choose color");
    if (color.isValid()){
        ui->textEdit->setTextColor(color);
    }
}


void MainWindow::on_actionBackground_Color_triggered()
{
    QColor color =QColorDialog:: getColor(Qt::white,this,"Choose color");
    if (color.isValid()){
        ui->textEdit->setTextBackgroundColor(color);
    }
}


void MainWindow::on_actionWindow_Color_triggered()
{
    QColor color =QColorDialog:: getColor(Qt::white,this,"Choose color");
    if (color.isValid()){
        ui->textEdit->setPalette( QPalette (color));
    }
}

