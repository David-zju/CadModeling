/* 主窗体设计 */
#pragma once
#include <QTimer>
#include <QMainWindow>
#include <QTextBrowser>
#include <QDateTime>
#include <QLabel>
#include <QFileDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QGroupBox>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QToolBar>
#include"glwidget.h"
#include "Entity.h"
#include "BrepModel.h"
#define GUI MainWindow::GetEntityPtr()
enum InfoType
{
    info=0,
    warning,
    error
};
enum ELUROP{
    null=-1,
    mvfs,
    mev,
    mef,
    kemr,
    kfmrh,
    semv,
    sweep
};
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow,public Entity<MainWindow>
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static MainWindow& GetEntity(){return Entity<MainWindow>::GetEntity();}
    static MainWindow* GetEntityPtr(){return Entity<MainWindow>::GetEntityPtr();}   
public slots:
    void print(QString message,InfoType it=InfoType::info);
    void reposX();
    void reposY();
    void reposZ();
    void changeViewTypeTriggered();
    void reDo();
    void unDo();
    void fitView();
    void commandLineChangedTrigered();
    void updateTreeTable();
    void updateGLWidget();

private slots:
    void resizeEvent(QResizeEvent *q);

    void clearLayout(QLayout *layout);
    void on_actionAbout_Qt_triggered();

    void on_actionAbout_2_triggered();

    void on_treeWidget_itemPressed(QTreeWidgetItem *item, int column);

    void on_mvfsButton_released();

    void on_mevButton_released();

    void on_mefButton_released();

    void on_kemrButton_released();

    void on_kfmrhButton_released();

    void on_semvButton_released();

    void on_buttonBox_accepted();

    void on_sweepButton_released();

    void on_genExampleButton_released();

    void on_deleteAllButton_released();

private:
    GLWidget *glw;
    QAction *action;
    QAction *posX;
    QAction *posY;
    QAction *posZ;
    QAction *chaVisType;
    QAction *fit;
    QAction* redo;
    QAction* undo;
    QAction* showMesh;
    QAction* infoWidgetShow;
    QAction* opWidgetShow;
    QAction* messaWidgetShow;
    ELUROP op;

    QSpinBox* intInputBox;
    QLineEdit* commandLine;
    QLabel *statusLabel;
    QPushButton* genMesh;
    QToolBar *grid;
    QToolBar *visualization;
    Ui::MainWindow *ui;
    QProgressBar *qpb;
    QTextBrowser *messageBox;

    void addActionToToolBar(QAction *qa, QToolBar *qtb,bool checkable=false);
    inline void hideAll();
};

