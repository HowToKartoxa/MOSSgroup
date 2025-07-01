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

    zubzub* zub;
    QList<zubzub*> zubList;

    QTimer* updateTimer;
    QTimer* cursorUpdateTimer;

    bool gameGoing;

    QList<QPixmap*> cursorFrames;
    unsigned int currentCursorFrame;

    QSoundEffect* mainTheme;



private slots:
    void paintEvent(QPaintEvent* ev) override;
    void resizeEvent(QResizeEvent* ev) override;
    void updateEvent();
    void mousePressEvent(QMouseEvent* ev) override;
    void onZubZubDied(zubzub*);
    void keyPressEvent(QKeyEvent *event) override;

    void cursorUpdateEvent();

    void changeTheme();

signals:
    void finishedGame();
};

#endif // GAMESPACE_H
