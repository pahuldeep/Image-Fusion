#ifndef PANCHROMATIC_H
#define PANCHROMATIC_H

#include "mainwindow.h"
#include <opencv2/opencv.hpp>

using namespace cv;

Mat QImageToMat(const QImage &image);

Mat weightedIHS(const Mat& rgb, const Mat& pan, double weight);
Mat weightedMean(const Mat& rgb, const Mat& pan, float rgbWeight, float panWeight);

Mat weightedBrovey(const Mat& rgb, const Mat& pan, double redWeight, double greenWeight, double blueWeight, double infraredWeight);
Mat weightedEsri(const Mat& rgb, const Mat& pan, double redWeight, double greenWeight, double blueWeight, double infraredWeight);

#endif // PANCHROMATIC_H
