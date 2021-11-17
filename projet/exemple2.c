#include "../lib/libgraphique.h"
#include "./charge_labyrinthe.h"
#include <stdio.h>

#define COTE 10
#define resX 900
#define resY 650

#define couleur1 bleu
#define couleur2 red

/******************************************************************************/
/* FONCTIONS                                                                  */
/******************************************************************************/
int check_mur (Point p1,Point p2,char tab[LIG][COL],char direction,int touche)
	{
	int x,y;
	if (direction == 'Z'){
		x = 0; y = -10;}
	if (direction == 'S'){
		x = 0; y = 10;}
	if (direction == 'Q'){
		x = -10; y = 0;}
	if (direction == 'D'){
		x = 10; y = 0;}

	if (touche == SDLK_UP || touche == SDLK_DOWN || touche == SDLK_LEFT || touche == SDLK_RIGHT)
		{
		if (tab[(p1.y+y)/10][(p1.x+x)/10] == '*')
			return 1;
		else
			{
			if (x == 10 || x == -10)
				return (p1.x+x);
			if (y == 10 || y == -10)
				return (p1.y+y);
			}
		}
	else if (touche == SDLK_z || touche == SDLK_q || touche == SDLK_s || touche == SDLK_d)
		{
		if (tab[(p2.y+y)/10][(p2.x+x)/10] == '*')
			return 1;
		else
			{
			if (x == 10 || x == -10)
				return (p2.x+x);
			if (y == 10 || y == -10)
				return (p2.y+y);
			}
		}
	}


void deplacement (Point p1,Point p2,char tab[LIG][COL]) // Coordonnées des joueurs 1 et 2 en entrée
	{
	int touche;
	int tour = 1;
	char direction;
	int x1,y1,x2,y2;
	Point Avant1;
	Point Avant2;
	Point coin = {0,resY-20};
	while (touche != SDLK_ESCAPE) // Ferme la fenêtre dès lors que la touche échap est enfoncée
		{
		touche = attendre_touche_duree(100) ;
		if (touche == SDLK_UP || touche == SDLK_DOWN || touche == SDLK_LEFT || touche == SDLK_RIGHT || touche == SDLK_z || touche == SDLK_q || touche == SDLK_s || touche == SDLK_d)
			{
			if (touche == SDLK_UP || touche == SDLK_z)
				direction = 'Z';
			if (touche == SDLK_DOWN || touche == SDLK_s)
				direction = 'S';
			if (touche == SDLK_LEFT || touche == SDLK_q)
				direction = 'Q';
			if (touche == SDLK_RIGHT || touche == SDLK_d)
				direction = 'D';
			if (check_mur(p1,p2,tab,direction,touche) != 1)
				{
				x1 = p1.x; y1 = p1.y; x2 = p2.x; y2 = p2.y; // Sauvegarde les positions
				Avant1.x = x1; Avant1.y = y1; Avant2.x = x2; Avant2.y = y2; // Les met sous forme de point
				if (check_mur(p1,p2,tab,direction,touche) == p1.x+10 || check_mur(p1,p2,tab,direction,touche) == p1.x-10 || tour == 1)
					p1.x = check_mur(p1,p2,tab,direction,touche);
				else if (check_mur(p1,p2,tab,direction,touche) == p1.y+10 || check_mur(p1,p2,tab,direction,touche) == p1.y-10 || tour == 1)
					p1.y = check_mur(p1,p2,tab,direction,touche);
				else if (check_mur(p1,p2,tab,direction,touche) == p2.x+10 || check_mur(p1,p2,tab,direction,touche) == p2.x-10 || tour == 2)
					p2.x = check_mur(p1,p2,tab,direction,touche);
				else
					p2.y = check_mur(p1,p2,tab,direction,touche);
				}

			// Affichage 
			if (x1!=p1.x || y1 !=p1.y) // Seulement si la position du joueur 1 a changé
				{
				dessiner_rectangle(Avant1 , 10, 10, black) ;
				dessiner_rectangle(p1 , 10, 10, couleur1) ;
				tour = 2;
				dessiner_rectangle(coin , resX, 20, couleur2) ; // Barre supérieure (tour de jouer)
				}
			if (x2!=p2.x || y2 !=p2.y) // Seulement si la position du joueur 2 a changé
				{
				dessiner_rectangle(Avant2 , 10, 10, black) ;
				dessiner_rectangle(p2 , 10, 10, couleur2) ;
				tour = 1;
				dessiner_rectangle(coin , resX, 20, couleur1) ; // Barre supérieure
				}
			actualiser();
			}
		}
	fermer_fenetre();
	}

/******************************************************************************/
/* MAIN                                                                       */
/******************************************************************************/
int main(void)
    {
    char tab[LIG][COL];
    int l,c;

	ouvrir_fenetre(resX,resY);

	Point p1 ={(resX/2)+210,100} ;
	dessiner_rectangle(p1 , 10, 10, couleur1) ;
	Point p2 = {210,100};
	dessiner_rectangle(p2 , 10, 10, couleur2) ;
	Point select = {5,resY-20}; // Redéfini pour placer divers objets
	afficher_texte("Echap pour quitter",12,select, blanc); // Affiche du texte dans la fenêtre
	select.x=resX-185;
	afficher_texte("Au joueur bleu de commencer",12,select, blanc);
	select.x=0; select.y=20;
	Point select2;
	select2.x = select.x + resX/2; select2.y = select.y;

/* Cette ligne charge le fichier 'maze' dans le tableau tab                   */
	charge_labyrinthe("./maze",tab);

/* Affichage du labyrinthe                                                    */
	for(l=0;l!=LIG;l++)
        	{
		select.y=10*l-10;
		select2.y = select.y;
		for(c=0;c!=COL;c++)
			{
			if (tab[l][c] == '*')
				{
				select.x=10*c;
				select2.x = select.x + resX/2;
				dessiner_rectangle(select , 10, 10, blanc);
				dessiner_rectangle(select2 , 10, 10, blanc);
				}
			}
	        }

	Point haut={resX/2,0};
	Point bas ={resX/2,resY};
	dessiner_ligne(haut,bas,black);
	actualiser();
	
	deplacement(p1,p2,tab);    

/* Fin du programme                                                           */
	actualiser();
	attendre_clic();

    return(0);
    }


