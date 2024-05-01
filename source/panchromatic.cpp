#include "Panchromatic.h"

Mat ihsPansharpen(const Mat &rgb, const Mat &pan, double weight)
{

    pan.convertTo(pan, rgb.type());

    Mat panGrey;
    cvtColor(pan, panGrey, cv::COLOR_BGR2GRAY);

    Mat ihs;
    cvtColor(rgb, ihs, cv::COLOR_BGR2HSV);

    std::vector<Mat> channels(3);
    split(ihs, channels);

    cv::resize(panGrey, panGrey, channels[2].size());

    channels[2] = panGrey - channels[2] * weight;

    merge(channels, ihs);

    Mat result;
    cvtColor(ihs, result, COLOR_HSV2BGR);

    result.convertTo(result, CV_8U);
    return result;
}
