#include "calendar.h"
#include "ui_calendar.h"

#include <QCalendarWidget>
#include <QPushButton>
#include <iostream>


Calendar::Calendar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Calendar),
    selectedDate_(QDate::currentDate()) {

    ui->setupUi(this);
    ui->calendarWidget->setSelectedDate(QDate::currentDate());
    ui->selectedDateEdit->setDate(QDate::currentDate());
}


Calendar::~Calendar() {
    delete ui;
}


QString Calendar::get_selectedDate(void) const {
    return selectedDate_.toString("yyyy-MM-dd");
}


void Calendar::on_acceptRejectButtons_accepted() {
    accept();
}


void Calendar::on_acceptRejectButtons_rejected() {
    reject();
}


void Calendar::on_calendarWidget_clicked(const QDate &date) {
    selectedDate_ = ui->calendarWidget->selectedDate();
    ui->selectedDateEdit->setDate(selectedDate_);
}

