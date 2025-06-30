#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <gamespace.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GameSpace* gameSpace;
    int difficulty;
    bool showVectors;
    bool playSounds;
    bool useFullscreen;

    QSoundEffect* mainMenuTheme;

private slots:
    void onDifficultySelected(int);
    void onVectorsToggled();
    void onStartButtonPressed();
    void onExitButtonPressed();
    void onSoundsToggled();
    void onFullscreenToggled();
    void onFinishedGame();
};
#endif // MAINWINDOW_H
