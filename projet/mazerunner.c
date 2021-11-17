#include "../lib/libgraphique.h"
#include "./charge_labyrinthe.h"
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>


#define resX 900
#define resY 650
#define resXi 603
#define resYi 315

#define couleur1 bleu
#define couleur2 red
#define couleurTrace1 lightblue
#define couleurTrace2 pink
#define	CARRE 10

typedef struct
	{
	Point courant;
	Point prec;
	Couleur couleur;
	Couleur couleurTrace;
	}Joueur;

Joueur joueur1,joueur2;

typedef struct
	{
	Point corner;
	Couleur couleur;
	int longueur;
	}Bouton;

Bouton bouton1,bouton2,bouton3,boutonLabPlus,boutonLabMoins;

typedef struct
	{
	Point newPos;
	Point coin; // Utilisé pour dessiner un fond monochrome
	Point sortie1; // Sortie du joueur 1
	Point sortie2; // Sortie du joueur 2
	}Dot;

Point coin = {0,0};
Point sortie1 = {(resX/4)+5,resY-CARRE};
Point sortie2 = {(3*resX/4)+5,resY-CARRE};

Dot position1, position2, poscoin;

// Variables globales
int tour = 0;
char tab[LIG][COL];
int l,c;
int touche;
int victoire; // Etat de victoire (booléen)
int choix_labyrinthe = 1;
int random_spawn = 1; // Apparition aléatoire ou prédéfinie
int mode_de_jeu = 0;

