#include "graphview.h"
#include "../graph/algorithmes.h"
#include "../mainwindow.h"

// CONSTRUCTEURS

graphNode::graphNode(int index, QPoint coordonnees)
    : d_index{index}, d_coordonnee{coordonnees}
{
}

// GETTERS

int graphNode::indice() const
{
    return d_index;
}

QPoint graphNode::coordonnees() const
{
    return d_coordonnee;
}

// CONSTRUCTEURS

graphView::graphView(graphalgo::graph &g, QWidget *parent)
    : QWidget{parent}
{
    g.fs_aps(fs, aps);
}

graphView::~graphView()
{
}

// CRÉATION DE L'INTERFACE

QSize graphView::sizeHint() const
{
    return {600, 800};
}

void graphView::paintEvent(QPaintEvent *)
{
    QPainter painter{this};
    // painter.setRenderHint(QPainter::Antialiasing);
    // painter.fillRect(0, 0, width(), height(), Qt::white);
    dessineGraph(painter);
}

// MÉTHODE BACK-END

graphNode graphView::find(int index) const
{
    // On initialise les variables tampons
    bool estTrouve = false;
    unsigned i = 0;
    // On déroule la liste
    while(!estTrouve && i < d_listeNoeuds.size()) {
        // On compare
        if(d_listeNoeuds[i].indice() == index)
            estTrouve = true;
        // On incrémente
        else i++;
    }
    // On retourne le Noeud
    return d_listeNoeuds[i];
}

// MÉTHODES DE DESSIN

void graphView::dessineNoeud(QPainter &painter, const graphNode &node)
{
    // Dessiner le cercle en noir
    painter.setBrush(Qt::black);
    painter.drawEllipse(node.coordonnees(), 10, 10);

    // Dessiner le numéro du nœud en blanc
    painter.setPen(Qt::white);
    // On centre le numéro de l'item
    painter.drawText(node.coordonnees() - QPoint(node.indice() > 9 ? 7 : 4, -5), QString::number(node.indice()));
}

void graphView::dessineArete(QPainter &painter, const graphNode &node1,
                             const graphNode &node2, int cout, bool oriented)
{
    // Calcul des vecteurs directeurs des arêtes
    QPoint direction = node2.coordonnees() - node1.coordonnees();
    double distance = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());
    direction /= distance;

    // Calcul des points de départ et d'arrivée sur le bord des cercles
    QPoint depart = node1.coordonnees() + direction * 20;
    QPoint arrivee = node2.coordonnees() - direction * 20;

    // On trace la ligne en noir
    painter.setPen(Qt::black);
    painter.drawLine(depart, arrivee);


    // On teste si le cout est renseigné
    if(cout != __INT_MAX__)
    {
        // On calcule les coordonnées du cout et on le dessine
        QPoint milieu {(depart + arrivee) / 2};
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

std::vector<graphNode> graphView::calculePositions(const std::vector<int> &rang)
{
    // Vector résultat contenant les coordonnées des Noeuds
    std::vector<graphNode> listeNoeuds;
    // On récupère le nombre de rang
    int nbRang = *std::max_element(rang.begin() + 1, rang.end()) + 1;
    // On calcule le gap entre les Noeuds
    int largeur = (width() - 40) / (nbRang + 2);

    vector<int> hauteurs(nbRang + 2, 0);
    // On compte le nombre d'items de chaque rang
    for(int i = 1; i <= rang[0]; i++)
        if(rang[i] >= 0) {
            hauteurs[rang[i]]++;
        } else {
            hauteurs[nbRang + 1]++;
        }
    // On déduit le gap entre chaque groupe d'items (selon leur rang)
    for(unsigned i = 0; i < hauteurs.size(); i++)
        hauteurs[i] = height() / (hauteurs[i] + 1);

    // On alloue un compteur d'items pour chaque rang
    vector<int> nbItemsRang(nbRang + 2, 0);
    for(int i = 1; i <= rang[0]; i++) {
        // On récupère le rang
        int r = rang[i];
        if(r >= 0){
            nbItemsRang[r]++;
            // On consrtuit le Noeud
            QPoint p{(r + 1) * largeur, (nbItemsRang[r]) * hauteurs[r]};
            graphNode node{static_cast<int>(i), p};
            listeNoeuds.push_back(node);
        } else {
            nbItemsRang[nbRang + 1]++;
            QPoint p{(nbRang + 1) * largeur, (nbItemsRang[nbRang + 1]) * hauteurs[nbRang + 1]};
            graphNode node{static_cast<int>(i), p};
            listeNoeuds.push_back(node);
        }
    }
    return listeNoeuds;
}

void graphView::dessineGraph(QPainter &painter)
{
    // On récupère le rang des sommets
    vector<int> rang = graphalgo::rang(fs, aps);

    // Récupérer les positions des nœuds
    d_listeNoeuds = calculePositions(rang);

    // On dessine les nœuds
    for(const auto item : d_listeNoeuds)
        dessineNoeud(painter, item);

    // On récupére les arêtes du graphe
    const graphalgo::graph g{fs, aps};
    std::vector<graphalgo::vtx> vtxs = g.vertexes();

    // On dessine les arêtes
    for (const auto& v : vtxs) {
        int s = v.s - 1;
        int t = v.t - 1;
        dessineArete(painter, d_listeNoeuds[s], d_listeNoeuds[t], v.p, g.oriented());
    }
}

void graphView::graphChanged(graphalgo::graph &g)
{
    // On libère la mémoire allouée
    d_listeNoeuds.clear();

    // On libère les tableaux de données et on implémente les nouvelles données
    fs.clear();
    aps.clear();
    g.fs_aps(fs, aps);
    // On redessine le QWidget
    update();
}
