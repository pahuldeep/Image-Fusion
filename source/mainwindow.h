#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();
    void on_actionOpen_triggered();
    void on_actionzoomIn_triggered();
    void on_actionzoomOut_triggered();

    void on_save_clicked();
    void on_process_clicked();


private:
    QGraphicsView* getCurrentView() const;


    void showImage(const QString &path, QGraphicsView *view, QGraphicsScene *scene);
    void showImage(const QImage& image, QGraphicsView* view, QGraphicsScene* scene);

    void zoomView(QGraphicsView* view, qreal scaleFactor);

    void setupShortcuts();

private:
    Ui::MainWindow *ui;

    QGraphicsScene *sourceImageScene;
    QGraphicsScene *targetImageScene;
    QGraphicsScene *outputImageScene;

    // QString currentImagePath;
    QString targetFilePath;
    QString sourceFilePath;

};
#endif // MAINWINDOW_H
