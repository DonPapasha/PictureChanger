// PictureChanger.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


using namespace cv;
using namespace std;


void Compression(); //Функция выполняющая компрессию качества изображения
void SizeCropper(); //Функция сжимающая размер изображения
void Stretching();  //Функция растягивающая размер изображения
void Rotation();    //Функция поворачивающая изображение на выбранные угол (допустим отрицательный угол)
void Loop();        //Функция основного меню, циклична
void Difference();  //Функция измеряющая длину разницы между изображениями

Mat userImg; //Переменная изначального изображения
Mat oldImg;  //Переменная прошлого шага изменения изображения
Mat newImg;  //Переменная изменённого изображения

string pathUserImg = "Images/Input.jpg"; //Путь до изначального изображения
string pathNewImg = "Images/Output.jpg"; //Путь где сохраняется и хранится изменённое изображение

int main()  //Основной метод, запускающий программу
{
    setlocale(LC_ALL, "Russian");
    waitKey(1);
    userImg = imread(pathUserImg);
    imshow("ImageInput", userImg);
    waitKey(2);
    oldImg = userImg;
    Loop();
}

void Loop()
{
    int _choice = 0;
    cout << "Введите необходый номер команды: " << endl;
    cout << "1 - Компрессия" << endl;
    cout << "2 - Сжатие" << endl;
    cout << "3 - Растяжение" << endl;
    cout << "4 - Поворот" << endl;
    cout << "5 - Сравнение моментов (длина разницы)" << endl;
    cout << "0 - Выход" << endl;

    cin >> _choice;

    if (_choice == 1)
        Compression();

    else if (_choice == 2)
        SizeCropper();

    else if (_choice == 3)
        Stretching();

    else if (_choice == 4)
        Rotation();

    else if (_choice == 5)
        Difference();

    else if (_choice == 0) 
        cout << "До свидания." << endl;
    else
    {
        cout << "Вы ввели неверное значение. Повторите попытку." << endl;
        destroyAllWindows();
    }


    imshow("ImageOutput", newImg);
    waitKey(2);

    oldImg = newImg;

    if (_choice != 0) Loop();
    
}

void Compression()
{
    vector<int> params;
    params.push_back(IMWRITE_JPEG_QUALITY);
    params.push_back(5);   // that's percent, so 100 == no compression, 1 == full 
    imwrite(pathNewImg, oldImg, params);
    newImg = imread(pathNewImg);
}

void SizeCropper()
{
    double fx = 0.5, fy = 0.5;
    resize(oldImg, newImg, Size(), fx, fy, 0.1);
}

void Stretching()
{
    double fx = 1.5, fy = 1.5;
    resize(oldImg, newImg, Size(), fx, fy, 1);
}

void Rotation()
{
    cout << "Напишите угол (целое число) поворота картинки: ";
    Point2f center((oldImg.cols - 1) / 2.0, (oldImg.rows - 1) / 2.0);
    int angle;
    cin >> angle;
    Mat RotNewImg = getRotationMatrix2D(center, angle, 1);
    warpAffine(oldImg, newImg, RotNewImg, oldImg.size());
}
// Compression + Rotation, Streching + Rotation

void Difference()
{

    imwrite(pathNewImg, newImg);
    waitKey(2);
    newImg = imread(pathNewImg, IMREAD_GRAYSCALE);
    userImg = imread(pathUserImg, IMREAD_GRAYSCALE);
    waitKey(1);

    Mat threshUserImg, threshNewImg;
    threshold(userImg, threshUserImg, 127, 255, THRESH_BINARY);
    threshold(newImg, threshNewImg, 127, 255, THRESH_BINARY);

    Moments momUserImg = moments(threshUserImg);
    Moments momNewImg = moments(threshNewImg);

    double huMomUserImg[7];
    double huMomNewImg[7];
    HuMoments(momUserImg, huMomUserImg);
    HuMoments(momNewImg, huMomNewImg);

    for (int i = 0; i < 7; i++) {
        cout << "Моменты изначального изображения H[" << i <<"] : " << huMomUserImg[i] << endl;
    }
    cout << "" << endl;
    for (int i = 0; i < 7; i++) {
        cout << "Моменты изменённого изображения H[" << i << "] : " << huMomNewImg[i] << endl;
    }
    
    double _difference1 = matchShapes(threshUserImg, threshNewImg, CONTOURS_MATCH_I1, 0);
    double _difference2 = matchShapes(threshUserImg, threshNewImg, CONTOURS_MATCH_I2, 0);
    double _difference3 = matchShapes(threshUserImg, threshNewImg, CONTOURS_MATCH_I3, 0);
    
    cout << "Difference #1 = " << _difference1 << endl;
    cout << "Difference #2 = " << _difference2 << endl;
    cout << "Difference #3 = " << _difference3 << endl;

    waitKey(5);
    userImg = imread(pathUserImg);
    newImg = imread(pathNewImg);
    imshow("ImageInput", userImg);
    waitKey(5);
}


