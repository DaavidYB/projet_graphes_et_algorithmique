#include "mainwindow.h"
#include "composents/fsapsInput.h"
#include "composents/vertexinput.h"
#include "composents/matadjinput.h"
#include "composents/graphview.h"
#include "composents/outputalgo.h"
#include <fstream>
#include <QScreen>
#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QList>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

// CONSTRUCTEURS

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // loadGraph();
    createInterface();
}

MainWindow::~MainWindow()
{
    // Delete les pointeurs
    delete d_listeAlgorithmes;
    delete d_buttonLancerAlgo;
    delete d_graphview;
    if (d_currentInputWindow) {
        d_currentInputWindow->close();
        delete d_currentInputWindow;
    }
}

// CRÉATION DE L'INTERFACE

void MainWindow::createInterface()
{
    // On paramètre les dimensons de l'interface
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    setMinimumSize(width - (width * 0.40), height - (height * 0.20));

    // On crée le widget principal
    auto mainWidget {new QWidget{this}};
    this->setCentralWidget(mainWidget);
    // On crée et implémente le layout vertical central
    auto mainVerticalLayout {new QVBoxLayout};
    mainWidget->setLayout(mainVerticalLayout);

    // Ajout du widget de graph
    d_graphview = new graphView{d_graph, this};
    mainVerticalLayout->addWidget(d_graphview);

    // Ajout de l'affichage textuel du graph courant
    d_labelFS = new QLabel{};
    mainVerticalLayout->addWidget(d_labelFS);
    d_labelAPS = new QLabel{};
    mainVerticalLayout->addWidget(d_labelAPS);
    afficheFSAPS();

    // On crére et implémente le layout horithal
    auto horizonthalLayout {new QHBoxLayout};
    mainVerticalLayout->addLayout(horizonthalLayout);
    // On crée le QVBoxLayout contenant la liste des algorithmes
    auto listeAlgoLayout {new QVBoxLayout};
    horizonthalLayout->addLayout(listeAlgoLayout);
    // On génère la liste des algorithmes
    d_listeAlgorithmes =  new QComboBox{};
    d_listeAlgorithmes->addItem("Calcul des distances");
    d_listeAlgorithmes->addItem("Détermination du rang des sommets");
    d_listeAlgorithmes->addItem("Détermination des composantes fortement connexes selon Tarjan");
    d_listeAlgorithmes->addItem("Problème d'ordonnancement");
    d_listeAlgorithmes->addItem("Chemins les plus courts selon Dijkstra");
    d_listeAlgorithmes->addItem("Chemins les plus courts selon Dantzig");
    d_listeAlgorithmes->addItem("Arbre recouvrant minimal d'un graphe non orienté selon Kruskal");
    d_listeAlgorithmes->addItem("Codage de Prüfer");
    setOptions();
    d_buttonLancerAlgo = new QPushButton{"Lancer"};

    // On implémente le QVBoxLayout
    listeAlgoLayout->addWidget(d_listeAlgorithmes);
    listeAlgoLayout->addWidget(d_buttonLancerAlgo);
    listeAlgoLayout->addStretch(1);

    // On crée et imlémente le layout vertical contenant les boutons d'actions
    auto buttonLayout {new QVBoxLayout};
    horizonthalLayout->addLayout(buttonLayout);
    // On génère le bouton de saisie graphique
    auto buttonDessin {new QPushButton{"Dessiner un nouveau graph"}};
    buttonLayout->addWidget(buttonDessin);

    // On génère les options de saisies textuelles
    auto saisieLayout {new QHBoxLayout};
    buttonLayout->addLayout(saisieLayout);
    // On génère la liste déroulante de saisie
    d_listeSaisie = new QComboBox;
    d_listeSaisie->addItem("Saisie par FS APS");
    d_listeSaisie->addItem("Saisie par Matrice d'Adjacence");
    d_listeSaisie->addItem("Saisie par liste de sommets");
    // On génère le bouton de saisie
    auto buttonSaisie {new QPushButton{"Saisir"}};
    saisieLayout->addWidget(d_listeSaisie);
    saisieLayout->addWidget(buttonSaisie);

    // On génère le bouton de sauvegarde
    auto buttonFichier {new QPushButton{"Télécharger un graph"}};
    buttonLayout->addWidget(buttonFichier);
    // On génère le bouton de sauvegarde
    auto buttonSauvegarder {new QPushButton{"Sauvegarder le graph"}};
    buttonLayout->addWidget(buttonSauvegarder);


    // connect(buttonDessin, &QPushButton::clicked, this, &MainWindow::onDessine);
    connect(buttonSaisie, &QPushButton::clicked, this, &MainWindow::onSaisie);
    connect(buttonFichier, &QPushButton::clicked, this, &MainWindow::onTelecharge);
    connect(buttonSauvegarder, &QPushButton::clicked, this, &MainWindow::onSauvegarde);

    connect(d_buttonLancerAlgo, &QPushButton::clicked, this, &MainWindow::onExecAlgo);
}

