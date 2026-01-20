#ifndef sudoku_hpp
#define sudoku_hpp
#include <iostream>
#include <vector>
#include <complex>
using namespace std;

//===========================================================
// classe Grille ============================================
//===========================================================

typedef unsigned int suint;
class Grille : vector<vector<int>>
{public :
    int n = 0; // ordre de la grille
    mutable vector<pair<suint,suint>> casesVides; // vecteur des cases vides
    mutable vector<suint> vs,vl,vc,vb; // vecteurs de travail
};

//===========================================================
// classe Sudoku ============================================
//===========================================================
class Sudoku 
{
    public :
    int ordre = 0; // ordre du Sudoku
    int nbcase = 0; // nombre de cases à initialiser
    bool allSol = false; // recherche de toutes les solutions si true 
    bool uniqueSol = false; // pour le test d'unicité
    Grille grill_ini ; // grille initiale
    list<Grille> grille_sol; // liste des grilles solutions
    void affiche_sudoku(); // affichage du Sudoku
};
Sudoku Solution(int n); // Recherches de solutions du Sudoku, n représente l'indice d'une case vide

#endif 

//test commit