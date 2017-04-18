#include "LayersWidget.h"
#include <QInputDialog>

LayersWidget::LayersWidget(QWidget* parent)
        : QDockWidget(parent), layerButtons(this) {
    setupUi(this);
    QVBoxLayout* layout = new QVBoxLayout();

    mmm(layout, "0");

    layout->addSpacerItem(new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layers->setLayout(layout);
}

void LayersWidget::mmm(QVBoxLayout* layout, const char* string) {
    QPushButton* button = new QPushButton(string, this);
    button->setCheckable(true);
    layerButtons.addButton(button);
    layout->addWidget(button);
    connect(button, &QAbstractButton::toggled, [this](bool checked){
        if (checked) {
            int i = -2 - layerButtons.checkedId();
            QString result = QInputDialog::getText(this, "WTF", "How the fuck", QLineEdit::Normal, QString("ID is %1").arg(i));
            printf("result is \"%s\" (isNull %s isEmpty %s)\n", result.toUtf8().data(), result.isNull() ? "true" : "false", result.isEmpty() ? "true" : "false");
            selectedLayer((uint32_t) i); // TODO WHAT THE ACTUAL FUCK COULD YOU NOT
        }
    });
}
