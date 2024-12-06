
#include "produits.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTableView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QPen>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QEventLoop>
#include <QTimer>
#include <QDateTime>


produits::produits(int id,int id_f,QString nom, float prix,QString categorie,int quantite) {
    this->ID_P=id;
    this->ID_F_P=id_f;
    this->NOM_P=nom;
    this->PRIX_P=prix;
    this->CATEGORIE_P=categorie;
    this->QUANTITE_P=quantite;
}

bool produits::ajouter() {
    QSqlQuery query;

    // Préparation de la requête SQL
    query.prepare("INSERT INTO WASSIM.PRODUITS (ID_P,ID_F_P,NOM_P,PRIX_P,CATEGORIE_P,QUANTITE_P) "
                  "VALUES (:ID_P, :ID_F_P,:NOM_P,:PRIX_P,:CATEGORIE_P,:QUANTITE_P)");

    // Liaison des paramètres
    query.bindValue(":ID_P",ID_P);
    query.bindValue(":ID_F_P",ID_F_P);
    query.bindValue(":NOM_P", NOM_P);
    query.bindValue(":PRIX_P",PRIX_P);
    query.bindValue(":CATEGORIE_P",CATEGORIE_P);
    query.bindValue(":QUANTITE_P",QUANTITE_P);


    // Exécution de la requête
    if (query.exec()) {

        return true; // Insertion réussie
    } else {
        qDebug() << "Erreur lors de l'insertion : " << query.lastError();
        return false; // Échec de l'insertion
    }
}

bool produits::supprimer(int idproduit) {
    QSqlQuery query;

    // Vérification de l'existence du services avant la suppression
    query.prepare("SELECT COUNT(*) FROM WASSIM.PRODUITS WHERE ID_P= :ID_P");
    query.bindValue(":ID_P", idproduit);

    if (!query.exec() || (query.first() && query.value(0).toInt() == 0)) {
        qDebug() << "Le produit avec l'id" << idproduit << " n'existe pas.";
        return false;
    }

    // Suppression de l'enregistrement
    query.prepare("DELETE FROM PRODUITS WHERE ID_P = :ID_P");
    query.bindValue(":ID_P", idproduit);
    if (query.exec()) {
        //QSqlQueryModel *model =afficher();
        //tableView->setModel(model);
        return true;
    } else {
        qDebug() << "Erreur lors de la suppression : " << query.lastError();
        return false;
    }
}

QSqlQueryModel *produits::afficher()
{
    QSqlQueryModel * model= new QSqlQueryModel();

    model->setQuery("SELECT * FROM PRODUITS");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_P"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID_F_P"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NOM_P"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("PRIX_P"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("CATEGORIE_P"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("QUANTITE_P"));

    return model;
}

QSqlQueryModel* produits::chercherParType(const QString& type) {
    QSqlQueryModel* model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare("SELECT * FROM PRODUITS WHERE CATEGORIE_P = :type");
    query.bindValue(":type", type);

    if (query.exec()) {
        model->setQuery(query);
    } else {
        qDebug() << "Erreur de recherche par type : " << query.lastError().text();
    }

    return model;
}QSqlQueryModel* produits::trierParPrix()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    // Requête SQL pour sélectionner les produits triés par prix croissant
    query.prepare("SELECT ID_P, ID_F_P, NOM_P, PRIX_P, CATEGORIE_P, QUANTITE_P FROM PRODUITS ORDER BY PRIX_P ASC");
    query.exec();

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_P"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID_F_P"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NOM_P"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("PRIX_P"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("CATEGORIE_P"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("QUANTITE_P"));

    return model;
}


QSqlQueryModel* produits::trierParType()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    // Requête SQL pour sélectionner les services triés par type alphabétiquement
    query.prepare("SELECT ID_P, ID_F_P, NOM_P,PRIX_P,CATEGORIE_P,QUANTITE_P FROM PRODUITS ORDER BY CATEGORIE_P ASC");
    query.exec();

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_P"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID_F_P"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NOM_P"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("PRIX_P"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("CATEGORIE_P"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("QUANTITE_P"));

    return model;
}


bool produits::modifier() {
    QSqlQuery query;

    // Vérifier si l'ID existe dans la base de données avant de modifier
    query.prepare("SELECT COUNT(*) FROM WASSIM.PRODUITS WHERE ID_P = :idp");
    query.bindValue(":idp",ID_P);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la vérification de l'ID : " << query.lastError();
        return false; // Erreur lors de la requête SELECT
    }

    // Récupérer le résultat de la requête
    query.next();
    int count = query.value(0).toInt();

    // Si l'ID n'existe pas, retourner false
    if (count == 0) {
        qDebug() << "Aucun produits trouvé avec l'id : " <<ID_P;
        return false; // Le produit, modification impossible
    }
    query.exec();

    // Préparation de la requête SQL pour la modification
    query.prepare("UPDATE WASSIM.PRODUITS SET ID_P = :idp, ID_F_P = :idfp, NOM_P = :nom, CATEGORIE_P = :categorie, QUANTITE_P = :quantite, PRIX_P = :prix "
                  "WHERE ID_P = :idp");


    // Liaison des paramètres aux valeurs actuelles de l'objet
    query.bindValue(":idp", ID_P);
    query.bindValue(":idfp", ID_F_P);
    query.bindValue(":nom",NOM_P);
    query.bindValue(":categorie", CATEGORIE_P);
    query.bindValue(":prix",PRIX_P);
    query.bindValue(":quantite", QUANTITE_P);

    // Exécution de la requête
    if (query.exec()) {
        return true; // Modification réussie
    } else {
        qDebug() << "Erreur lors de la modification : " << query.lastError();
        return false; // Échec de la modification
    }
}



bool produits::generateCategoryStatistics() {
    // SQL query to get the count of products grouped by category
    QSqlQuery query("SELECT CATEGORIE_P, COUNT(*) FROM WASSIM.PRODUITS GROUP BY CATEGORIE_P");

    if (!query.isActive()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return false;
    }

    int totalProducts = 0;
    QPieSeries *series = new QPieSeries();

    // First pass to calculate the total number of products
    while (query.next()) {
        totalProducts += query.value(1).toInt();
    }

    // Reset query to the first result and populate the pie series with category percentages
    query.first();
    do {
        QString category = query.value(0).toString();
        int count = query.value(1).toInt();
        float percentage = (float(count) / totalProducts) * 100;
        series->append(category, percentage);
    } while (query.next());

    // Highlight the first slice for visibility
    if (!series->slices().isEmpty()) {
        QPieSlice *slice = series->slices().at(0);
        slice->setExploded();
        slice->setLabelVisible();
        slice->setPen(QPen(Qt::darkBlue, 2));
        slice->setBrush(Qt::darkBlue);
    }

    // Create the chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Percentage of Products by Category");
    chart->legend()->setVisible(true);

    // Display the chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(600, 400);
    chartView->show();

    return true;
}
