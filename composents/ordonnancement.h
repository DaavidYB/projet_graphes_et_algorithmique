#ifndef ORDONNANCEMENT_H
#define ORDONNANCEMENT_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include "graph/graph.h"
#include "composents/graphview.h"

class ordonnancement : public QWidget
{
    Q_OBJECT

public:
    explicit ordonnancement(QWidget *parent = nullptr);
    ~ordonnancement();

private:
    QLabel* d_title;
    QHBoxLayout* d_layoutNbTasks;
    QLabel* d_labelNbTasks;
    QLineEdit* d_editNbTasks;

    QHBoxLayout* d_layoutGraph;

    QVBoxLayout* d_layoutTasks;
    QVector<QLineEdit*> d_lignesNom;
    QVector<QLineEdit*> d_lignesDuree;
    QVector<QLineEdit*> d_lignesPredecesseurs;
    QVector<QLabel*> d_labelsTaches;

    QLabel* d_resultsLabel;

    QPushButton* d_validateButton;


    graphView* graph;


    void createInterface();
    void createLines(int nbLines);
    bool validationInput(std::vector<graphalgo::Tache>& taches);
    void effacerLignes();
    void effaceAffichage();
    graphalgo::graph buildGraph(const std::vector<graphalgo::Tache>& taches);
    void updateResults(const std::vector<graphalgo::Tache>& taches, graphalgo::graph &g, const std::vector<int>& longueurCritique, const std::vector<graphalgo::Tache>& cheminsCritique);
private slots:
    void onNbChange();
    void onValidate();

};

#endif // ORDONNANCEMENT_H
