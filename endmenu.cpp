#include "endmenu.h"
#include "ui_endmenu.h"

endMenu::endMenu(bool playSounds_, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::endMenu)
    , playSounds(playSounds_)
{
    ui->setupUi(this);
    setWindowTitle("The end");
    setWindowIcon(QIcon(":/zub/resources/0.png"));
    ui->label_2->setStyleSheet("font-size: 30px;");
    setFixedSize(this->width(), this->height());
    QPixmap pixmap(":/statPictures/pictures/grob.png");
    pixmap = pixmap.scaledToWidth(this->width()/3, Qt::SmoothTransformation);
    ui->label->setPixmap(pixmap);
    endMenuTheme = new QSoundEffect(this);
    endMenuTheme->setSource(QUrl("qrc:///snd/sounds/final_song.wav"));
    endMenuTheme->setLoopCount(QSoundEffect::Infinite);
    endMenuTheme->setVolume(0.3f);

    if(playSounds) endMenuTheme->play();
}

endMenu::~endMenu()
{
    delete ui;
}

void endMenu::on_pushButton_clicked()
{
    if(playSounds){
        QSoundEffect* effect = new QSoundEffect(this);
        effect->setVolume(0.5f);
        effect->setSource(QUrl("qrc:///snd/sounds/menusound3.wav"));
        effect->play();
    }
    endMenuTheme->stop();
    setVisible(false);
    emit finishedGame();
}

void endMenu::on_pushButton_2_clicked()
{
    if(playSounds){
        QSoundEffect* effect = new QSoundEffect(this);
        effect->setVolume(0.5f);
        effect->setSource(QUrl("qrc:///snd/sounds/menusound3.wav"));
        effect->play();
    }
    endMenuTheme->stop();
    setVisible(false);
    emit restart();
}