void MainWindow::setOptions() const
{
    // On regarde si le graph possède des coûts
    bool c = d_graph.graphaveccout();
    if(d_graph.oriented()){
        if(c){
            // On active les algorithmes : dijkstra, dantzig
            d_listeAlgorithmes->setItemData(0, false, Qt::UserRole - 1);
            d_listeAlgorithmes->setItemData(1, false, Qt::UserRole - 1);
            d_listeAlgorithmes->setItemData(2, false, Qt::UserRole - 1);
            d_listeAlgorithmes->setItemData(3, false, Qt::UserRole); // Ordonnancement
            d_listeAlgorithmes->setItemData(4, false, Qt::UserRole); // Dijkstra
            d_listeAlgorithmes->setItemData(5, false, Qt::UserRole); // Dantzig
            d_listeAlgorithmes->setItemData(6, false, Qt::UserRole - 1);
            d_listeAlgorithmes->setItemData(7, false, Qt::UserRole - 1);
            // On règle l'indice courant
            d_listeAlgorithmes->setCurrentIndex(3);

        } else {
            // On active les algorithmes : distance, rang, tarjan
            d_listeAlgorithmes->setItemData(0, false, Qt::UserRole); // Distance
            d_listeAlgorithmes->setItemData(1, false, Qt::UserRole); // Rang
            d_listeAlgorithmes->setItemData(2, false, Qt::UserRole); // Tarjan
            d_listeAlgorithmes->setItemData(3, false, Qt::UserRole);
            d_listeAlgorithmes->setItemData(4, false, Qt::UserRole);
            d_listeAlgorithmes->setItemData(5, false, Qt::UserRole);
            d_listeAlgorithmes->setItemData(6, false, Qt::UserRole - 1);
            d_listeAlgorithmes->setItemData(7, false, Qt::UserRole - 1);
            // On règle l'indice courant
            d_listeAlgorithmes->setCurrentIndex(0);
        }

    } else if(c){
        // On active les algorithmes : Kruskal
        d_listeAlgorithmes->setItemData(0, false, Qt::UserRole - 1);
        d_listeAlgorithmes->setItemData(1, false, Qt::UserRole - 1);
        d_listeAlgorithmes->setItemData(2, false, Qt::UserRole - 1);
        d_listeAlgorithmes->setItemData(3, false, Qt::UserRole);
        d_listeAlgorithmes->setItemData(4, false, Qt::UserRole - 1);
        d_listeAlgorithmes->setItemData(5, false, Qt::UserRole - 1);
        d_listeAlgorithmes->setItemData(6, false, Qt::UserRole); // Kruskal
        d_listeAlgorithmes->setItemData(7, false, Qt::UserRole - 1);
        // On règle l'indice courant
        d_listeAlgorithmes->setCurrentIndex(6);

    } else {
        // On active l'algorithme : Prufer
        d_listeAlgorithmes->setItemData(0, false, Qt::UserRole - 1);
        d_listeAlgorithmes->setItemData(1, false, Qt::UserRole - 1);
        d_listeAlgorithmes->setItemData(2, false, Qt::UserRole - 1);
        d_listeAlgorithmes->setItemData(3, false, Qt::UserRole);
        d_listeAlgorithmes->setItemData(4, false, Qt::UserRole - 1);
        d_listeAlgorithmes->setItemData(5, false, Qt::UserRole - 1);
        d_listeAlgorithmes->setItemData(6, false, Qt::UserRole);
        d_listeAlgorithmes->setItemData(7, false, Qt::UserRole); // Prüfer
        // On règle l'indice courant
        d_listeAlgorithmes->setCurrentIndex(7);
    }
}

void MainWindow::afficheFSAPS()
{
    // On vide les anciens textes
    d_labelFS->clear();
    d_labelAPS->clear();

    // On récupère les tableaux fs et aps
    vector<int> fs, aps;
    d_graph.fs_aps(fs, aps);

    // On prépare l'affichage sous forme de QString
    QString fsStr;
    for (int val : fs)
        fsStr += QString::number(val) + ", ";

    // On supprime la dernière virgule et l'espace
    fsStr.chop(2);

    QString apsStr;
    for (int val : aps)
        apsStr += QString::number(val) + ", ";

    // On supprime la dernière virgule et l'espace
    apsStr.chop(2);

    // On affiche les valeurs de fs et aps
    d_labelFS->setText({"FS: " + fsStr});
    d_labelAPS->setText({"APS: " + apsStr});
}

