#include "widgets/ColorChooserWidget.h"

ColorChooserWidget::ColorChooserWidget(QWidget* parent): QDockWidget(parent) {
    setupUi(this);
    selectColor(QColor(255, 0, 255));

    connect(editR, SIGNAL(textEdited(const QString&)), SLOT(updateColor(const QString&)));
    connect(editG, SIGNAL(textEdited(const QString&)), SLOT(updateColor(const QString&)));
    connect(editB, SIGNAL(textEdited(const QString&)), SLOT(updateColor(const QString&)));
    connect(editH, SIGNAL(textEdited(const QString&)), SLOT(updateColor(const QString&)));
    connect(editS, SIGNAL(textEdited(const QString&)), SLOT(updateColor(const QString&)));
    connect(editL, SIGNAL(textEdited(const QString&)), SLOT(updateColor(const QString&)));
}

void ColorChooserWidget::selectColor(const QColor& c) {
    color = c;

    QPalette p = selectedColor->palette();
    p.setColor(QPalette::Background, c);
    selectedColor->setPalette(p);

    editR->setText(QString("%1").arg(c.red()));
    editG->setText(QString("%1").arg(c.green()));
    editB->setText(QString("%1").arg(c.blue()));
    if (c.hue() != -1) {
        editH->setText(QString("%1").arg(c.hue()));
        editS->setText(QString("%1").arg(c.saturation()));
    }
    editL->setText(QString("%1").arg(c.lightness()));
    sliderL->setValue(c.lightness());
}



void ColorChooserWidget::updateColor(const QString& s) {
    QObject* senderObject = sender();
    bool ok = false;
    int value = s.toInt(&ok);
    if (!ok) {
        return;
    }

    QColor c = color;
    if (senderObject == editR){
        if (value < 0 || value > 255){
            return;
        }
        c.setRed(value);
    } else if(senderObject == editG){
        if (value < 0 || value > 255){
            return;
        }
        c.setGreen(value);
    } else if(senderObject == editB){
        if (value < 0 || value > 255){
            return;
        }
        c.setBlue(value);
    } else if(senderObject == editH){
        if (value < 0 || value > 359){
            return;
        }
        c.setHsl(value, c.saturation(), c.lightness());
    } else if(senderObject == editS){
        if (value < 0 || value > 255){
            return;
        }
        c.setHsl(c.hue(), value, c.lightness());
    } else if(senderObject == editL){
        if (value < 0 || value > 255){
            return;
        }
        c.setHsl(c.hue(), c.saturation(), value);
    }
    
    selectColor(c);
}

void ColorChooserWidget::on_sliderL_valueChanged(int value) {
    QColor c = color;
    if (value < 0 || value > 255){
        return;
    }
    c.setHsl(c.hue(), c.saturation(), value);
    selectColor(c);
}
