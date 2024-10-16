#ifndef __ENTETE_H_
#define __ENTETE_H_

#include <cstdio>
#include <cstdlib> 
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>  
#include <cmath>
#include <vector>
#include <functional>
using namespace std;

enum TProblemType { MIN, MAX };					//** Definition du sens de l'optimisation du probleme

struct TProblem									//**Definition du probleme:
{
	std::string Nom;							//**Nom du fichier de donnees
	int N;										//**Taille du probleme: Nombre unites (elements) / d'emplacements (sites)
	std::vector<std::vector <int> > Distance;	//**Distance entre chaque paire d'emplacements (sites). NB: Tableau de 0 � N-1.  Indice 0 utilise
	std::vector<std::vector <int> > Flux;		//**Flux entre chaque paire d'unites (�l�ments). NB: Tableau de 0 � N-1.  Indice 0 utilise
	TProblemType Type = MIN;					//**Type du probleme (e.g.Maximisation ou minimisation)
};

struct TSolution								//**Definition d'une solution: 
{
	std::vector <int> Site;				//**Indique la permutation i.e. l'UNITE affectee a l'EMPLACEMENT (site) donnee en indice. NB: Tableau de 0 a N-1
	long FctObj = INT_MAX;				//**Valeur de la fonction obj: Cout total de transit (distances * flux)
	bool Valide = false;				//**Etat de validite de la solution (presence de chacune des unites (sans doublons) + longueur de la solution + domaine admissible des variables)
	int NbEvaltoGet;					//**Nombre d'evalutions de solutions necessaires pour trouver cette solution... pour �valuer convergence	//***MODIF***
};

struct TAlgo							//D�finition de la Recherche avec Tabous:
{
	int		LngListeTabous;				//**Longueur de la liste des tabous													//***NEW***
	
	int		CptEval=0;					//**COMPTEUR DU NOMBRE DE SOLUTIONS EVALUEES. SERT POUR CRITERE D'ARRET.
	int		NB_EVAL_MAX;				//**CRITERE D'ARRET: MAXIMUM "NB_EVAL_MAX" EVALUATIONS.
	int		TailleVoisinage;			//**Nombre de solutions voisines g�n�r�es � chaque it�ration
	long	FctObjSolDepart;			//**Valeur de la fonction objectif de la solution au d�part de l'algorithme
};

#endif