/******************************************************************************/
/* FONCTIONS                                                                  */
/******************************************************************************/
int menu()
	{
// Initialisation
	bouton1.couleur = steelblue;
	bouton2.couleur = violet;
	bouton3.couleur = lightgreen;
	boutonLabPlus.couleur = lightgrey; boutonLabMoins.couleur = lightgrey;
	bouton1.longueur = 255; bouton2.longueur = 180; bouton3.longueur = 235; boutonLabPlus.longueur= 20; boutonLabMoins.longueur= 20; // Longueur des boutons
	bouton1.corner.x = resXi/4-(bouton1.longueur/2); bouton1.corner.y = 5*resYi/6;
	bouton2.corner.x = 3*resXi/4-(bouton2.longueur/2); bouton2.corner.y = 5*resYi/6;
	bouton3.corner.x = resXi/2-(bouton3.longueur/2); bouton3.corner.y = 5;
	boutonLabPlus.corner.x = 5; boutonLabPlus.corner.y = 50;
	boutonLabMoins.corner.x = 30; boutonLabMoins.corner.y = 50;
	int hauteur = 45;
	int hauteur2 = 20;

// Affichage

	afficher_image("mazerunner.bmp",poscoin.coin); // Fond

	// Bouton gauche
	dessiner_rectangle(bouton1.corner , bouton1.longueur, hauteur, bouton1.couleur);
	bouton1.corner.x+=CARRE; bouton1.corner.y+=CARRE; // Se décalle à l'intérieur
	afficher_texte("Mode de jeu tour par tour",18,bouton1.corner, blanc);
	bouton1.corner.x-=CARRE; bouton1.corner.y-=CARRE;
	// Bouton droit
	dessiner_rectangle(bouton2.corner , bouton2.longueur, hauteur, bouton2.couleur);
	bouton2.corner.x+=CARRE; bouton2.corner.y+=CARRE;
	afficher_texte("Mode de jeu libre",18,bouton2.corner, blanc);
	bouton2.corner.x-=CARRE; bouton2.corner.y-=CARRE;
	//Bouton haut
	dessiner_rectangle(bouton3.corner , bouton3.longueur, hauteur, bouton3.couleur);
	bouton3.corner.x+=CARRE; bouton3.corner.y+=CARRE;
	afficher_texte("Activer/desactiver random",16,bouton3.corner, black);
	bouton3.corner.x-=CARRE; bouton3.corner.y-=CARRE;
	// Bouton choix labyrinthe (+)
	dessiner_rectangle(boutonLabPlus.corner , boutonLabPlus.longueur, hauteur2, boutonLabPlus.couleur);
	boutonLabPlus.corner.x+=3; boutonLabPlus.corner.y-=4;
	afficher_texte("+",18,boutonLabPlus.corner, black);
	boutonLabPlus.corner.x-=3; boutonLabPlus.corner.y+=4;
	// Bouton choix labyrinthe (-)
	dessiner_rectangle(boutonLabMoins.corner , boutonLabMoins.longueur, hauteur2, boutonLabMoins.couleur);
	boutonLabMoins.corner.x+=6; boutonLabMoins.corner.y-=3;
	afficher_texte("-",18,boutonLabMoins.corner, black);
	boutonLabMoins.corner.x-=6; boutonLabMoins.corner.y+=3;

	afficher_texte("Echap pour quitter",12,poscoin.coin, blanc); // Affiche du texte dans la fenêtre
	poscoin.coin.x=7*resXi/10; // à 70% de la fenêtre
	afficher_texte("Au joueur bleu de commencer",12,poscoin.coin, blanc);
	poscoin.coin.x=0;
	actualiser();

	while (mode_de_jeu == 0)
		{
		Point centre = {resXi/2+(resXi/30),resYi-(resYi/10)};
		Point clic = attendre_clic();
		if (clic.x>=bouton1.corner.x && clic.x <= bouton1.corner.x+bouton1.longueur && clic.y >= bouton1.corner.y && clic.y <= bouton1.corner.y+hauteur) // Si clic sur bouton 1
			{
			mode_de_jeu = 1; // Tour par tour
			dessiner_disque(centre,15,bouton1.couleur); // Indicateur de validation
			bouton2.couleur = bouton1.couleur;
			}			
		if (clic.x>=bouton2.corner.x && clic.x <= bouton2.corner.x+bouton2.longueur && clic.y >= bouton2.corner.y && clic.y <= bouton2.corner.y+hauteur) // Bouton 2
			{
			mode_de_jeu = 2; // mode libre
			dessiner_disque(centre,15,bouton2.couleur);
			bouton1.couleur = bouton2.couleur;
			}
		if (clic.x>=bouton3.corner.x && clic.x <= bouton3.corner.x+bouton3.longueur && clic.y >= bouton3.corner.y && clic.y <= bouton3.corner.y+hauteur) // Bouton 3
			{
			if (random_spawn == 0)
				{
				random_spawn = 1;
				bouton3.couleur = lightgreen;
				}
			else
				{
				random_spawn = 0;
				bouton3.couleur = tomato;
				}
			dessiner_rectangle(bouton3.corner , bouton3.longueur, hauteur, bouton3.couleur);
			bouton3.corner.x+=CARRE; bouton3.corner.y+=CARRE;
			afficher_texte("Activer/desactiver random",16,bouton3.corner, black);
			bouton3.corner.x-=CARRE; bouton3.corner.y-=CARRE;
			actualiser();
			}
		if ((clic.x>=boutonLabPlus.corner.x && clic.x <= boutonLabPlus.corner.x+boutonLabPlus.longueur && clic.y >= boutonLabPlus.corner.y && clic.y <= boutonLabPlus.corner.y+hauteur2 && choix_labyrinthe !=10) || (clic.x>=boutonLabMoins.corner.x && clic.x <= boutonLabMoins.corner.x+boutonLabMoins.longueur && clic.y >= boutonLabMoins.corner.y && clic.y <= boutonLabMoins.corner.y+hauteur2 && choix_labyrinthe !=1)) // Bouton choix labyrinthe + ou -
			{
			if (clic.x>=boutonLabPlus.corner.x && clic.x <= boutonLabPlus.corner.x+boutonLabPlus.longueur && clic.y >= boutonLabPlus.corner.y && clic.y <= boutonLabPlus.corner.y+hauteur2 && choix_labyrinthe !=10) // Si clic bouton +
				choix_labyrinthe++;
			else // Sinon clic sur -
				choix_labyrinthe--;
			boutonLabPlus.corner.y-=hauteur2;
			dessiner_rectangle(boutonLabPlus.corner , 45, hauteur2, boutonLabPlus.couleur);
			switch (choix_labyrinthe) // Affiche le labyrinthe sélectionné au-dessus des boutons + et -
				{
				case 1:
					afficher_texte("   1",16,boutonLabPlus.corner, black);
					break;
				case 2:
					afficher_texte("   2",16,boutonLabPlus.corner, black);
					break;
				case 3:
					afficher_texte("   3",16,boutonLabPlus.corner, black);
					break;
				case 4:
					afficher_texte("   4",16,boutonLabPlus.corner, black);
					break;
				case 5:
					afficher_texte("   5",16,boutonLabPlus.corner, black);
					break;
				case 6:
					afficher_texte("   6",16,boutonLabPlus.corner, black);
					break;
				case 7:
					afficher_texte("   7",16,boutonLabPlus.corner, black);
					break;
				case 8:
					afficher_texte("   8",16,boutonLabPlus.corner, black);
					break;
				case 9:
					afficher_texte("   9",16,boutonLabPlus.corner, black);
					break;
				case 10:
					afficher_texte("  10",16,boutonLabPlus.corner, black);
					break;
				}
			boutonLabPlus.corner.y+=hauteur2;
			actualiser();
			}
		}
	actualiser();
	SDL_Delay(1000);
	return mode_de_jeu;
	}

