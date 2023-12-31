#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include "matiererecyclee.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QPdfWriter>
#include <QPainter>
#include <QTextDocument>
#include <QFileDialog>
#include <QSqlError>
#include "pointdecollect.h"

#include <QPieSeries>
#include <QPieSlice>
#include <QChart>
#include <QChartView>


#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMouseEvent>
#include <QScrollArea>
#include <QImageWriter>
#include <QScrollBar>
#include <QSettings>
#include <QApplication>
#include <QDebug>
#include "qrcodegen.h"

#include <QMap>

#include <QQuickWidget>
#include <QtWidgets/QWidget>
#include <QtCharts/QChart>
#include <QQuickItem>
// sabee
#include "employe.h"
#include "chart.h"
#include "pdf.h"
#include "chart.h"
#include "csv.h"


#include <QSqlTableModel>
#include <QObject>
#include <QDate>
#include<QIntValidator>
#include<QtPrintSupport/QPrinter>



#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QStandardPaths>
// sabee
const QString getIniPath()
{
    const static QString iniPath( qApp->applicationDirPath() + "/settings.ini" );
    return iniPath;
}
QString saveFormats()
{
    static QString suffix;
    foreach ( const QByteArray & format , QImageWriter::supportedImageFormats() )
        suffix += QString( "%1 ( *.%2 )\n" )
                .arg( QString( format ).toUpper() )
                .arg( QString( format ) );

    return suffix;
}

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent) , ui(new Ui::MainWindow)  , successfulEncoding(false)

{
    ui->setupUi(this);
    QPixmap pix("C:/Users/HP/OneDrive/Bureau/img/LOGO");
    ui->label_pic->setPixmap(pix.scaled(150,150,Qt::KeepAspectRatio));
    ui->label_pic_4->setPixmap(pix.scaled(150,150,Qt::KeepAspectRatio));

    ///////////////////////////////////////////////////////////////////////////// matiere recyclee

ui->stackedWidget->setCurrentIndex(0);

ui->lineEdit_setId->setValidator(new QIntValidator(0,999999,this));
ui->lineEdit_prix->setValidator(new QIntValidator(0,999999,this));
ui->lineEdit_Quantite->setValidator(new QIntValidator(0,999999,this));

ui->scrollArea->installEventFilter( this );

QSettings ini( getIniPath(), QSettings::IniFormat );
ui->splitter_2->restoreState( ini.value( ui->splitter_2->objectName() ).toByteArray() );
ui->splitter->restoreState( ini.value( ui->splitter->objectName() ).toByteArray() );
ui->sBoxScale->setValue( ini.value( ui->sBoxScale->objectName(), 4 ).toInt() );
restoreState( ini.value( "State" ).toByteArray() );
restoreGeometry( ini.value( "Geometry" ).toByteArray() );

setScale( ui->sBoxScale->value() );
updateQRImage();
ProdStatisticsChartView=nullptr;
///////////////////////////////////////////////////////////////////////////// matiere recyclee

//////////////////////////////////////////////////////////////////////////// gestion collect
quntiteStatisticsChartView = nullptr;


ui->label_pic_2->setPixmap(pix.scaled(150, 150, Qt::KeepAspectRatio));
ui->lineEdit_quntite->setValidator(new QIntValidator(0, 99999, this));

PointDeCollect p;
//ui->comboBox_ID->setModel(p.afficher());
ui->tableView_ID->setModel(p.afficherID());
ui->tab_PC->setModel(p.afficher());

ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/map.qml")));
ui->quickWidget->show();

auto obj = ui->quickWidget->rootObject();
connect(this, SIGNAL(setCenter(QVariant, QVariant)), obj, SLOT(setCenter(QVariant, QVariant)));
connect(this, SIGNAL(addMarker(QVariant, QVariant)), obj, SLOT(addMarker(QVariant, QVariant)));


emit setCenter(36.83,10.21);
emit addMarker(36.83,10.20);
emit addMarker(36.82,10.21);
emit addMarker(36.81,10.20);
emit addMarker(36.898,10.190);

///////////////////////////////////////////////////////////////////////////// gestion collect

//////////////////////////////////////////////////////////////////////////////  gestion employee
ui->geaffichage->setModel(Emp.afficherEmployes());
ui->label_pic_3->setPixmap(pix.scaled(150,150,Qt::KeepAspectRatio));

setChartView_Dep(ui->horizontalFrame);
setChartView_Poste(ui->horizontalFrame_2);

ui->gecin->setValidator(new QIntValidator(0,999999,this));
ui->genumtel->setValidator(new QIntValidator(0,999999,this));
ui->gesalaire->setValidator(new QIntValidator(0,999999,this));
ui->gesupp->setValidator(new QIntValidator(0,999999,this));
ui->gehfixe->setValidator(new QIntValidator(0,9999,this));
ui->gehsupp->setValidator(new QIntValidator(0,9999,this));
}



MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::on_b_update_clicked()
{
    ui->table_matiereR->setModel(mR.afficher());
    ui->select_ID->setModel(mR.afficher());

    ui->select_id2->setModel(mR.afficher());

}




