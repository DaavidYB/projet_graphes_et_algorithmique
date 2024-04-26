#ifndef OUTPUTALGO_H
#define OUTPUTALGO_H

#include <QObject>
#include <QWidget>
#include <QBoxLayout>
#include <QComboBox>
#include <QLabel>
#include "graph/graph.h"

class outputAlgo :  public QWidget
{
    Q_OBJECT
public:
    explicit outputAlgo(int indiceOutput, graphalgo::graph &g, QWidget *parent = nullptr);
    ~outputAlgo();
private slots:
    void calculerDistances(graphalgo::graph &g);
private:
    // QVBoxLayout* d_layoutDistance;
    QComboBox* d_comboBoxDistance;
    QLabel* d_resultatsDistance;

    void distance(graphalgo::graph &g);
    void rang(graphalgo::graph &g);
    void dijkstra();
    void dantzig();
    void kruskal();
    void prufer();
};

#endif // OUTPUTALGO_H
