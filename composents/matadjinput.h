#ifndef MATADJINPUT_H
#define MATADJINPUT_H

#include "../graph/graph.h"
#include <vector>
#include <QObject>
#include <QDialog>

class QTableWidget;
class QCheckBox;

class matAdjInput : public QWidget
{
    Q_OBJECT
public:
    explicit matAdjInput(QWidget *parent = nullptr);
    ~matAdjInput();

private :
    /**
     * @brief Composant indiquant si le graph est orienté ou non
     */
    QCheckBox *d_radioButtonOriente;
    /**
     * @brief Composant contenant la matrice à remplir
     */
    QTableWidget *d_matrice;

    /**
     * @brief Méthode privée générant les composants de l'interface
     */
    void createInterface();
    /**
     * @brief Méthode privée générant les case de la matrice
     * @param i Le nombre de sommets compris dans la matrice
     */
    void createMatrice(unsigned i);
    /**
     * @brief méthode privée vidant la matrice
     */
    void deleteMatrice();

    /**
     * @brief matrticeValide
     * @return TRUE - la matrice est valide | FALSE - la matrice n'est pas valide
     */
    bool recupereValide(std::vector<std::vector<int>> &matAdj) const;

    /**
     * @brief Méthode privé générant le graph
     */
    void createGraph();

private slots :
    /**
     * @brief Méthode privée générant la matrice d'adjacence à compléter
     */
    void onNbSommets(unsigned i);

signals:
    /**
     * @brief Signal privé émettant le nouveau graph
     * @param g Le graph saisi
     */
    void graphe(const graphalgo::graph &g);
};

#endif // MATADJINPUT_H
