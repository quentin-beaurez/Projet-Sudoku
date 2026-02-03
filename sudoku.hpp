#ifndef sudoku_hpp
#define sudoku_hpp
#include <iostream>
#include <vector>
#include <complex>
#include <algorithm> //nouveau !
#include <cmath> //nouveau !
#include <random> //nouveau !
#include <fstream>
#include <list>

using namespace std;

//===========================================================
// classe Grille ============================================
//===========================================================

typedef unsigned int suint;
class Grille : public vector<vector<suint>>
{public :
    int n = 0; // ordre de la grille
    mutable vector<pair<suint,suint>> casesVides; // vecteur des cases vides
    mutable vector<suint> vs,vl,vc,vb; // vecteurs de travail
    Grille(); // constructeur par défaut
    Grille(int ordre); //constructeur pour n choisi
    void generation(float densite = 0.3); // fonction génératrice d'une grille 
    void afficher(); // fonction d'affichage d'une grille
    void importer(const string& nomFichier = "grille.txt"); // fonction  pour importer une grille d'un fichier
    void exporter(const string& nomFichier = "grille.txt"); // fonction  pour exporter une grille dans un fichier
    void majcasesVides(); // fonction qui génère une liste avec les cases vides
    bool estPresent(const vector<suint>& liste, suint valeur); //détermine si 'valeur' est présente dans la liste 'liste'
    vector<suint> listeadmissibles(pair<suint,suint> coord); // fonction qui donne les valeurs admissibles pour une case vide
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
    Grille grille_ini ; // grille initiale
    list<Grille> grille_sol; // liste des grilles solutions
    void affiche_sudoku(); // affichage du Sudoku

    // Constructeurs 
    Sudoku(int n); // Constructeur par défaut avec l'ordre 
    Sudoku(const Grille& g); // Construction à partir de la classe Grille 

    bool Solution(int n); // Recherches de solutions du Sudoku, n représente l'indice d'une case vide
};

#endif 

//test commit