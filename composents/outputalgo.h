#ifndef OUTPUTALGO_H
#define OUTPUTALGO_H

#include <QObject>
#include <QWidget>
#include <QBoxLayout>
#include <QComboBox>
#include <QLabel>
#include "composents/graphview.h"
#include "graph/graph.h"

class outputAlgo :  public QWidget
{
    Q_OBJECT
public:
    explicit outputAlgo(int indiceOutput, graphalgo::graph &g, QWidget *parent = nullptr);
    ~outputAlgo();
private slots:
    void calculerDistances(graphalgo::graph &g);
    void calculerDijkstra(graphalgo::graph& g);
private:
    QComboBox* d_comboBoxDistance;
    QLabel* d_resultatsDistance;

    QComboBox* d_comboBoxDijkstra;
    graphView* d_graphViewDijkstra;

    void distance(graphalgo::graph &g);
    void rang(graphalgo::graph &g);
    void dijkstra(graphalgo::graph &g);
    void dantzig();
    void kruskal(graphalgo::graph &g);
    void prufer(graphalgo::graph& g);
};

#endif // OUTPUTALGO_H