void MainWindow::on_b_supp_p_clicked()
{
    matiereRecyclee m1;

    m1.setId_mr(ui->lineEdit_suppID->text().toInt());

    bool test=m1.supprimer(m1.getId_mr());
    QMessageBox msgBox;
    if(test)
    {
     msgBox.setText("suppression avec succès");
     ui->table_matiereR->setModel(mR.afficher());
     ui->select_ID->setModel(mR.afficher());
     ui->select_id2->setModel(mR.afficher());

    }
    else
    {
     msgBox.setText("l'identifiant n'existe pas");
    }
    ui->lineEdit_suppID->clear();

    msgBox.exec();
}



void MainWindow::on_save_clicked()
{
    int id=ui->lineEdit_setId->text().toInt();

    QString type1;
        if (ui->r_carton->isChecked()) {
            type1 = "Carton";
        } else if (ui->r_plastique->isChecked()) {
            type1 = "Plastique";
        } else if (ui->r_verre->isChecked()) {
            type1 = "Verre";
        } else {
            // Handle the case where no radio button is selected or an unknown option
            type1 = "Unknown";
        }

    QDate date=ui->dateEdit_DE->date();

    float pr=ui->lineEdit_prix->text().toFloat();

    int qt=ui->lineEdit_Quantite->text().toInt();

    matiereRecyclee m(id,type1,pr,date,qt);

   bool test=m.ajouter();
   ui->table_matiereR->setModel(mR.afficher());
    ui->select_ID->setModel(mR.afficher());
    ui->select_id2->setModel(mR.afficher());

    ui->lineEdit_setId->clear();
    ui->lineEdit_prix->clear();
    ui->lineEdit_Quantite->clear();
    ui->lineEdit_setId->setReadOnly(false);

    ui->dateEdit_DE->setDate(QDate::currentDate());


   if(test)
   { //RefreShe


       QMessageBox::information(nullptr, QObject::tr("database is open"),
                   QObject::tr("un nouveau produit a été ajouté.\n"
                               "Click Cancel to exit."), QMessageBox::Cancel);

}
   else
       QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                   QObject::tr("L'identifiant existe déjà.\n"
                               "Click Cancel to exit."), QMessageBox::Cancel);

}


void MainWindow::on_Binit_clicked()
{
    ui->lineEdit_setId->clear();
    ui->lineEdit_prix->clear();
    ui->lineEdit_Quantite->clear();
    ui->lineEdit_setId->setReadOnly(false);

    ui->dateEdit_DE->setDate(QDate::currentDate());
}


void MainWindow::on_b_modif_clicked()
{
            QMessageBox msgBox;
            int id=ui->lineEdit_setId->text().toInt();

            QString type1;
                if (ui->r_carton->isChecked()) {
                    type1 = "Carton";
                } else if (ui->r_plastique->isChecked()) {
                    type1 = "Plastique";
                } else if (ui->r_verre->isChecked()) {
                    type1 = "Verre";
                } else {
                    // Handle the case where no radio button is selected or an unknown option
                    type1 = "Unknown";
                }

            QDate date=ui->dateEdit_DE->date();

            float pr=ui->lineEdit_prix->text().toFloat();

            int qt=ui->lineEdit_Quantite->text().toInt();

            matiereRecyclee m(id,type1,pr,date,qt);

           bool test=m.modif();


            if(test)
            { //RefreShe


                QMessageBox::information(nullptr, QObject::tr("database is open"),
                            QObject::tr("modification avec succès.\n"
                                        "Click Cancel to exit."), QMessageBox::Cancel);

         }
            else
            {
                QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                            QObject::tr("L'identifiant n'existe pas.\n"
                                        "Click Cancel to exit."), QMessageBox::Ok);
}



            ui->table_matiereR->setModel(mR.afficher());

            ui->lineEdit_setId->clear();
            ui->lineEdit_prix->clear();
            ui->lineEdit_Quantite->clear();
            ui->lineEdit_setId->setReadOnly(false);

            ui->dateEdit_DE->setDate(QDate::currentDate());



}






void MainWindow::on_Btri_clicked()
{
  ui->table_matiereR->setModel(mR.sortTableByQTT());


}

void MainWindow::on_lineEdit_recherche_textChanged(const QString &arg1)
{
    QSqlQueryModel *model = new QSqlQueryModel();
       model->setQuery("SELECT ID_MR, TYPE_PROD, PRIX, DATE_ENTREE, QUANTITÉS FROM matiere_recyclee WHERE TYPE_PROD LIKE '%" + arg1 + "%' OR ID_MR LIKE '%" + arg1 + "%' OR PRIX LIKE '%" + arg1 + "%' OR DATE_ENTREE LIKE '%" + arg1 + "%' OR QUANTITÉS LIKE '%" + arg1 + "%'");
       ui->table_matiereR->setModel(model);
}

