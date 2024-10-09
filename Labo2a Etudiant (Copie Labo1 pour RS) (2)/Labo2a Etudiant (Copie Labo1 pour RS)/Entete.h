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
	std::vector<std::vector <int> > Distance;	//**Distance entre chaque paire d'emplacements (sites). NB: Tableau de 0 à N-1.  Indice 0 utilise
	std::vector<std::vector <int> > Flux;		//**Flux entre chaque paire d'unites (éléments). NB: Tableau de 0 à N-1.  Indice 0 utilise
	TProblemType Type = MIN;					//**Type du probleme (e.g.Maximisation ou minimisation)
};

struct TSolution								//**Definition d'une solution: 
{
	std::vector <int> Site;				//**Indique la permutation i.e. l'UNITE affectee a l'EMPLACEMENT (site) donnee en indice. NB: Tableau de 0 a N-1
	long FctObj = INT_MAX;				//**Valeur de la fonction obj: Cout total de transit (distances * flux)
	bool Valide = false;				//**Etat de validite de la solution (presence de chacune des unites (sans doublons) + longueur de la solution + domaine admissible des variables)
	int NbEvaltoGet;					//**Nombre d'evalutions de solutions necessaires pour trouver cette solution... pour évaluer convergence	//***MODIF***
};

struct TAlgo							//Définition du Recuit Simulé:
{
	double	Temperature;				//Temperature courante du systeme																		//***NEW***
	int		TempInit;					//Temperature initiale du systeme 
	int		Delta;						//Dégradation courante entre la solution Courante et la solution Next									//***NEW***
	double	Alpha;						//Schema de "reduction de la température" : Temp(t+1) = Alpha * Temp(t)									//***NEW***
	int		NbPalier;					//Schema de "Duree de la température": Duree Temperature = NB_EVAL_MAX/NbPalier							//***NEW***

	int		CptEval=0;					//**COMPTEUR DU NOMBRE DE SOLUTIONS EVALUEES. SERT POUR CRITERE D'ARRET.
	int		NB_EVAL_MAX;				//**CRITERE D'ARRET: MAXIMUM "NB_EVAL_MAX" EVALUATIONS.
	int		TailleVoisinage;			//**Nombre de solutions voisines generees a chaque iteration
	long	FctObjSolDepart;			//**Valeur de la fonction objectif de la solution au depart de l'algorithme
};

#endif