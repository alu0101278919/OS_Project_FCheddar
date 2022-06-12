#ifndef CALENDAR_H
#define CALENDAR_H

#include <QDialog>
#include <QDate>

namespace Ui {
    class Calendar;
}

class Calendar : public QDialog {
    Q_OBJECT

public:
    explicit Calendar(QWidget *parent = nullptr);
    ~Calendar();

    QString get_selectedDate(void) const;

private slots:
    void on_acceptRejectButtons_accepted();
    void on_acceptRejectButtons_rejected();
    void on_calendarWidget_clicked(const QDate &date);

private:
    Ui::Calendar *ui;
    QDate selectedDate_;
};

#endif // CALENDAR_H
