#include "zubzub.h"
#include <QTransform>

zubzub::zubzub(Vector2 startPos
               , double _mass
               , unsigned int _health
               , double _forceMultiplier
               , double _borderForceMultiplier
               , double _dragFactor
               , double _maxSpeed
               , double _xLimit
               , double _yLimit
               , double _ambulanceSpeed)
    :   pos(startPos)
    ,   ambulancePos(-140, -140)
    ,   mass(_mass)
    ,   forceMultiplier(_forceMultiplier)
    ,   borderForceMultiplier(_borderForceMultiplier)
    ,   dragFactor(_dragFactor)
    ,   maxSpeed(_maxSpeed)
    ,   health(_health)
    ,   xLimit(_xLimit)
    ,   yLimit(_yLimit)
    ,   isDying(false)
    ,   isPickedUp(false)
    ,   ambulanceSpeed(_ambulanceSpeed)
{
    QFileInfoList lst = QDir(":/zub/resources").entryInfoList();
    unsigned int i = 0;
    unsigned int length = lst.length();
    QPixmap* temp;
    while(i != length){
        temp = new QPixmap(lst[i].absoluteFilePath());
        spriteFrames.push_back(temp);
        spriteFrames.push_back(temp);
        spriteFrames.push_back(temp);
        i++;
    }
    currentFrame = 0;

    lst = QDir(":/ambulance/ambulance").entryInfoList();
    i = 0;
    length = lst.length();
    while(i != length){
        temp = new QPixmap(lst[i].absoluteFilePath());
        ambulanceFrames.push_back(temp);
        i++;
    }
    spriteWidthHalf = spriteFrames[0]->width() / 2;
    spriteHeightHalf = spriteFrames[0]->height() / 2;
    ambulanceHeightHalf = ambulanceFrames[0]->height() / 2;
    ambulanceWidthHalf = ambulanceFrames[0]->width() / 2;
    if(xLimit > yLimit) engageRadius = yLimit / 4;
    else engageRadius = xLimit / 4;
    borderRadius = engageRadius / 2;
}

void zubzub::graphicsProcess(QPainter* painter, Vector2 cursorPos, bool drawVectors, double vectorMultiplier){
    if(!isDying){
        if(drawVectors){
            double x = pos.xP();
            double y = pos.yP();
            painter->setPen(Qt::red);
            painter->drawLine(x,y,(force.xP()*vectorMultiplier+x),(force.yP()*vectorMultiplier+y));
            painter->setPen(Qt::darkRed);
            painter->drawLine(x,y,(drag.xP()*vectorMultiplier+x),(drag.yP()*vectorMultiplier+y));
            painter->setPen(Qt::yellow);
            painter->drawLine(x,y,(borderForce.xP()*vectorMultiplier+x),(borderForce.yP()*vectorMultiplier+y));
            painter->setPen(Qt::cyan);
            painter->drawLine(x,y,(resultingForce.xP()*vectorMultiplier+x),(resultingForce.yP()*vectorMultiplier+y));
            painter->setPen(Qt::blue);
            painter->drawLine(x,y,(acceleration.xP()*vectorMultiplier+x),(acceleration.yP()*vectorMultiplier+y));
            painter->setPen(Qt::green);
            painter->drawLine(x,y,(speed.xP()*vectorMultiplier+x),(speed.yP()*vectorMultiplier+y));
        }
        if(cursorPos.xP() > pos.xP()){
            if(speed.length() == 0) painter->drawPixmap(pos.xP() - spriteWidthHalf, pos.yP() - spriteHeightHalf, *spriteFrames[9]);
            else painter->drawPixmap(pos.xP() - spriteWidthHalf, pos.yP() - spriteHeightHalf, *spriteFrames[currentFrame]);
        }
        else{
            if(speed.length() == 0) painter->drawImage(pos.xP() - spriteWidthHalf, pos.yP() - spriteHeightHalf, spriteFrames[9]->toImage().mirrored(true, false));
            else painter->drawImage(pos.xP() - spriteWidthHalf, pos.yP() - spriteHeightHalf, spriteFrames[currentFrame]->toImage().mirrored(true, false));
        }
        currentFrame++;
        if(currentFrame == spriteFrames.length()) currentFrame = 0;
    }
    else{
        if(!isPickedUp && (ambulancePos.xP() > pos.xP() || ambulancePos.yP() > pos.yP())) isPickedUp = true;
        if(!isPickedUp){
            painter->drawImage(pos.xP() - spriteWidthHalf, pos.yP() - spriteHeightHalf, spriteFrames[3]->toImage().transformed(QTransform().rotate(270)));
            painter->drawPixmap(ambulancePos.xP() - ambulanceWidthHalf, ambulancePos.yP() - ambulanceHeightHalf, *ambulanceFrames[currentFrame]);
            ambulancePos += pos.normalized(ambulanceSpeed);
            currentFrame++;
            if(currentFrame == ambulanceFrames.length()) currentFrame = 0;
        }
        else{
            painter->drawPixmap(ambulancePos.xP() - ambulanceWidthHalf, ambulancePos.yP() - ambulanceHeightHalf, *ambulanceFrames[currentFrame]);
            ambulancePos += Vector2(xLimit, yLimit).normalized(ambulanceSpeed);
            currentFrame++;
            if(currentFrame == ambulanceFrames.length()) currentFrame = 0;
        }
        if((ambulancePos.xP() > xLimit + ambulanceWidthHalf) || (ambulancePos.yP() > yLimit + ambulanceHeightHalf)){
            emit died(this);
            delete this;
        }
    }
}

