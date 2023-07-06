#include "mainwindow.h"
#include "ui_mainwindow.h"

// 构造函数
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::Window);
    // 默认情况设置“保存文件”按钮禁用
    ui->saveFileButton->setEnabled(false);
    ui->inputTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->jsonTreeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

// 析构函数
MainWindow::~MainWindow()
{
    delete ui;
}

// 处理格式化或者压缩的主要功能，接受JsonFormat作为序列化的参数
void MainWindow::handleButtonClick(QJsonDocument::JsonFormat format)
{
    // 从文本框读取输入
    QString inputText = ui->inputTextEdit->toPlainText();

    // 为空时
    if(inputText.isEmpty()) {
        ui->jsonTreeView->setModel(nullptr);
        return;
    }

    try {
        // 利用 JsonHandler 来进行解析
        QJsonValue jsonValue = jsonHandler.deserializeToJsonValue(inputText);
        QStandardItemModel* model = jsonHandler.valueToModel(jsonValue);

        // 设置保存文件按钮有效
        ui->saveFileButton->setEnabled(true);

        // 将JSON转换为指定格式的字符串
        QString formattedJson = jsonHandler.serializeValue(jsonValue, format);
        ui->inputTextEdit->setPlainText(formattedJson);

        // 将整棵树显示出来
        ui->jsonTreeView->setModel(model);
            ui->jsonTreeView->expandAll();
    } catch(const std::runtime_error& e) {
        QStandardItemModel* model = new QStandardItemModel();
        QStandardItem* rootItem = new QStandardItem(QString("Parse Error: %1").arg(e.what()));
        model->appendRow(rootItem);
        ui->jsonTreeView->setModel(model);
        ui->saveFileButton->setEnabled(false);
    }

    return;
}

// 格式化，含有缩进和换行
void MainWindow::on_formatButton_clicked()
{
    handleButtonClick(QJsonDocument::Indented);
}

// 紧缩格式
void MainWindow::on_compressButton_clicked()
{
    handleButtonClick(QJsonDocument::Compact);
}



// 全部展开
void MainWindow::on_exbandButton_clicked() {
    ui->jsonTreeView->expandAll();
}

// 全部收缩
void MainWindow::on_shrinkButton_clicked() {
    ui->jsonTreeView->collapseAll();
}



// 读取本地的json文件
void MainWindow::on_loadFileButton_clicked() {
    // 弹出选择文件的框，选择读取的json文件
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("JSON Files (*.json)"));

    if(filePath.isEmpty())
        return;

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString fileText = in.readAll();
    file.close();

    // 读完将内容放在 inputTextEdit 里，也可以不放
    ui->inputTextEdit->setPlainText(fileText);
}

// 保存格式化后的json文件
void MainWindow::on_saveFileButton_clicked() {
    // 弹出选择文件的框，选择保存的json文件
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("JSON Files (*.json)"));

    if(filePath.isEmpty())
        return;

    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    // 这里从 inputTextEdit 输出到文件中
    QTextStream out(&file);
    out << ui->inputTextEdit->toPlainText();
    file.close();
}



// 清空，注意两个都清空，并且设置保存文件按钮无效
void MainWindow::on_clearButton_clicked() {
    ui->inputTextEdit->clear();
    ui->jsonTreeView->setModel(nullptr);

    ui->saveFileButton->setEnabled(false);
}

// 复制，复制 inputTextEdit 内容到剪贴板
void MainWindow::on_copyButton_clicked() {
    QString text = ui->inputTextEdit->toPlainText();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(text);
}
