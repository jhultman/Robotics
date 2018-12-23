#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>


using namespace cv;
using namespace std;


void printIncorrectNumArgsMsg(int argc)
{
    char msg[50];
    sprintf(msg, "Expected %d args, received %d.", 3, argc);
    cout << msg << endl;
}


void showImage(Mat img)
{
    imshow("Ben", img);
    waitKey(0);
}


void printSize(Mat img)
{
    int m = img.rows;
    int n = img.cols;

    Size sz = img.size();
    assert(m == sz.height);
    assert(n == sz.width);

    cout << "Size: " << sz << endl;
    cout << "Num channels: " << img.channels() << endl;
}


Vec3b blockAverageV1(Mat img, int rowStart, int colStart, int blockSize)
{
    int baseIndex;
    int b, g, r;
    b = g = r = 0;
    int blockArea = blockSize * blockSize;

    for (int row = rowStart; row < rowStart + blockSize; ++row)
    {
        for (int col = colStart; col < colStart + blockSize; ++col)
        {
            baseIndex = img.channels() * (img.cols * row + col);
            baseIndex = img.channels() * (img.step * row + col);
            b += img.data[baseIndex + 0];
            g += img.data[baseIndex + 1];
            r += img.data[baseIndex + 2];
        }
    }

    b = (uchar) b / blockArea;
    g = (uchar) g / blockArea;
    r = (uchar) r / blockArea;

    Vec3b pixel = Vec3b(b, g, r);
    return pixel;
}


Vec3b blockAverageV2(Mat img, int xStart, int yStart, int blockSize)
{
    Rect roi(xStart, yStart, blockSize, blockSize);
    Scalar bgr = mean(img(roi));
    Vec3b pixel = Vec3b(bgr[0], bgr[1], bgr[2]);
    return pixel;
}


Mat downsample(Mat imgIn, int blockSize)
{
    int xOut, yOut;
    int m = imgIn.rows / blockSize;
    int n = imgIn.cols / blockSize;

    Mat3b imgOut = Mat(m, n, CV_8UC3);
    Vec3b pixel;

    for(int xIn = 0; xIn < imgIn.cols - blockSize; xIn += blockSize)
    {
        for(int yIn = 0; yIn < imgIn.rows - blockSize; yIn += blockSize) 
        {
            xOut = xIn / blockSize;
            yOut = yIn / blockSize;

            pixel = blockAverageV2(imgIn, xIn, yIn, blockSize);
            imgOut(yOut, xOut) = pixel;
        }
    }
    return imgOut;
}


int main(int argc, char** argv)
{
    string fpath;
    if(argc > 1)
    {
        fpath = argv[1];
    }
    else
    {
        printIncorrectNumArgsMsg(argc);
        return -1;
    }

    istringstream iss(argv[2]);
    int blockSize;
    bool success = iss >> blockSize;

    if (!success)
    {
        cout << "Expected an integer block size." << endl;
        return -1;
    }

    Mat img = imread(fpath.c_str(), IMREAD_COLOR);
    if(img.empty())
    {
        cout << "Could not open or find the image"  << endl;
        return -1;
    }

    Mat imgDownsampled = downsample(img, blockSize);
    showImage(imgDownsampled);
    return 0;
}
