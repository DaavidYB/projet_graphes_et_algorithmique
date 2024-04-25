#include "graphview.h"

// CONSTRUCTEURS

graphView::graphView(QWidget *parent)
    : QWidget{parent}
{
    //
}

graphView::~graphView()
{
    // delete les pointeurs
}

// CRÉATION DE L'INTERFACE

QSize graphView::sizeHint() const
{
    return {800, 600};
}


// !! MÉTHODE TEMPORAIRE À SUPPRIMER !! //
void graphView::paintEvent(QPaintEvent *)
{
    QPainter painter{this};
    painter.setRenderHint(QPainter::Antialiasing);


    QPoint p1{200, 200}, p2{400, 300};
    dessineNoeud(painter, p1, 3);
    dessineNoeud(painter, p2, 12);
    dessineArete(painter, p1, p2, 1, true);
}

// MÉTHODES DE DESSIN

void graphView::dessineNoeud(QPainter &painter, const QPoint &p, int index)
{
    // Dessiner le cercle en noir
    painter.setBrush(Qt::black);
    painter.drawEllipse(p, 20, 20);

    // Dessiner le numéro du nœud en blanc
    painter.setPen(Qt::white);
    painter.drawText(p - QPoint(index > 9 ? 7 : 4, -5), QString::number(index));
}

void graphView::dessineArete(QPainter &painter, const QPoint &noeud1, const QPoint &noeud2, int cout, bool oriented)
{
    // Calcul des vecteurs directeurs des arêtes
    QPoint direction = noeud2 - noeud1;
    double distance = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());
    direction /= distance;

    // Calcul des points de départ et d'arrivée sur le bord des cercles
    QPoint depart = noeud1 + direction * 20;
    QPoint arrivee = noeud2 - direction * 20;

    // On trace la ligne en noir
    painter.setPen(Qt::black);
    painter.drawLine(depart, arrivee);


    // On teste si le cout est renseigné
    if(cout != __INT_MAX__)
    {
        // On calcule les coordonnées du cout
        QPoint milieu {static_cast<int>((depart.x() + arrivee.x()) / 2.0),
                      static_cast<int>((depart.y() + arrivee.y()) / 2.0)};
        // On dessine le cout
        painter.drawText(milieu, QString::number(cout));
    }

    if(oriented)
    {
        // Calcul de la direction de la flèche
        QPoint direction = arrivee - depart;
        double angle = atan2(direction.y(), direction.x());

        // Dessin de la flèche
        painter.save();
        painter.translate(arrivee);
        painter.rotate(angle * 180.0 / M_PI);
        painter.drawLine(0, 0, -6, -6);
        painter.drawLine(0, 0, -6, 6);
        painter.restore();
    }
}


void graphView::dessineGraph(const graphalgo::graph &g)
{
    // On déclare notre peintre
    QPainter painter{this};
    // On active l'anticrénelage
    painter.setRenderHint(QPainter::Antialiasing);
}

void graphView::effaceGraph()
{

}
