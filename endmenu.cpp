#include "endmenu.h"
#include "ui_endmenu.h"

endMenu::endMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::endMenu)
{
    ui->setupUi(this);
    setWindowTitle("The end");
    setWindowIcon(QIcon(":/zub/resources/0.png"));
    ui->label_2->setStyleSheet("font-size: 30px;");
    setFixedSize(this->width(), this->height());
    QPixmap pixmap(":/statPictures/pictures/grob.png");
    pixmap = pixmap.scaledToWidth(this->width()/3, Qt::SmoothTransformation);
    ui->label->setPixmap(pixmap);
}

endMenu::~endMenu()
{
    delete ui;
}

void endMenu::on_pushButton_clicked()
{
    setVisible(false);
    emit finishedGame();
}

void endMenu::on_pushButton_2_clicked()
{
    setVisible(false);
    emit restart();
}

