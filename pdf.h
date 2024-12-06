#ifndef PDF_H
#define PDF_H
#include <QPdfWriter>
#include <QPainter>
#include <QSqlQueryModel>
class pdf
{
public:
    pdf();
    static void genererPDF(QSqlQueryModel* model, const QString& nomFichierPDF);
};

#endif // PDF_H
