#ifndef OUTPUTALGO_H
#define OUTPUTALGO_H

#include <QObject>
#include <QWidget>
#include <QBoxLayout>
#include <QComboBox>
#include <QLabel>
#include "composents/graphview.h"
#include "graph/graph.h"

/**
 * @brief QWidget permettant l'affichage des résultats des algorithmes vus en cours appliqués au graph courant
 */
class outputAlgo :  public QWidget
{
    Q_OBJECT
public:
    explicit outputAlgo(int indiceOutput, graphalgo::graph &g, QWidget *parent = nullptr);
    ~outputAlgo();

private slots:
    /**
     * @brief Méthode privée permettant de lancer l'algorithme des distances sur le graph saisi
     * @param g Le graph
     */
    void calculerDistances(graphalgo::graph &g);
    /**
     * @brief Méthode privée permettant de lancer l'algorithme de dijkstra sur le graph saisi
     * @param g
     */
    void calculerDijkstra(graphalgo::graph& g);
private:
    QComboBox* d_comboBoxDistance;
    QLabel* d_resultatsDistance;

    QComboBox* d_comboBoxDijkstra;
    graphView* d_graphViewDijkstra;

    /**
     * @brief Méthode privée permettant de construire le QWidget afin d'afficher les résultats de l'algorithme des distances
     * @param g Le graph sur lequel appliqué l'algorithme
     */
    void distance(graphalgo::graph &g);
    /**
     * @brief Méthode privée permettant de construire le QWidget afin d'afficher les résultats de l'algorithme du rang
     * @param g Le graph sur lequel appliqué l'algorithme
     */
    void rang(graphalgo::graph &g);
    /**
     * @brief Méthode privée permettant de construire le QWidget afin d'afficher les résultats de l'algorithme de tarjan
     * @param g Le graph sur lequel appliqué l'algorithme
     */
    void tarjan(graphalgo::graph &g);
    /**
     * @brief Méthode privée permettant de construire le QWidget afin d'afficher les résultats de l'algorithme d'ordonnancement
     * @param g Le graph sur lequel appliqué l'algorithme
     */
    void ordonnancement(graphalgo::graph &g);
    /**
     * @brief Méthode privée permettant de construire le QWidget afin d'afficher les résultats de l'algorithme de dijkstra
     * @param g Le graph sur lequel appliqué l'algorithme
     */
    void dijkstra(graphalgo::graph &g);
    /**
     * @brief Méthode privée permettant de construire le QWidget afin d'afficher les résultats de l'algorithme de dantzig
     * @param g Le graph sur lequel appliqué l'algorithme
     */
    void dantzig(graphalgo::graph &g);
    /**
     * @brief Méthode privée permettant de construire le QWidget afin d'afficher les résultats de l'algorithme de kruskal
     * @param g Le graph sur lequel appliqué l'algorithme
     */
    void kruskal(graphalgo::graph &g);
    /**
     * @brief Méthode privée permettant de construire le QWidget afin d'afficher les résultats de l'algorithme de prufer
     * @param g Le graph sur lequel appliqué l'algorithme
     */
    void prufer(graphalgo::graph& g);
};

#endif // OUTPUTALGO_H
