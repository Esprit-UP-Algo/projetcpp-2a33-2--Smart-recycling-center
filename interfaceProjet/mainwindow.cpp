#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pointdecollect.h"
#include "activitydialog.h"
#include "connect.h"

#include <QtCharts/QChart>
#include <QPixmap>
#include <QMessageBox>
#include <QComboBox>
#include <QFileDialog>
#include <QPainter>
#include <QTextStream>
#include <QTextDocument>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts>
#include <QPieSlice>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWidget>
#include <QUrl>
#include <QQmlContext>

#include <QtWidgets/QWidget>
#include <QtCharts/QChart>
#include <QQuickItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    quntiteStatisticsChartView = nullptr;
    QPixmap pix("C:/Users/HP/OneDrive/Bureau/img/LOGO");

    ui->label_pic->setPixmap(pix.scaled(150, 150, Qt::KeepAspectRatio));
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

    int ret=A.connect_arduino(); // lancer la connexion à arduino
    switch(ret){
    case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
        break;
    case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
       break;
    case(-1):qDebug() << "arduino is not available";
    }
     //QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer
     //le slot update_label suite à la reception du signal readyRead (reception des données).



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_label()
{
    data=A.read_from_arduino();
    qDebug() << "-------------------" <<data;
    ui->label_3->setText(data);

    QString id = "44541545";

        QSqlQuery query;
        query.prepare("SELECT * FROM matiere_recyclee WHERE ID_MR = :id");
        query.bindValue(":id", id);
        query.exec();

        if (query.next()) {
            QString type_prod = query.value(1).toString().trimmed();
            float prix = query.value(2).toFloat();
            QDate date_entree = query.value(3).toDate();
            int quantites = query.value(4).toInt();

            //QByteArray data;

            data1.append("existe,");
            data1.append(id + ",");
            data1.append(type_prod + ",");
            data1.append(QString::number(prix) + ",");
            data1.append(date_entree.toString() + ",");
            data1.append(QString::number(quantites) + "\n");

            // Assuming A is an instance of your class for Arduino communication
            A.write_to_arduino(data1);
        } else {
            QByteArray data;
            data.append("existe not\n");
            // Assuming A is an instance of your class for Arduino communication
            A.write_to_arduino(data1);
        }




}


void MainWindow::on_B_ajouter_clicked()
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
    bool test= p.supprime(ui->lineEdit_idpc->text().toInt());
    QMessageBox msgBox;
    if (test)
    {
        ui->tableView_ID->setModel(p.afficherID());
        ui->tab_PC->setModel(p.afficher());
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
    ui->quickWidget->setSource(QUrl("qrc:/Maps.qml"));
    update_label();

}



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


