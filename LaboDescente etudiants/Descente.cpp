#include "Entete.h"
#pragma comment (lib,"DescenteDLL.lib")  
//%%%%%%%%%%%%%%%%%%%%%%%%% IMPORTANT: %%%%%%%%%%%%%%%%%%%%%%%%% 
//Le fichier de probleme (.txt) et les fichiers de la DLL (DescenteDLL.dll et DescenteDLL.lib) doivent se trouver dans le repertoire courant du projet pour une execution a l'aide du compilateur. 
//Indiquer les arguments du programme dans les proprietes du projet - debogage - arguments.
//Sinon, utiliser le repertoire execution.
//NB: le projet actuel doit etre compile dans le meme mode (DEBUG ou RELEASE) que les fichiers de DLL - par defaut en RELEASE

//*****************************************************************************************
// Prototype des fonctions se trouvant dans la DLL 
//*****************************************************************************************
//DESCRIPTION:	Lecture du Fichier probleme et initialiation de la structure Problem
extern "C" _declspec(dllimport) void LectureProbleme(std::string FileName, TProblem& unProb, TAlgo& unAlgo);

//DESCRIPTION: Fonction d'affichage a l'ecran permettant de voir si les donnees du fichier probleme ont ete lues correctement
extern "C" _declspec(dllimport) void AfficherProbleme(TProblem unProb);

//DESCRIPTION: Affichage d'une solution a l'ecran
extern "C" _declspec(dllimport) void AfficherSolution(const TSolution uneSolution, TProblem unProb, std::string Titre);

//DESCRIPTION:	Evaluation de la fonction objectif d'une solution et MAJ du compteur d'evaluations. La fonction objectif represente cout total de transit (distance * flux) dans l'affectation.
//				NB: L'évaluation d'une solution inclue la validation de la solution
extern "C" _declspec(dllimport) void EvaluerSolution(TSolution& uneSol, TProblem unProb, TAlgo& unAlgo);

//DESCRIPTION:	Creation d'une affectation aleatoire des N unites aux N sites (en indice) et appel à l'evaluation de la fonction objectif. Allocation dynamique de memoire pour le detail de la solution (.Site)
extern "C" _declspec(dllimport) void CreerSolutionAleatoire(TSolution& uneSolution, TProblem unProb, TAlgo& unAlgo);

//DESCRIPTION: Affichage a l'ecran de la solution finale (et de son statut de validite), du nombre d'evaluations effectuees et de certains parametres
extern "C" _declspec(dllimport) void AfficherResultats(const TSolution uneSol, TProblem unProb, TAlgo unAlgo);

//DESCRIPTION: Affichage dans un fichier (en append) de la solution finale (et de son statut de validite), du nombre d'evaluations effectuees et de certains parametres
extern "C" _declspec(dllimport) void AfficherResultatsFichier(const TSolution uneSol, TProblem unProb, TAlgo unAlgo, std::string FileName);

//DESCRIPTION:	Liberation de la memoire allouee dynamiquement pour les differentes structures en parametre
extern "C" _declspec(dllimport) void	LibererMemoireFinPgm(TSolution uneCourante, TSolution uneNext, TSolution uneBest, TProblem unProb);

//*****************************************************************************************
// Prototype des fonctions locales 
//*****************************************************************************************

//DESCRIPTION:	Creation d'une solution voisine a partir de la solution uneSol. Definition la STRATEGIE D'ORIENTATION (Parcours/Regle de pivot).
//NB:uneSol ne doit pas etre modifiee (const)
TSolution GetSolutionVoisine(const TSolution uneSol, TProblem unProb, TAlgo& unAlgo);

//DESCRIPTION:	Application du type de voisinage selectionne. La fonction retourne la solution voisine obtenue suite a l'application du type de voisinage.
//NB:uneSol ne doit pas etre modifiee (const)
TSolution	AppliquerVoisinage(const TSolution uneSol, TProblem unProb, TAlgo& unAlgo, int boucle);

//... vous pouvez ajouter vos fonctions locales

