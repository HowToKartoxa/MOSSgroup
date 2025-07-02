#include "gamespace.h"
#include "ui_gamespace.h"
#include <QMouseEvent>

GameSpace::GameSpace(QWidget *parent, int _difficulty, bool _showVectors, bool _useFullscreen, bool _playSounds)
    : QWidget(parent)
    , ui(new Ui::GameSpace)
    , nightmareMode(false)
    , showVectors(_showVectors)
    , useFullscreen(_useFullscreen)
    , playSounds(_playSounds)
    , zub(nullptr)
    , zubList()
    , updateTimer(nullptr)
    , cursorUpdateTimer(nullptr)
    , gameGoing(false)
    , cursorFrames()
    , difficulty (_difficulty)
{

    srand(time(0));
    ui->setupUi(this);
    ui->progressBar->setValue(100);
    ui->progressBar->setStyleSheet(R"(
QProgressBar {
        border: 1px solid #555;
        border-radius: 0px;
        background-color: #eee;
        text-align: center;
    }

    QProgressBar::groove {
        height: 8px;
        margin: 0px;
    }
QProgressBar::chunk {
        background-color: #ed2e46;
        border-radius: 0px;
        min-width: 8px;
    }
)");
    setFocusPolicy(Qt::StrongFocus);
    setWindowIcon(QIcon(":/zub/resources/0.png"));
    if(useFullscreen) showFullScreen();
    else show();
    if(difficulty == 0){
        nightmareMode = false;
        zub = new zubzub(Vector2(width() / 2 // start pos
                         , height() / 2)     //
                         , 20                // mass
                         , 3    // hp
                         , 30   // forceMult
                         , 60   // borderForceMult
                         , 0.01 // dragCoeff
                         , 50   // maxSpeed
                         , width()           // xLimit
                         , height()          // yLimit
                         , 8);               // ambulanceSpeed
        difficulty = zub->hp();
    }
    else if(difficulty == 1){
        nightmareMode = false;
        zub = new zubzub(Vector2(width() / 2 // start pos
                                 , height() / 2)     //
                         , 25                // mass
                         , 6    // hp
                         , 70   // forceMult
                         , 120   // borderForceMult
                         , 0.01 // dragCoeff
                         , 50   // maxSpeed
                         , width()           // xLimit
                         , height()          // yLimit
                         , 8);
        difficulty = zub->hp();
    }
    else if(difficulty == 2){
        nightmareMode = false;
        zub = new zubzub(Vector2(width() / 2 // start pos
                                 , height() / 2)     //
                         , 40               // mass
                         , 10    // hp
                         , 230   // forceMult
                         , 400   // borderForceMult
                         , 0.01 // dragCoeff
                         , 90   // maxSpeed
                         , width()           // xLimit
                         , height()          // yLimit
                         , 8);
        difficulty = zub->hp();
    }
    else{
        nightmareMode = true;
        zubzub* temp = new zubzub(Vector2(width() / 3 // start pos
                                          , height() / 4)     //
                                  , 40               // mass
                                  , 10    // hp
                                  , 230   // forceMult
                                  , 400   // borderForceMult
                                  , 0.01 // dragCoeff
                                  , 90   // maxSpeed
                                  , width()           // xLimit
                                  , height()          // yLimit
                                  , 8);
        zubList.push_back(temp);
        temp = new zubzub(Vector2(width() / 3 * 2 // start pos
                                          , height() / 4)     //
                                  , 40               // mass
                                  , 10    // hp
                                  , 230   // forceMult
                                  , 400   // borderForceMult
                                  , 0.01 // dragCoeff
                                  , 90   // maxSpeed
                                  , width()           // xLimit
                                  , height()          // yLimit
                                  , 8);
        zubList.push_back(temp);
        temp = new zubzub(Vector2(width() / 2 // start pos
                                          , height() / 4 * 3)     //
                                  , 40               // mass
                                  , 10    // hp
                                  , 230   // forceMult
                                  , 400   // borderForceMult
                                  , 0.01 // dragCoeff
                                  , 90   // maxSpeed
                                  , width()           // xLimit
                                  , height()          // yLimit
                                  , 8);
        zubList.push_back(temp);
    }
    updateTimer = new QTimer(this);
    cursorUpdateTimer = new QTimer(this);
    updateTimer->setInterval(10);
    cursorUpdateTimer->setInterval(12);
    connect(updateTimer, &QTimer::timeout, this, &GameSpace::updateEvent);
    connect(cursorUpdateTimer, &QTimer::timeout, this, &GameSpace::cursorUpdateEvent);
    updateTimer->start();

    QFileInfoList lst = QDir(":/bat/cursor").entryInfoList();
    unsigned int i = 0;
    unsigned int length = lst.length();
    QPixmap* temp;
    while(i != length){
        temp = new QPixmap(lst[i].absoluteFilePath());
        *temp = temp->scaledToHeight(50, Qt::SmoothTransformation);
        cursorFrames.push_back(temp);
        i++;
    }
    currentCursorFrame = 0;

    this->setCursor(QCursor(*cursorFrames[0], 46, 7));

    if(_playSounds){
        mainTheme = new QSoundEffect(this);
        mainTheme->setSource(QUrl(QString("qrc:///snd/sounds/mainTheme%1.wav").arg(rand()%3)));
        mainTheme->setVolume(0.2f);
        mainTheme->play();
        connect(mainTheme, &QSoundEffect::playingChanged, this, &GameSpace::changeTheme);
    }

    if(nightmareMode){
        connect(zubList[0], &zubzub::died, this, &GameSpace::onZubZubDied);
        connect(zubList[1], &zubzub::died, this, &GameSpace::onZubZubDied);
        connect(zubList[2], &zubzub::died, this, &GameSpace::onZubZubDied);
    }
    else{
        connect(zub, &zubzub::died, this, &GameSpace::onZubZubDied);
    }
    gameGoing = true;
}

