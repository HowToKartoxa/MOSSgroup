#ifndef ZUBZUB_H
#define ZUBZUB_H
#include <QObject>
#include "vector2.h"
#include <QPainter>
#include <QDir>
#include <QResizeEvent>

class zubzub : public QObject
{
    Q_OBJECT

private:
    QList<QPixmap*> spriteFrames;
    QList<QPixmap*> ambulanceFrames;
    Vector2 force;
    Vector2 drag;
    Vector2 borderForce;
    Vector2 resultingForce;
    Vector2 speed;
    Vector2 acceleration;
    Vector2 pos;
    Vector2 ambulancePos;

    double mass;
    double forceMultiplier;
    double borderForceMultiplier;
    double dragFactor;
    double maxSpeed;

    unsigned int health;
    unsigned int currentFrame;

    double spriteWidthHalf;
    double spriteHeightHalf;

    double ambulanceWidthHalf;
    double ambulanceHeightHalf;

    double xLimit;
    double yLimit;
    double engageRadius;
    double borderRadius;

    bool isDying;
    bool isPickedUp;

    double ambulanceSpeed;

public:
    zubzub(Vector2 startPos
           , double _mass
           , unsigned int _health
           , double _forceMultiplier
           , double _borderForceMultiplier
           , double _dragFactor
           , double _maxSpeed
           , double _xLimit
           , double _yLimit
           , double _ambulanceSpeed);
    void graphicsProcess(QPainter* painter, Vector2 cursorPos, bool drawVactors = false, double vectorMultiplier = 1000);
    void physicsProcess(Vector2 cursor, double timeDeltaMS);
    Vector2 position();
    void takeHit(bool isFatal = false);

    unsigned int hp() const;
    void setHp(unsigned int);

    bool isCurrentlyDying();


signals:
    void died(zubzub* self);


public slots:
    void changeLimits(QResizeEvent* ev);
};

#endif // ZUBZUB_H
