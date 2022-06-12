#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
    class Settings;
}

class Settings : public QDialog {
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    explicit Settings(const Settings& otherSettings, QWidget *parent = nullptr);
    ~Settings();

    QColor get_base_color(void) const;
    QColor get_active_color(void) const;
    int x_size(void) const;

    void set_color(QColor);
    void set_active_color(QColor);
    void set_x_size(int);

private slots:
    void on_colorButton_clicked();
    void on_activeColorButton_clicked();
    void on_acceptDenyBox_accepted();

private:
    Ui::Settings *ui;
    QColor baseColor;
    QColor activeColor;
    int xAxis;
};

#endif // SETTINGS_H
