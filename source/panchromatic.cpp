#include "Panchromatic.h"

Mat weightedIHS(const Mat &rgb, const Mat &pan, double weight)
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

Mat weightedMean(const Mat& rgb, const Mat& pan, float rgbWeight, float panWeight) {

    Mat rgbFloat, panFloat;

    rgb.convertTo(rgbFloat, CV_32F);
    pan.convertTo(panFloat, CV_32F);

    resize(panFloat, panFloat, rgbFloat.size());

    Mat weightedRgb = rgbWeight * rgbFloat;
    Mat weightedPan = panWeight * panFloat;

    Mat result;
    result = weightedRgb + weightedPan;

    result.convertTo(result, CV_8U);
    return result;
}

Mat color_codes(const Mat& image, int code) {
    int rows = image.rows;
    int cols = image.cols;

    std::vector<Mat> channels(3);
    cv::split(image, channels);
    // red
    if (code == 1)
    {
        channels[1] = Mat::zeros(rows, cols, channels[1].type());
        channels[2] = Mat::zeros(rows, cols, channels[2].type());
    }
    // green
    else if (code == 2)
    {
        channels[0] = Mat::zeros(rows, cols, channels[0].type());
        channels[2] = Mat::zeros(rows, cols, channels[2].type());
    }
    // blue
    else if (code == 3)
    {
        channels[0] = Mat::zeros(rows, cols, channels[0].type());
        channels[1] = Mat::zeros(rows, cols, channels[1].type());
    }
    cv::Mat color;
    cv::merge(channels, color);

    return color;
}

Mat weightedBrovey(const Mat& rgb, const Mat& pan, double redWeight, double greenWeight, double blueWeight, double infraredWeight) {

    // Check if the dimensions of rgb and pan match
    Mat rgbFloat;
    if (rgb.size() != pan.size()) {
        resize(rgb, rgbFloat, pan.size());
    }

    // Convert pan to floating point
    Mat panFloat;
    cvtColor(pan, panFloat, cv::COLOR_BGR2GRAY);
    panFloat.convertTo(panFloat, CV_32F);

    // Extract red, green, and blue channels from RGB image
    std::vector<Mat> rgbChannels(3);
    split(rgbFloat, rgbChannels);

    Mat red = rgbChannels[2];
    Mat green = rgbChannels[1];
    Mat blue = rgbChannels[0];

    // Convert red, green, and blue channels to floating point
    red.convertTo(red, CV_32F);
    green.convertTo(green, CV_32F);
    blue.convertTo(blue, CV_32F);

    normalize(red, red, 0, 1, cv::NORM_MINMAX, CV_32F);
    normalize(green, green, 0, 1, cv::NORM_MINMAX, CV_32F);
    normalize(blue, blue, 0, 1, cv::NORM_MINMAX, CV_32F);

    // Normalize the infrared channel
    Mat temp;
    cvtColor(rgbFloat, temp, COLOR_RGB2GRAY);
    Mat infrared = 255 - temp;
    normalize(infrared, infrared, 0, 1, cv::NORM_MINMAX, CV_32F);

    // Calculate the weighted mean for each channel
    Mat varInfrared = infraredWeight * infrared;
    Mat varRed = redWeight * red;
    Mat varGreen = greenWeight * green;
    Mat varBlue = blueWeight * blue;

    Mat denominator = varRed + varGreen + varBlue + varInfrared;
    Mat dnf = (panFloat - varInfrared) / denominator;

    // Apply the Brovey transformation to each band
    Mat outputRed = red.mul(dnf);
    Mat outputGreen = green.mul(dnf);
    Mat outputBlue = blue.mul(dnf);

    // Merge the channels back into a color image
    std::vector<Mat> channels = { outputBlue, outputGreen, outputRed };
    Mat output;
    merge(channels, output);

    // Convert the output image to 8-bit unsigned integer (optional)
    output.convertTo(output, CV_8U);
    return output;
}



Mat weightedEsri(const Mat& rgb, const Mat& pan, double redWeight, double greenWeight, double blueWeight, double infraredWeight) {
    return rgb;
}
