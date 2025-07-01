#include "gamespace.h"
#include "ui_gamespace.h"
#include <QMouseEvent>

GameSpace::GameSpace(QWidget *parent, int difficulty, bool _showVectors, bool _useFullscreen, bool _playSounds)
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
{
    srand(time(0));
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    setWindowTitle("Chuchelo");
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
    }
    else{
        nightmareMode = true;

        //////////////////////////////////////

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


        ///////////////////////////////////////

    }
    else{
        connect(zub, &zubzub::died, this, &GameSpace::onZubZubDied);
        gameGoing = true;
    }
}

void GameSpace::updateEvent(){
    if(nightmareMode){

        //////////////////////////////////////

    }
    else if(gameGoing) zub->physicsProcess(Vector2(mapFromGlobal(this->cursor().pos()).rx(), mapFromGlobal(this->cursor().pos()).ry()), 25);
    repaint();
}

void GameSpace::paintEvent(QPaintEvent* ev){
    QPainter painter(this);
    if(nightmareMode){

        ////////////////////////////////////////

    }
    else if(gameGoing) zub->graphicsProcess(&painter, Vector2(mapFromGlobal(this->cursor().pos()).rx(), mapFromGlobal(this->cursor().pos()).ry()), showVectors);
    QWidget::paintEvent(ev);
}

void GameSpace::mousePressEvent(QMouseEvent* ev){
    if(!cursorUpdateTimer->isActive())cursorUpdateTimer->start();
    if(nightmareMode){

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

        //////////////////////////////////

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
    if(nightmareMode){

        ///////////////////////////////////

    }
    else if(gameGoing) zub->changeLimits(ev);
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
