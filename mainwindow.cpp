#include "mainwindow.h"
#include "composents/fsapsInput.h"
#include "composents/vertexinput.h"
#include "composents/matadjinput.h"
#include "composents/graphview.h"
#include <QScreen>
#include <QWidget>
#include <QBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QList>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

// CONSTRUCTEURS

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createInterface();
}

MainWindow::~MainWindow()
{
    // Delete les pointeurs
    delete d_listeAlgorithmes;
    delete d_buttonLancerAlgo;
    delete d_saisieGroup;
    delete fichierGroup;
    if (d_currentInputWindow) {
        d_currentInputWindow->close();
        delete d_currentInputWindow;
    }
}

// CRÉATION DE L'INTERFACE

void MainWindow::createInterface()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    setMinimumSize(width -600, height- 200);
    // On crée le widget principal
    auto mainWidget {new QWidget{this}};
    this->setCentralWidget(mainWidget);

    // On crée et implémente le layout vertical central
    auto mainVerticalLayout {new QVBoxLayout};
    mainWidget->setLayout(mainVerticalLayout);

    // Ajout du widget de graph
    d_graphview = new graphView{d_graph};
    mainVerticalLayout->addWidget(d_graphview);

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
    auto buttonSauvegarder {new QPushButton{"Sauvegarder le graph"}};

    // On crée le layout contenant les sous-options de saisie textuelle
    auto buttonSaisieLayout {new QVBoxLayout{}};
    auto bSaisieFsAps {new QPushButton{"par FS APS"}};
    auto bSaisieMatriceAdj {new QPushButton{"par matrice d'adjacence"}};
    auto bSaisieListeSommets {new QPushButton{"par liste de sommets"}};

    buttonSaisieLayout->addWidget(bSaisieFsAps);
    buttonSaisieLayout->addWidget(bSaisieMatriceAdj);
    buttonSaisieLayout->addWidget(bSaisieListeSommets);
    // On génère le groupe de boutons de saisie textuelle
    d_saisieGroup = new QButtonGroup{};
    d_saisieGroup->addButton(bSaisieFsAps);
    d_saisieGroup->addButton(bSaisieMatriceAdj);
    d_saisieGroup->addButton(bSaisieListeSommets);
    hideSaisieGroup();

    // On crée le layout contenant les sous-options de saisie par fichier
    auto buttonFichierLayout {new QVBoxLayout{}};
    auto bFichierFsAps {new QPushButton{"par FS APS"}};
    auto bFichierMatriceAdj {new QPushButton{"par matrice d'adjacence"}};
    buttonFichierLayout->addWidget(bFichierFsAps);
    buttonFichierLayout->addWidget(bFichierMatriceAdj);
    // On génère le groupe de boutons de saisie par fichier
    fichierGroup = new QButtonGroup{};
    fichierGroup->addButton(bFichierFsAps);
    fichierGroup->addButton(bFichierMatriceAdj);
    hideFichierGroup();

    // On implémente le buttonLayout
    buttonLayout->addWidget(buttonDessin);
    buttonLayout->addWidget(buttonSaisie);
    buttonLayout->addLayout(buttonSaisieLayout);
    buttonLayout->addWidget(buttonFichier);
    buttonLayout->addLayout(buttonFichierLayout);
    buttonLayout->addWidget(buttonSauvegarder);

    // On connecte les boutons à leur action
    // connect(buttonDessin, &QPushButton::clicked, this, ...);
    connect(buttonSaisie, &QPushButton::clicked, this, &MainWindow::onSaisie);
    connect(buttonFichier, &QPushButton::clicked, this, &MainWindow::onFichier);

    connect(bSaisieFsAps, &QPushButton::clicked, this, &MainWindow::onSaisieFsAps);
    connect(bSaisieMatriceAdj, &QPushButton::clicked, this, &MainWindow::onSaisieMatAdj);
    connect(bSaisieListeSommets, &QPushButton::clicked, this, &MainWindow::onSaisieListeSommets);

    connect(bFichierFsAps, &QPushButton::clicked, this, &MainWindow::onFichierFsAps);
    // connect(bFichierMatriceAdj, &QPushButton::clicked, this, ...);

    connect(buttonSauvegarder, &QPushButton::clicked, this, &MainWindow::onSauvegarde);
}

