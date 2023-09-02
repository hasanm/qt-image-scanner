#include <QtWidgets>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include "mainwindow.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  sliderValue(50),
  pixmap(nullptr)
{
  QWidget *root = new QWidget(this);
  QWidget *top = new QWidget(this);
  QWidget *secondLine = new QWidget(this);
  QWidget *content = new QWidget(this);

  /* Top Layout */
  QHBoxLayout *topLayout = new QHBoxLayout(top);
  loadButton = new QPushButton(QString("Reload"), this);
  connect(loadButton, &QPushButton::clicked, this, &MainWindow::onLoad );
  topLayout->addWidget(loadButton);
  
  quitButton = new QPushButton(QString ("Quit"), this);
  quitButton->setToolTip(QString("A tooltip"));
  QFont font ("Courier");
  quitButton->setFont(font);
  QIcon icon("/home/p-hasan/work/qt/mrchester/my_icon.png");
  quitButton->setIcon(icon);
  connect(quitButton, &QPushButton::clicked, qApp, &QApplication::quit);
  topLayout->addWidget(quitButton);



  thresholdSlider = new QSlider(Qt::Horizontal, this);
  thresholdSlider->setMinimum(0);
  thresholdSlider->setMaximum(255);
  thresholdSlider->setValue(sliderValue);
  connect(thresholdSlider, &QSlider::valueChanged, this, &MainWindow::onSlider);
  topLayout->addWidget(thresholdSlider);

  sliderLabel = new QLabel(QString("Slider Value : "), this);
  topLayout->addWidget(sliderLabel);

  thresholdButton = new QPushButton(QString("Apply Threshold"), this);
  connect(thresholdButton, &QPushButton::clicked, this, &MainWindow::basicThreshold);
  topLayout->addWidget(thresholdButton);

  /* Second Layout */
  QHBoxLayout *secondLayout = new QHBoxLayout(secondLine);
  dimensionLabel = new QLabel(QString("No Image Loaded"), this);
  secondLayout->addWidget(dimensionLabel);



  /* Content Layout */
  contentLayout = new QVBoxLayout(content);
  scaleFactor = 0.18;

  scene = new QGraphicsScene(this);
  view = new MyGraphicsView(this);
  view->setScene(scene);
  view->scale(scaleFactor,scaleFactor);
  view->setBackgroundRole(QPalette::Base);
  contentLayout->addWidget(view);

  /* Root Layout */
  QVBoxLayout *rootLayout = new QVBoxLayout(root);
  rootLayout->addWidget(top);
  rootLayout->addWidget(secondLine);
  rootLayout->addWidget(content);
  setCentralWidget(root);

  exitAction = new QAction(tr("E&xit"));
  basicThresholdAction = new QAction(tr("&Basic Threshold"));
  connect (exitAction, &QAction::triggered,
           qApp, &QApplication::quit);

  connect (basicThresholdAction, &QAction::triggered,
           this, &MainWindow::basicThreshold);

  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(basicThresholdAction);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAction);



  resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setImage(const Mat &src)
{

  Mat dest;
  cvtColor(src, dest,COLOR_BGR2RGB);
  const QImage newImage((uchar*) dest.data, dest.cols, dest.rows, dest.step, QImage::Format_RGB888);

  image = newImage;
  QPixmap pix;
  pix = QPixmap::fromImage(image);
  QRect r1(100, 200, 11, 16);
}

void MainWindow::setImageGray(const Mat &src)
{

  Mat dest;
  cvtColor(src, dest,COLOR_BGR2RGB);
  const QImage newImage((uchar*) dest.data, dest.cols, dest.rows, dest.step, QImage::Format_RGB888);

  image = newImage;
  QPixmap pix;
  pix = QPixmap::fromImage(image);

  scene->removeItem(pixmap);
  pixmap = scene->addPixmap(pix);
}


void MainWindow::basicThreshold()
{

  Mat gray, dst;
  int threshold_value = sliderValue;
  int threshold_type = 3;
  int const max_value = 255;
  int const max_type = 4;
  int const max_binary_value = 255;

  cvtColor(mat, gray, COLOR_BGR2GRAY);
  threshold(gray, dst, threshold_value, max_binary_value, threshold_type);

  setImageGray(dst);
}


void MainWindow::onSlider(int value) {
  sliderLabel->setText(QString("Slider Value: %1").arg(value));
  sliderValue = value;
}

void MainWindow::defaultLoad()
{
    mat = imread("/data/homeworks/test_data/11.JPG", IMREAD_COLOR);

    Mat dest;
    cvtColor(mat,dest, COLOR_BGR2RGB);
    const QImage image((uchar *) dest.data, dest.cols, dest.rows, dest.step, QImage::Format_RGB888);


    QPixmap pix = QPixmap::fromImage(image);
    if (pixmap != nullptr) {
        scene->removeItem(pixmap);
    }
    pixmap = scene->addPixmap(pix);
}

void MainWindow::onLoad() {

    qDebug() << "Reloading"; 
    defaultLoad();
} 