void zubzub::physicsProcess(Vector2 cursorPos, double timeDeltaMS){
    // if(!isDying){

    //     double coefficient = yLimit / xLimit;

    //     if(pos.xP() * coefficient > pos.yP()){
    //         if((xLimit - pos.xP()) * coefficient < pos.yP()){ // 1 тр
    //             if((xLimit - pos.xP()) < borderRadius) borderForce = Vector2(-1, 0).normalized(borderForceMultiplier / (xLimit - pos.xP()));
    //             else borderForce = Vector2(0,0);
    //         }
    //         else{                      // 2 тр
    //             if(pos.yP() < borderRadius) borderForce = Vector2(0, 1).normalized(borderForceMultiplier / pos.yP());
    //             else borderForce = Vector2(0,0);
    //         }
    //     }
    //     else{
    //         if(yLimit - pos.yP() > (pos.xP() * coefficient)){ // 3 тр
    //             if(pos.xP() < borderRadius) borderForce = Vector2(1,0).normalized(borderForceMultiplier / pos.xP());
    //             else borderForce = Vector2(0,0);
    //         }
    //         else{                      // 4 тр
    //             if((yLimit - pos.yP()) < borderRadius) borderForce = Vector2(0, -1).normalized(borderForceMultiplier / (yLimit - pos.yP()));
    //             else borderForce = Vector2(0,0);
    //         }
    //     }

    //     if((pos - cursorPos).length() < engageRadius){
    //         force = (pos - cursorPos).normalized(forceMultiplier / (pos - cursorPos).length());
    //         drag = -(force + borderForce).normalized(dragFactor * mass);
    //         if(drag.length() > force.length()) drag = drag.normalized(force.length());
    //     }
    //     else{
    //         force = Vector2(0,0);
    //         drag = -(speed + borderForce).normalized(dragFactor * mass);
    //     }

    //     if(speed.length() < 0.01 && force.length() == 0 && borderForce.length() == 0){
    //         resultingForce = Vector2(0,0);
    //         acceleration = Vector2(0,0);
    //         speed = Vector2(0,0);
    //         drag = Vector2(0,0);
    //     }
    //     else{
    //         resultingForce = (force + drag) + borderForce;
    //         acceleration = resultingForce / mass;
    //         speed += acceleration * timeDeltaMS / 100;
    //         if(speed.length() > maxSpeed) speed = speed.normalized(maxSpeed);
    //         pos += speed * timeDeltaMS;
    //     }
    //     if(pos.xP() < 0){
    //         pos += Vector2(xLimit, 0);
    //         speed = Vector2(0,0);
    //     }
    //     else if(pos.xP() > xLimit){
    //         pos -= Vector2(xLimit, 0);
    //         speed = Vector2(0,0);
    //     }
    //     else if(pos.yP() < 0){
    //         pos += Vector2(0, yLimit);
    //         speed = Vector2(0,0);
    //     }
    //     else if(pos.yP() > yLimit){
    //         pos -= Vector2(0, yLimit);
    //         speed = Vector2(0,0);
    //     }
    // }
}

Vector2 zubzub::position(){
    return pos;
}

void zubzub::takeHit(bool isFatal){
    if(!isDying){
        if(isFatal){
            currentFrame = 0;
            isDying = true;
        }
    }
}

void zubzub::changeLimits(QResizeEvent* ev){
    pos = Vector2(pos.xP() / xLimit * ev->size().width(), pos.yP() / yLimit * ev->size().height());
    xLimit = ev->size().width();
    yLimit = ev->size().height();
    if(xLimit > yLimit) engageRadius = yLimit / 4;
    else engageRadius = xLimit / 4;
    borderRadius = engageRadius / 2;

}

unsigned int zubzub::hp() const{
    return health;
}

void zubzub::setHp(unsigned int _health){
    health = _health;
}

bool zubzub::isCurrentlyDying(){
    return isDying;
}
