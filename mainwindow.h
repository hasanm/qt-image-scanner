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
#include "MyGraphicsView.h"

class MyGraphicsView;

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

    void onMousePressed(QMouseEvent* event);
    void onMouseReleased(QMouseEvent* event);
    void onMouseMoved(QMouseEvent* event);

protected:

private slots:
    void basicThreshold();
    void onSlider(int value);
    void onLoad();
    void onNormalize();
    void onGray();
    void onZoomIn();
    void onZoomOut();
    void onAdaptiveThreshold();
    void onCut();
    void onQuitShortcut();
    void onShadeCorrection();
    void onTextChanged(const QString &item);


private:
    void setImage(const cv::Mat &src);
    void setImageGray(const cv::Mat &src);

    QPushButton *quitButton;
    QPushButton *loadButton;
    QPushButton *cutButton;
    QPushButton *thresholdButton;

    QPushButton *normalizeButton;
    QPushButton *grayButton;
    QPushButton *adaptiveThresholdButton;
    QPushButton *shadeCorrectionButton;
    QLineEdit *matrixSizeEdit;

    QPushButton *zoomInButton;
    QPushButton *zoomOutButton;

    QSlider *thresholdSlider;

    QLabel *sliderLabel;
    QLabel *dimensionLabel;
    QLabel *mouseLabel;

    int sliderValue;

    QShortcut *quitShortcut;

    QHBoxLayout *contentLayout;
    QImage image;
    double scaleFactor;
    cv::Mat mat;
    QString fileName;
    QString tmpFileName; 

    QListWidget *fileListWidget;

    // Menu Variables
    QMenu *fileMenu;
    QAction *exitAction;
    QAction *basicThresholdAction;

    // GraphicsView Items
    MyGraphicsView *view;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *pixmap;
    QGraphicsRectItem *rectangle;
    QPointF top;
    QPointF bottom;
    QBrush blueBrush;
    QPen outlinePen;
    int active;

};

#endif // MAINWINDOW_H
