#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "ui_mainwindow.h"
#include "produits.h"
#include <QTableView>
#include <QString>
#include "pdf.h"
#include "produits.h"
#include "arduino.h"

using namespace std ;
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_pushButtonAjouter_clicked();
    void on_pushButtonSupprimer_clicked();
    void on_pushButtonAfficher_clicked();
    void on_pushButtonPDF_clicked();
    void on_pushButtonRecherche_clicked();
    void on_pushButtonTrierType_clicked();
    void on_pushButtonTrierPrix_clicked();
    void on_pushButtonModifier_clicked();
    void on_generatetypeButton_clicked();
    void on_SendSmsButton_clicked();
    void readFromArduino() ;

   // void envoyerAlertesStockBas();
private:
    Ui::MainWindow *ui;
    produits ProduitsObj;
    QSqlQueryModel *model;
    arduino A;
    QTimer *stockCheckTimer;  // Timer to trigger the SMS check every 5 minutes
};


#endif // MAINWINDOW_H

