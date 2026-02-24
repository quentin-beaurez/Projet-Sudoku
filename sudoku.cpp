#include "sudoku.hpp"
#include <iostream>
#include <vector>
#include <complex>
#include <algorithm> 
#include <cmath> 
#include <random> 
#include <list>
#include <fstream>


using namespace std;

//===========================================================
// classe Grille ============================================
//===========================================================

//constructeur d'une grille d'ordre quelconque (grille remplie de zéros <=> vide)
Grille::Grille(int ordre){
    this->n = ordre; //on fixe l'ordre de la grille
    int taille_cote = n*n; //on calcule les dimensions de la grille. ex : n = 3 donne une grille de 9x9
    this->resize(taille_cote); //on resize pour avoir le bon nombre de lignes
    
    //pour chaque ligne de 'this' on la resize pour avoir le bon nombre de colonnes et on
    //les rempli de 0 (pour avoir une grille vide)
    for(int i=0; i<taille_cote; i++){ 
        (*this)[i].resize(taille_cote,0);
    }
}

//constructeur de grille par défaut : ordre 3 (appelle le constructeur ci-dessus)
Grille::Grille() : Grille(3){}

// Détermine si une valeur est présente dans une liste //
bool Grille::estPresent(const vector<suint>& liste, suint valeur) {
    return find(liste.begin(), liste.end(), valeur) != liste.end();
}

// Détermine l'ensemble des valeurs admissibles pour une case dont les coordonnées sont données //
vector<suint> Grille::listeadmissibles(pair<suint,suint> coord){
    suint ligne = coord.first;
    suint col = coord.second;
    suint taille_cote = n*n;
   
    //on détermine le coin haut gauche du bloc dans lequel se trouve la case d'intérêt
    suint k_ligne = (ligne / n) * n; //division entière (tronque la partie décimale) : division entière par défaut car on divise un entier par un entier
    suint k_col = (col / n) * n;
    
    // On crée un tableau de booléens initialisés à 'false'.
    // Taille + 1 pour pouvoir utiliser l'index direct
    vector<bool> est_interdit(taille_cote + 1, false);

    //on parcourt la ligne
    for(int j=0; j<taille_cote; j++){
        suint val = (*this)[ligne][j];
        if (val != 0) est_interdit[val] = true;
    }
    
    //on parcourt la colonne
    for(int i=0; i<taille_cote; i++){
        suint val = (*this)[i][col];
        if (val != 0) est_interdit[val] = true;
    }
    //on parcourt le bloc
    for(int i = k_ligne; i < k_ligne + n; i++){
        for(int j = k_col; j  < k_col + n; j++){
            suint val = (*this)[i][j];
            if (val != 0) est_interdit[val] = true;
        }
    }

    // Gestion des diagonales principales

    if(this->diagonale){
        if(ligne == col) // Diagonale principale : coeffs (0,0), (1,1), ..., (n,n) de la grille
        {
            for(int k = 0; k < taille_cote; k++)
            {
                suint val = (*this)[k][k];
                if (val != 0) est_interdit[val] = true;
            }
        }
        if(ligne + col == taille_cote - 1) // Diagonale secondaire : coeffs (0,n), ..., (n,n^2+1-n) de la grille
        {
            for(int k = 0; k < taille_cote; k++)
            {
                suint val = (*this)[k][taille_cote - 1 - k];
                if (val != 0) est_interdit[val] = true;
            }
        }
    }

    //On récupère les valeurs admissibles
    vector<suint> resultats;
    for(suint val = 1; val <= taille_cote; val++){
        if (est_interdit[val] == false){ // Si pas interdit
            resultats.push_back(val);
        }
    }
    return resultats;
}

//fonction permettant de mettre à jour les cases vides de la grille
void Grille::majcasesVides(){
    vector<pair<suint,suint>> cases;
    int taille_cote = (*this).n * (*this).n;
    for(int i = 0; i< taille_cote; i++){
        for(int j = 0; j< taille_cote; j++){
            if( (*this)[i][j] == 0){
                cases.push_back({i,j});
            }
        }
    }
    (*this).casesVides = cases;
}

// Trie les cases vides pour mettre celles avec le moins d'options en premier
void Grille::optimiserCasesVides() {
    // On utilise une fonction lambda pour trier
    sort(casesVides.begin(), casesVides.end(), 
        [this](const pair<suint,suint>& a, const pair<suint,suint>& b) {
            return listeadmissibles(a).size() < listeadmissibles(b).size();
        }
    );
}