Point case_to_pixel (Point uneCase) // Convertir un point de case vers pixel
	{
	uneCase.x*=CARRE;
	uneCase.y*=CARRE;
	return uneCase;
	}

Point pixel_to_case (Point pixel) // Convertir un point de pixel vers case
	{
	pixel.x/=CARRE;
	pixel.y/=CARRE;
	return pixel;
	}

Joueur av_un_pas (Dot position, Joueur joueur, int tour)
	{
	int decallage;
	if (tour == 1) // S'il s'agit du joueur 1..
		decallage = 2; // ..le décallage vers le bas vaut 2 cases
	else // S'il s'agit du joueur 2..
		decallage = 3; // ..il en vaut 3
	if (tab[position.newPos.y-decallage][position.newPos.x] == ' ') // Si la case est libre
		{
		position.newPos = case_to_pixel(position.newPos); // Convertit en pixels
		joueur.prec = joueur.courant;  // Sauvegarde la position avant déplacement
		joueur.courant = position.newPos;  // Définit nouvelle position
		dessiner_rectangle(joueur.prec , CARRE, CARRE, joueur.couleurTrace) ; // Affiche une trace à la position précédente
		dessiner_rectangle(joueur.courant , CARRE, CARRE, joueur.couleur) ; // Affiche le joueur en sa nouvelle position
		}
	return joueur;
	}

void direction ()
	{
	if (tour == 0 || mode_de_jeu == 2)
		{
		position1.newPos = joueur1.courant;
		position1.newPos = pixel_to_case(position1.newPos);
		if (mode_de_jeu == 1)
			dessiner_rectangle(poscoin.coin , resX, 2*CARRE, couleur1) ; // Barre supérieure
		tour = 1;
		switch (touche)
			{
			case(SDLK_z):
				position1.newPos.y-=1;
				break;
			case(SDLK_s):
				position1.newPos.y+=1;
				break;
			case(SDLK_q):
				position1.newPos.x-=1;
				break;
			case(SDLK_d):
				position1.newPos.x+=1;
				break;
			default:
				tour = 0;
			}
		joueur1=av_un_pas(position1,joueur1,tour);
		}

	if (tour == 1 || mode_de_jeu == 2)
		{
		position2.newPos = joueur2.courant;
		position2.newPos = pixel_to_case(position2.newPos);
		if (mode_de_jeu == 1)
			dessiner_rectangle(poscoin.coin , resX, 2*CARRE, couleur2) ; // Barre supérieure
		tour = 0;
		switch (touche)
			{
			case(SDLK_UP):
				position2.newPos.y-=1;
				break;
			case(SDLK_DOWN):
				position2.newPos.y+=1;
				break;
			case(SDLK_LEFT):
				position2.newPos.x-=1;
				break;
			case(SDLK_RIGHT):
				position2.newPos.x+=1;
				break;
			default:
				tour = 1;	
			}
		joueur2=av_un_pas(position2,joueur2,tour);
		}
	}

