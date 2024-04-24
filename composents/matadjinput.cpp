#include "matadjinput.h"
#include <QBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QTableWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include <QFrame>

// CONSTRUCTEURS

matAdjInput::matAdjInput(QWidget *parent)
    : QWidget{parent}
{
    createInterface();
}

matAdjInput::~matAdjInput()
{
    // delete les pointeurs
    delete d_radioButtonOriente;
    delete d_matrice;
}

// CRÉATION DE L'INTERFACE

void matAdjInput::createInterface()
{
    // On génère le layout principal
    auto mainLayout {new QVBoxLayout{this}};

    // On ajoute le titre
    auto title {new QLabel{"Saisie d'un graph selon une matrice d'adjacence"}};
    mainLayout->addWidget(title);

    // On ajoute la ligne de séparation
    auto titleLigne {new QFrame{this}};
    titleLigne->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    mainLayout->addWidget(titleLigne);

    // On génère un QHBoxLayout pour les données de pré-saisie
    auto preSaisieLayout {new QHBoxLayout{this}};
    mainLayout->addLayout(preSaisieLayout);

    // On génère les inputs et bales de saisie
    auto lNbSommets {new QLabel{"Saisir le nombre de sommets : "}};
    preSaisieLayout->addWidget(lNbSommets);
    auto inputnbSommets{new QLineEdit{}};
    preSaisieLayout->addWidget(inputnbSommets);
    auto lOrientation {new QLabel{"Orienté : "}};
    preSaisieLayout->addWidget(lOrientation);
    d_radioButtonOriente  = new QCheckBox{};
    preSaisieLayout->addWidget(d_radioButtonOriente);

    // On génère le layout horizonthal contenant la matrice et le bouton de validation
    auto horizonthalLayout {new QHBoxLayout{}};
    mainLayout->addLayout(horizonthalLayout);

    // On génère la matrice
    d_matrice = new QTableWidget{};
    horizonthalLayout->addWidget(d_matrice);

    // On génère le layout contenant le bouton de validation
    auto valideLayout {new QVBoxLayout};
    horizonthalLayout->addLayout(valideLayout);

    // On génère le bouton de validation
    auto bValide {new QPushButton{"Valider"}};
    valideLayout->addStretch(1);
    valideLayout->addWidget(bValide);

    connect(inputnbSommets, &QLineEdit::textChanged, [=](const QString &text) {
        bool ok;
        unsigned value = text.toUInt(&ok);

        if (ok) onNbSommets(value);
    });
    connect(bValide, &QPushButton::clicked, this, &matAdjInput::createGraph);
}

void matAdjInput::createMatrice(unsigned i)
{
    // On alloue les lignes et colonnes
    d_matrice->setColumnCount(i);
    d_matrice->setRowCount(i);

    // On génère les cases
    for(unsigned lignes = 0; lignes < i; lignes++){
        for(unsigned colonnes = 0; colonnes < i; colonnes++){
            // On génère la case
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText("0");
            // On la rend éditable
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            // On l'ajoute dans la matrice
            d_matrice->setItem(lignes, colonnes, item);
        }
    }
}

void matAdjInput::deleteMatrice()
{
    d_matrice->clear();
}

bool matAdjInput::recupereValide(std::vector<std::vector<int>> &matAdj) const
{
    // On déclare les variables tampons
    bool valide = true;
    int nbLignes = d_matrice->rowCount();

    // On alloue le tableau
    matAdj.clear();
    matAdj.resize(nbLignes, std::vector<int>(nbLignes));

    // On fait défiler les lignes
    int i = 0;
    while(valide && i < nbLignes){
        // On fait défiler les colonnes
        int j = 0;
        while(valide && j < nbLignes){
            bool isNumber;
            // On récupère la valeur de la case
            int number = d_matrice->item(i, j)->text().toInt(&isNumber);
            if (!isNumber || number < 0)
                valide = false;
            else
                matAdj[i][j] = number;
            j++;
        }
        i++;
    }
    // On retourne la validation
    return valide;
}

// !! Modifier pour orienter le graph !!
void matAdjInput::createGraph()
{
    // générér le graph selon un tableau 2D d'entier
    std::vector<std::vector<int>> matAdj;
    if(recupereValide(matAdj)){
        // On construit le graph
        graphalgo::graph *g;
        // Graph orienté
        if(d_radioButtonOriente->isChecked())
            g = new graphalgo::graph{matAdj};
        //Graph non-orienté
        else
            g = new graphalgo::graph{matAdj};

        // On retourne le graph
        graphe(*g);
        this->close();
    } else {
        QMessageBox::critical(nullptr, "Erreur de saisie", "La matrice d'adjacence est invalide.");
    }
}

// MÉTHODES ONCLIC

void matAdjInput::onNbSommets(unsigned i)
{
    deleteMatrice();
    createMatrice(i);
}


















