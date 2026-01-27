#include "sudoku.hpp"
using namespace std;

int main(){
    Grille Test(2);
    Test.importer("grille1.txt");
    Test.afficher();
    return 0;
}