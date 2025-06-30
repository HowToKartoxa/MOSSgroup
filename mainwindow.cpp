#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , gameSpace(nullptr)
    , difficulty(0)
    , showVectors(false)
    , playSounds(true)
    , useFullscreen(false)
    , mainMenuTheme(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Main menu");
    setWindowIcon(QIcon(":/zub/resources/0.png"));
    mainMenuTheme = new QSoundEffect(this);
    mainMenuTheme->setSource(QUrl("qrc:///snd/sounds/mainmenu.wav"));
    mainMenuTheme->setLoopCount(QSoundEffect::Infinite);
    mainMenuTheme->setVolume(0.1f);

    connect(ui->actionshowvectors, &QAction::toggled, this, &MainWindow::onVectorsToggled);
    connect(ui->actionfullscreen, &QAction::toggled, this, &MainWindow::onFullscreenToggled);
    connect(ui->actionsounds, &QAction::toggled, this, &MainWindow::onSoundsToggled);
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::onDifficultySelected);
    connect(ui->startbtn, &QAbstractButton::clicked, this, &MainWindow::onStartButtonPressed);
    connect(ui->exitbtn, &QAbstractButton::clicked, this, &MainWindow::onExitButtonPressed);\

    if(playSounds) mainMenuTheme->play();
}

void MainWindow::onDifficultySelected(int _difficulty){
    difficulty = _difficulty;
    if(playSounds){
        QSoundEffect* effect = new QSoundEffect(this);
        effect->setVolume(0.5f);
        effect->setSource(QUrl("qrc:///snd/sounds/menusound3.wav"));
        effect->play();
    }
}

void MainWindow::onVectorsToggled(){
    showVectors = !showVectors;
    if(playSounds){
        QSoundEffect* effect = new QSoundEffect(this);
        effect->setVolume(0.5f);
        effect->setSource(QUrl("qrc:///snd/sounds/menusound3.wav"));
        effect->play();
    }
}

void MainWindow::onSoundsToggled(){
    playSounds = !playSounds;
    if(!playSounds) mainMenuTheme->stop();
    else mainMenuTheme->play();
}

void MainWindow::onFullscreenToggled(){
    useFullscreen = !useFullscreen;
    if(playSounds){
        QSoundEffect* effect = new QSoundEffect(this);
        effect->setVolume(0.5f);
        effect->setSource(QUrl("qrc:///snd/sounds/menusound3.wav"));
        effect->play();
    }
}

void MainWindow::onStartButtonPressed(){
    if(playSounds){
        QSoundEffect* effect = new QSoundEffect(this);
        effect->setVolume(0.5f);
        effect->setSource(QUrl("qrc:///snd/sounds/menusound3.wav"));
        effect->play();
        mainMenuTheme->stop();
    }
    delete gameSpace;
    gameSpace = new GameSpace(nullptr, difficulty, showVectors, useFullscreen, playSounds);
    connect(gameSpace, &GameSpace::finishedGame, this, &MainWindow::onFinishedGame);
    setVisible(false);
}

void MainWindow::onExitButtonPressed(){
    if(playSounds){
        QSoundEffect* effect = new QSoundEffect(this);
        effect->setVolume(0.5f);
        effect->setSource(QUrl("qrc:///snd/sounds/menusound3.wav"));
        effect->play();
    }
    delete this;
}

void MainWindow::onFinishedGame(){
    //delete this;
    show();
    if(playSounds) mainMenuTheme->play();
}

MainWindow::~MainWindow()
{
    if(gameSpace != nullptr) delete gameSpace;
    delete ui;
}
