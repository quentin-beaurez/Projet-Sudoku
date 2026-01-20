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

class Sudoku
{

};

#endif 


//test commit