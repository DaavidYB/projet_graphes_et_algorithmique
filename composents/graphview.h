#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include "../graph/graph.h"
#include <vector>
#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPoint>

/**
 * @brief QWidget affaichant la représentation graphique d'un graphalgo::graph
 */
class graphView : public QWidget
{
    Q_OBJECT
public:
    explicit graphView(QWidget *parent = nullptr);
    ~graphView();

    /**
     * @brief Redéfinition de la méthode sizeHint - On redimensionne le QWidget par défaut
     * @return
     */
    QSize sizeHint() const override;

private :
    /**
     * @brief Structure privé stockant les coordonnées des points formant une arete ainsi que le coût de cette denière
     */
    struct arete{
        /**
         * @brief Les points représentant les Noeuds du graph
         */
        QPoint *p1, *p2;
        /**
         * @brief Le coût de l'arete
         */
        int cout;
    };

    /**
     * @brief La liste des Noeuds du graph
     */
    std::vector<QPoint*> listeNoeuds;
    /**
     * @brief La liste des aretes du graph
     */
    std::vector<arete*> listeAretes;

private :
    /**
     * @brief Méthode dessinant le composant
     */
    void paintEvent(QPaintEvent *) override;


    // Calcul des positions

    /**
     * @brief Méthode privée traçant un noeud
     * @param p Les coordonnées du Noeud
     * @param index Le numéro du Noeud
     */
    void dessineNoeud(QPainter &painter, const QPoint &p, int index);
    /**
     * @brief dessineArete Méthode privée reliant deux noeuds dans l'interface
     * @param noeud1 Le premier Noeud
     * @param noeud2 Le second
     * @param oriented Booléen indiquant si le graph est orienté
     */
    void dessineArete(QPainter &painter, const QPoint &noeud1, const QPoint &noeud2, int cout, bool oriented = false);

    /**
     * @brief Méthode générant la représentation graphique d'un graph
     * @param g Le graph à illustrer
     */
    void dessineGraph(const graphalgo::graph &g);
    /**
     * @brief Méthode effaçant les QPoint représentant le graph
     */
    void effaceGraph();
};

#endif // GRAPHVIEW_H
