#include "mainwindow.h"
#include "Panchromatic.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sourceImageScene = new QGraphicsScene(this);
    targetImageScene = new QGraphicsScene(this);
    outputImageScene = new QGraphicsScene(this);

    ui->sourceView->setScene(sourceImageScene);
    ui->targetView->setScene(targetImageScene);
    ui->outputView->setScene(outputImageScene);

    setupShortcuts();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    MainWindow::close();
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dialog(this);
    dialog.setWindowTitle("Open Image");
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Images files (*.png *.jpg *.tiff);;Text files (*.txt);;All files (*)"));

    QStringList filePaths;
    if (dialog.exec()) {
        filePaths = dialog.selectedFiles();

        if (filePaths.isEmpty()) {
            return;
        }

        if (ui->target->isChecked()) {
            showImage(filePaths.at(0), ui->targetView, targetImageScene);
            targetFilePath = filePaths.at(0);
        }
        else if (ui->source->isChecked()) {
            showImage(filePaths.at(0), ui->sourceView, sourceImageScene);
            sourceFilePath = filePaths.at(0);
        }
    }
}

void MainWindow::showImage(const QString& path, QGraphicsView* view, QGraphicsScene* scene)
{
    QPixmap pixmap(path);
    if (pixmap.isNull()) {
        qDebug() << "Error loading image: " << path;
        return;
    }

    showImage(pixmap.toImage(), view, scene);
}

void MainWindow::showImage(const QImage& image, QGraphicsView* view, QGraphicsScene* scene)
{
    scene->clear();
    view->resetTransform();

    QPixmap pixmap = QPixmap::fromImage(image);
    if (pixmap.isNull()) {
        qDebug() << "Error converting image to pixmap.";
        return;
    }

    QGraphicsPixmapItem* pixmapItem = scene->addPixmap(pixmap);

    view->fitInView(pixmapItem, Qt::KeepAspectRatio);
    view->setSceneRect(pixmap.rect());

    QString status = QString("Image, %1x%2, %3 Bytes").arg(image.width()).arg(image.height()).arg(image.byteCount());
    qDebug() << status;
}

QGraphicsView* MainWindow::getCurrentView() const
{
    if (ui->target->isChecked()) {
        return ui->targetView;

    } else if (ui->source->isChecked()) {
        return ui->sourceView;
    } else {
        return ui->outputView;
    }
}

void MainWindow::on_actionzoomIn_triggered()
{
    zoomView(getCurrentView(), 1.2);
}

void MainWindow::on_actionzoomOut_triggered()
{
    zoomView(getCurrentView(), 1.0 / 1.2);
}

void MainWindow::zoomView(QGraphicsView* view, qreal scaleFactor)
{
    if (view) {
        view->scale(scaleFactor, scaleFactor);
    }
}

// void MainWindow::on_process_clicked()
// {
//     if (targetFilePath.isEmpty() || sourceFilePath.isEmpty()){
//         QMessageBox::information(this, "Information", "Error, Missing file!");
//     }

//     QPixmap source(sourceFilePath);
//     QImage image_source = source.toImage().convertToFormat(QImage::Format_RGB888);

//     QPixmap target(targetFilePath);
//     QImage image_target = target.toImage().convertToFormat(QImage::Format_RGB888);


//     Mat sourceMat = cv::Mat(image_source.height(), image_source.width(), CV_8UC3, const_cast<uchar*>(image_source.bits()), image_source.bytesPerLine());
//     Mat targetMat = cv::Mat(image_target.height(), image_target.width(), CV_8UC3, const_cast<uchar*>(image_target.bits()), image_target.bytesPerLine());


//     // Get the current value of the slider
//     double weight = static_cast<double>(ui->weights->value()) / 100.0;

//     QString weightString = QString::number(weight);
//     ui->percent->setText("Weight: " + weightString);

//     // process source/target and save into output
//     Mat output = ihsPansharpen(sourceMat, targetMat, weight);

//     QImage image_edited = QImage(output.data, output.cols, output.rows, output.step, QImage::Format_RGB888);
//     showImage(image_edited, ui->outputView, outputImageScene);
// }


void MainWindow::on_process_clicked()
{
    if (targetFilePath.isEmpty() || sourceFilePath.isEmpty()){
        QMessageBox::information(this, "Information", "Error, Missing file!");
        return;
    }

    QPixmap source(sourceFilePath);
    QImage image_source = source.toImage().convertToFormat(QImage::Format_RGB888);

    QPixmap target(targetFilePath);
    QImage image_target = target.toImage().convertToFormat(QImage::Format_RGB888);

    Mat sourceMat = cv::Mat(image_source.height(), image_source.width(), CV_8UC3, const_cast<uchar*>(image_source.bits()), image_source.bytesPerLine());
    Mat targetMat = cv::Mat(image_target.height(), image_target.width(), CV_8UC3, const_cast<uchar*>(image_target.bits()), image_target.bytesPerLine());

    // Get the current value of the slider
    double weight = static_cast<double>(ui->weights->value()) / 100.0;

    QString weightString = QString::number(weight);
    ui->percent->setText("Weight: " + weightString);

    // Determine which operation to perform based on the selected radio button
    Mat output;
    if (ui->IHS->isChecked()) {
        output = ihsPansharpen(sourceMat, targetMat, weight);
    }
    else if (ui->BROVEY->isChecked()) {
        qDebug() << "PERFORM BROVEY";

    }
    else if (ui->MEAN->isChecked()) {
        qDebug() << "PERFORM MEAN";

    }
    else if (ui->ESRI->isChecked()){
        qDebug() << "PERFORM ESRI";
    }

    else {
        QMessageBox::information(this, "Information", "Please select a fusion method.");
        return;
    }

    QImage image_edited = QImage(output.data, output.cols, output.rows, output.step, QImage::Format_RGB888);
    showImage(image_edited, ui->outputView, outputImageScene);
}


void MainWindow::on_save_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save As", QString(), "Images files (*.png *.jpg *.tiff);;Text files (*.txt);;All files (*)");

    if (!filename.isEmpty()) {
        QPixmap output;
        if (!outputImageScene->items().isEmpty() && outputImageScene->items().first()->type() == QGraphicsPixmapItem::Type) {
            output = dynamic_cast<QGraphicsPixmapItem*>(outputImageScene->items().first())->pixmap();
        }

        if (!output.isNull()) {
            if (output.save(filename)) {
                QMessageBox::information(this, "Information", "Image saved successfully.");
                return;
            }
        }
    }
    QMessageBox::information(this, "Information", "Error, Bad Format!");
}



void MainWindow::setupShortcuts(){
    QList<QKeySequence> shortcuts;

    shortcuts.clear();
    shortcuts << Qt::Key_Plus << Qt::Key_Equal;
    ui->actionzoomIn->setShortcuts(shortcuts);

    shortcuts.clear();
    shortcuts << Qt::Key_Minus << Qt::Key_Underscore;
    ui->actionzoomOut->setShortcuts(shortcuts);

    shortcuts.clear();
    shortcuts << Qt::CTRL + Qt::Key_O;
    ui->actionOpen->setShortcuts(shortcuts);

    shortcuts.clear();
    shortcuts << Qt::CTRL + Qt::Key_Q;
    ui->actionExit->setShortcuts(shortcuts);

    shortcuts.clear();
    shortcuts << Qt::Key_S;
    ui->source->setShortcut(shortcuts[0]);

    shortcuts.clear();
    shortcuts << Qt::Key_T;
    ui->target->setShortcut(shortcuts[0]);
}