void MainWindow::updateGraph()
{
    d_graphview->graphChanged(d_graph);
    afficheFSAPS();
    setOptions();
}


// MÉTHODES ONCLIC

void MainWindow::onGrapheReceived(const graphalgo::graph& g)
{
    // Mettre à jour l'affichage du graphe
    d_graph = g;
    updateGraph();

    // Fermer la fenêtre de saisie
    if(d_currentInputWindow) {
        d_currentInputWindow->close();
        d_currentInputWindow = nullptr;
    }
}

void MainWindow::onExecAlgo() {
    int i = d_listeAlgorithmes->currentIndex();
    auto output {new outputAlgo{i, d_graph}};
    output->show();
}

void MainWindow::onSaisie()
{
    int i = d_listeSaisie->currentIndex();
    switch (i)
    {
        case 0 :
            onSaisieFsAps();
            break;

        case 1 :
            onSaisieMatAdj();
            break;

        case 2 :
            onSaisieListeSommets();
            break;

    }
}

void MainWindow::onSaisieMatAdj() {
    // On crée la fenêtre
    d_currentInputWindow = new matAdjInput{};

    // On connecte le signal transmis après la création
    connect(static_cast<matAdjInput*>(d_currentInputWindow), &matAdjInput::graphe, this, &MainWindow::onGrapheReceived);
    // On affiche la fenêtre
    d_currentInputWindow->show();
}

void MainWindow::onSaisieFsAps()
{
    // On crée la fenêtre
    d_currentInputWindow = new fsapsInput{};
    // On connecte le signal transmis après la création
    connect(static_cast<fsapsInput*>(d_currentInputWindow), &fsapsInput::graphe, this, &MainWindow::onGrapheReceived);
    // On affiche la fenêtre
    d_currentInputWindow->show();
}

void MainWindow::onSaisieListeSommets()
{
    // On crée la fenêtre
    d_currentInputWindow = new vertexInput{};
    // On connecte le signal transmis après la création
    connect(static_cast<vertexInput*>(d_currentInputWindow), &vertexInput::graphe, this, &MainWindow::onGrapheReceived);
    // On affiche la fenêtre
    d_currentInputWindow->show();
}

void MainWindow::onTelecharge()
{
    // On ouvre une boîte de dialogue pour permettre à l'utilisateur de choisir le fichier
    QString fileName = QFileDialog::getOpenFileName(this, "Sauvegarder le graphe", "", "Fichiers texte (*.graph)");
    if(fileName.isEmpty()) {
        return; // L'utilisateur a annulé la sauvegarde
    }

    // On ouvre le fichier en écriture
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur de lecture", "Impossible d'ouvrir le fichier en lecture.");

    } else {
        QTextStream in(&file);
        QString fileContent = in.readAll();
        file.close();

        if (fileContent.isEmpty()) {
            QMessageBox::critical(this, "Erreur de lecture", "Le fichier sélectionné est vide.");

        } else {
            std::ifstream ifs {fileName.toStdString()};
            if(ifs.good()){
                graphalgo::graph g{};
                g.load(ifs);
                d_graph = g;
                // On met à jour l'affichage
                updateGraph();
                //d_graphview->graphChanged(g);
            } else {
                QMessageBox::critical(this, "Erreur d'importation", "Impossible de lire le fichier.");
            }
        }
    }
    file.close();
}

void MainWindow::onSauvegarde()
{
    // On ouvre une boîte de dialogue pour permettre à l'utilisateur de choisir le fichier
    QString fileName = QFileDialog::getSaveFileName(this, "Sauvegarder le graphe", "", "Fichiers texte (*.graph)");
    if(fileName.isEmpty()) {
        return; // L'utilisateur a annulé la sauvegarde
    }

    // On vérifie que l'utilisateur a choisi un fichier compatible
    QString extension {".graph"};
    if(!fileName.endsWith(extension)) {
        fileName += extension;
    }

    // On ouvre le fichier en écriture
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur d'écriture", "Impossible d'ouvrir le fichier en écriture.");

    // On écrit
    } else {
        std::ofstream ofs(fileName.toStdString());
        d_graph.save(ofs);
    }
    file.close();
}