void affiche_labyrinthe(Point select,Point select2)
	{
	switch(choix_labyrinthe)
		{
		case 1:
			charge_labyrinthe("mazeS/maze1",tab); // Cette ligne charge le fichier 'maze' dans le tableau tab
			break;
		case 2:
			charge_labyrinthe("mazeS/maze2",tab);
			break;
		case 3:
			charge_labyrinthe("mazeS/maze3",tab);
			break;
		case 4:
			charge_labyrinthe("mazeS/maze4",tab);
			break;
		case 5:
			charge_labyrinthe("mazeS/maze5",tab);
			break;
		case 6:
			charge_labyrinthe("mazeS/maze6",tab);
			break;
		case 7:
			charge_labyrinthe("mazeS/maze7",tab);
			break;
		case 8:
			charge_labyrinthe("mazeS/maze8",tab);
			break;
		case 9:
			charge_labyrinthe("mazeS/maze9",tab);
			break;
		case 10:
			charge_labyrinthe("mazeS/maze10",tab);
			break;
		}

/* Affichage du labyrinthe                                                    */
	for(l=0;l!=LIG;l++) // Pour chaque ligne
        	{
		select.y=CARRE*l+2*CARRE;
		select2.y = select.y;
		for(c=0;c!=COL;c++) // Pour chaque colonne
			{
			if (tab[l][c] == '*') // S'il s'agit d'un mur
				{
				select.x=CARRE*c;
				select2.x = select.x + resX/2;
				switch (entier_aleatoire(4)) // Place des textures aléatoires afin d'éviter les répétitions
					{
					case 0:
						afficher_image("./cobble/cobble_dark.bmp",select); // Premier labyrinthe
						afficher_image("./cobble/cobble_dark.bmp",select2); // Second labyrinthe
						break;				
					case 1:
						afficher_image("./cobble/cobble_clear.bmp",select);				
						afficher_image("./cobble/cobble_clear.bmp",select2);
						break;
					case 2:
						afficher_image("./cobble/cobble_grey.bmp",select);
						afficher_image("./cobble/cobble_grey.bmp",select2);
						break;
					case 3:
						afficher_image("./cobble/cobble_broken.bmp",select);
						afficher_image("./cobble/cobble_broken.bmp",select2);
						break;
					}
				}
			}
	        }
	}

void placer_joueurs()
	{
	Point pos={0,0};
	if (random_spawn == 0) // Si la position de départ aléatoire est désactivée
		{
		joueur1.courant.x = (resX/4)+5; joueur1.courant.y=2*CARRE;
		joueur2.courant.x = ((3*resX)/4)+5; joueur2.courant.y=2*CARRE;
		}
	else
		{
		while(tab[pos.y-2][pos.x] != ' ') // Tant que la case n'est pas libre
			{
			pos.x = entier_aleatoire(resX/(2*CARRE)); // Les positions sont générées aléatoirement
			pos.y = entier_aleatoire(resY/CARRE);
			}
		joueur1.courant.x = pos.x*CARRE;
		joueur1.courant.y=pos.y*CARRE;
		joueur2.courant.x = (resX/2)+pos.x*CARRE;
		joueur2.courant.y=pos.y*CARRE;
		}
	}

