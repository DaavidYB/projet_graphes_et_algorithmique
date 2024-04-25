#ifndef FSAPSINPUT_H
#define FSAPSINPUT_H

#include <QWidget>
#include <QLineEdit>
#include "graph/graph.h"

class fsapsInput : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe fsapsInput
     * @param parent Le widget parent
     */
    explicit fsapsInput(QWidget *parent = nullptr);
    /**
     * @brief Destructeur de la classe fsapsInput
     */
    ~fsapsInput();

private slots:
    /**
     * @brief Slot appelé lorsque l'utilisateur clique sur le bouton "Valider"
     */
    void onValidate();

signals:
    /**
     * @brief Signal émis lorsque le graphe a été construit à partir des tableaux FS et APS
     * @param g Le graphe construit
     */
    void graphe(const graphalgo::graph& g);

private:
    /**
     * @brief Pointeurs vers les champs de saisie des tableaux FS et APS
     */
    QLineEdit *d_inputFS, *d_inputAPS;
    /**
     * @brief Méthode privée pour créer l'interface graphique
     */
    void createInterface();
};

/**
     * @brief Méthode pour valider la saisie des tableaux FS et APS
     * @param saisieFS La saisie du tableau FS
     * @param saisieAPS La saisie du tableau APS
     * @param fs Le vecteur qui contiendra les valeurs du tableau FS
     * @param aps Le vecteur qui contiendra les valeurs du tableau APS
     * @return true si la saisie est valide, false sinon
     */
bool validationFsAps(const QString& saisieFS, const QString& saisieAPS, std::vector<int>& fs, std::vector<int>& aps);

#endif // FSAPSINPUT_H
