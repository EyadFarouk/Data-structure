// BackgroundWidget.h
#pragma once
#include <QWidget>
#include <QPixmap>
#include <QPainter>

class BackgroundWidget : public QWidget {
    Q_OBJECT
public:
    explicit BackgroundWidget(const QString& imgPath, QWidget* parent=nullptr)
        : QWidget(parent), bg(imgPath) {}

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.drawPixmap(rect(), bg);           // draws & scales to fill
        QWidget::paintEvent(nullptr);      // paint child widgets on top
    }

private:
    QPixmap bg;
};