void MainWindow::on_pushButton_7_clicked()
{

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

void MainWindow::load(){
    QFile file(log);
    if(file.open(QIODevice::ReadOnly)){
        QTextStream in(&file);
        while(!in.atEnd()){
            QStringList dati;
            QString riga = in.readLine();
            if(riga.length()>0){
                dati = riga.split('\t');
                activities.push_back(attivita(dati));
            }
        }
    }
    file.close();
}

void MainWindow::showActivities(){
    QStringList headers = {"Tipo Attività", "Titolo", "Ora Inizio", "Ora Fine", "Ripeti", "Azioni"};
    QDate data = ui->calendarWidget->selectedDate();
    std::vector<int> pos;
    for(unsigned int i=0; i<activities.size(); i++){
        if(activities.at(i).finish.date() >= data){
            QString rep = activities.at(i).repeats;
            int r = -1;
            if(rep == "Mai")
                r = 0;
            if(rep == "Ogni giorno")
                r = 1;
            if(rep == "Ogni mese")
                r = 2;
            if(rep == "Ogni anno")
                r = 3;
            if(rep == "Ogni settimana")
                r = 4;
            switch(r){
            case 0:
                if(activities.at(i).data==data){
                    pos.push_back(i);
                }
                break;
            case 1:
                if(activities.at(i).data <= data){
                    pos.push_back(i);
                }
                break;
            case 2:
                if(activities.at(i).data.day()==data.day() && activities.at(i).data <= data){
                    pos.push_back(i);
                }
                break;
            case 3:
                if(activities.at(i).data.day()==data.day() && activities.at(i).data.month()==data.month() && activities.at(i).data <= data){
                    pos.push_back(i);
                }
                break;
            case 4:
                if(activities.at(i).data.dayOfWeek() == data.dayOfWeek() && activities.at(i).data <= data){
                    pos.push_back(i);
                }
                break;
            default: break;
            }
        }
    }
    ui->Attivita->setColumnCount(6);
    ui->Attivita->setColumnWidth(0, 130);
    ui->Attivita->setColumnWidth(1, 250);
    ui->Attivita->setColumnWidth(2, 75);
    ui->Attivita->setColumnWidth(3, 75);
    ui->Attivita->setColumnWidth(4, 120);
    ui->Attivita->setColumnWidth(5, 80);
    ui->Attivita->setRowCount(pos.size());
    ui->Attivita->setFont(QFont ("Times", 10, QFont::Bold));
    //ui->Attivita->setHorizontalHeaderLabels(headers.last(6));
    ui->Attivita->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->Attivita->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->Attivita->setStyleSheet("QTableWidget::item { padding: 0px }");
    ui->Attivita->setSortingEnabled(false);
    for(unsigned int i=0; i<pos.size(); i++){
        ui->Attivita->setRowHeight(i, 40);
        QStringList ActInfo = activities.at(pos.at(i)).getInfo();
        for(int j=0; j<5; j++){
            QTableWidgetItem *el = new QTableWidgetItem();
            el->setData(Qt::EditRole, ActInfo.at(j));
            ui->Attivita->setItem(i, j, el);
            ui->Attivita->item(i, j)->setFlags((ui->Attivita->item(i,j)->flags() ^ Qt::ItemIsEditable));
        }
        QToolButton *del = new QToolButton(this);
        QToolButton *edit = new QToolButton(this);
        del->setToolButtonStyle(Qt::ToolButtonIconOnly);
        edit->setToolButtonStyle(Qt::ToolButtonIconOnly);
        del->setIcon(QIcon::fromTheme("list-add", QIcon("delete.png")));
        edit->setIcon(QIcon::fromTheme("list-add", QIcon("edit.png")));
        del->setText(QString::number(pos.at(i)));
        edit->setText(QString::number(pos.at(i)));
        QGroupBox *box = new QGroupBox();
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget(del);
        layout->addWidget(edit);
        box->setLayout(layout);
        connect(del, &QToolButton::clicked , this, &MainWindow::removeActivity);
        connect(edit, &QToolButton::clicked , this, &MainWindow::editActivity);
        ui->Attivita->setCellWidget(i, 5, box);
    }
    ui->Attivita->setSortingEnabled(true);
    ui->Attivita->sortByColumn(2, Qt::AscendingOrder);
}



void MainWindow::on_AddActivity_clicked()
{
    QDate selectedDate = ui->calendarWidget->selectedDate();
    activitydialog *act = new activitydialog(this, selectedDate, &activities);
    if(act->exec()){
        showActivities();
    }
}
void MainWindow::addActivity(attivita a){
    activities.push_back(a);
}
void MainWindow::removeActivity(){
    QToolButton *sender = qobject_cast<QToolButton*>(QObject::sender());
    int pos = std::stoi(sender->text().toStdString());
    activities.erase(activities.begin()+pos);
    QFile file(log);
    int i=0;
    QStringList testo;
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while(!in.atEnd()){
            QString riga = in.readLine();
            if(i != pos){
                testo.append(riga);
            }
            i++;
        }
    }
    file.close();
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream in(&file);
        for(int i=0; i<testo.size(); i++){
            in << testo.at(i) << '\n';
        }
    }
    file.close();
    showActivities();
}

void MainWindow::editActivity(){
    QToolButton *sender = qobject_cast<QToolButton*>(QObject::sender());
    int pos = std::stoi(sender->text().toStdString());
    QFile file(log);
    int i=0;
    QStringList testo;
    int result = -1;
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while(!in.atEnd()){
            QString riga = in.readLine();
            if(i != pos){
                testo.append(riga);
            }else{
                QDate selectedDate = activities.at(i).getData();
                activitydialog *act = new activitydialog(this, selectedDate, &activities, true, pos);
                result = act->exec();
                if(result == QDialog::Accepted){
                    activities.at(pos) = act->edited_activity;
                    int y = selectedDate.year();
                    int m = selectedDate.month();
                    int d = selectedDate.day();
                    QString dt = QString::number(y)+":"+QString::number(m)+":"+QString::number(d);
                    QStringList info = act->edited_activity.getInfo();
                    QString fin = info.at(5);;
                    if(info.at(3).length() == 0)
                        fin += " 23:59:59";
                    else
                        fin += " " + info.at(3);
                    testo.append(info.at(0)+'\t'+info.at(1)+'\t'+info.at(2)+'\t'+fin+'\t'+info.at(4)+'\t'+dt);
                    riga = "";
                }
            }
            i++;
        }
    }
    file.close();
    if(result == QDialog::Accepted){
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream in(&file);
            for(int i=0; i<testo.size(); i++){
                in << testo.at(i) << '\n';
            }
        }
        file.close();
    }
    showActivities();
}



void MainWindow::on_calendarWidget_selectionChanged()
{
    showActivities();
}

void MainWindow::on_calendarWidget_activated(const QDate &date)
{
    showActivities();
}

void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    showActivities();
}