void GameSpace::updateEvent(){
    if(gameGoing){
        if(nightmareMode){
            zubList[0]->physicsProcess(Vector2(mapFromGlobal(this->cursor().pos()).rx(), mapFromGlobal(this->cursor().pos()).ry()), 25);
            zubList[1]->physicsProcess(Vector2(mapFromGlobal(this->cursor().pos()).rx(), mapFromGlobal(this->cursor().pos()).ry()), 25);
            zubList[2]->physicsProcess(Vector2(mapFromGlobal(this->cursor().pos()).rx(), mapFromGlobal(this->cursor().pos()).ry()), 25);
        }
        else zub->physicsProcess(Vector2(mapFromGlobal(this->cursor().pos()).rx(), mapFromGlobal(this->cursor().pos()).ry()), 25);
    }
    repaint();


}

void GameSpace::paintEvent(QPaintEvent* ev){
    QPainter painter(this);
    if(gameGoing){
        if(nightmareMode){
            zubList[0]->graphicsProcess(&painter, Vector2(mapFromGlobal(this->cursor().pos()).rx(), mapFromGlobal(this->cursor().pos()).ry()), showVectors);
            zubList[1]->graphicsProcess(&painter, Vector2(mapFromGlobal(this->cursor().pos()).rx(), mapFromGlobal(this->cursor().pos()).ry()), showVectors);
            zubList[2]->graphicsProcess(&painter, Vector2(mapFromGlobal(this->cursor().pos()).rx(), mapFromGlobal(this->cursor().pos()).ry()), showVectors);
        }
        else zub->graphicsProcess(&painter, Vector2(mapFromGlobal(this->cursor().pos()).rx(), mapFromGlobal(this->cursor().pos()).ry()), showVectors);
    }
    QWidget::paintEvent(ev);

}

void GameSpace::mousePressEvent(QMouseEvent* ev){
    if(!cursorUpdateTimer->isActive())cursorUpdateTimer->start();
    if(nightmareMode){
        unsigned int i = 0;
        bool played_sound = false;
        while(i < 3){
            if((Vector2(mapFromGlobal(this->cursor().pos()).rx(), mapFromGlobal(this->cursor().pos()).ry()) - zubList[i]->position()).length() < 25){
                if(playSounds){
                    QSoundEffect* effect = new QSoundEffect(this);
                    effect->setVolume(0.5f);
                    effect->setSource(QUrl("qrc:///snd/sounds/hit.wav"));
                    effect->play();
                    played_sound = true;
                }
                if(!zubList[i]->isCurrentlyDying()){
                    unsigned int eval = zubList[i]->hp();
                    eval--;
                    if(eval <= 0){
                        if(playSounds){
                            QSoundEffect* effect = new QSoundEffect(this);
                            effect->setVolume(0.5f);
                            effect->setSource(QUrl("qrc:///snd/sounds/scream.wav"));
                            effect->play();
                            QSoundEffect* effect1 = new QSoundEffect(this);
                            effect1->setVolume(0.5f);
                            effect1->setSource(QUrl("qrc:///snd/sounds/ambulance.wav"));
                            effect1->play();
                        }
                        zubList[i]->takeHit(true);
                    }
                    else zubList[i]->takeHit();
                    zubList[i]->setHp(eval);
                }
            }
            else{
                if(playSounds && !played_sound){
                    QSoundEffect* effect = new QSoundEffect(this);
                    effect->setVolume(0.2f);
                    effect->setSource(QUrl("qrc:///snd/sounds/miss.wav"));
                    effect->play();
                    played_sound = true;
                }
            }
            i++;
        }
        ///////////////////////////////////////

    }
    else{
        if((Vector2(mapFromGlobal(this->cursor().pos()).rx(), mapFromGlobal(this->cursor().pos()).ry()) - zub->position()).length() < 25){
            if(playSounds){
                QSoundEffect* effect = new QSoundEffect(this);
                effect->setVolume(0.5f);
                effect->setSource(QUrl("qrc:///snd/sounds/hit.wav"));
                effect->play();
            }
            if(!zub->isCurrentlyDying()){
                unsigned int eval = zub->hp();
                eval--;

                if(eval <= 0){
                    if(playSounds){

                        QSoundEffect* effect = new QSoundEffect(this);
                        effect->setVolume(0.5f);
                        effect->setSource(QUrl("qrc:///snd/sounds/scream.wav"));
                        effect->play();
                        QSoundEffect* effect1 = new QSoundEffect(this);
                        effect1->setVolume(0.5f);
                        effect1->setSource(QUrl("qrc:///snd/sounds/ambulance.wav"));
                        effect1->play();
                    }
                    zub->takeHit(true);
                }
                else zub->takeHit();
                zub->setHp(eval);
                updateZubProgressBar(zub);
            }
        }
        else{
            if(playSounds){
                QSoundEffect* effect = new QSoundEffect(this);
                effect->setVolume(0.2f);
                effect->setSource(QUrl("qrc:///snd/sounds/miss.wav"));
                effect->play();
            }
        }
    }
    QWidget::mousePressEvent(ev);
}