void MainWindow::on_b_convPDF_clicked()
{
        QString fileName = QFileDialog::getSaveFileName(this, "Export PDF",QString(), "*.pdf");
        if (!fileName.isEmpty()) {
            if (QFileInfo(fileName).suffix().isEmpty()) {
                fileName.append(".pdf");
            }

            QPdfWriter writer(fileName);
            QPainter painter(&writer);

            int rowCount = ui->table_matiereR->model()->rowCount();
            int columnCount = ui->table_matiereR->model()->columnCount();

            QString strStream;
            QTextStream out(&strStream);

            out <<  "<html>\n"
                "<head>\n"
                "<meta Content=\"Text/html; charset=Windows-1251\">\n"
                <<  QString("<title>%1</title>\n").arg("TITLE")
                <<  "</head>\n"
                "<body bgcolor=#ffffff link=#5000A0>\n"
                "<style> table { font-size: 20px; } </style>\n"  // Adjust the font size here
                "<table border=1 cellspacing=0 cellpadding=2 width='100%'>\n";

            // headers
            out << "<thead><tr bgcolor=#f0f0f0>";
            for (int column = 0; column < columnCount; column++)
                if (!ui->table_matiereR->isColumnHidden(column))
                    out << QString("<th width='%1%'>%2</th>").arg(100/columnCount).arg(ui->table_matiereR->model()->headerData(column, Qt::Horizontal).toString());
            out << "</tr></thead>\n";

            // data table
            for (int row = 0; row < rowCount; row++) {
                out << "<tr>";
                for (int column = 0; column < columnCount; column++) {
                    if (!ui->table_matiereR->isColumnHidden(column)) {
                        QString data = ui->table_matiereR->model()->data(ui->table_matiereR->model()->index(row, column)).toString().simplified();
                        out << QString("<td bkcolor=0 width='%1%'>%2</td>").arg(100/columnCount).arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                    }
                }
                out << "</tr>\n";
            }
            out <<  "</table>\n"
                "</body>\n"
                "</html>\n";

            QTextDocument *document = new QTextDocument();
            document->setHtml(strStream);

            // scale the painter
            double scale = 15.0;
            painter.scale(scale, scale);

            document->drawContents(&painter);

            delete document;
        }


}



void MainWindow::on_select_ID_currentIndexChanged(QString id_M)
{

    /*QString*/ id_M = ui->select_ID->currentText();
    ui->lineEdit_setId->setReadOnly(true);

        QSqlQuery query;
        query.prepare("SELECT * FROM matiere_recyclee WHERE ID_MR='" + id_M + "'");

        if(query.exec())
        {
            while(query.next())
            {
                ui->lineEdit_setId->setText(query.value(0).toString());
                QString type = query.value(1).toString().trimmed();  // it removes the trailing spaces
                qDebug() << "Type: " << type;  // print out the type value

                if (type == "Carton") {
                    ui->r_carton->setChecked(true);
                } else if (type == "Plastique") {
                    ui->r_plastique->setChecked(true);
                } else if (type == "Verre") {
                    ui->r_verre->setChecked(true);
                }

                ui->lineEdit_prix->setText(query.value(2).toString());
                ui->dateEdit_DE->setDate(query.value(3).toDate());
                ui->lineEdit_Quantite->setText(query.value(4).toString());
            }
        }
}






void MainWindow::on_confirmer_login_clicked()
{
    QString UserName = ui -> lineEdit_username->text();
    QString Password= ui-> lineEdit_password -> text();

    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYEE WHERE ADRESSE_EMAIL = :username AND PASSWORD = :password");
    query.bindValue(":username", UserName);
    query.bindValue(":password", Password);

    if (query.exec() && query.next()) {
        QMessageBox::information(this,"CONNECTED","Connecté");
        clearProdStatisticsChart();
        updateProdStatisticsChart();


        ui->stackedWidget->setCurrentIndex(1);
        ui->select_ID->setModel(mR.afficher());
        ui->select_id2->setModel(mR.afficher());
        ui->table_matiereR->setModel(mR.afficher());
        ui->lineEdit_setId->clear();
        ui->lineEdit_prix->clear();
        ui->lineEdit_Quantite->clear();
        ui->lineEdit_setId->setReadOnly(false);

        ui->dateEdit_DE->setDate(QDate::currentDate());    }
    else {
        QMessageBox::warning(this,"INVALID","veuillez entrer un nom d'utilisateur ou un mot de passe valide");
    }
}



void MainWindow::on_annuler_login_clicked()
{
    QMessageBox::StandardButton reply;

    reply= QMessageBox::question(this,"close?","are you sure to close the app?");
    if(reply==QMessageBox::Yes)
    {
QApplication::quit();
    }
}




