#ifndef GAMESPACE_H
#define GAMESPACE_H

#include <QWidget>
#include "zubzub.h"
#include <QTimer>
#include <QSoundEffect>
#include "endmenu.h"

namespace Ui {
class GameSpace;
}

class GameSpace : public QWidget
{
    Q_OBJECT

public:
    explicit GameSpace(QWidget *parent, int difficulty, bool _showVectors, bool _useFullscreen, bool _playSounds);
    ~GameSpace();

private:
    Ui::GameSpace *ui;

    bool nightmareMode;
    bool showVectors;
    bool useFullscreen;
    bool playSounds;

    zubzub* zub;
    QList<zubzub*> zubList;

    QTimer* updateTimer;
    QTimer* cursorUpdateTimer;

    bool gameGoing;

    QList<QPixmap*> cursorFrames;
    unsigned int currentCursorFrame;

    QSoundEffect* mainTheme;

    endMenu* endm;


private slots:
    void paintEvent(QPaintEvent* ev) override;
    void resizeEvent(QResizeEvent* ev) override;
    void updateEvent();
    void mousePressEvent(QMouseEvent* ev) override;
    void onZubZubDied(zubzub*);
    void keyPressEvent(QKeyEvent *event) override;

    void cursorUpdateEvent();

    void changeTheme();

    void onFinishedGame();
    void restart();

signals:
    void finishedGame();
    void restartSignal();
};

#endif // GAMESPACE_H
