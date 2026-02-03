#include "sudoku.hpp"
using namespace std;

int main(){
    //test affichage et importation de grille
    Grille Test(2);
    Test.importer("grille1.txt");
    Test.afficher();

    //test generation de grille
    Grille gr(3);
    gr.generation();
    gr.afficher();
    return 0;
}