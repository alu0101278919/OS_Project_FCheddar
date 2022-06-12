#include "settings.h"
#include "ui_settings.h"

#include<QColorDialog>

// Settings Constructor
Settings::Settings(QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::Settings)
    ,baseColor(Qt::black) // Default base line color for tasks
    ,activeColor(Qt::blue) // Default active line color for tasks
{
    ui->setupUi(this);
    xAxis = 5;
}

// Settings Destructor
Settings::~Settings()
{
    delete ui;
}

// GETTERS
QColor Settings::get_base_color(void) {
    return baseColor;
}


QColor Settings::get_active_color(void) {
    return activeColor;
}

int Settings::x_size(void) {
    if (ui->xAxisBox->value()) {
        xAxis = ui->xAxisBox->value();
       return xAxis;
    } else {
       return xAxis; // Default x-axis separation
    }
}

// SETTERS
void Settings::set_color(QColor color) {
    baseColor = color;
}

void Settings::set_active_color(QColor color) {
    activeColor = color;
}

void Settings::set_x_size(int xSize) {
    xAxis = xSize;
}

void Settings::on_colorButton_clicked()
{
    baseColor = QColorDialog::getColor(Qt::black, this, "Task Base Line Colour");
}


void Settings::on_activeColorButton_clicked()
{
    activeColor = QColorDialog::getColor(Qt::blue, this, "Task Active Line Colour");
}