//******************************************************************************************
// Fonction main
//*****************************************************************************************
int main(int NbParam, char* Param[])
{
	TSolution Courante;		//Solution active au cours des iterations
	TSolution Next;			//Solution voisine retenue a une iteration
	TSolution Best;			//Meilleure solution depuis le début de l'algorithme	//Non utilisee pour le moment 
	TProblem LeProb;		//Definition de l'instance de probleme
	TAlgo LAlgo;			//Definition des parametres de l'agorithme
	string NomFichier;

	//**Lecture des parametres
	NomFichier.assign(Param[1]);
	LAlgo.TailleVoisinage = atoi(Param[2]);
	LAlgo.NB_EVAL_MAX = atoi(Param[3]);

	srand((unsigned)time(NULL));		//**Precise un germe pour le generateur aleatoire

	//**Lecture du fichier de donnees
	LectureProbleme(NomFichier, LeProb, LAlgo);
	AfficherProbleme(LeProb);

	//**Creation de la solution initiale 
	CreerSolutionAleatoire(Courante, LeProb, LAlgo);
	AfficherSolution(Courante, LeProb, "SOLUTION INITIALE: ");
	//**Enregistrement des infos concernant la meilleure solution
	LAlgo.FctObjSolDepart = Courante.FctObj;
	LAlgo.NbEvalBest = LAlgo.CptEval;

	do
	{
		Next = GetSolutionVoisine(Courante, LeProb, LAlgo);
		//AfficherSolution(Courante, LeProb, "COURANTE: ");
		//AfficherSolution(Next, LeProb, "NEXT: ");
		if (Next.FctObj <= Courante.FctObj)	//**amelioration ou egalite
		{
			//Si amelioration stricte: Affichage et conservation du moment ou celle-ci est trouvee (apres combien d'evaluations de solutions)
			if (Next.FctObj < Courante.FctObj)
			{
				cout << "CPT_EVAL: " << LAlgo.CptEval << "\t\tNEW COURANTE/OBJ: " << Next.FctObj << endl;
				AfficherSolution(Next, LeProb, "NOUVELLE COURANTE: ");
				LAlgo.NbEvalBest = LAlgo.CptEval;
			}
			//Modification de la solution courante
			Courante = Next;
		}
	} while (LAlgo.CptEval < LAlgo.NB_EVAL_MAX && Courante.FctObj != 0); //Critere d'arret (ne pas enlever/modifier)

	AfficherResultats(Courante, LeProb, LAlgo);
	AfficherResultatsFichier(Courante, LeProb, LAlgo, "Resultats.txt");

	LibererMemoireFinPgm(Courante, Next, Best, LeProb);

	//system("PAUSE");

	return 0;
}

//*** A MODIFIER ***
//DESCRIPTION: Creation d'une solution voisine a partir de la solution courante (uneSol) qui ne doit pas etre modifiee.
//Dans cette fonction, appel de la fonction AppliquerVoisinage() pour obtenir une solution voisine selon un TYPE DE VOISINAGE selectionne + Definition la STRATEGIE D'ORIENTATION (Parcours/Regle de pivot).
//Ainsi, si la ReGLE DE PIVOT necessite l'etude de plusieurs voisins (TailleVoisinage>1), la fonction "AppliquerVoisinage()" sera appelee plusieurs fois.
TSolution GetSolutionVoisine(const TSolution uneSol, TProblem unProb, TAlgo& unAlgo)
{
	//Type (structure) de voisinage : 	A MODIFIER... dans la fonction AppliquerVoisinage()							NB: selon la configuration presente: Il n'y a pas de modification/voisinage
	//Parcours dans le voisinage : 		A MODIFIER...(Deterministe, Aleatoire, Oriente ou partiellement Oriente)  	NB: selon la configuration presente: Il n'y a pas de modification/voisinage
	//Regle de pivot : 					A MODIFIER...(First-Impove, Best-Impove, k-Impove/Alea ou k-Improve/Best)  	NB: selon la configuration presente: First-Improve (k = TailleVoisinage)

	//Best-Improve
	TSolution unVoisin, unAutreVoisin;
	int i;

	i = 0;
	while (i <= unAlgo.TailleVoisinage)
	{
		if (unAlgo.CptEval > (unAlgo.NB_EVAL_MAX * 0.75)) {
			unVoisin = AppliquerVoisinage(uneSol, unProb, unAlgo, (int)unProb.N / 6);
		}
		else {
			unVoisin = AppliquerVoisinage(uneSol, unProb, unAlgo, (int)unProb.N / 2);

		}
		if (unVoisin.FctObj < unAutreVoisin.FctObj) unAutreVoisin = unVoisin;
		else i++;
	};


	return unAutreVoisin;
}

//*** A MODIFIER *** selon le type (structure) de voisinage selectionne : echange, insertion, inversion ss-seq, etc.
//DESCRIPTION: Fonction appliquant le type de voisinage selectionne. La fonction retourne la solution voisine obtenue suite a l'application du type de voisinage.
//NB: La solution courante (uneSol) ne doit pas etre modifiee (const)
TSolution AppliquerVoisinage(const TSolution uneSol, TProblem unProb, TAlgo& unAlgo, int boucle)
{
	//Type (structure) de voisinage : 	A DETERMINER...	
	TSolution Copie;

	//Utilisation d'une nouvelle TSolution pour ne pas modifier La solution courante (uneSol)
	Copie = uneSol;

	for (int i = 0; i < boucle; i++) {
		// Générer deux indices aléatoires
		int idx1 = std::rand() % unProb.N;
		int idx2 = std::rand() % unProb.N;

		// S'assurer que les deux indices sont différents et bien ordonnés
		while (idx1 == idx2) {
			idx2 = std::rand() % unProb.N;
		}

		// S'assurer que idx1 est inférieur à idx2
		if (idx1 > idx2) {
			std::swap(idx1, idx2);
		}

		// Inverser la sous-séquence entre idx1 et idx2 (inclus)
		std::swap(Copie.Site[idx1], Copie.Site[idx2]);
	}

	//Le nouveau voisin doit etre evalue et retourne
	EvaluerSolution(Copie, unProb, unAlgo);
	return(Copie);

}