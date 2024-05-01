#ifndef PANCHROMATIC_H
#define PANCHROMATIC_H

#include "mainwindow.h"
#include <opencv2/opencv.hpp>

using namespace cv;

Mat QImageToMat(const QImage &image);
Mat ihsPansharpen(const Mat& rgb, const Mat& pan, double weight);

#endif // PANCHROMATIC_H
