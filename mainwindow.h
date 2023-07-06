#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QClipboard>
#include <QDebug>
#include "JsonHandler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    JsonHandler jsonHandler;
    void handleButtonClick(QJsonDocument::JsonFormat format);

private slots:
    void on_formatButton_clicked();
    void on_compressButton_clicked();
    void on_exbandButton_clicked();
    void on_shrinkButton_clicked();
    void on_loadFileButton_clicked();
    void on_saveFileButton_clicked();
    void on_clearButton_clicked();
    void on_copyButton_clicked();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
