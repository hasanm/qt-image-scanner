#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QCloseEvent>
#include <QMessageBox>
#include <QSettings>
#include <QImage>

#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class QComboBox;
class QVBoxLayout;
class QIcon;
class QTableWidget;
class QGridLayout;
class QScrollArea;
class QScrollBar;
class QSlider; 
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void defaultLoad();

protected:

private slots:
    void on_inputPushButton_pressed();
    void onLoad();
    void onGraphic();
    void on_outputPushButton_pressed();
  void onZoomIn();
  void onZoomOut();

  void basicThreshold();
  void onSlider(int value);


private:
  void setImage(const cv::Mat &src);
  void setImageGray(const cv::Mat &src);
  void scaleImage();

  QPushButton *quitButton;
  QPushButton *loadButton;
  QPushButton *graphicButton;
  QPushButton *zoomInButton;
  QPushButton *zoomOutButton;
  QPushButton *thresholdButton;
  QSlider *thresholdSlider;

  QLabel *sliderLabel;
  QLabel *dimensionLabel; 
  
  QVBoxLayout *contentLayout;
  QImage image;
  QLabel *imageLabel;
  QScrollArea* scrollArea;
  double scaleFactor;
  cv::Mat mat;

  // Menu Variables
  QMenu *fileMenu;
  QAction *exitAction;
  QAction *basicThresholdAction;

  int sliderValue; 
  
};

#endif // MAINWINDOW_H
