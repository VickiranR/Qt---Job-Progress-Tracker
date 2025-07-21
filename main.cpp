#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QThread>
#include <QDebug>

// Worker thread that emits progress updates
class Worker : public QThread
{
    Q_OBJECT

signals:
    void progress(int value);

protected:
    void run() override {
        for (int i = 1; i <= 100; ++i) {
            QThread::sleep(1);          // Simulate work
            emit progress(i);           // Notify UI thread
        }
    }
};

// Main GUI Window
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow() {
        label = new QLabel("Click 'Start Task' to begin", this);
        progressBar = new QProgressBar(this);
        progressBar->setRange(0, 100);
        button = new QPushButton("Start Task", this);

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(label);
        layout->addWidget(progressBar);
        layout->addWidget(button);

        setLayout(layout);
        setWindowTitle("Qt Job Progress Tracker");
        setGeometry(300, 300, 350, 180);

        connect(button, &QPushButton::clicked, this, &MainWindow::startTask);
    }

private slots:
    void startTask() {
        // Prevent starting multiple threads
        if (worker && worker->isRunning()) return;

        worker = new Worker();
        connect(worker, &Worker::progress, this, &MainWindow::updateProgress);
        connect(worker, &QThread::finished, worker, &QObject::deleteLater);
        worker->start();
    }

    void updateProgress(int value) {
        progressBar->setValue(value);
        label->setText(QString("Task Progress: %1%").arg(value));
        if (value == 100) {
            label->setText("Task Completed!");
        }
    }

private:
    QLabel* label;
    QPushButton* button;
    QProgressBar* progressBar;
    Worker* worker = nullptr;
};

#include "main.moc"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