void MainWindow::updateQRImage()
{
    int sizeText = ui->pTextEditQRText->toPlainText().size();
    ui->labelSizeText->setText( QString::number( sizeText ) );

    int levelIndex = 1;
    int versionIndex = 0;
    bool bExtent = true;
    int maskIndex = -1;
    QString encodeString = ui->pTextEditQRText->toPlainText();

    successfulEncoding = qrEncode.EncodeData( levelIndex, versionIndex, bExtent, maskIndex, encodeString.toUtf8().data() );
    if ( !successfulEncoding )
    {
        ui->image_label->clear();
        ui->image_label->setText( tr("QR Code...") );
        ui->labelSize->clear();
        ui->pButtonSave->setEnabled( successfulEncoding );
        return;
    }

    int qrImageSize = qrEncode.m_nSymbleSize;

    encodeImageSize = qrImageSize + ( QR_MARGIN * 2 );
    QImage encodeImage( encodeImageSize, encodeImageSize, QImage::Format_Mono );
    encodeImage.fill( 1 );

    for ( int i = 0; i < qrImageSize; i++ )
        for ( int j = 0; j < qrImageSize; j++ )
            if ( qrEncode.m_byModuleData[i][j] )
                encodeImage.setPixel( i + QR_MARGIN, j + QR_MARGIN, 0 );

    ui->image_label->setPixmap( QPixmap::fromImage( encodeImage ) );

    setScale(ui->sBoxScale->value());
    ui->pButtonSave->setEnabled( successfulEncoding );
}
void MainWindow::setScale(int scale)
{
    if ( successfulEncoding )
    {
        int scale_size = encodeImageSize * scale;

        const QPixmap & scale_image = ui->image_label->pixmap()->scaled( scale_size, scale_size );
        ui->image_label->setPixmap( scale_image );

        const QString & size_info = QString( "%1x%2" ).arg( scale_size ).arg( scale_size );
        ui->labelSize->setText( size_info );
    }
}

void MainWindow::on_pButtonSave_clicked()
{
    const QString & path = QFileDialog::getSaveFileName( this, QString::null, "qrcode", saveFormats() );
    if ( path.isNull() )
        return;

    ui->image_label->pixmap()->save( path );
}
void MainWindow::on_sBoxScale_valueChanged(int arg1)
{
    setScale( arg1 );
}
void MainWindow::on_pTextEditQRText_textChanged()
{
    updateQRImage();
}
void MainWindow::on_pButtonQuit_clicked()
{
ui->pTextEditQRText->clear();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    QSettings ini( getIniPath(), QSettings::IniFormat );
    ini.setValue( ui->splitter->objectName(), ui->splitter->saveState() );
    ini.setValue( ui->splitter_2->objectName(), ui->splitter_2->saveState() );
    ini.setValue( ui->sBoxScale->objectName(), ui->sBoxScale->value() );
    ini.setValue( "State", saveState() );
    ini.setValue( "Geometry", saveGeometry() );

    qApp->quit();
}
bool MainWindow::eventFilter( QObject * object, QEvent * event )
{
    QScrollArea * scrollArea = ui->scrollArea;

    if ( object == scrollArea )
    {
        if ( event->type() == QEvent::MouseButtonPress )
        {
            QMouseEvent * mouseEvent = static_cast < QMouseEvent * > ( event );
            if ( mouseEvent->button() == Qt::LeftButton )
            {
                lastPos = mouseEvent->pos();

                if( scrollArea->horizontalScrollBar()->isVisible()
                        || scrollArea->verticalScrollBar()->isVisible() )
                    scrollArea->setCursor( Qt::ClosedHandCursor );
                else
                    scrollArea->setCursor( Qt::ArrowCursor );
            }

        }else if ( event->type() == QEvent::MouseMove )
        {
            QMouseEvent *mouseEvent = static_cast < QMouseEvent * > ( event );

            if ( mouseEvent->buttons() == Qt::LeftButton )
            {
                lastPos -= mouseEvent->pos();

                int hValue = scrollArea->horizontalScrollBar()->value();
                int vValue = scrollArea->verticalScrollBar()->value();

                scrollArea->horizontalScrollBar()->setValue( lastPos.x() + hValue );
                scrollArea->verticalScrollBar()->setValue( lastPos.y() + vValue );

                lastPos = mouseEvent->pos();
            }

        }else if ( event->type() == QEvent::MouseButtonRelease )
            scrollArea->setCursor( Qt::ArrowCursor );
    }

    return QWidget::eventFilter(object, event);
}



void MainWindow::on_tabWidget_2_tabBarClicked(int index)
{
    // Récupérer l'index de l'onglet "Statistiques" sous l'onglet "CRUD"
       int statisticsTabIndex = ui->tabWidget_2->indexOf(ui->tab);  // Remplacez "statistiquesTab" par le nom réel de votre onglet "Statistiques"

       // Vérifier si l'onglet "Statistiques" a été cliqué
       if (index == statisticsTabIndex) {
           // Mettre à jour les statistiques ici
          updateProdStatisticsChart();
       } else {
           // Si l'onglet actif n'est pas "Statistiques", supprimer le graphique s'il existe
          clearProdStatisticsChart();

       }

}

