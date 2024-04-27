#include "outputalgo.h"
#include "graph/algorithmes.h"
#include "graphview.h"
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
        case 4:
            dijkstra(g);
        case 6:
            kruskal(g);
            break;
        case 7:
            prufer(g);
            break;
        default:
            break;
    }
}

outputAlgo::~outputAlgo() {
    delete d_comboBoxDistance;
    delete d_resultatsDistance;
    delete d_graphViewDijkstra;
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

    // On remplit la combobox
    d_comboBoxDistance = new QComboBox;
    for(int i = 1; i <= g.n(); i++) {
        d_comboBoxDistance->addItem(QString::number(i));
    }
    // On fait les ajouts aux différents layouts
    layoutSommet->addWidget(sommetLabel);
    layoutSommet->addWidget(d_comboBoxDistance);
    mainLayout->addLayout(layoutSommet);

    // Création du label pour afficher les résultats
    d_resultatsDistance = new QLabel;
    mainLayout->addWidget(d_resultatsDistance);

    // Connexion de la méthode pour le changement de valeur au calcul des distances
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

void outputAlgo::prufer(graphalgo::graph& g)
{
    // Création du layout
    auto mainLayout = new QVBoxLayout{this};

    // Titre de la fenêtre
    auto title = new QLabel("Affichage du codage de Prufer");
    mainLayout->addWidget(title);

    // Ajout d'une ligne de séparation
    auto separationLine = new QFrame;
    separationLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    mainLayout->addWidget(separationLine);

    // Création du label pour afficher les résultats
    auto resultatsLabel = new QLabel;
    mainLayout->addWidget(resultatsLabel);

    // Calculer le codage de Prufer
    std::vector<std::vector<int>> adj = g.mat_adj();
    std::vector<int> prf = graphalgo::prufer(adj);

    // Mettre à jour l'affichage des résultats
    QString resultatStr = "Codage de Prufer : ";
    for (unsigned i = 0; i < prf.size(); i++) {
        resultatStr += QString::number(prf[i] + 1) + " ";
    }
    resultatsLabel->setText(resultatStr);
}

void outputAlgo::kruskal(graphalgo::graph& g) {
    // Création du layout
    auto mainLayout = new QVBoxLayout{this};

    // Titre de la fenêtre
    auto title = new QLabel("Affichage de l'arbre recouvrant minimal d'un graphe non orienté selon Kruskal");
    mainLayout->addWidget(title);

    // Ajout d'une ligne de séparation
    auto separationLine = new QFrame;
    separationLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    mainLayout->addWidget(separationLine);

    // Récupérer le résultat de Kruskal
    graphalgo::graph T = graphalgo::kruskal(g);

    // TESTS
    /*graphalgo::graph T(7, false);
    T.add_successor(1, 2, 1);
    T.add_successor(1, 6, 2);
    T.add_successor(1, 7, 1);
    T.add_successor(2, 3, 3);
    T.add_successor(2, 7, 1);
    T.add_successor(3, 7, 1);
    T.add_successor(3, 4, 2);
    T.add_successor(4, 5, 1);
    T.add_successor(4, 7, 2);
    T.add_successor(5, 6, 2);
    T.add_successor(6, 7, 1);

    graphalgo::graph P = graphalgo::kruskal(T);*/

    // On crée un graphView pour son affichage
    // auto graphview {new graphView{P}};
    auto graphview {new graphView{T}};
    mainLayout->addWidget(graphview);
}

/*void outputAlgo::dijkstra(graphalgo::graph &g) {
    // Création du layout
    auto mainLayout {new QVBoxLayout{this}};

    // Titre de la fenêtre
    auto title = new QLabel("Affichage des chemins les plus courts selon Dijkstra");
    mainLayout->addWidget(title);

    // Ajout d'une ligne de séparation
    auto separationLine = new QFrame;
    separationLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    mainLayout->addWidget(separationLine);

    // Création de la combobox pour choisir le sommet et du layout
    auto layoutSommet {new QHBoxLayout{}};
    auto sommetLabel = new QLabel{"Choisissez le sommet de référence :"};
    d_comboBoxDijkstra = new QComboBox;

    // On remplit la combobox
    d_comboBoxDistance = new QComboBox;
    for(int i = 1; i <= g.n(); i++) {
        d_comboBoxDijkstra->addItem(QString::number(i));
    }
    // On fait les ajouts aux différents layouts
    layoutSommet->addWidget(sommetLabel);
    layoutSommet->addWidget(d_comboBoxDijkstra);
    mainLayout->addLayout(layoutSommet);

    // Récupération du résultat du sommet 1 et création du graphView
    graphalgo::graph sommet1 = graphalgo::dijkstra(1, g);
    d_graphViewDijkstra = new graphView{sommet1};
    // Création du graphView pour afficher les résultats
    mainLayout->addWidget(d_graphViewDijkstra);

    connect(d_comboBoxDistance, &QComboBox::currentIndexChanged, [this, &g] {
        calculerDijkstra(g);
    });
}*/

// TESTS
void outputAlgo::dijkstra(graphalgo::graph &g) {
    // Création du layout
    auto mainLayout {new QVBoxLayout{this}};

    // Titre de la fenêtre
    auto title = new QLabel("Affichage des chemins les plus courts selon Dijkstra");
    mainLayout->addWidget(title);

    // Ajout d'une ligne de séparation
    auto separationLine = new QFrame;
    separationLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    mainLayout->addWidget(separationLine);

    // Création de la combobox pour choisir le sommet et du layout
    auto layoutSommet {new QHBoxLayout{}};
    auto sommetLabel = new QLabel{"Choisissez le sommet de référence :"};
    d_comboBoxDijkstra = new QComboBox;

    graphalgo::graph p{6, true};

    // On remplit la combobox
    d_comboBoxDijkstra = new QComboBox;
    for(int i = 1; i <= p.n(); i++) {
        d_comboBoxDijkstra->addItem(QString::number(i));
    }
    // On fait les ajouts aux différents layouts
    layoutSommet->addWidget(sommetLabel);
    layoutSommet->addWidget(d_comboBoxDijkstra);
    mainLayout->addLayout(layoutSommet);

    p.add_successor(1, 2, 10);
    p.add_successor(1, 3, 3);
    p.add_successor(1, 5, 6);
    p.add_successor(2, 1, 0);
    p.add_successor(3, 2, 4);
    p.add_successor(3, 5, 2);
    p.add_successor(4, 3, 1);
    p.add_successor(4, 5, 3);
    p.add_successor(5, 2, 0);
    p.add_successor(5, 6, 1);
    p.add_successor(6, 1, 2);
    p.add_successor(6, 2, 1);

    /*p.add_successor(1, 3, 18);
    p.add_successor(1, 5, 3);
    p.add_successor(2, 1, 8);
    p.add_successor(3, 2, 4);
    p.add_successor(4, 2, 1);
    p.add_successor(5, 2, 10);
    p.add_successor(5, 4, 2);*/


    // Récupération du résultat du sommet 1 et création du graphView
    graphalgo::graph sommet1 = graphalgo::dijkstra(1, p);
    d_graphViewDijkstra = new graphView{sommet1};
    // Création du graphView pour afficher les résultats
    mainLayout->addWidget(d_graphViewDijkstra);

    connect(d_comboBoxDijkstra, &QComboBox::currentIndexChanged, [this, &p] {
        calculerDijkstra(p);
    });
}

void outputAlgo::calculerDijkstra(graphalgo::graph& g)
{
    // Récupérer le sommet choisi dans la combobox
    int sommet = d_comboBoxDijkstra->currentIndex() + 1;

    // Calculer les chemins les plus courts par rapport au sommet choisi
    graphalgo::graph T = graphalgo::dijkstra(sommet, g);

    // Mettre à jour le graph
    d_graphViewDijkstra->graphChanged(T);

}
