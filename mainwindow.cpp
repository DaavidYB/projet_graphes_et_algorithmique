#include "mainwindow.h"
#include <QWidget>
#include <QBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QList>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->createInterface();
}

MainWindow::~MainWindow()
{
    // Delete les pointeurs
    delete d_listeAlgorithmes;
}

void MainWindow::createInterface()
{
    // On crée le widget principal
    auto mainWidget {new QWidget{this}};
    this->setCentralWidget(mainWidget);

    // On crée et implémente le layout vertical central
    auto mainVerticalLayout {new QVBoxLayout};
    mainWidget->setLayout(mainVerticalLayout);

    // Ajout du widget de graph
    auto futurGraph {new QLabel{"Furtur Widget Graph"}};
    mainVerticalLayout->addWidget(futurGraph);

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
    d_listeAlgorithmes->addItem("Arbre recouvrant minimal d'un graphe non orienté selon Kruskal");
    d_listeAlgorithmes->addItem("Codage de Prüfer");
    d_buttonLancerAlgo = new QPushButton{"Lancer"};
    // On implémente le QVBoxLayout
    listeAlgoLayout->addWidget(d_listeAlgorithmes);
    listeAlgoLayout->addWidget(d_buttonLancerAlgo);
    listeAlgoLayout->addStretch(1);

    // On crée et imlémente le layout vertical contenant les boutons d'actions
    auto buttonLayout {new QVBoxLayout};
    horizonthalLayout->addLayout(buttonLayout);

    // On génère les boutons
    auto buttonDessin {new QPushButton{"Dessiner un nouveau graph"}};
    auto buttonSaisie {new QPushButton{"Saisie textuelle"}};
    auto buttonFichier {new QPushButton{"Télécharger un fichier"}};
    auto buttonSauvegarder {new QPushButton{"Sauvegarderle graph"}};

    // On crée le layout contenant les sous-options de saisie textuelle
    auto buttonSaisieLayout {new QVBoxLayout{}};
    auto bDessinFsAps {new QPushButton{"par FS APS"}};
    auto bDessinMatriceAdj {new QPushButton{"par matrice d'adjacence"}};
    auto bDessinListeSommets {new QPushButton{"par liste de sommets"}};

    buttonSaisieLayout->addWidget(bDessinFsAps);
    buttonSaisieLayout->addWidget(bDessinMatriceAdj);
    buttonSaisieLayout->addWidget(bDessinListeSommets);
    // On génère le groupe de boutons de saisie textuelle
    d_saisieGroup = new QButtonGroup{};
    d_saisieGroup->addButton(bDessinFsAps);
    d_saisieGroup->addButton(bDessinMatriceAdj);
    d_saisieGroup->addButton(bDessinListeSommets);
    hideSaisieGroup();

    // On crée le layout contenant les sous-options de saisie par fichier
    auto buttonFichierLayout {new QVBoxLayout{}};
    auto bSaisieFsAps {new QPushButton{"par FS APS"}};
    auto bSaisieMatriceAdj {new QPushButton{"par matrice d'adjacence"}};
    buttonFichierLayout->addWidget(bSaisieFsAps);
    buttonFichierLayout->addWidget(bSaisieMatriceAdj);
    // On génère le groupe de boutons de saisie par fichier
    fichierGroup = new QButtonGroup{};
    fichierGroup->addButton(bSaisieFsAps);
    fichierGroup->addButton(bSaisieMatriceAdj);
    hideFichierGroup();

    // On implémente le buttonLayout
    buttonLayout->addWidget(buttonDessin);
    buttonLayout->addWidget(buttonSaisie);
    buttonLayout->addLayout(buttonSaisieLayout);
    buttonLayout->addWidget(buttonFichier);
    buttonLayout->addLayout(buttonFichierLayout);
    buttonLayout->addWidget(buttonSauvegarder);

    // On connecte les boutons à leur action
    connect(buttonSaisie, &QPushButton::clicked, this, &MainWindow::onSaisie);
    connect(buttonFichier, &QPushButton::clicked, this, &MainWindow::onFichier);
}

void MainWindow::showSaisieGroup()
{
    QList<QAbstractButton*> buttons = d_saisieGroup->buttons();
    for(auto b : buttons)
        b->setVisible(true);
}

void MainWindow::showFichierGroup()
{
    QList<QAbstractButton*> buttons = fichierGroup->buttons();
    for(auto b : buttons)
        b->setVisible(true);
}

void MainWindow::hideSaisieGroup()
{
    QList<QAbstractButton*> buttons = d_saisieGroup->buttons();
    for(auto b : buttons)
        b->setVisible(false);
}

void MainWindow::hideFichierGroup()
{
    QList<QAbstractButton*> buttons = fichierGroup->buttons();
    for(auto b : buttons)
        b->setVisible(false);
}

void MainWindow::onSaisie()
{
    if(saisieGroupVisible){
        saisieGroupVisible = false;
        this->hideSaisieGroup();
    } else {
        saisieGroupVisible = true;
        this->showSaisieGroup();
    }
}

void MainWindow::onFichier()
{
    if(fichierGroupVisible){
        fichierGroupVisible = false;
        this->hideFichierGroup();
    } else {
        fichierGroupVisible = true;
        this->showFichierGroup();
    }
}




















