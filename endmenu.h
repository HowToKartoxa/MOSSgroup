#ifndef ENDMENU_H
#define ENDMENU_H

#include <QWidget>

namespace Ui {
class endMenu;
}

class endMenu : public QWidget
{
    Q_OBJECT

public:
    explicit endMenu(QWidget *parent = nullptr);
    ~endMenu();

private:
    Ui::endMenu *ui;

signals:
    void finishedGame();
    void restart();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // ENDMENU_H
