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


static const char INPUT_STRING[] = "/data/tmp_images/in.png";
static const char OUTPUT_STRING[] = "/data/tmp_images/out.png";
static const char OUTPUT_PREFIX[] = "/data/tmp_images/";

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  sliderValue(50),
  pixmap(nullptr),
  outlinePen(Qt::black),
  blueBrush(Qt::blue),
  rectangle(nullptr),
  tmpFileName(QString("/tmp/out.png"))
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

  cutButton = new QPushButton(QString("Cut"), this);
  connect(cutButton, &QPushButton::clicked, this, &MainWindow::onCut);
  topLayout->addWidget(cutButton);

  zoomInButton = new QPushButton(QString("Zoom In"), this);
  connect(zoomInButton, &QPushButton::clicked, this, &MainWindow::onZoomIn);
  topLayout->addWidget(zoomInButton);

  zoomOutButton = new QPushButton(QString("Zoom Out"), this);
  connect(zoomOutButton, &QPushButton::clicked, this, &MainWindow::onZoomOut);
  topLayout->addWidget(zoomOutButton);

  /* Second Layout */
  QHBoxLayout *secondLayout = new QHBoxLayout(secondLine);
  dimensionLabel = new QLabel(QString("No Image Loaded"), this);
  secondLayout->addWidget(dimensionLabel);

  mouseLabel = new QLabel(QString("Mouse: "), this);
  secondLayout->addWidget(mouseLabel);

  normalizeButton = new QPushButton(QString("Normalize"), this);
  connect(normalizeButton, &QPushButton::clicked, this, &MainWindow::onNormalize);
  secondLayout->addWidget(normalizeButton);

  grayButton = new QPushButton(QString ("Gray"), this);
  connect(grayButton, &QPushButton::clicked, this, &MainWindow::onGray);
  secondLayout->addWidget(grayButton);

  adaptiveThresholdButton = new QPushButton(QString("Adaptive Threshold"), this);
  connect(adaptiveThresholdButton, &QPushButton::clicked, this, &MainWindow::onAdaptiveThreshold);
  secondLayout->addWidget(adaptiveThresholdButton);

  matrixSizeEdit = new QLineEdit(this);
  matrixSizeEdit->setValidator( new QIntValidator(15, 5001, this) );
  matrixSizeEdit->setText(QString("1001"));
  secondLayout->addWidget(matrixSizeEdit);

  shadeCorrectionButton = new QPushButton(QString("ShadeCorrection"), this);
  connect(shadeCorrectionButton, &QPushButton::clicked, this, &MainWindow::onShadeCorrection);
  secondLayout->addWidget(shadeCorrectionButton);




  /* Content Layout */
  contentLayout = new QHBoxLayout(content);
  scaleFactor = 0.18;

  fileListWidget = new QListWidget(this);

  new QListWidgetItem(tr("/data/homeworks/test_data/11.JPG"), fileListWidget);
  new QListWidgetItem(tr("/data/homeworks/test_data/1.JPG" ), fileListWidget);
  new QListWidgetItem(tr("/data/homeworks/test_data/2.JPG" ), fileListWidget);
  new QListWidgetItem(tr("/data/homeworks/test_data/3.JPG" ), fileListWidget);
  new QListWidgetItem(tr("/data/homeworks/test_data/4.JPG" ), fileListWidget);
  new QListWidgetItem(tr("/data/homeworks/test_data/5.JPG" ), fileListWidget);

  fileListWidget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding));

  connect(fileListWidget, &QListWidget::currentTextChanged,
          this, &MainWindow::onTextChanged);

  contentLayout->addWidget(fileListWidget);

  scene = new QGraphicsScene(this);
  view = new MyGraphicsView(this);
  view->setMouseTracking(true);
  view->setScene(scene);
  view->scale(scaleFactor,scaleFactor);
  view->setBackgroundRole(QPalette::Base);
  contentLayout->addWidget(view);
  active = 0;

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

  quitShortcut = new QShortcut(QKeySequence(tr("Ctrl+W", "Close")),
                               this);
  connect(quitShortcut, &QShortcut::activated, this, &MainWindow::onQuitShortcut);

  resize(QGuiApplication::primaryScreen()->availableSize()  / 2);
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
    scene->removeItem(pixmap);
    pixmap = scene->addPixmap(pix);
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
    mat = imread(INPUT_STRING, IMREAD_COLOR);

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
}


void MainWindow::onNormalize() {
    Mat channel[3];
    Mat normalized;
    Mat gray;


    Mat mDil, mBlur, mDiff, mNorm;

    mat = imread(INPUT_STRING, IMREAD_COLOR);
    
    split(mat, channel);
    normalized.create(mat.size(), mat.type());

    for (int i=0; i < 3; i++) {
        dilate(channel[i], mDil, Mat::ones(7,7, CV_8UC1), Point(-1, -1));
        medianBlur(mDil, mBlur, 21);
        absdiff(channel[i], mBlur, mDiff);
        mDiff = 255 - mDiff;
        normalize(mDiff, mNorm, 0, 255, NORM_MINMAX, CV_8UC1);
        insertChannel(mNorm, normalized, i);
    }

    cvtColor(normalized, gray, COLOR_BGR2GRAY);
    setImageGray(gray);
    auto const pos = fileName.toStdString().find_last_of('/');
    auto const leaf = fileName.toStdString().substr(pos + 1);
    String input_prefix(OUTPUT_PREFIX);
    imwrite(input_prefix + leaf, gray);
}

