#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

    QColor get_base_color(void);
    QColor get_active_color(void);
    int x_size(void);

    void set_color(QColor);
    void set_active_color(QColor);
    void set_x_size(int);

private slots:
    void on_colorButton_clicked();
    void on_activeColorButton_clicked();


private:
    Ui::Settings *ui;
    QColor baseColor;
    QColor activeColor;
    int xAxis;
};

#endif // SETTINGS_H
