#include "widgets/ColorChooserWidget.h"

ColorChooserWidget::ColorChooserWidget(QWidget* parent): QDockWidget(parent) {
    setupUi(this);
    selectColor(QColor(255, 0, 255));

    connect(editR, SIGNAL(textChanged(const QString&)), SLOT(updateColor()));
    connect(editG, SIGNAL(textChanged(const QString&)), SLOT(updateColor()));
    connect(editB, SIGNAL(textChanged(const QString&)), SLOT(updateColor()));
    connect(editH, SIGNAL(textChanged(const QString&)), SLOT(updateColor()));
    connect(editS, SIGNAL(textChanged(const QString&)), SLOT(updateColor()));
    connect(editL, SIGNAL(textChanged(const QString&)), SLOT(updateColor()));
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
    sliderL->setValue(c.lightness());

    isUpdating = false;
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

void ColorChooserWidget::on_sliderL_valueChanged(int value) {
    editL->setText(str(value));
}
