#ifndef PRODUITS_H
#define PRODUITS_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
class produits
{
    int ID_P;
    int ID_F_P;
    QString NOM_P;
    float PRIX_P;
    QString CATEGORIE_P;
    int QUANTITE_P;
public:
    produits(){}
    produits(int id,int id_f,QString nom, float prix,QString categorie,int quantite);
    //getters
    int getID_P(){return ID_P;}
    int getID_F_P(){return ID_F_P;}
    QString getNOM_P(){return NOM_P;}
    float getPRIX_P(){return PRIX_P;}
    QString getCATEGORIE_P(){return CATEGORIE_P;}
    int getQUANTITE_P(){return QUANTITE_P;}
    //setters
    void setID_P(int idproduit){ID_P=idproduit;}
    void setID_F_P(int idfournisseur){ID_F_P=idfournisseur;}
    void setNOM_P(QString nom){NOM_P=nom;}
    void setPRIX_P(float prix){ PRIX_P=prix;}
    void setCATEGORIE_P(QString categ){ CATEGORIE_P=categ;}
    void setQUANTITE_P(int quantite){ QUANTITE_P=quantite;}
    //methodes
    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimer(int idproduit);
    QSqlQueryModel* chercherParType(const QString& type);
    QSqlQueryModel* trierParType();
    bool modifier();
    bool generateCategoryStatistics();
    bool sendSms(const QString &to, const QString &message) ;
    void initializeDailySms();
    void envoyerAlertesStockFaible(const QString &to);
    void envoyerAlertesQuotidiennes();
    void sendProductNotificationSms(const QString &to, int id);
    void on_SendSmsButton_clicked();
    QSqlQueryModel* chercherParId(int id);
    //void envoyerAlertesStockBas();

   QSqlQueryModel* trierParPrix();


};

#endif // PRODUITS_H


