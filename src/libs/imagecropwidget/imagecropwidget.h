#pragma once

#include <memory>

#include <QDialog>
#include <QPoint>

class ImageCropWidget : public QDialog
{
    Q_OBJECT
public:
    explicit ImageCropWidget();
    void setImage(std::shared_ptr<QPixmap> image);
    QPixmap getCroppedImage();
    bool proceed();

private:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    QPixmap *_source;
    bool _isSelecting;
    QPoint _startPoint;
    QPoint _endPoint;
};