void MainWindow::updateProdStatisticsChart()
{
    // Récupérer les statistiques réelles
    QMap<QString, int> statistics = mR.getProdStatistics();
    int totaleproduit = 0;

    // Calculer le nombre total de produit
    for (const QString &prodGroup : statistics.keys()) {
        totaleproduit += statistics[prodGroup];
    }

    // Créer une série pour le graphique circulaire
    QPieSeries *series = new QPieSeries();

    // Définir différentes nuances de vert pour chaque tranche
    QStringList greenColors = {"#55AA00", "#458C68", "#005500"};

    int colorIndex = 0;

    // Ajouter des tranches au graphique circulaire pour chaque groupe de produit
    for (const QString &prodGroup : statistics.keys()) {
        int count = statistics[prodGroup];
        QPieSlice *slice = series->append(prodGroup, count);

        // Afficher le pourcentage sur chaque tranche
        slice->setLabel(prodGroup + ": " + QString::number(static_cast<double>(count) / totaleproduit * 100, 'f', 2) + "%");

        // Définir la couleur des tranches
        if (colorIndex < greenColors.size()) {
            slice->setBrush(QColor(greenColors[colorIndex]));
        } else {
            slice->setBrush(QColor(Qt::green));  // Utiliser une couleur par défaut si vous avez épuisé les nuances de vert
        }

        // Passer à la prochaine nuance de vert
        colorIndex++;
    }

    // Supprimer le graphique précédent s'il existe
     clearProdStatisticsChart();

    // Créer un graphique circulaire et le configurer
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition par groupe de matiere");

    // Définir le fond du graphique comme beige
    chart->setBackgroundBrush(QBrush(QColor("#F5F5DC")));  // Couleur beige

    // Créer une vue de graphique et l'ajouter à votre interface utilisateur
    ProdStatisticsChartView = new QChartView(chart);
    ProdStatisticsChartView->setRenderHint(QPainter::Antialiasing);
    ProdStatisticsChartView->setParent(ui->tab);

    // Ajuster la taille du graphique (par exemple, 600x400 pixels)
    ProdStatisticsChartView->setGeometry(70, 50, 600, 400);

    // Afficher le graphique
    ProdStatisticsChartView->show();
}

void MainWindow::clearProdStatisticsChart()
{
    if (ProdStatisticsChartView)
    {
        delete ProdStatisticsChartView;
        ProdStatisticsChartView = nullptr;
    }
}



void MainWindow::on_select_id2_currentIndexChanged( QString id_M)
{

        id_M = ui->select_id2->currentText();

        QSqlQuery query;
        query.prepare("SELECT * FROM matiere_recyclee WHERE ID_MR='" + id_M + "'");

        if(query.exec())
        {
            while(query.next())
            {
                ui->pTextEditQRText->clear(); // Clear the text edit before adding new text
                ui->pTextEditQRText->appendPlainText("ID: " + query.value(0).toString());
                QString type = query.value(1).toString().trimmed();  // it removes the trailing spaces
                qDebug() << "Type: " << type;  // print out the type value

                ui->pTextEditQRText->appendPlainText("Type: " + type);
                ui->pTextEditQRText->appendPlainText("Prix: " +query.value(2).toString());
                // ui->pTextEditQRText->setDate(query.value(3).toDate()); // QPlainTextEdit does not have a setDate() method
                ui->pTextEditQRText->appendPlainText("Quantité: " +query.value(4).toString());
            }
        }
   }


///////////////////////////////////////////////////////////////////////////////// point de collecte

void MainWindow::on_b_ajouter_clicked()
{
    int id=ui->lineEdit_idpc->text().toInt();
    QDate date= ui->dateEdit->date();
    QString lieu = ui->lineEdit_lieu->text();
    int quntte=ui->lineEdit_quntite->text().toInt();
    PointDeCollect p(id,date,lieu,quntte);
    bool test=p.ajouter();
    if (test)
    {
        ui->tableView_ID->setModel(p.afficherID());
        ui->tab_PC->setModel(p.afficher());
    }
}

void MainWindow::on_B_supp_clicked()
{
    PointDeCollect p1;

    p1.setid(ui->lineEdit_idpc->text().toInt());
    bool test= p1.supprime(ui->lineEdit_idpc->text().toInt());
    QMessageBox msgBox;
    if (test)
    {
        ui->tableView_ID->setModel(p1.afficherID());
        ui->tab_PC->setModel(p1.afficher());
        msgBox.setText("suppreme avec succe");
    }
     else
        msgBox.setText("no suppreme ");
    msgBox.exec();
}

void MainWindow::on_B_modifier_clicked()
{
    QMessageBox msgBox;
    int id=ui->lineEdit_idpc->text().toInt();
    QDate date= ui->dateEdit->date();
    QString lieu = ui->lineEdit_lieu->text();
    int quntite=ui->lineEdit_quntite->text().toInt();
    PointDeCollect p(id,date,lieu,quntite);
    bool test=p.modifier();
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->tab_PC->setModel(p.afficher());
    ui->tableView_ID->setModel(p.afficherID());
    //ui->comboBox_ID->setModel(p.afficherID());
    ui->tab_PC->setModel(p.tier_lieu());
    //ui->quickWidget->setSource(QUrl("qrc:/Maps.qml"));

}
/*
void MainWindow::on_comboBox_ID_currentIndexChanged(const QString &arg1)
{
    QMessageBox msgBox;
    QString id_pc= ui->comboBox_ID->currentText();

    QSqlQuery qry;

          qry.prepare("SELECT * FROM points_de_collectes where id_pc='"+id_pc+"' ");
          if (qry.exec())
          {
              while (qry.next())
              {
                  ui->lineEdit_idpc->setText(qry.value(0).toString());
                  ui->lineEdit_lieu->setText(qry.value(1).toString());
                  ui->dateEdit->setDate(qry.value(2).toDate());
                  ui->lineEdit_quntite->setText(qry.value(3).toString());
              }

          }

}*/


