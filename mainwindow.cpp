#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include<QFileDialog>
produits p;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(ProduitsObj.afficher()); // Affiche tous les services
    if (A.connect_arduino() == 0) {
        qDebug() << "Arduino connecté sur le port:" << A.getarduino_port_name();

        QMessageBox::aa(nullptr, "Arduino connecté sur le port:", A.getarduino_port_name());

    } else {
        qDebug() << "Échec de la connexion à l'Arduino.";
        QMessageBox::aa(nullptr, "Échec de la connexion à l'Arduino.", A.getarduino_port_name());

    }
    connect(A.get_serial(), &QSerialPort::readyRead,this,&MainWindow::readFromArduino);
}

void MainWindow::readFromArduino()
{
    QByteArray data = A.read_from_arduino();
    QString etat = QString::fromUtf8(data).trimmed(); // Convertir les données en chaîne

    // qDebug()<<etat;
    if(etat=="E")
    {

            //QMessageBox::warning(this, "Alerte", "humidite elevee  !");
        qDebug()<<"humidite elevee  !";

            QSqlQuery query;
        query.prepare("UPDATE PRODUITS SET ETAT='Haut' ");

            query.exec();
        produits P;
        //ui->tableView->setModel(P.afficher()); // Affiche tous les services

    }
    if(etat=="N")
    {
        QSqlQuery query1;
        query1.prepare("UPDATE PRODUITS SET ETAT='Normal' ");

        query1.exec();
         produits P;

        //ui->tableView->setModel(P.afficher()); // Affiche tous les services

    }


}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonAjouter_clicked()
{
    // Récupération des informations saisies
    int ID_P = ui->lineEdit_Id_P->text().toInt();
    int ID_F_P = ui->lineEdit_Id_F_P->text().toInt();
    QString NOM_P= ui->lineEdit_Name->text();
    float PRIX_P= ui->lineEdit_Prix->text().toFloat();
    QString CATEGORIE_P= ui->lineEdit_Categorie->text();
    int QUANTITE_P=ui->lineEdit_Quantite->text().toInt();
    // Créer et ajouter le service

    produits p(ID_P,ID_F_P,NOM_P,PRIX_P, CATEGORIE_P, QUANTITE_P);
    bool test = p.ajouter();

    if (test) {
        QMessageBox::aa(this, "Ajout effectué", "Le service a été ajouté avec succès.");
    } else {
        QMessageBox::critical(this, "Échec de l'ajout", "L'ajout du service a échoué.");
    }
}



void MainWindow::on_pushButtonSupprimer_clicked()
{
    int matriculeASupprimer = ui->lineEdit_idsupp->text().toInt();

    if ( ProduitsObj.supprimer(matriculeASupprimer)) {
        QMessageBox::aa(this, "Succès", "Service supprimé avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression du service.");
    }
}
void MainWindow::on_pushButtonAfficher_clicked()
{
    // Affiche les données existantes dans le QTableView
    ui->tableView->setModel(ProduitsObj.afficher()); // Affiche tous les services
    QMessageBox::aa(this, "Affichage", "Liste des services affichée.");
}
void MainWindow::on_pushButtonPDF_clicked() {
    // Ouvrir une boîte de dialogue pour choisir le nom et l'emplacement du fichier PDF
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer en PDF", "", "Fichiers PDF (*.pdf)");

    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Exportation annulée", "Vous n'avez pas choisi de fichier pour enregistrer le PDF.");
        return;
    }

    // Récupérer le modèle de données à partir de l'objet de services
    QSqlQueryModel* model = ProduitsObj.afficher(); // Assurez-vous que cette fonction retourne bien un QSqlQueryModel*

    // Vérifier que le modèle contient des données
    if (model->rowCount() == 0) {
        QMessageBox::aa(this, "Aucune donnée", "Il n'y a pas de données à exporter.");
        return;
    }

    // Générer le PDF en utilisant la classe pdf
    pdf::genererPDF(model, filePath);

    // Message de confirmation de succès
    QMessageBox::aa(this, "PDF exporté", "Le fichier PDF a été exporté avec succès à l'emplacement spécifié.");
}
void MainWindow::on_pushButtonRecherche_clicked() {
    QString typeRecherche = ui->lineEdit_4->text();

    if (typeRecherche.isEmpty()) {
        QMessageBox::warning(this, "Champ vide", "Veuillez entrer un type de service à rechercher.");
        return;
    }

    // Effectuer la recherche
    QSqlQueryModel* model = ProduitsObj.chercherParType(typeRecherche);

    // Vérifier si des résultats ont été trouvés
    if (model->rowCount() == 0) {
        QMessageBox::aa(this, "Aucun résultat", "Aucun service trouvé pour le type : " + typeRecherche);
    } else {
        // Afficher les résultats dans la vue
        ui->tableView->setModel(model);
    }
}

void MainWindow::on_pushButtonTrierType_clicked()
{
    QSqlQueryModel* model = ProduitsObj.trierParType();
    ui->tableView->setModel(model); // Affiche la liste triée par type dans la vue
}

void MainWindow::on_pushButtonTrierPrix_clicked()
{
    QSqlQueryModel* model = ProduitsObj.trierParPrix();
    ui->tableView->setModel(model); // Affiche la liste triée par type dans la vue
}


void MainWindow::on_pushButtonModifier_clicked()
{
    // Récupération des informations saisies dans les champs
    int ID_P = ui->lineEdit_5->text().toInt();  // ID du partenaire
    int ID_F_P = ui->lineEdit_6->text().toInt();     // Nom du partenaire
    QString NOM_P = ui->lineEdit_7->text();  // Contact du partenaire
    float PRIX_P= ui->lineEdit_8->text().toFloat();  // ID du partenaire
    QString CATEGORIE_P= ui->lineEdit_9->text();  // ID du partenaire
    int QUANTITE_P= ui->lineEdit_10->text().toInt();  // ID du partenaire


    // Création de l'objet Partenaire avec les nouvelles informations
    produits p(ID_P,ID_F_P,NOM_P,PRIX_P,CATEGORIE_P,QUANTITE_P);// Crée un objet avec les nouvelles valeurs

    // Appel à la méthode modifier() de Partenaire pour mettre à jour les informations dans la base de données
    bool resultat = p.modifier();  // Passe les valeurs à modifier()

    // Vérification du résultat de la modification
    if (resultat) {
        ui->tableView_2->setModel(p.afficher());  // Mise à jour de l'affichage dans le QTableView
        QMessageBox::aa(this, "Succès", "Partenaire modifié avec succès");
    } else {
        QMessageBox::critical(this, "Erreur", "Aucun partenaire trouvé avec cet ID ou échec de la modification");
    }
}


void MainWindow::on_generatetypeButton_clicked()
{
    // Call the function to generate the marque statistics chart
    if (p.generateCategoryStatistics()) {
        qDebug() << "Chart generated successfully.";
    }
    else {
        qDebug() << "Failed to generate chart.";
    }
}
void MainWindow::on_SendSmsButton_clicked()
{
    // Create an object of the Piece class (you may need to pass the necessary arguments here)
    produits p;
    int ID_P = ui->lineEdit_Idsms->text().toInt();

    // Call the method to send SMS for low stock pieces
    p.sendProductNotificationSms("+21653129178",ID_P);}
