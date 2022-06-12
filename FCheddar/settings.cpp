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
    ui->xAxisBox->setValue(xAxis);
}


// Settings Constructor
Settings::Settings(const Settings& otherSettings, QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::Settings)
    ,baseColor(otherSettings.get_base_color()) // Default base line color for tasks
    ,activeColor(otherSettings.get_active_color()) // Default active line color for tasks
{
    ui->setupUi(this);
    xAxis = otherSettings.x_size();
}


// Settings Destructor
Settings::~Settings()
{
    delete ui;
}


// GETTERS
QColor Settings::get_base_color(void) const{
    return baseColor;
}


QColor Settings::get_active_color(void) const{
    return activeColor;
}

int Settings::x_size(void) const{
    return xAxis;
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



void Settings::on_acceptDenyBox_accepted()
{
    xAxis = ui->xAxisBox->value();
}

