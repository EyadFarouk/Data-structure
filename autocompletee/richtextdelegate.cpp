#include "richtextdelegate.h"


void RichTextDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyle *style = option.widget->style();
    style->drawControl(QStyle::CE_ItemViewItem, &option, painter, option.widget);

    QTextDocument doc;
    doc.setHtml(index.data(Qt::DisplayRole).toString());
    QFont widgetFont = option.widget->font();
    doc.setDefaultFont(widgetFont);
    painter->save();
    painter->translate(option.rect.topLeft() + QPoint(16, 8));
    doc.drawContents(painter);
    painter->restore();
    // *p
    // p*
}

QSize RichTextDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QTextDocument doc;
    doc.setHtml(index.data(Qt::DisplayRole).toString());

    QFont widgetFont = option.widget->font();
    doc.setDefaultFont(widgetFont);
    doc.setTextWidth(option.rect.width());

    QFontMetrics metrics(widgetFont);
    const int verticalPadding = metrics.height() / 2;

    return QSize(
        doc.idealWidth() + 32, // Horizontal padding (16px * 2)
        doc.size().height() + verticalPadding
        );

}