void MainWindow::on_lineEdit_RE_textChanged(const QString &arg1)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT ID_PC FROM points_de_collectes WHERE ID_PC LIKE '%" + arg1 + "%' ");
    ui->tableView_ID->setModel(model);
}


void MainWindow::on_b_trier_clicked()
{
    ui->tab_PC->setModel(p.tier_lieu());
}

void MainWindow::on_lineEdit__textChanged(const QString &arg1)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT ID_PC,LIEU,DAT_PC,QUNTITER FROM points_de_collectes WHERE  LIEU LIKE '%" + arg1 + "%'");
    //model->setQuery("SELECT ID_PC,LIEU,DAT_PC,QUNTITER FROM points_de_collectes WHERE ID_PC LIKE '%" + arg1 + "%' OR LIEU LIKE '%" + arg1 + "%' OR DAT_PC LIKE '%" + arg1 + "%' OR QUNTITER LIKE '%" + arg1 + "%'");
    ui->tab_PC->setModel(model);

}

void MainWindow::on_B_convPDF_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF",
                                                            QString(), "*.pdf");
            if (!fileName.isEmpty()) {
                if (QFileInfo(fileName).suffix().isEmpty()) {
                    fileName.append(".pdf");
                }

                QPdfWriter writer(fileName);
                QPainter painter(&writer);

                int rowCount = ui->tab_PC->model()->rowCount();
                int columnCount = ui->tab_PC->model()->columnCount();

                QString strStream;
                QTextStream out(&strStream);

                out <<  "<html>\n"
                    "<head>\n"
                    "<meta Content=\"Text/html; charset=Windows-1251\">\n"
                    <<  QString("<title>%1</title>\n").arg("TITLE")
                    <<  "</head>\n"
                    "<body bgcolor=#ffffff link=#5000A0>\n"
                    "<style> table { font-size: 20px; } </style>\n"  // Adjust the font size here
                    "<table border=1 cellspacing=0 cellpadding=2 width='100%'>\n";

                // headers
                out << "<thead><tr bgcolor=#f0f0f0>";
                for (int column = 0; column < columnCount; column++)
                    if (!ui->tab_PC->isColumnHidden(column))
                        out << QString("<th width='%1%'>%2</th>").arg(100/columnCount).arg(ui->tab_PC->model()->headerData(column, Qt::Horizontal).toString());
                out << "</tr></thead>\n";

                // data table
                for (int row = 0; row < rowCount; row++) {
                    out << "<tr>";
                    for (int column = 0; column < columnCount; column++) {
                        if (!ui->tab_PC->isColumnHidden(column)) {
                            QString data = ui->tab_PC->model()->data(ui->tab_PC->model()->index(row, column)).toString().simplified();
                            out << QString("<td bkcolor=0 width='%1%'>%2</td>").arg(100/columnCount).arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                        }
                    }
                    out << "</tr>\n";
                }
                out <<  "</table>\n"
                    "</body>\n"
                    "</html>\n";

                QTextDocument *document = new QTextDocument();
                document->setHtml(strStream);

                // Scale the painter
                double scale = 15.0;  // Adjust the scale factor here
                painter.scale(scale, scale);

                document->drawContents(&painter);

                delete document;
            }
}


void MainWindow::on_tableView_ID_activated(const QModelIndex &index)
{
     QMessageBox msgBox;
    QString id=ui->tableView_ID->model()->data(index).toString();
    QSqlQuery qry;
    qry.prepare("SELECT * FROM points_de_collectes where id_pc='"+id+"' ");
    if (qry.exec())
    {
        while (qry.next())
        {
            ui->lineEdit_idpc->setText(qry.value(0).toString());
            ui->lineEdit_lieu->setText(qry.value(1).toString());
            ui->dateEdit->setDate(qry.value(2).toDate());
            ui->lineEdit_quntite->setText(qry.value(3).toString());
        }

    }
    else
    {
        msgBox.setText("no  XD  ");
    }



}



