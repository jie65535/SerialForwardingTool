#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    /**
     * @brief MainWindow::openSerialPort 根据用户设置打开串口，返回是否打开成功
     * @return 是否打开成功
     */
    bool openSerialPort();
    /**
     * @brief MainWindow::closeSerialPort 关闭串口
     */
    void closeSerialPort();

private slots:
    /**
     * @brief MainWindow::on_btnStart_clicked 开始转发按钮点击事件
     */
    void on_btnStart_clicked();

    /**
     * @brief MainWindow::comAReceiveInfo COMA接收消息事件
     */
    void comAReceiveInfo();
    /**
     * @brief MainWindow::comBReceiveInfo COMB接收消息事件
     */
    void comBReceiveInfo();

    void on_btnClear_clicked();

    void on_checkBox_stateChanged(int);
protected:
     void closeEvent(QCloseEvent *);

private:
    Ui::MainWindow *ui;

    /**
     * @brief 串口A
     */
    QSerialPort *m_serialPortA;
    /**
     * @brief 串口B
     */
    QSerialPort *m_serialPortB;

    bool m_isMonitor;
};

#endif // MAINWINDOW_H
