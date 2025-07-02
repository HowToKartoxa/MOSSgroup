#ifndef ENDMENU_H
#define ENDMENU_H

#include <QWidget>
#include <QSoundEffect>

namespace Ui {
class endMenu;
}

class endMenu : public QWidget
{
    Q_OBJECT

public:
    explicit endMenu(bool playSounds_ = true, QWidget *parent = nullptr);
    ~endMenu();

private:
    Ui::endMenu *ui;
    QSoundEffect *endMenuTheme;

    bool playSounds;

signals:
    void finishedGame();
    void restart();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // ENDMENU_H
