#include "Kit.h"
#include "Abe.h"
#include "Alex.h"
#include "Asynt.h"
#include "Code.h"
#include "Deque.h"
#include "Erreur.h"
#include "Grammaire.h"
#include "Grhom.h"
#include "Graphe.h"
#include "Foret.h"
#include "Tas.h"
#include "Tds.h"
#include "Tri.h"

void ModuleAmorceR(){
	//amorce tous les modules (code à exécuter une fois pour toutes AVANT d'utiliser un quelconque module depuis le main)
	KitAMORCER();//NE PAS DECLASSER:doit toujours être appelé en premier
	//amorcer TOUS les modules autres que Kit mentionnés dans les include de main.c
		AbeAMORCER();
		TdsAMORCER();
		AlexAMORCER();
		AsyntAMORCER();
		ErreurAMORCER();
		GrammaireAMORCER();
		GrhomAMORCER();
		GrapheAMORCER();
		DequeAMORCER();
		ForetAMORCER();
		TasAMORCER();
		TriAMORCER();
}//ModuleAmorceR

void DiagnostiqueR(char *sContexte,int bSucces){
	printf("Le code source %s valide au niveau %s%s",sEst(bSucces),sContexte,sChoix0(!bSucces,sC2(" ",sP(sPluriel(nErreurEnTout(),"erreur")))));
	printf("%c\n",(bSucces)?'.':':');
}//DiagnostiqueR

void CompileR(int nSource){
	const int bEclaireur=kV;
	int bSourceConnu=kV,bSucces;
	char *sSource;
	//Grammaire algébrique LL(1):
		//Programme->Instructions || FinFichier
		//		Instructions->Instruction Separateur Programme
		//Instruction->Emprunt || Reservation || Achat || Restitution || Radiation || Affectation  || Affichage || Autre 
		//		Emprunt->"<" Livre Nom			  Livre répertorié et Nom en TDS
		//		Reservation->"?" Livre Nom		  Livre répertorié et Nom en TDS
		//		Achat->Prix Livre				  réel>0 et Livre non encore répertorié
		//		Restitution->">" Livre Nom		  Livre répertorié et Nom en TDS 
		//		Radiation->Nom					  Nom en TDS
		//		Affectation->Expression Nom       Nom en TDS
		//		Affichage->"[" Identifiant"]"	  Identifiant en TDS
		//		Autre->Entier Suite				  Entier est une durée>0 pr une Interdiction et une date pr une inscription
		//Suite->Interdiction || Inscription
		//		Interdiction->Livre				  Livre répertorié
		//		Inscription->Nom				  Nom hors TDS
		//		Expression->"(" Chose ")"
		//		Chose->Somme || Produit || Terme || Expression  
		//		Somme->Terme "+" Chose 
		//		Produit->Terme "*" Chose 
		//		Terme->Entier || Identifiant 
		//		Nom->Identificateur 
		//		Identifiant->Identificateur		déjà affecté,dc en TDS 
	
	switch (nSource){
		case 0: sSource="1234     (3)@T1 Trac;(3..14) ;52 Trec $";
				break;
		case 1: sSource="19.90 \"livre\";8.05 \"autrelivre\";240317 MICHEL;< \"livre\" MICHEL;stock;";
				break;
		case 2: //livre est acheté;LUI et ELLE s'inscrivent;Lui emprunte le livre et le restitue puis ELLE l'emprunte
				sSource="19.90 livre;8.05 autrelivre;240317 LUI;250317 ELLE; < livre LUI; >livre LUI; < livre ELLE;";//***********
				break;
		case 3: //1 bug sémantique: livre est acheté;LUI et ELLE s'inscrivent;Lui emprunte le livre donc ELLE ne peut pas l'emprunter
				sSource="19.90 livre; 240317 LUI; 250317 ELLE; < livre LUI; < livre ELLE;";
				break;
		case 4: //livre est acheté;LUI et ELLE s'inscrivent;Lui emprunte le livre donc ELLE ne peut pas l'emprunter
				sSource="19.90 livre; 240317 LUI; 50 livre; 2500 livre; < livre LUI; < livre ELLE;";
				break;
		case 7: sSource="240317 ELLE; 0.5 dune; < dune ELLE; 0 dune; < dune ELLE; > dune ELLE ; 440317 MOI  ; 3.5 machin ; 55 Truc ;$";
				break;
		case 8: sSource="240218 MOI; 0.5 livre; < livre MOI; 0 elle; < livre MOI; > livre MOI ; 440317 MOI  ; 3.5 machin ; 55 Truc ;$";
				break;
		default: bSourceConnu=kF;printf("Clause default: code source n°%d inconnu.\n",nSource);break;
	}
	bSucces=bSourceConnu;
	bSucces=bSucces && bAlexAnalyser(sSource); DiagnostiqueR("lexical",bSucces);
	bSucces=bSucces && bAsyntAnalyser(mAsyntSyntaxique,bEclaireur); DiagnostiqueR(sC2b("syntaxique",sP(sC2b((bEclaireur)?"avec":"sans","éclaireur"))),bSucces);
	bSucces=bSucces && bEclaireur && bAsyntAnalyser(mAsyntSemantique,bEclaireur); DiagnostiqueR("sémantique",bSucces);
	bSucces=bSucces && bEclaireur && bAsyntAnalyser(mAsyntGeneration,bEclaireur); DiagnostiqueR("génération du code",bSucces);
	ErreurAfficherTout();
	//if (bSucces)//lancer l'exécution du code C généré par le compilateur à partir de sSource
	//	CodeExecuter();
}//CompileR

int main (int argc, const char * argv[]) {
	Appel0("");//NE PAS TOUCHER; ce code doit toujours être placé ici, au début du main
		ModuleAmorceR();//NE PAS TOUCHER; ce code doit toujours suivre immédiatement Appel0("")
		CompileR(1);
		//TdsTESTER(2);
		//GrhomTESTER(11);
		//GrhomTESTER(12);
	Appel1("");//NE PAS TOUCHER; ce code doit toujours être placé ici, à la fin du main, juste avant le return()
    return 0;
}//main
