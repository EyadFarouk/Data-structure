#pragma once
#include <QStyledItemDelegate>
#include <QTextDocument>
#include <QPainter>

class RichTextDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        // Draw default styling (background, selection, etc.)
        QStyle *style = option.widget->style();
        style->drawControl(QStyle::CE_ItemViewItem, &option, painter, option.widget);

        // Draw HTML text
        QTextDocument doc;
        doc.setHtml(index.data(Qt::DisplayRole).toString());
        doc.setDefaultStyleSheet("span { color: white; }"); // Enforce white color
        QFont widgetFont = option.widget->font();
        doc.setDefaultFont(widgetFont);
        // Adjust position for padding
        painter->save();
        painter->translate(option.rect.topLeft() + QPoint(16, 8));
        doc.drawContents(painter);
        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QTextDocument doc;
        doc.setHtml(index.data(Qt::DisplayRole).toString());

        // Use the widget's font for accurate metrics
        QFont widgetFont = option.widget->font();
        doc.setDefaultFont(widgetFont);
        doc.setTextWidth(option.rect.width());

        // Calculate vertical padding based on font metrics
        QFontMetrics metrics(widgetFont);
        const int verticalPadding = metrics.height() / 2; // Adjust as needed

        return QSize(
            doc.idealWidth() + 32, // Horizontal padding (16px * 2)
            doc.size().height() + verticalPadding
            );
    }
};
