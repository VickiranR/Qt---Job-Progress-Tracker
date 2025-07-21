#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QThread>
#include <QDebug>

// Worker Thread
class Worker : public QThread
{
    Q_OBJECT

signals:
    void progress(int value);

protected:
    void run() override {
        for (int i = 1; i <= 100; ++i) {
            QThread::sleep(1); // Simulate work by sleeping
            emit progress(i); // Emit the progress signal
        }
    }
};

// Main Window
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow() {
        label = new QLabel("Click start to run the task", this);
        button = new QPushButton("Start Task", this);

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(label);
        layout->addWidget(button);

        setLayout(layout);
        setWindowTitle("QThread Example");
        setGeometry(300, 300, 300, 200);

        connect(button, &QPushButton::clicked, this, &MainWindow::startTask);
    }

private slots:
    void startTask() {
        worker = new Worker();
        connect(worker, &Worker::progress, this, &MainWindow::updateLabel);
        worker->start();
    }

    void updateLabel(int value) {
        label->setText(QString("Task Progress: %1%").arg(value));
        if (value == 100) {
            label->setText("Task Completed!");
            worker->quit(); // Stop the worker thread
            worker->wait(); // Ensure thread has finished
            delete worker; // Clean up
        }
    }

private:
    QLabel* label;
    QPushButton* button;
    Worker* worker;
};

#include "main.moc"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