// SHOW / HIDE

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

void MainWindow::hideButtonGroup()
{
    hideSaisieGroup();
    hideFichierGroup();
}

// MÉTHODES ONCLIC

void MainWindow::onGrapheReceived(const graphalgo::graph& g)
{
    // Mettre à jour l'affichage du graphe
    d_graph = g;
    // Appel de la méthode pour mettre à jour l'affichage du graphe

    // Fermer la fenêtre de saisie
    if(d_currentInputWindow) {
        d_currentInputWindow->close();
        d_currentInputWindow = nullptr;
    }
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

void MainWindow::onSaisieMatAdj() {
    hideButtonGroup();
    // On crée la fenêtre
    d_currentInputWindow = new matAdjInput{};

    // On connecte le signal transmis après la création
    connect(static_cast<matAdjInput*>(d_currentInputWindow), &matAdjInput::graphe, this, &MainWindow::onGrapheReceived);
    // On affiche la fenêtre
    d_currentInputWindow->show();
}

void MainWindow::onSaisieFsAps()
{
    hideButtonGroup();
    // On crée la fenêtre
    d_currentInputWindow = new fsapsInput{};
    // On connecte le signal transmis après la création
    connect(static_cast<fsapsInput*>(d_currentInputWindow), &fsapsInput::graphe, this, &MainWindow::onGrapheReceived);
    // On affiche la fenêtre
    d_currentInputWindow->show();
}

void MainWindow::onSaisieListeSommets()
{
    hideButtonGroup();
    // On crée la fenêtre
    d_currentInputWindow = new vertexInput{};
    // On connecte le signal transmis après la création
    connect(static_cast<vertexInput*>(d_currentInputWindow), &vertexInput::graphe, this, &MainWindow::onGrapheReceived);
    // On affiche la fenêtre
    d_currentInputWindow->show();
}

void MainWindow::onFichierFsAps()
{
    hideButtonGroup();

    // Ouvrir une boîte de dialogue pour sélectionner le fichier
    QString fileName = QFileDialog::getOpenFileName(this, "Sélectionner un fichier FS-APS", "", "Fichiers texte (*.txt)");
    if(fileName.isEmpty()) {
        return; // L'utilisateur a annulé la sélection
    }

    // Lire le contenu du fichier
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur de lecture", "Impossible d'ouvrir le fichier.");
        return;
    }

    QTextStream in(&file);
    QString fsLine = in.readLine();
    QString apsLine = in.readLine();

    // Vérifier la validité des données
    std::vector<int> fs, aps;
    if(!validationFsAps(fsLine, apsLine, fs, aps)) {
        file.close();
        return;
    }


    // Construire le graphe à partir de FS et APS
    graphalgo::graph g{fs, aps};

    // Émettre le graphe
    onGrapheReceived(g);

    file.close();
}

void MainWindow::onSauvegarde()
{
    // Ouvrir une boîte de dialogue pour permettre à l'utilisateur de choisir le fichier
    QString fileName = QFileDialog::getSaveFileName(this, "Sauvegarder le graphe", "", "Fichiers texte (*.txt)");
    if(fileName.isEmpty()) {
        // L'utilisateur a annulé la sauvegarde
        return;
    }

    // Ouvrir le fichier en écriture
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur d'écriture", "Impossible d'ouvrir le fichier en écriture.");
        return;
    }

    // Récupérer les tableaux FS et APS
    std::vector<int> fs, aps;
    std::vector<std::vector<int>> matAdj = d_graph.mat_adj();
    d_graph.fs_aps(fs, aps);

    // Écrire les tableaux FS et APS dans le fichier
    QTextStream out(&file);
    for(unsigned i = 0; i < fs.size() - 1; i++) {
        out << fs[i] << " ";
    }
    out << fs[fs.size() - 1];
    out << "\n";
    for(unsigned i = 0; i < aps.size() - 1; i++) {
        out << aps[i] << " ";
    }
    out << aps[aps.size() - 1];

    file.close();
}
