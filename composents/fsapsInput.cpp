#include "fsapsInput.h"
#include <QStringList>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

fsapsInput::fsapsInput(QWidget *parent)
    : QWidget(parent)
{
    createInterface();
}

fsapsInput::~fsapsInput() {
    delete d_inputFS;
    delete d_inputAPS;
}

void fsapsInput::createInterface()
{
    // Création du layout principal
    auto mainLayout = new QVBoxLayout(this);

    // Titre de la fenêtre
    auto title = new QLabel("Saisie d’un graph selon des tableaux fs-aps");
    mainLayout->addWidget(title);

    // Ajout d'une ligne de séparation
    auto separationLine {new QFrame{}};
    separationLine->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    mainLayout->addWidget(separationLine);

    // On crée les inputs
    d_inputFS = new QLineEdit{};
    d_inputAPS = new QLineEdit{};
    // On crée les labels
    auto labelFS {new QLabel{"Saisissez le tableau FS :"}};
    auto labelAPS {new QLabel{"Saisissez le tableau APS :"}};

    // On les ajoute au layout
    mainLayout->addWidget(labelFS);
    mainLayout->addWidget(d_inputFS);
    mainLayout->addWidget(labelAPS);
    mainLayout->addWidget(d_inputAPS);

    // Ajout d'un espacement vertical
    mainLayout->addSpacing(10);

    // On ajoute le bouton Valider
    auto validateButton = new QPushButton("Valider");
    auto buttonLayout = new QHBoxLayout{};
    buttonLayout->addStretch();
    buttonLayout->addWidget(validateButton);
    mainLayout->addLayout(buttonLayout);
    // On le connecte
    connect(validateButton, &QPushButton::clicked, this, &fsapsInput::onValidate);
}

void fsapsInput::onValidate()
{
    std::vector<int> fs;
    std::vector<int> aps;

    // Vérifier la saisie des tableaux FS et APS
    if (!validationFsAps(d_inputFS->text(), d_inputAPS->text(), fs, aps)) {
        return; // Sortir de la méthode si la saisie est invalide
    }

    // Construire le graphe à partir de FS et APS
    graphalgo::graph g{fs, aps};

    emit graphe(g);
}

bool validationFsAps(const QString& saisieFS, const QString& saisieAPS, std::vector<int>& fs, std::vector<int>& aps) {
    // Vérifier la saisie du tableau FS
    QStringList nombresFS = saisieFS.split(" ", Qt::SkipEmptyParts);
    if (nombresFS.isEmpty()) {
        QMessageBox::critical(nullptr, "Erreur de saisie", "Le tableau FS est vide.");
        return false;
    }

    for(const QString& valeur : nombresFS) {
        bool ok;
        int nombre = valeur.toInt(&ok);
        if(!ok || nombre < 0 || nombre > INT_MAX) {
            QMessageBox::critical(nullptr, "Erreur de saisie", "Le tableau FS contient une valeur invalide.");
            return false;
        }
        fs.push_back(nombre);
    }

    // Vérifier la saisie du tableau APS
    QStringList nombresAPS = saisieAPS.split(" ", Qt::SkipEmptyParts);
    if (nombresAPS.isEmpty()) {
        QMessageBox::critical(nullptr, "Erreur de saisie", "Le tableau APS est vide.");
        return false;
    }

    bool ok;
    int premierAPS = nombresAPS[0].toInt(&ok);
    if(!ok) {
        QMessageBox::critical(nullptr, "Erreur de saisie", "Le tableau APS contient une valeur invalide.");
        return false;
    }
    aps.push_back(premierAPS);

    int dernierAPS = 0;
    for(int i = 1; i < nombresAPS.size(); i++) {
        int nombre = nombresAPS[i].toInt(&ok);
        if(!ok || nombre < 1 || nombre > INT_MAX || nombre <= dernierAPS || nombre > fs.size()) {
            QMessageBox::critical(nullptr, "Erreur de saisie", "Le tableau APS contient une valeur invalide.");
            return false;
        }
        aps.push_back(nombre);
        dernierAPS = nombre;
    }

    return true;
}