void GameSpace::onZubZubDied(zubzub* _zub){
    if(nightmareMode){
        bool indicator = true;
        unsigned int i = 0;
        while(indicator){
            if(zubList[i] == _zub){
                disconnect(zub, &zubzub::died, this, &GameSpace::onZubZubDied);
                zubList.removeAt(i);
                indicator = false;
            }
            i++;
        }
        if(zubList.isEmpty()){
            gameGoing = false;
            emit finishedGame();
            hide();
            disconnect(mainTheme, &QSoundEffect::playingChanged, this, &GameSpace::changeTheme);
            mainTheme->stop();
        }
    }
    else{
        gameGoing = false;
        disconnect(zub, &zubzub::died, this, &GameSpace::onZubZubDied);
        //emit finishedGame();
        //delete this;
        hide();
        disconnect(mainTheme, &QSoundEffect::playingChanged, this, &GameSpace::changeTheme);
        mainTheme->stop();
        endMenu* endm = new endMenu;
        endm->show();
        connect(endm, &endMenu::finishedGame, this, &GameSpace::onFinishedGame);
        connect(endm, &endMenu::restart, this, &GameSpace::restart);
    }
}

void GameSpace::onFinishedGame()
{
    emit finishedGame();
}

void GameSpace::restart()
{
    setVisible(false);
    emit restartSignal();
}

void GameSpace::resizeEvent(QResizeEvent* ev){
    if(gameGoing){
        if(nightmareMode){
            zubList[0]->changeLimits(ev);
            zubList[1]->changeLimits(ev);
            zubList[2]->changeLimits(ev);
        }
        else zub->changeLimits(ev);
    }
}

void GameSpace::cursorUpdateEvent(){
    if(currentCursorFrame == cursorFrames.length()){
        cursorUpdateTimer->stop();
        currentCursorFrame = 0;
        setCursor(QCursor(*cursorFrames[currentCursorFrame], 46, 7));
    }
    else{
        setCursor(QCursor(*cursorFrames[currentCursorFrame], 46, 7));
        currentCursorFrame++;
    }

}

GameSpace::~GameSpace()
{
    unsigned int iter = 0;
    while(iter != zubList.length()){
        delete zubList[iter];
        iter++;
    }
    iter = 0;
    while(iter != cursorFrames.length()){
        delete cursorFrames[iter];
        iter++;
    }
    updateTimer->stop();
    cursorUpdateTimer->stop();
    delete updateTimer;
    delete cursorUpdateTimer;
    mainTheme->stop();
    delete mainTheme;
    delete ui;
}

void GameSpace::changeTheme(){
    if(playSounds && mainTheme->isPlaying() == false){
        mainTheme->setSource(QUrl(QString("qrc:///snd/sounds/mainTheme%1.wav").arg(rand()%3)));
        mainTheme->setVolume(0.2f);
        mainTheme->play();
    }
}

void GameSpace::updateZubProgressBar(zubzub *zub)
{

    //if (!zub || zub->isCurrentlyDying()) return;

    unsigned int maxHP = difficulty;
    int currHP = zub->hp();
    int percent = static_cast<int>(100.0 * currHP / maxHP);
    ui->progressBar->setValue(percent);

}

void GameSpace::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F11)
    {
        if(!useFullscreen)
        {
            useFullscreen = true;
            showFullScreen();
        }
        else
        {
            useFullscreen = false;
            showNormal();
        }
    }
}