void MainWindow::onAdaptiveThreshold() {
    Mat channel[3];
    Mat normalized;
    Mat gray;
    Mat dst;


    Mat mDil, mBlur, mDiff, mNorm;
    split(mat, channel);
    normalized.create(mat.size(), mat.type());

    for (int i=0; i < 3; i++) {
        dilate(channel[i], mDil, Mat::ones(7,7, CV_8UC1), Point(-1, -1));
        medianBlur(mDil, mBlur, 21);
        absdiff(mBlur, channel[i], mDiff);
        absdiff(255, mDiff, mDiff);

        normalize(mDiff, mNorm, 0, 255, NORM_MINMAX, CV_8UC1);
        insertChannel(mNorm, normalized, i);
    }
    setImage(normalized);
}

void MainWindow::onGray() {
    Mat gray;

    cvtColor(mat, gray, COLOR_BGR2GRAY);
    setImageGray(gray);
}


void MainWindow::onZoomIn() {
    scaleFactor = 1.2;
    view->scale(scaleFactor,scaleFactor);
}

void MainWindow::onZoomOut() {
    scaleFactor = 0.8;
    view->scale(scaleFactor,scaleFactor);
}


void MainWindow::onMousePressed(QMouseEvent* event) {
    active = 1;
    top = view->mapToScene(event->pos());
    bottom = top + QPoint (100, 100);
    outlinePen.setWidth(2);
    if (rectangle != nullptr) {
        scene->removeItem(rectangle);
    }

    rectangle = new QGraphicsRectItem(QRectF(top, bottom));
    scene->addItem(rectangle);
}
void MainWindow::onMouseReleased(QMouseEvent* event) {
    active = 0;
}
void MainWindow::onMouseMoved(QMouseEvent* event) {
    if (active == 1) {
        if (rectangle != nullptr) {
            scene->removeItem(rectangle);
        }
        bottom = view->mapToScene(event->pos());
        rectangle = new QGraphicsRectItem(QRectF(top, bottom));
        scene->addItem(rectangle);

        QPointF pos = view->mapToScene(event->pos());
        QPoint q = pos.toPoint();
        QPoint p = top.toPoint();
        mouseLabel->setText(QString("Mouse : %1,%2 -> %3,%4").arg(p.x()).arg(p.y()).arg(q.x()).arg(q.y()));
    }
}

void MainWindow::onCut() {
    QPoint p = top.toPoint();
    QPoint q = bottom.toPoint();

    mat = imread(INPUT_STRING, IMREAD_COLOR);
    Rect myRoi(Point(p.x(), p.y()),Point(q.x(), q.y()));
    Mat cropped(mat,myRoi);
    imwrite(INPUT_STRING, cropped);
    mat = imread(INPUT_STRING, IMREAD_COLOR);    
    Mat dest;


    cvtColor(cropped,dest, COLOR_BGR2RGB);
    const QImage image((uchar *) dest.data, dest.cols, dest.rows, dest.step, QImage::Format_RGB888);

    QPixmap pix = QPixmap::fromImage(image);
    if (pixmap != nullptr) {
        scene->removeItem(pixmap);
    }
    pixmap = scene->addPixmap(pix);
    if (rectangle != nullptr) {
        scene->removeItem(rectangle);
        rectangle = nullptr;
    }

}

void MainWindow::onShadeCorrection() {
    Mat gray, dst, result;
    mat = imread(INPUT_STRING, IMREAD_COLOR);
    cvtColor(mat, gray, COLOR_BGR2GRAY);
    int matrixSize = matrixSizeEdit->text().toInt();
    GaussianBlur(gray, dst, Size(matrixSize, matrixSize), 255);
    divide(gray, dst, result, 255);

    setImage(result);
}

void MainWindow::onQuitShortcut()
{
  qApp->quit();
}


void MainWindow::onTextChanged(const QString &item)
{
    qDebug() << item;
    fileName = item;

    mat = imread(fileName.toStdString(), IMREAD_COLOR);
    imwrite(INPUT_STRING, mat);
    Mat dest;
    cvtColor(mat,dest, COLOR_BGR2RGB);
    const QImage image((uchar *) dest.data, dest.cols, dest.rows, dest.step, QImage::Format_RGB888);


    QPixmap pix = QPixmap::fromImage(image);
    if (pixmap != nullptr) {
        scene->removeItem(pixmap);
    }
    pixmap = scene->addPixmap(pix);
}
