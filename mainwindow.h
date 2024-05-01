#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graph/graph.h"
#include <QMainWindow>
#include <QButtonGroup>
#include "composents/graphview.h"

class QComboBox;
class QPushButton;

/**
 * @brief Fenêtre principale de l'application
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    bool saisieGroupVisible = false, fichierGroupVisible = false;
    /**
     * @brief Le graph courant qui s'affiche
     */
    graphalgo::graph d_graph{
        {31, 2, 3, 0, 4, 9, 0, 4, 6, 8, 0, 8, 9, 11, 0, 3, 6, 0, 7, 8, 0, 8, 0, 10, 0, 0, 11, 0, 10, 12, 0, 0},
        {12, 1, 4, 7, 11, 15, 18, 21, 23, 25, 26, 28, 31}
    };

    /**
     * @brief La liste des algorithmes
     */
    QComboBox *d_listeAlgorithmes, *d_listeSaisie;
    /**
     * @brief Le bouton lançant l'algorithme sélectionné
     */
    QPushButton *d_buttonLancerAlgo;
    /**
     * @brief Pointeur vers le widget de la fenêtre de saisie en cours
     */
    QWidget* d_currentInputWindow = nullptr;

    /**
     * @brief Méthode privée générant les composants de la mainWindow
     */
    void createInterface();
    /**
     * @brief QWidget affichant la représentation graphique de d_graph
     */
    graphView *d_graphview;

    /**
     * @brief Méthode privée chargeant le graph par défaut
     */
    void loadGraph();
    /**
     * @brief Méthode privée permettant d'adapter la sélectioner d'algorithmes (d_listeAlgorithmes) au graph courant (d_graph)
     */
    void setOptions() const;

private slots:
    /**
     * @brief Méthode gérant la réception des signaux, à la suite de la saisie d'un graphe
     * @param g - le graphe reçu
     */
    void onGrapheReceived(const graphalgo::graph& g);

    /**
     * @brief Méthode privée réagissant au clic du bouton d_buttonLancerAlgo et permettant l'exécution de l'algorithme choisi
     */
    void onExecAlgo();

    /**
     * @brief Méthode privée réagissant au clic du bouton buttoninfo et permetttant l'affichage des explications des algorithmes
     */
    void onInfo();
    /**
     * @brief Méthode privée réagissant au clic du bouton buttonSaisie et permettant la saisie textuelle d'un graph
     */
    void onSaisie();

    /**
     * @brief Méthoed privée réagissant au clic du bouton bSaisieFsAps
     */
    void onSaisieFsAps();
    /**
     * @brief Méthoed privée réagissant au clic du bouton bSaisieMatriceAdj
     */
    void onSaisieMatAdj();
    /**
     * @brief Méthoed privée réagissant au clic du bouton bSaisieListeSommets
     */
    void onSaisieListeSommets();

    /**
     * @brief Méthode privée réagissant au clic du bouton buttonFichier et permettant le téléchargement d'un graphe
     */
    void onTelecharge();
    /**
     * @brief Méthode privée réagissant au clic du bouton buttonSauvegarder et permettant la sauvegarde d'un graphe
     */
    void onSauvegarde();
};
#endif // MAINWINDOW_H