//Génère une grille aléatoire : pas forcément résolvable, avec des valeurs aléatoires dans des positions aléatoires, en densité précisée
void Grille::generation(float densite){
    //Grille grille(ordre); //on construit une grille d'ordre 'ordre' vide à l'aide du constructeur de la classe
    
    //détermination du nombre de cases remplies initial ( = 0)
    int taille_cote = this->n * this->n;
    int nbr_cases = taille_cote*taille_cote; 
    int casesRemplies = 0;

    //détermination du nombre de cases à remplir
    int cases_a_remplir = ceil(densite*nbr_cases);

    //on détermine une ditribution de nombres aléatoires entre 1 et taille_cote pour avoir des coordonnées aléatoires
    //le static permet de pas recréer le moteur aléatoire à chaque appel de la fonction (optionnel mais permet d'aller plus vite si on appel la fonction de nombreuses fois)
    static random_device rd;
    static default_random_engine eng(rd());
    
    //distribution de valeurs des coordonnées
    uniform_int_distribution<int> distrib_coord(0, taille_cote-1);

    //distibution de valeurs aléatoires à rentrer dans la case désignée
    uniform_int_distribution<int> distrib_val(1, taille_cote); //valeur aléatoire à rentrer dans la case désignée
    
    // Par sécurité : permet d'éviter une boucle infinie si la densité est trop haute
    int tentatives_max = cases_a_remplir * 100; 
    int tentatives = 0;

    while(casesRemplies < cases_a_remplir && tentatives < tentatives_max){
        tentatives++;
        int i_ = distrib_coord(eng);
        int j_ = distrib_coord(eng);

        if((*this)[i_][j_] == 0){ //on modifie uniquement si la case est vide pour ne pas faire des changements pour rien
            int val = distrib_val(eng); //valeur à mettre dans la case (i_,j_)
            vector<suint> possibles = (*this).listeadmissibles({(suint)i_, (suint)j_});

            if ((*this).estPresent(possibles, val)) {
                (*this)[i_][j_] = val;
                casesRemplies++;
            }
        }
        
    }
    (*this).majcasesVides(); //on met à jour les cases vides restantes dans la grille
}

// Permet d'importer une grille depuis un fichier texte //
void Grille::importer(const string& nomFichier){
    ifstream fichier(nomFichier);    //ouverture du fichier en lecture

    if (!fichier.is_open()){
        cout<<"Erreur : impossible d'ouvrir le fichier"<<endl;
        return;}

    int taille = n*n;
    
    for (int i = 0;i<taille;i++){
        for(int j =0;j<taille;j++){
        fichier>>(*this)[i][j];
        }
    }
    fichier.close();
    cout<<"Grille importée avec succès depuis "<<nomFichier<<endl;

}

// Permet d'exporter une grille dans un fichier texte //
void Grille::exporter(const string& nomFichier){
    ofstream fichier(nomFichier);    //ouverture du fichier en écriture

    if (!fichier.is_open()){
        cout<<"Erreur : impossible d'ouvrir le fichier"<<endl;
        return;
    }
    int taille = n*n;

    for(int i=0;i<taille;i++){
        for(int j=0;j<taille;j++){
            fichier<<(*this)[i][j]<<" ";
        }
        fichier << endl;
    }
    fichier.close();
    cout<<"Grille exportée avec succès dans "<<nomFichier<<endl;

}

// Permet d'afficher une grille dans un format précis //
void Grille::afficher() {
    int taille = n * n;

    // Caractères Unicode
    const char* gTL = u8"╔"; const char* gTR = u8"╗";
    const char* gBL = u8"╚"; const char* gBR = u8"╝";
    const char* gH  = u8"═"; const char* gV  = u8"║";
    const char* gC  = u8"╬"; const char* gTT = u8"╦";
    const char* gTB = u8"╩"; const char* gTLf = u8"╠";
    const char* gTRg = u8"╣";
    const char* sV = u8"│"; const char* sH = u8"─"; const char* sC = u8"┼";

    // BORDURE DU HAUT 
    cout << gTL;
    for (int b = 0; b < n; b++) {
        for (int k = 0; k < n; k++) {
            cout << gH << gH << gH;     // Largeur de la case
            if (k < n - 1) cout << gH;  // Largeur de la barre fine │
        }
        if (b < n - 1) cout << gTT;     // Jonction barre épaisse ║
    }
    cout << gTR << endl;

    // CONTENU 
    for (int i = 0; i < taille; i++) {
        cout << gV; // Bordure gauche
        for (int j = 0; j < taille; j++) {
            // Chiffre
            if ((*this)[i][j] == 0) cout << "   ";
            else cout << " " << (*this)[i][j] << " ";

            // Séparateur vertical
            if ((j + 1) % n == 0) {
                if (j + 1 < taille) cout << gV; // Barre épaisse
            } else {
                cout << sV; // Barre fine
            }
        }
        cout << gV << endl; // Bordure droite

        // SÉPARATEURS HORIZONTAUX
        if (i < taille - 1) {
            if ((i + 1) % n == 0) {
                // Ligne épaisse entre les blocs
                cout << gTLf;
                for (int b = 0; b < n; b++) {
                    for (int k = 0; k < n; k++) {
                        cout << gH << gH << gH;
                        if (k < n - 1) cout << gH;
                    }
                    if (b < n - 1) cout << gC;
                }
                cout << gTRg << endl;
            } else {
                // Ligne fine à l'intérieur d'un bloc
                cout << gV;
                for (int b = 0; b < n; b++) {
                    for (int k = 0; k < n; k++) {
                        cout << sH << sH << sH;
                        if (k < n - 1) cout << sC;
                    }
                    if (b < n - 1) cout << gV;
                }
                cout << gV << endl;
            }
        }
    }

    // BORDURE DU BAS 
    cout << gBL;
    for (int b = 0; b < n; b++) {
        for (int k = 0; k < n; k++) {
            cout << gH << gH << gH;
            if (k < n - 1) cout << gH;
        }
        if (b < n - 1) cout << gTB;
    }
    cout << gBR << endl;
}