void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    // Récupérer l'index de l'onglet "Statistiques" sous l'onglet "CRUD"
    int statisticsTabIndex = ui->tabWidget->indexOf(ui->stat);  // Remplacez "statistiquesTab" par le nom réel de votre onglet "Statistiques"

    // Vérifier si l'onglet "Statistiques" a été cliqué
    if (index == statisticsTabIndex) {
        // Mettre à jour les statistiques ici
        updatequntiterStatisticsChart();
    } else {
        // Si l'onglet actif n'est pas "Statistiques", supprimer le graphique s'il existe
        clearquntiterStatisticsChart();
    }
}
void MainWindow::updatequntiterStatisticsChart()
{
    // Récupérer les statistiques réelles
    QMap<QString, int> statistics = p.getquntiterStatistics();
    int totalQuntite = 0;

    // Calculer le nombre total d'employés
    for (const QString &quntiteGroup : statistics.keys()) {
        totalQuntite += statistics[quntiteGroup];
    }

    // Créer une série pour le graphique circulaire
    QPieSeries *series = new QPieSeries();

    // Définir différentes nuances de vert pour chaque tranche
    QStringList greenColors = {"#55AA00", "#458C68", "#005500"};

    int colorIndex = 0;

    // Ajouter des tranches au graphique circulaire pour chaque groupe d'âge
    for (const QString &quntiteGroup : statistics.keys()) {
        int count = statistics[quntiteGroup];
        QPieSlice *slice = series->append(quntiteGroup, count);

        // Afficher le pourcentage sur chaque tranche
        slice->setLabel(quntiteGroup + ": " + QString::number(static_cast<double>(count) / totalQuntite * 100, 'f', 2) + "%");

        // Définir la couleur des tranches
        if (colorIndex < greenColors.size()) {
            slice->setBrush(QColor(greenColors[colorIndex]));
        } else {
            slice->setBrush(QColor(Qt::green));  // Utiliser une couleur par défaut si vous avez épuisé les nuances de vert
        }

        // Passer à la prochaine nuance de vert
        colorIndex++;
    }

    // Supprimer le graphique précédent s'il existe
    clearquntiterStatisticsChart();

    // Créer un graphique circulaire et le configurer
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition par groupe d'âge");

    // Définir le fond du graphique comme beige
    chart->setBackgroundBrush(QBrush(QColor("#F5F5DC")));  // Couleur beige

    // Créer une vue de graphique et l'ajouter à votre interface utilisateur
    quntiteStatisticsChartView = new QChartView(chart);
    quntiteStatisticsChartView->setRenderHint(QPainter::Antialiasing);
    quntiteStatisticsChartView->setParent(ui->stat);

    // Ajuster la taille du graphique (par exemple, 600x400 pixels)
    quntiteStatisticsChartView->setGeometry(70, 50, 600, 400);

    // Afficher le graphique
    quntiteStatisticsChartView->show();
}

void MainWindow::clearquntiterStatisticsChart()
{
    if (quntiteStatisticsChartView)
    {
        delete quntiteStatisticsChartView;
        quntiteStatisticsChartView = nullptr;
    }
}




void MainWindow::on_B_gestiondecollecte_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

}

void MainWindow::on_B_gestiondeproduit_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}
// gestion employee




void MainWindow::on_geajouter_clicked()
{

    QString nompren=ui->genom->text();
    QString mdp=ui->gemdp->text();
    QString adresse_email=ui->geadresse->text();
    int cin_emp = ui->gecin->text().toInt();
    int num_tel = ui->genumtel->text().toInt();
    QDate date_derecru= ui->gedate->date();
    QString departement= ui->gedepartement->text();
    QString poste= ui->geposte->text();
    int salaire= ui->gesalaire->text().toInt();
    int hfixe=ui->gehfixe->text().toInt();
    int hsupp=ui->gehsupp->text().toInt();

    bool cinValid = false;
        int cinValue = ui->gecin->text().toInt(&cinValid);
        if (!cinValid || ui->gecin->text().length() < 6)
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("Veuillez entrer un CIN valide (au moins 6 chiffres)."), QMessageBox::Cancel);
        return;
    }

        QSqlQueryModel *EmployeExistant = Emp.rechercheremp(cin_emp);
        if (EmployeExistant->rowCount() > 0)
     {
         QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("Le CIN de cet employé existe déjà."), QMessageBox::Cancel);
         delete EmployeExistant;
         return;
     }

     if (nompren.isEmpty() || !nompren.contains(QRegularExpression("^[a-zA-Z ]+$")))
     {
                QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("Veuillez entrer un nom et prénom correcte."), QMessageBox::Cancel);
                return;
            }
     if (adresse_email.isEmpty() || !adresse_email.contains(QRegularExpression("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$"))) {
         QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("Veuillez entrer une adresse_Email valide."), QMessageBox::Cancel);
         return;
     }
     if (departement.isEmpty() || !departement.contains(QRegularExpression("^[a-zA-Z ]+$"))) {
         QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("Veuillez entrer un département valide."), QMessageBox::Cancel);
         return;
     }
     if (poste.isEmpty() || !poste.contains(QRegularExpression("^[a-zA-Z  ]+$"))) {
         QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("Veuillez entrer un poste valide."), QMessageBox::Cancel);
         return;
     }

     Employe Emp(nompren,adresse_email,mdp,cin_emp,num_tel,date_derecru,departement,poste,salaire,hfixe,hsupp);
     bool test = Emp.ajouter();

     if (test)
     {
         QMessageBox::information(nullptr, QObject::tr("Ajout effectué"),
                                              QObject::tr("Ajout effectué.\n Cliquer Cancel pour sortir."), QMessageBox::Cancel);
     }
     else
     {
         QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("Erreur lors de l'ajout .\n Cliquer Cancel pour sortir."), QMessageBox::Cancel);
     }
}




