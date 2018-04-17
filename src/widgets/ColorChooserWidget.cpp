#include "widgets/ColorChooserWidget.h"
#include <QPainter>

ColorChooserWidget::ColorChooserWidget(QWidget* parent): QDockWidget(parent) {
    setupUi(this);
    rainbowHS->setPaintFunction([this](const CustomPaintWidget& widget, QPainter& painter) {
        QImage img(widget.size(), QImage::Format_RGB32);
        int L = 128;

        int w = img.width();
        int h = img.height();
        for (int x = 0; x < w; ++x) {
            for (int y = 0; y < h; ++y) {
                int H = x * 359 / (w - 1);
                int S = 255 - y * 255 / (h - 1);
                img.setPixelColor(x, y, QColor::fromHsl(H, S, L));
            }
        }

        int H0 = color.hslHue();
        int S0 = color.hslSaturation();

        int x0 = H0 * (w - 1) / 359;
        int y0 = (255 - S0) * (h - 1) / 255;
        const QColor& cursorColor = QColor(0, 0, 0);
        static const QPoint mask[] = {
                {-3, 0}, {-4, 0}, {-5, 0},
                {0, -3}, {0, -4}, {0, -5},
                {3,  0}, {4,  0}, {5,  0},
                {0,  3}, {0,  4}, {0,  5},
        };
        for (QPoint offset : mask) {
            int x = x0 + offset.x();
            int y = y0 + offset.y();
            if (x >= 0 && x < w && y >= 0 && y < h) {
                img.setPixelColor(x, y, cursorColor);
            }
        }

        painter.drawImage(QPoint(), img);
    });
    gradientV->setPaintFunction([this](const CustomPaintWidget& widget, QPainter& painter) {
        QImage img(widget.size(), QImage::Format_RGB32);
        int H = color.hslHue();
        int S = color.hslSaturation();

        int w = img.width();
        int h = img.height();
        for (int x = 0; x < w; ++x) {
            for (int y = 0; y < h; ++y) {
                int L = 255 - y * 255 / (h - 1);
                img.setPixelColor(x, y, QColor::fromHsl(H, S, L));
            }
        }

        int L0 = color.lightness();

        int y0 = (255 - L0) * (h - 1) / 255;

        painter.drawImage(QPoint(), img);
        QPainterPath path1 = QPainterPath();
        QPainterPath path2 = QPainterPath();
        QPolygon polygon1 = QPolygon{{{w / 2 + 4, y0}, {w / 2 + 8, y0 - 4}, {w / 2 + 8, y0 + 4}}};
        QPolygon polygon2 = QPolygon{{{w / 2 - 5, y0}, {w / 2 - 9, y0 - 4}, {w / 2 - 9, y0 + 4}}};
        path1.addPolygon(polygon1);
        path2.addPolygon(polygon2);
        painter.fillPath(path1, {Qt::white});
        painter.fillPath(path2, {Qt::white});

        painter.setPen(Qt::black);
        painter.drawPolygon(polygon2);
        painter.drawPolygon(polygon1);
    });

    connect(editR, &QLineEdit::textChanged, this, &updateColor);
    connect(editG, &QLineEdit::textChanged, this, &updateColor);
    connect(editB, &QLineEdit::textChanged, this, &updateColor);
    connect(editH, &QLineEdit::textChanged, this, &updateColor);
    connect(editS, &QLineEdit::textChanged, this, &updateColor);
    connect(editL, &QLineEdit::textChanged, this, &updateColor);
}

template<typename T>
QString str(const T& t) {
    return QString("%1").arg(t);
}

void printColor(const QColor& c) {
    int r, g, b, h, s, l;
    c.getRgb(&r, &g, &b);
    c.getHsl(&h, &s, &l);
    printf("r = %d, g = %d, b = %d, h = %d, s = %d, l = %d\n", r, g, b, h, s, l);
}

void ColorChooserWidget::selectColor(const QColor& c) {
    isUpdating = true;

    color = c;

    QPalette p = selectedColor->palette();
    p.setColor(QPalette::Background, c);
    selectedColor->setPalette(p);

    editR->setText(str(c.red()));
    editG->setText(str(c.green()));
    editB->setText(str(c.blue()));
    if (c.hue() != -1) {
        editH->setText(str(c.hslHue()));
        editS->setText(str(c.hslSaturation()));
    }
    editL->setText(str(c.lightness()));

    rainbowHS->update();
    gradientV->update();

    isUpdating = false;
    emit colorSelected(c);
}

#define GET_FROM_EDIT(VAR_NAME, MIN_VAR, MAX_VAR)                    \
int VAR_NAME;                                                        \
{                                                                    \
    bool ok;                                                         \
    VAR_NAME = edit##VAR_NAME->text().toInt(&ok);                    \
    if (!ok || VAR_NAME < (MIN_VAR) || VAR_NAME > (MAX_VAR)) {       \
        return;                                                      \
    }                                                                \
}


void ColorChooserWidget::updateColor() {
    if (isUpdating) {
        return;
    }
    QObject* senderObject = sender();
    QColor c;

    if (senderObject == editR || senderObject == editG || senderObject == editB){
        GET_FROM_EDIT(R, 0, 255);
        GET_FROM_EDIT(G, 0, 255);
        GET_FROM_EDIT(B, 0, 255);
        c.setRgb(R, G, B);
    } else if (senderObject == editH || senderObject == editS || senderObject == editL){
        GET_FROM_EDIT(H, 0, 359);
        GET_FROM_EDIT(S, 0, 255);
        GET_FROM_EDIT(L, 0, 255);
        c.setHsl(H, S, L);
    }

    selectColor(c);
}

void ColorChooserWidget::on_gradientV_mouseDrag(int _, int y) {
    int y1 = gradientV->height() - 1;
    y = qBound(0, y, y1);
    int L = 255 - y * 255 / y1;
    QColor c;
    c.setHsl(color.hslHue(), color.hslSaturation(), L);
    selectColor(c);
}

void ColorChooserWidget::on_rainbowHS_mouseDrag(int x, int y) {
    int x1 = rainbowHS->width() - 1;
    x = qBound(0, x, x1);
    int y1 = rainbowHS->height() - 1;
    y = qBound(0, y, y1);

    int H = x * 359 / x1;
    int S = 255 - y * 255 / y1;

    QColor c;
    c.setHsl(H, S, color.lightness());
    selectColor(c);
}

void ColorChooserWidget::setBrushSize(int bs) {
    brushSize->setValue(bs);
}