//========================================
// Classe Sudoku =========================
// =======================================

// Constructeurs //
Sudoku :: Sudoku(int n) : ordre(n), grille_ini(n) {}// Constructeur par défaut 

Sudoku :: Sudoku(const Grille& g): grille_ini(g), ordre(g.n) // Constructeur à partir de la classe Grille 
{
    this->grille_ini.majcasesVides();
}


// Détermine (toutes) les solutions d'une grille partielle //
bool Sudoku::Solution(int n)
{
    int N = grille_ini.casesVides.size();
    if(n==N) // Condition d'arrêt si n = N 
    {
        return true;
    }
    suint ligne = grille_ini.casesVides[n].first;
    suint col = grille_ini.casesVides[n].second;

    vector<suint> admissibles = grille_ini.listeadmissibles({ligne, col});

    for(suint val : admissibles)
    {
        grille_ini[ligne][col] = val;
        if (Solution(n + 1))
        {
            if (n < N-1){return true;} 
            
            grille_sol.push_back(grille_ini);
            
            if (!allSol){return true;} // si on veut une seule solution, on s'arrête

            if (this->maxSol > 0 && grille_sol.size() >= this->maxSol) {return true;} // si on veut un nombre fini de solutions
        }
        grille_ini[ligne][col] = 0; //val ne convient pas
    }
    
    return false;
}


// Affichage de la solution //
void Sudoku::affiche_sudoku(int limite) 
{
    cout << "========================================" << endl;
    cout << "          GRILLE INITIALE" << endl;
    cout << "========================================" << endl;
    grille_ini.afficher();

    cout << endl;
    cout << "Nombre de solutions trouvees : " << grille_sol.size() << endl;
    
    if (limite < grille_sol.size()){
        cout << "On affiche les " << limite << " premieres solutions" << endl;
    }

    int i = 1;
    for (auto& solution : grille_sol) 
    {
        if(i==limite+1){break;}
        cout << endl;
        cout << ">>> Solution " << i << " <<<" << endl;
        solution.afficher();
        i++;
    }
    cout << "========================================" << endl;
}

