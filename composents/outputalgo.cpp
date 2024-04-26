#include "outputalgo.h"
#include "graph/algorithmes.h"
#include <QPushButton>

outputAlgo::outputAlgo(int indiceOutput, graphalgo::graph &g, QWidget *parent)
    : QWidget(parent)
{
    switch(indiceOutput) {
        case 0:
            distance(g);
            break;
        case 1:
            rang(g);
            break;
    }
}

outputAlgo::~outputAlgo() {
    delete d_comboBoxDistance;
    delete d_resultatsDistance;
}

void outputAlgo::distance(graphalgo::graph &g) {
    // Création du layout
    auto mainLayout {new QVBoxLayout{this}};

    // Titre de la fenêtre
    auto title = new QLabel("Affichage des distances par rapport à un sommet");
    mainLayout->addWidget(title);

    // Ajout d'une ligne de séparation
    auto separationLine = new QFrame;
    separationLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    mainLayout->addWidget(separationLine);

    // Création de la combobox pour choisir le sommet et du layout
    auto layoutSommet {new QHBoxLayout{}};
    auto sommetLabel = new QLabel("Choisissez le sommet de référence :");
    d_comboBoxDistance = new QComboBox;

    d_comboBoxDistance = new QComboBox;
    for(int i = 1; i <= g.n(); i++) {
        d_comboBoxDistance->addItem(QString::number(i));
    }
    layoutSommet->addWidget(sommetLabel);
    layoutSommet->addWidget(d_comboBoxDistance);

    mainLayout->addLayout(layoutSommet);

    // Création du bouton pour calculer les distances
    // auto calculerButton = new QPushButton("Calculer les distances");
    // d_layoutDistance->addWidget(calculerButton);

    // Création du label pour afficher les résultats
    d_resultatsDistance = new QLabel;
    mainLayout->addWidget(d_resultatsDistance);

    // Connexion du bouton au calcul des distances
    connect(d_comboBoxDistance, &QComboBox::currentIndexChanged, [this, &g] {
        calculerDistances(g);
    });
}

void outputAlgo::calculerDistances(graphalgo::graph &g)
{
    // Récupérer le sommet choisi dans la combobox
    int sommet = d_comboBoxDistance->currentIndex() + 1;

    // Récupérer les tableaux FS et APS du graphe
    std::vector<int> fs, aps;
    g.fs_aps(fs, aps);

    // Calculer les distances par rapport au sommet choisi
    std::vector<int> dist = graphalgo::distance(sommet, fs, aps);

    // Mettre à jour l'affichage des résultats
    QString resultatStr;
    for (int i = 1; i <= dist[0]; i++) {
        resultatStr += "Sommet " + QString::number(i) + " : " + QString::number(dist[i]) + "\n";
    }
    d_resultatsDistance->setText(resultatStr);
}


void outputAlgo::rang(graphalgo::graph &g)
{
    // Création du layout
    auto mainLayout {new QVBoxLayout{this}};

    // Titre de la fenêtre
    auto title = new QLabel("Affichage du rang des sommets");
    mainLayout->addWidget(title);

    // Ajout d'une ligne de séparation
    auto separationLine = new QFrame;
    separationLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    mainLayout->addWidget(separationLine);

    // Création du label pour afficher les résultats
    auto resultatsRang {new QLabel};
    mainLayout->addWidget(resultatsRang);

    // Calculer les rangs
    std::vector<int> fs, aps;
    g.fs_aps(fs, aps);
    std::vector<int> rang = graphalgo::rang(fs, aps);

    // Mettre à jour l'affichage des résultats
    QString resultatStr;
    for (int i = 1; i <= rang[0]; i++) {
        resultatStr += "Sommet " + QString::number(i) + " : " + QString::number(rang[i]) + "\n";
    }
    resultatsRang->setText(resultatStr);
}