void MainWindow::on_geafficher_clicked()
{
    Employe Emp ;
    ui->geaffichage->setModel(Emp.afficherEmployes());
    ui->geselectii->setModel(Emp.afficherEmployes());

}

void MainWindow::on_gesupprimer_clicked()
{
    Employe Emp;
    Emp.setCin(ui->gesupp->text().toInt());

    QSqlQueryModel *EmployeExistant = Emp.rechercheremp(ui->gesupp->text().toInt());
        if (EmployeExistant->rowCount() == 0)
        {
            QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("L'employé avec ce CIN n'existe pas."), QMessageBox::Cancel);
            delete EmployeExistant;
            return;
        }
    bool test=Emp.supprimer(Emp.getCin());

    if (test)
    {
            QMessageBox::information(nullptr, QObject::tr("SUCCES"), QObject::tr("Suppression effectuée"),
                                     QMessageBox::Ok);
            ui->geaffichage->setModel(Emp.afficherEmployes());
            ui->geselectii->setModel(Emp.afficherEmployes());

    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("ERREUR"), QObject::tr("Suppression échouée"),
                                 QMessageBox::Cancel);
    }
}


void MainWindow::on_gemodifier_clicked()
{
    QMessageBox messbox;
    QString nompren=ui->genom->text();
    QString mdp=ui->gemdp->text();
    QString adresse_email=ui->geadresse->text();
    int cin_emp=ui->gecin->text().toInt();
    int num_tel=ui->genumtel->text().toInt();
    QDate date_derecru=ui->gedate->date();
    QString departement=ui->gedepartement->text();
    QString poste=ui->geposte->text();
    int salaire=ui->gesalaire->text().toInt();
    int hfixe=ui->gehfixe->text().toInt();
    int hsupp=ui->gehsupp->text().toInt();
    Employe Emp(nompren,adresse_email,mdp,cin_emp,num_tel,date_derecru,departement,poste,salaire,hfixe,hsupp);
    bool test=Emp.modifier();
    if(test)
    {
            QMessageBox::information(nullptr, QObject::tr("OUI"),
                        QObject::tr("Modification accomplie\n"
                                    "Cliquer Cancel pour sortir."), QMessageBox::Cancel);




    }
        else
            { QMessageBox::critical(nullptr, QObject::tr("NON"),
                        QObject::tr("Modification inaccopmlie.\n"
                                    "Cliquer Cancel pour sortir."), QMessageBox::Cancel);

    }

}





void MainWindow::on_getrier_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString query = "SELECT * from EMPLOYEE ORDER BY ";

    switch (ui->comboBox_2->currentIndex()) {
    case NOM:
        query += "NOMPREN";
        break;
    case DEPARTEMENT:
        query += "DEPARTEMENT";
        break;
    case POSTE:
        query += "POSTE";
        break;
    case SALAIRE:
        query += "SALAIRE";
        break;
    }

    query += ui->gerechText->text();
    model->setQuery(query);
    ui->geaffichage->setModel(model);

}

void MainWindow::on_gechercher_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString query = "SELECT * from EMPLOYEE WHERE ";

    switch (ui->comboBox->currentIndex()) {
    case NOM:
        query += "NOMPREN='";
        break;
    case DEPARTEMENT:
        query += "DEPARTEMENT='";
        break;
    case POSTE:
        query += "POSTE='";
        break;
    }

    query += ui->gerechText->text() + "'";
    model->setQuery(query);
    ui->geaffichage->setModel(model);
}


void MainWindow::on_gePDF_clicked()
{
    exportTableViewToPDF(ui->geaffichage);
}

void MainWindow::on_geExcel_clicked()
{
    exportCSV();
}

void MainWindow::on_geselectii_currentIndexChanged( QString cin_emp )
{

    cin_emp =ui->geselectii->currentText();

          QSqlQuery query;
          query.prepare("SELECT *FROM EMPLOYEE WHERE CIN_EMP='" +cin_emp+ "'");

          if(query.exec())
          {
              while(query.next())
              {
                  ui->gecin->setText(query.value(0).toString());
                  QString nompren = query.value(1).toString().trimmed();
                  ui->geadresse->setText(query.value(2).toString());
                  ui->gedepartement->setText(query.value(4).toString());
                  ui->geposte->setText(query.value(6).toString());
                  ui->genumtel->setText(query.value(3).toString());
                  ui->gedate->setDate(query.value(7).toDate());
                  ui->gesalaire->setText(query.value(8).toString());
                  ui->gemdp->setText(query.value(5).toString());
                  ui->gehfixe->setText(query.value(9).toString());
                  ui->gehsupp->setText(query.value(10).toString());



              }
          }
}


// deconnexion
void MainWindow::on_B_gestiondemployee_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);

}

void MainWindow::on_Decon_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

}

void MainWindow::on_Decon_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

}

void MainWindow::on_Decon_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

}
// deconnexion

void MainWindow::on_b_gest_emp_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_B_gestiondeproduit_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}

void MainWindow::on_B_gestiondecollecte_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

}