// Construit une grille à solution unique
Grille Sudoku::Solution_unique(float densite_obj, bool force){
    
    // Paramètres
    int ordre_ = this->ordre;
    float densite_init = 0.30; 
    float erreur_admise = 0.005;

    int max_retry = 500; // On peut se permettre bcp d'essais maintenant
    Grille meilleure_grille(ordre_);
    float meilleure_densite = 1.0;

    // Préparation pour le mélange aléatoire
    static random_device rd;
    static default_random_engine eng(rd());   

    int compteur = 0;
    for (int essai = 0; essai < max_retry; essai++) {
        compteur++;
        if (compteur%20 == 0){cout << "essai n°" << compteur << endl;}
        if (compteur == (max_retry/2)){cout << "meilleure densité atteinte actuellement : " << meilleure_densite << endl;}
        // Génération d'une grille pleine valide
        Grille grille_complete(ordre_);
        bool complete = false;
        int tests = 0;
        while(!complete && tests<1000){
            tests++;
            Grille g(ordre_);
            g.generation(densite_init);
            Sudoku s(g);
            s.allSol = false;
            if(s.Solution(0) && !s.grille_sol.empty()){
                grille_complete = s.grille_sol.front();
                complete = true;
            }
        }

        if (!complete) continue; // Si échec génération, on passe à l'essai suivant

        // Création de la liste de toutes les coordonnées (0 à 80 pour un 9x9)
        vector<pair<int, int>> coords;
        int taille_cote = ordre_ * ordre_;
        for (int i = 0; i < taille_cote; i++) {
            for (int j = 0; j < taille_cote; j++) {
                coords.push_back({i, j});
            }
        }

        // On mélange la liste pour avoir un ordre de suppression aléatoire et on fait pas tout
        // en aléatoire comme avant comme ça on supprime pas la même case plusieurs fois
        shuffle(coords.begin(), coords.end(), eng);

        // On parcourt TOUTE la liste et on essaie d'enlever chaque case
        Grille g_travail = grille_complete;
        int cases_restantes = taille_cote * taille_cote;
        
        // Réduction
        for (auto& coord : coords) {
            int i = coord.first;
            int j = coord.second;
            int valeur_sauvegardee = g_travail[i][j];

            // On tente de vider la case
            g_travail[i][j] = 0;
            cases_restantes--;
            
            Sudoku testeur(g_travail);
            testeur.grille_ini.optimiserCasesVides(); 
            
            testeur.allSol = true; 
            testeur.maxSol = 2; // Stop à 2 solutions
            testeur.Solution(0);
            
            if (testeur.grille_sol.size() != 1) {
                // ECHEC : La suppression a cassé l'unicité ou rendu la grille impossible
                g_travail[i][j] = valeur_sauvegardee; // On remet la valeur de base
                cases_restantes++;
            }

            // Si on est déjà en dessous de la densité voulue, on peut s'arrêter
            float d = (float)cases_restantes / (taille_cote * taille_cote);
            if (densite_obj - erreur_admise <= d && d <= densite_obj + erreur_admise) break;
        }

        
        // Bilan de cet essai
        float densite_finale = (float)cases_restantes / (taille_cote * taille_cote);
        
        // Est-ce le meilleur résultat jusqu'ici ?
        if (abs(densite_finale - densite_obj) < abs(meilleure_densite - densite_obj)) {
            meilleure_densite = densite_finale;
            meilleure_grille = g_travail;
        }

        // Si on a atteint l'objectif, on sort de la boucle for
        if (densite_obj - erreur_admise <= densite_finale && densite_finale <= densite_obj + erreur_admise) { // petite marge tolérée
            break;
        }
    }
    
    // FIN
    cout << "Meilleure densite atteinte : " << meilleure_densite << " (" << compteur << " essai(s))" << endl;

    this->grille_sol.clear();
    // La solution de la grille à trous n'est plus forcément la grille_complete du début 
    // (car unicité garantie, donc une seule solution possible), mais on peut la retrouver.
    // Pour être propre, on peut relancer une résolution :
    Sudoku s_final(meilleure_grille);
    s_final.allSol = false;
    s_final.maxSol = 0;
    s_final.Solution(0);
    if (!s_final.grille_sol.empty()) {
        this->grille_sol.push_back(s_final.grille_sol.front());
    }

    this->grille_ini = meilleure_grille;
    this->grille_ini.majcasesVides();

    return meilleure_grille;
}



void Sudoku::jouer(){
    int taille_cote = ordre * ordre;

    cout << "--- DEBUT DE LA PARTIE ---" << endl;
    cout << "Saisissez vos coups sous la forme : 'ligne colonne valeur'" << endl;
    cout << "Exemple : '4 2 8' (Ligne 4, Col 2, Valeur 8)" << endl;
    cout << "Entrez '0 0 0' pour quitter." << endl;

    grille_ini.casesVides;

    // Boucle tant qu'il y a des cases à remplir
    while (!grille_ini.casesVides.empty()){
        grille_ini.afficher();

        int l_choix, c_choix, val_choix;

        cout<<"\n Votre coup (ligne colonne valeur)";
        cin>>l_choix>>c_choix>>val_choix;


        if (l_choix==0){break;} // Option pour quitter

        // Remettre les indices des cases en indicage info (premier = 0)
        int i = l_choix - 1;
        int j = c_choix - 1;

        // Vérification des bornes
        if (i < 0 || i >= taille_cote || j < 0 || j >= taille_cote) {
            cout << "[ERREUR] Coordonnees hors de la grille (1 a " << taille_cote << ")." << endl;
            continue;}

        // Vérification si la case est déjà remplie
        if(grille_ini[i][j]!=0){
            cout << "[Erreur] Cette case est déjà remplie !\n";
            continue;
        }

        // Vérification des règles de Sudoku (admissiblité)
        vector<suint> admissibles = grille_ini.listeadmissibles({(suint)i, (suint)j});
        if (grille_ini.estPresent(admissibles, (suint)val_choix)) {
            grille_ini[i][j] = val_choix;
            grille_ini.majcasesVides(); // On met à jour la liste des cases vides
            cout << "Coup valide ! Bien joue." << endl;
        } else {
            cout << "[ERREUR] Le chiffre " << val_choix << " n'est pas autorisé ici (regle ligne/colonne/bloc)." << endl;
        }
        }
        if (grille_ini.casesVides.empty()) {
        grille_ini.afficher();
        cout << "FELICITATIONS ! Vous avez complete la grille." << endl;
        } else {
            cout << "Partie interrompue." << endl;
        }
    
}