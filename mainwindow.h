#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graph/graph.h"
#include <QMainWindow>
#include <QButtonGroup>

class QComboBox;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private :
    bool saisieGroupVisible = false, fichierGroupVisible = false;
    /**
     * @brief Le graph courant qui s'affiche
     */
    graphalgo::graph d_graph;
    // QWidget perso -> affiche le graph
    /**
     * @brief La liste des algorithmes
     */
    QComboBox *d_listeAlgorithmes;
    /**
     * @brief Le bouton lançant l'algorithme sélectionné
     */
    QPushButton *d_buttonLancerAlgo;
    /**
     * @brief Group de boutons regroupant les différentes options de saisies textuelles / par fichiers
     */
    QButtonGroup *d_saisieGroup, *fichierGroup;


    /**
     * @brief Méthode privée générant les composants de la mainWindow
     */
    void createInterface();
    /**
     * @brief Méthode privée montrant les sous-options de saisies textuelles
     */
    void showSaisieGroup();
    /**
     * @brief Méthode privée montrant les sous-options de saisies par fichiers
     */
    void showFichierGroup();
    /**
     * @brief Méthode privée cachant les sous-options de saisies textuelles
     */
    void hideSaisieGroup();
    /**
     * @brief Méthode privée cachant les sous-options de saisies par fichiers
     */
    void hideFichierGroup();

private slots :
    /**
     * @brief Signal privé réagissant au clic du bouton buttonSaisie
     */
    void onSaisie();
    /**
     * @brief Signal privé réagissant au clic du bouton buttonFichier
     */
    void onFichier();
};
#endif // MAINWINDOW_H



























