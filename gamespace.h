#ifndef GAMESPACE_H
#define GAMESPACE_H

#include <QWidget>
#include "zubzub.h"
#include <QTimer>
#include <QSoundEffect>

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
    int difficulty;

    zubzub* zub;
    QList<zubzub*> zubList;

    QTimer* updateTimer;
    QTimer* cursorUpdateTimer;

    bool gameGoing;

    QList<QPixmap*> cursorFrames;
    unsigned int currentCursorFrame;

    QSoundEffect* mainTheme;



private slots:
    void paintEvent(QPaintEvent* ev);
    void resizeEvent(QResizeEvent* ev);
    void updateEvent();
    void mousePressEvent(QMouseEvent* ev);
    void onZubZubDied(zubzub*);

    void cursorUpdateEvent();

    void changeTheme();

    void updateZubProgressBar(zubzub *zub);

signals:
    void finishedGame();
};

#endif // GAMESPACE_H