/******************************************************************************/
/* MAIN                                                                       */
/******************************************************************************/
int main(void)
	{
	ouvrir_fenetre(resXi,resYi); // format de l'image de fond

// Initialisation variables joueur
	joueur1.couleur = couleur1;
	joueur2.couleur = couleur2;
	joueur1.couleurTrace = couleurTrace1;
	joueur2.couleurTrace = couleurTrace2;

	mode_de_jeu = menu();
	
	ouvrir_fenetre(resX,resY);
	actualiser();

	Point select ={0,20}; // Redéfini pour placer divers objets
	Point select2 = {resX/2,select.y};
	dessiner_rectangle(poscoin.coin , resX, resY, blanc);
	dessiner_rectangle(poscoin.coin , resX, 2*CARRE, bouton1.couleur) ; // Barre supérieure indiquant le mode de jeu sélectionné

// Affiche le labyrinthe
	affiche_labyrinthe(select,select2);

// Place les joueurs
	placer_joueurs();

// Affiche les joueurs et la sortie
	dessiner_rectangle(joueur1.courant, CARRE, CARRE, joueur1.couleur);
	dessiner_rectangle(joueur2.courant, CARRE, CARRE, joueur2.couleur) ;
	

	dessiner_rectangle(sortie1 , CARRE, CARRE, aqua);
	dessiner_rectangle(sortie2 , CARRE, CARRE, orangered);

	actualiser();

/*  Boucle jeu                                                               */
	
	while (touche != SDLK_ESCAPE && victoire == 0) // Ferme la fenêtre dès lors que la touche échap est enfoncée
		{
		touche = attendre_touche();
		direction();
		actualiser();
		if (joueur1.courant.x == sortie1.x && joueur1.courant.y == sortie1.y)
			victoire = 1;
		else if (joueur2.courant.x == sortie2.x && joueur2.courant.y == sortie2.y)
			victoire = 2;
		SDL_Delay(100);
		}

	if (victoire == 1)
		{
			{
   			SDL_Init(SDL_INIT_AUDIO);
   			if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1) //Initialisation de l'API Mixer
   				{
      				printf("%s", Mix_GetError());
   				}
   			Mix_AllocateChannels(32); //Allouer 32 canaux
   			Mix_Volume(1, MIX_MAX_VOLUME/2); //Mettre à mi-volume le post 1
   			Mix_Chunk *son;//Créer un pointeur pour stocker un .WAV
   			son = Mix_LoadWAV("Powerup.wav"); //Charger un wav dans un pointeur
   			Mix_VolumeChunk(son, MIX_MAX_VOLUME/3); //Mettre un volume pour ce wav
   			Mix_PlayChannel(1, son, 0);//Joue le son 1 sur le canal 1 ; le joue une fois (0 + 1)
			ouvrir_fenetre(760,563); // format de l'image de fond
			afficher_image("escape.bmp",poscoin.coin); // Fond
   			Mix_FreeChunk(son);//Libération du son 1
   			Mix_CloseAudio(); //Fermeture de l'API
   			SDL_Quit();
			}
		ouvrir_fenetre(760,563); // format de l'image de fond
		afficher_image("escape.bmp",poscoin.coin); // Fond
		poscoin.coin.x=(760/2)-(200/2); poscoin.coin.y=5;
		dessiner_rectangle(poscoin.coin , 200, 30, joueur1.couleurTrace);
		poscoin.coin.x+=10;
		afficher_texte("Le joueur 1 a gagne!",17,poscoin.coin, black);
		}
	else if (victoire == 2)
		{
			{
   			SDL_Init(SDL_INIT_AUDIO);
   			if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1) //Initialisation de l'API Mixer
   				{
      				printf("%s", Mix_GetError());
   				}
   			Mix_AllocateChannels(32); //Allouer 32 canaux
   			Mix_Volume(1, MIX_MAX_VOLUME/3); //Mettre à mi-volume le post 1
   			Mix_Chunk *son2;//Créer un pointeur pour stocker un .WAV
   			son2 = Mix_LoadWAV("Powerup.wav"); //Charger un wav dans un pointeur
   			Mix_VolumeChunk(son2, MIX_MAX_VOLUME/2); //Mettre un volume pour ce wav
                  	Mix_PlayChannel(1, son2, 0);//Joue le son 1 sur le canal 1 ; le joue une fois (0 + 1)
   			ouvrir_fenetre(760,563);
			afficher_image("escape.bmp", poscoin.coin);
   			Mix_FreeChunk(son2);//Libération du son 2
   			Mix_CloseAudio(); //Fermeture de l'API
   			SDL_Quit();
			}
		ouvrir_fenetre(760,563); // format de l'image de fond
		afficher_image("escape.bmp",poscoin.coin); // Fond
		poscoin.coin.x=(760/2)-(200/2); poscoin.coin.y=5;
		dessiner_rectangle(poscoin.coin , 200, 30, joueur2.couleurTrace);
		poscoin.coin.x+=10;
		afficher_texte("Le joueur 2 a gagne!",17,poscoin.coin, black);
		}


/* Fin du programme                                                           */
	actualiser();
	attendre_clic();
	fermer_fenetre();

	return(0);
	}

