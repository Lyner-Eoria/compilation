#include "Kit.h"
#include "Alex.h"
#include "Asynt.h"
#include "Erreur.h"
#include "Tds.h"
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
enum  eGrammaireRegle {grAlpha,grProgramme,grInstructions,grInstruction,grEmprunt,grReservation,grAchat,grRestitution,grRadiation,grAutre,grSuite,grInterdiction,grInscription,grOmega};
char *sGrammaireReglE="grAlpha,grProgramme,grInstructions,grInstruction,grEmprunt,grReservation,grAchat,grRestitution,grRadiation,grAutre,grSuite,grInterdiction,grInscription,grOmega";
enum {eGenererAlphA,eGenererAchaT,eGenererListE,eGenererEtaT,eGenererOmegA};
#define bEclaireR kF
#define kbVoiR kF
int bGenerE=kF;
FILE *fGenerE;//nom du fichier (suffixe .c vital) destiné à recevoir le code généré par le compilateur
int nAchaT;
int bAppeL;
int nAPPEL;//nombre total d'appels associés aux règles terminales de la grammaire,ie bAlexIdentificateur(), etc
int nECHEC;
int bAsyntAmorceR;
int mAsyntAnalyseR;
int nCiblE;
int bkEclaireuR;
int nMargE;
int nModeGeneratioN;
char cNoM;
char *sMACHINE="> MACHINE A PILE:";

int bAchaT(int iDebut,int *piFin);
int bAffectatioN(int iDebut,int *piFin);
int bAffichagE(int iDebut,int *piFin);
void AppeL0(char *sRoutine,int iDebut);
void AppeL1(char *sRoutine,int iDebut,int iFin);
int bAutrE(int iDebut,int *piFin);
int bCrochetFermanT(int iDebut,int *piFin);
int bCrochetOuvranT(int iDebut,int *piFin);
int bChosE(int iDebut,int *piFin);
int bEmprunT(int iDebut,int *piFin);
int bEntieR(int iDebut,int *piFin);
void ErreR(int eErreurNumero,int iDebut);
void ErrerSouS(int bSucces,int eErreurNumero,int iDebut);
int bEtaT(int iDebut,int *piFin);
int bExpressioN(int iDebut,int *piFin);
int bFinFichieR(int iDebut,int *piFin);
void GenerationTermineR();
void GenerE0();
void GenerE1(char *sA);
void GenerE2(char *sA,char *sB);
void GenerE3(char *sA,char *sB,char *sC);
void GenerE4(char *sA,char *sB,char *sC,char *sD);
void GenerE5(char *sA,char *sB,char *sC,char *sD,char *sE);
void GenereR(char *sCodeOp,char *sCommentaire);
void GenererChainE(char *sCodeOp,char *sChaine,char *sCommentaire);
void GenererEnT(char *sCodeOp,int nValeur,char *sCommentaire);
void GenererReeL(char *sCodeOp,float fValeur,char *sCommentaire);
void GenererSTX(char *sCodeOp,char *sChaine,char *sCommentaire);
int bHorairE(int iDebut,int *piFin);//dédié contrôle sémantique
int bIdentifianT(int iDebut,int *piFin);
int bIdentificateuR(int iDebut,int *piFin);
int bInferieurStricT(int iDebut,int *piFin);
int bInscriptioN(int iDebut,int *piFin);
int bInstructioN(int iDebut,int *piFin);
int bInstructionS(int iDebut,int *piFin);
int bInterdictioN(int iDebut,int *piFin);
int bInterrogatioN(int iDebut,int *piFin);
int bLivrE(int iDebut,int *piFin);
int bNoM(int iDebut,int *piFin);
int bOperateuR(char cOperateur,int iDebut,int *piFin);
int bParentheseFermantE(int iDebut,int *piFin);
int bParentheseOuvrantE(int iDebut,int *piFin);
int bPriX(int iDebut,int *piFin);
int bProduiT(int iDebut,int *piFin);
int bProgrammE(int iDebut,int *piFin);
int bRadiatioN(int iDebut,int *piFin);
int bReeL(int iDebut,int *piFin);
int bReservatioN(int iDebut,int *piFin);
int bRestitutioN(int iDebut,int *piFin);
int bSeparateuR(int iDebut,int *piFin);
int bSommE(int iDebut,int *piFin);
int bSuitE(int iDebut,int *piFin);
int bSuperieurStricT(int iDebut,int *piFin);
int bTermE(int iDebut,int *piFin);
int bTextE(int iDebut,int *piFin);

int bAchaT(int iDebut,int *piFin){
	//Achat->Prix Livre
	int bAjouter,bSucces,iFin,iMilieu,nItem;
	char *sLivre;
	float fPrix;
	AppeL0("bAchaT",iDebut);
		bSucces=bPriX(iDebut,&iMilieu) && bLivrE(iMilieu,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
		if (bSucces && mAsyntAnalyseR==mAsyntSemantique){
			sLivre=sAlexChaine(iMilieu);
			//ss("bAchaT",sLivre);
			bAjouter=bTdsAjouter(cNoM,sLivre);
			Assert1("bAchaT899",bAjouter);
			if ( bTdsContient(cNoM,sLivre,&nItem) )
				TdsValuer(cNoM,nItem,0);//indique que ce livre est disponible.
		}
		if (bSucces && mAsyntAnalyseR==mAsyntGeneration){
			sLivre=sAlexChaine(iMilieu);
			//ss("bAchaT",sLivre);
			bAjouter=bTdsAjouter(cNoM,sLivre);
			Assert1("bAchaT77",bAjouter);
			if ( bTdsContient(cNoM,sLivre,&nItem) )
				TdsValuer(cNoM,nItem,0);//indique que ce livre est disponible.

		}
		if (bSucces && nModeGeneratioN==eGenererAchaT){
			sLivre=sAlexChaine(iMilieu);
			bAjouter=bTdsAjouter(cNoM,sLivre);
			Assert1("bAchaT74",bAjouter);
			if ( bTdsContient(cNoM,sLivre,&nItem) )
				TdsValuer(cNoM,nItem,0);//indique que ce livre est disponible.
			fPrix=fAlexReel(iDebut);
			GenererReeL("EMP",fPrix,"achat");
			nAchaT++;
		}
		if (bSucces && nModeGeneratioN==eGenererListE){
			sLivre=sAlexChaine(iMilieu);
			bAjouter=bTdsAjouter(cNoM,sLivre);
			Assert1("bAchaT96",bAjouter);
			if ( bTdsContient(cNoM,sLivre,&nItem) )
				TdsValuer(cNoM,nItem,0);//indique que ce livre est disponible.
			fPrix=fAlexReel(iDebut);
			GenererSTX("EMP",sLivre,"achat");
			nAchaT++;
		}
	AppeL1("bAchaT",iDebut,*piFin);
	return(bSucces);
}//bAchaT

int bAffectatioN(int iDebut,int *piFin){
	//Affectation->Expression Nom
	int bSucces,iFin,iX,bAjouter,nItem;
	char *sNom;
	AppeL0("bAffectatioN",iDebut);
		bSucces=bExpressioN(iDebut,&iX) && bNoM(iX,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
		if (bSucces && mAsyntAnalyseR==mAsyntSemantique){/**/
			//TdsVoir(cNoM,"avant affectation");
			sNom=sAlexChaine(iFin-1);
			//s(sNom);
			bAjouter=bTdsAjouter(cNoM,sNom);
			if ( !bTdsContient(cNoM,sNom,&nItem) )
				ErreR(eSemAffectation,iDebut);
			//TdsVoir(cNoM,"après affectation");
		}
		if (bSucces && mAsyntAnalyseR==mAsyntGeneration){/**/
			//TdsVoir(cNoM,"avant affectation");
			sNom=sAlexChaine(iFin-1);
			//s(sNom);
			bAjouter=bTdsAjouter(cNoM,sNom);
			if ( !bTdsContient(cNoM,sNom,&nItem) )
				ErreR(eSemAffectation,iDebut);
			//TdsVoir(cNoM,"après affectation");
		}
		if (bSucces && mAsyntAnalyseR==mAsyntGeneration/**/){
			sNom=sAlexChaine(iFin-1);
			bAjouter=bTdsAjouter(cNoM,sNom);
			GenerE3("        int",sNom,"=");
			for (int uLxm=iDebut+1;uLxm<iX-1;uLxm++)
				GenerE1(sAlexChaine(uLxm));
			GenerE1(";");
			GenerE0();
		}
		//d(bSucces);
	AppeL1("bAffectatioN",iDebut,*piFin);
	return(bSucces);
}//bAffectatioN

int bAffichagE(int iDebut,int *piFin){
	//bAffichagE->"[" Identifiant "]"
	int bAjouter,bSucces,iFin,iX,iY,nItem;
	char *sNom;
	AppeL0("bAffichagE",iDebut);
		bSucces=bCrochetOuvranT(iDebut,&iX) && bIdentifianT(iX,&iY) && bCrochetFermanT(iY,&iFin);
		//d(bSucces);
		*piFin=(bSucces)? iFin:iDebut;
		if (bSucces && mAsyntAnalyseR==mAsyntSemantique/**/){
			sNom=sAlexChaine(iDebut+1);
			if ( !bTdsContient(cNoM,sNom,&nItem) )
				ErreR(eSemAffichage,iDebut);
		}
		if (bSucces && mAsyntAnalyseR==mAsyntGeneration/**/){
			sNom=sAlexChaine(iDebut+1);
			if ( !bTdsContient(cNoM,sNom,&nItem) )
				ErreR(eSemAffichage,iDebut);
		}
		if (bSucces && mAsyntAnalyseR==mAsyntGeneration/**/){
			sNom=sAlexChaine(iDebut+1);
			bAjouter=bTdsAjouter(cNoM,sNom);
			GenerE5("        printf(\"",sNom,"= %d.\\n\",",sNom,");");
			GenerE0();
		}
	AppeL1("bAffichagE",iDebut,*piFin);
	return(bSucces);
}//bAffichagE

void AppeL0(char *sRoutine,int iDebut){
	if (bAppeL){
		printf("%s n°%d, %s",sTab(++nMargE),iDebut,sRoutine);
		if (bAlexLexeme(iDebut))
			printf(" %s",sG(sAlexChaine(iDebut)));
		Ligne();
		nAPPEL++;
	}
}//AppeL0

void AppeL1(char *sRoutine,int iDebut,int iFin){
	if (bAppeL){
		printf("%s%d…%d,%s\n",sTab(nMargE--),iDebut,iFin,sRoutine);
		if (iDebut==iFin) Titrer("ECHEC");
		if (iDebut==iFin) nECHEC++;
	}
}//AppeL1

void AsyntAMORCER(){
	//amorce le présent module une fois pour toutes (donc sa complexité n'est pas critique)
	bkEclaireuR=kF;
	bAsyntAmorceR=kV;
}//AsyntAMORCER

int bAsyntAnalyser(int amMode,int bAvecEclaireur){
	int bAnalyser;
	const int kbDebug= kF;
	int iFin;
	char *sGeneration="des achats,de la liste de livres,céfini";
	char *sGenere="Code.c";
	//Appel0(sC2b("bAsyntAnalyser en mode analyse",sChoix(mAsyntAnalyseR==amMode,"syntaxique","sémantique")));
		bkEclaireuR=bAvecEclaireur;
		mAsyntAnalyseR=amMode;
		nAPPEL=nECHEC=nMargE=0;
		bAppeL=1 && (mAsyntAnalyseR==mAsyntSyntaxique);
		nModeGeneratioN=0;
		if (mAsyntAnalyseR==mAsyntSyntaxique){ 
			Appel0(sC3b("Analyse syntaxique",sChoix(bkEclaireuR,"avec","sans"),"éclaireur"));
				bAnalyser=bProgrammE(1,&iFin) && (nErreurEnTout()==0);
			Appel1(sC3b("Analyse syntaxique",sChoix(bkEclaireuR,"avec","sans"),"éclaireur"));
			if (bAppeL)
				dd(nAPPEL,nECHEC);
		}
		if (mAsyntAnalyseR==mAsyntSemantique){ 
			bAnalyser=bTdsAllouer(kV,&cNoM);
				bAnalyser=bAnalyser && bProgrammE(1,&iFin) && (nErreurEnTout()==0);
				//d(nAPPEL);
				TdsVoir(cNoM,"TDS après analyse sémantique");
			bBof=bTdsAllouer(kF,&cNoM);
		}
		if (mAsyntAnalyseR==mAsyntGeneration){ 
			bAnalyser=bTdsAllouer(kV,&cNoM);
				bAnalyser=bAnalyser && bProgrammE(1,&iFin) && (nErreurEnTout()==0);
				//d(nAPPEL);
				TdsVoir(cNoM,"TDS après analyse sémantique");
			bBof=bTdsAllouer(kF,&cNoM);
		}
		if (mAsyntAnalyseR==mAsyntGeneration){
			Titrer("Génération du code");
			//printf("Indiquer par un chiffre le type de génération demandée (1:achat; 2:liste livres; 0 :annuler): ");
			//scanf("%d",&nModeGeneratioN);
			nModeGeneratioN=2;
			if (bCroit(1,nModeGeneratioN,2)){
				if (kbDebug) printf("Génération %s en cours...\n",sItem(sGeneration,nModeGeneratioN));
					nAchaT=0;
					nCiblE=50;
						bAnalyser=bTdsAllouer(kV,&cNoM);
							fGenerE=fopen(sGenere,"w");/**/
								Assert1("bAsyntAnalyser1",fGenerE!=0);/**/
								GenerE1("#include \"Kit.h\"");/**/
								GenerE0();/**/
								GenerE1("void CodeExecuter(){");/**/
								GenerE0();/**/
									GenerE1("    Appel0(\"CodeExecuter\");");/**/
									GenerE0();/**/
										//TdsVoir(cNoM,"TDS AVANT Generation");
											bAnalyser=bAnalyser && bProgrammE(1,&iFin) && (nErreurEnTout()==0);
										//d(bAnalyser);
										//d(nAPPEL);
										//TdsVoir(cNoM,"TDS après génération du code");
									GenerE1("    Appel1(\"CodeExecuter\");");/**/
									GenerE0();/**/
								GenerE1("}");/**/
								GenerE0();/**/
							iBof=fclose(fGenerE);/**/
						bBof=bTdsAllouer(kF,&cNoM);
					//GenerationTermineR();
				if (kbDebug) printf("Génération %s terminée...\n",sItem(sGeneration,nModeGeneratioN));
			}else printf("La génération du code est annulée conformément à votre demande.\n");
		}
	//Appel1(sC2b("bAsyntAnalyser en mode analyse",sChoix(mAsyntAnalyseR==amMode,"syntaxique","sémantique")));
	return(bAnalyser);
}//bAsyntAnalyser

void AsyntINITIALISER(){//O(?)
	//relance le présent module
	Assert1("AsyntINITIALISER",bAsyntAmorceR);
}//AsyntINITIALISER

void AsyntTESTER(int iTest){
	//teste le présent module
	int bAnalyser=kF;
	int iFin;
	Appel0(sC2("AsyntTESTER,test n°",sEnt(iTest)));
		nAPPEL=nECHEC=nMargE=0;
		bkEclaireuR=kV;
		switch (iTest) {
			case  1:	bkEclaireuR=kF;bAnalyser=bProgrammE(1,&iFin);break;
			case  2:	bkEclaireuR=kV;bAnalyser=bProgrammE(1,&iFin);break;
			default:break;
		}
		printf("%s de l'analyse syntaxique après %s %s.\n",(bAnalyser)? "Succès":"Echec",sPluriel(nAPPEL,"appel"),(nECHEC)? sPluriel(nECHEC,"échec"):"");
	Appel1(sC2("AsyntTESTER,test n°",sEnt(iTest)));
}//AsyntTESTER	

int bAutrE(int iDebut,int *piFin){
	//Autre->Entier Suite			  Entier est une durée>0 pr une Interdiction et une date pr une inscription
	int bSucces,iFin,iMilieu;
	AppeL0("bAutrE",iDebut);
		bSucces=bEntieR(iDebut,&iMilieu) && bSuitE(iMilieu,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
	AppeL1("bAutrE",iDebut,*piFin);
	return(bSucces);
}//bAutrE

int bChosE(int iDebut,int *piFin){
	//Chose->Somme || Produit || Terme || Expression
	int nChoix2,bSucces,iFin;
	AppeL0("bChosE",-iDebut);
		if (bkEclaireuR){
			nChoix2=nAlexChose(iDebut);//pré-vision instruction courante: 1 si Entier, 2 si Reservation, et 0 si échec.
			if (bEclaireR) tee("iDebut et nChoix2 pr Terme",iDebut,nChoix2);
			switch(nChoix2){
				case  1:	bSucces=bSommE(iDebut,&iFin);break;
				case  2:	bSucces=bProduiT(iDebut,&iFin);break;
				case  3:	bSucces=bTermE(iDebut,&iFin);break;
				case  4:	bSucces=bExpressioN(iDebut,&iFin);break;
				default:	ErreR(eSyntChosE,iDebut);break;
			}
		}else bSucces=bSommE(iDebut,&iFin) || bProduiT(iDebut,&iFin) || bTermE(iDebut,&iFin) || bExpressioN(iDebut,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
		//d2(3,*piFin);
		//d(bSucces);
	AppeL1("bChosE",iDebut,*piFin);
	return(bSucces);
}//bChosE

int bCrochetFermanT(int iDebut,int *piFin){
	//sd("bCrochetFermanT",iDebut);
	//s(sAlexChaine(iDebut));
	int bSucces=bAlexParentheseur(iDebut,']');
	*piFin=(bSucces)? iDebut+1:iDebut;
	//sd("bParentheseFermantE",*piFin);
	return(bSucces);
}//bCrochetFermanT

int bCrochetOuvranT(int iDebut,int *piFin){
	int bSucces=bAlexParentheseur(iDebut,'[');
	*piFin=(bSucces)? iDebut+1:iDebut;
	return(bSucces);
}//bCrochetOuvranT

int bDatatioN(int iDebut,int *piFin){
	//Datation->Horaire
	int bSucces,iFin;
	AppeL0("bDatatioN",iDebut);
		bSucces=bReeL(iDebut,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
		//d(bSucces);
	AppeL1("bDatatioN",iDebut,*piFin);
	return(bSucces);
}//bDatatioN

int bEmprunT(int iDebut,int *piFin){
	//Emprunt->"<" Livre Nom
	const int knEmpruntDuree=30;
	int bConnu,bEmprunteurInscrit,iValeur,bSucces,iFin,iMilieu,iSuivant,nItem;
	char *sLivre;
	AppeL0("bEmprunT",iDebut);
		bSucces=bInferieurStricT(iDebut,&iMilieu) && bLivrE(iMilieu,&iSuivant) && bNoM(iSuivant,&iFin);
		if (bSucces && mAsyntAnalyseR==mAsyntSemantique){//Livre répertorié et Nom inscrit
			//TdsVoir("avant emprunt");
				sLivre=sAlexChaine(iMilieu);
				bConnu=bTdsPresent(cNoM,sLivre,&iValeur);
				if (bConnu){//répertorié = connu ET disponible
					//sd(sLivre,iValeur);
					if (iValeur==0){
						bEmprunteurInscrit=bTdsContient(cNoM,sAlexChaine(iSuivant),&iBof);
						if (!bEmprunteurInscrit)
							ErreR(eSemNonInscrit,iSuivant);
						else if ( bTdsContient(cNoM,sLivre,&nItem) )
							TdsValuer(cNoM,nItem,knEmpruntDuree);//ie sLivre est emprunté pour une durée de knEmpruntDuree jours maximum.
					} else ErreR(eSemLivrePris,iMilieu);
				}else ErreR(eSemLivreInconnu,iMilieu);
			//TdsVoir("après emprunt");
		}
		if (bSucces && mAsyntAnalyseR==mAsyntGeneration){//Livre répertorié et Nom inscrit
			//TdsVoir("avant emprunt");
				sLivre=sAlexChaine(iMilieu);
				bConnu=bTdsPresent(cNoM,sLivre,&iValeur);
				if (bConnu){//répertorié = connu ET disponible
					//sd(sLivre,iValeur);
					if (iValeur==0){
						bEmprunteurInscrit=bTdsContient(cNoM,sAlexChaine(iSuivant),&iBof);
						if (!bEmprunteurInscrit)
							ErreR(eSemNonInscrit,iSuivant);
						else if ( bTdsContient(cNoM,sLivre,&nItem) )
							TdsValuer(cNoM,nItem,knEmpruntDuree);//ie sLivre est emprunté pour une durée de knEmpruntDuree jours maximum.
					} else ErreR(eSemLivrePris,iMilieu);
				}else ErreR(eSemLivreInconnu,iMilieu);
			//TdsVoir("après emprunt");
		}
		*piFin=(bSucces)? iFin:iDebut;
		//d3(iDebut,bSucces,*piFin);
	AppeL1("bEmprunT",iDebut,*piFin);
	return(bSucces);
}//bEmprunT

int bEntieR(int iDebut,int *piFin){
	int bSucces;
	AppeL0("bEntieR",iDebut);
		bSucces=bAlexEntier(iDebut);
		*piFin=(bSucces)? iDebut+1:iDebut;
		//tee("bEntieR debut et fin",iDebut,*piFin);
	AppeL1("bEntieR",iDebut,*piFin);
	return(bSucces);
}//bEntieR

void ErreR(int eErreurNumero,int iDebut){
	//empile  l'erreur eErreurNumero ssi le mode correspondant à eErreurNumero est actif
	if (mAsyntAnalyseR==mAsyntSyntaxique && bCroitStrict(eSynt0,eErreurNumero,eSynt1) )
		ErreurEmpiler(eErreurNumero,iDebut);
	if (mAsyntAnalyseR==mAsyntSemantique && bCroitStrict(eSem0,eErreurNumero,eSem1) )
		ErreurEmpiler(eErreurNumero,iDebut);
	if (mAsyntAnalyseR==mAsyntGeneration && bCroitStrict(eSem0,eErreurNumero,eSem1) )
		ErreurEmpiler(eErreurNumero,iDebut);
}//ErreR

void ErrerSouS(int bSucces,int eErreurNumero,int iDebut){
	if (!bSucces){
		//if (eErreurNumero==eSyntSeparateur) tee("séparateur manque",eErreurNumero,iDebut);
		ErreR(eErreurNumero,iDebut);
	}
}//ErrerSouS

int bEtaT(int iDebut,int *piFin){
	//Etat->"achat" || "stock"
	int bSucces=kF,iFin=0,nChoix2,bAjouter;
	char *sNom;
	AppeL0("bEtaT",-iDebut);
		if (bkEclaireuR){
			nChoix2=nAlexEtat(iDebut);//pré-vision instruction courante: 1 si "achat", 2 si "stock", et 0 si échec.
			if (bEclaireR) tee("iDebut et nChoix2 pr bEtaT",iDebut,nChoix2);
			switch(nChoix2){
				case 1:	bSucces=bAlexMotClef(iDebut,mcAchat);break;
				case 2:	bSucces=bAlexMotClef(iDebut,mcStock);break;
				default:ErreR(eSyntEtat,iDebut);break;
			};
		}else bSucces=bAlexMotClef(iDebut,mcAchat) || bAlexMotClef(iDebut,mcStock);
		*piFin=(bSucces)? iDebut+1:iDebut;
		Assert1("bEtaT1",*piFin>0);
		if (bSucces && mAsyntAnalyseR==mAsyntGeneration/**/){
			int nItem=0,nValeur;
			switch(nChoix2) {
				case 1: TdsVoir(cNoM, "avant generation");
						GenerE1("	printf(\"ACHAT\\n\");");
						GenerE0();
						while(bTdsSymbole(cNoM,&nItem)) {
							sNom=sTdsSymbole(cNoM,nItem);
							GenerE3("		printf(\"%s\",",sNom,");");
							GenerE0();
						}
						GenerE1("	printf(\"FIN ACHAT\\n\");");
						GenerE0();
						break;
				case 2: TdsVoir(cNoM, "avant generation");
						GenerE1("	printf(\"STOCK\\n\");");
						GenerE0();
						while(bTdsSymbole(cNoM,&nItem)) {
							if(bTdsPresent(cNoM, sNom, &nValeur)) {
							sNom=sTdsSymbole(cNoM,nItem);
							GenerE3("		printf(\"%s\",",sNom,");");
							GenerE0();
							}
						}
						GenerE1("	printf(\"FIN STOCK\\n\");");
						GenerE0();
						break;
			}
		}
	AppeL1("bEtaT",iDebut,*piFin);
	return(bSucces);
}//bEtaT

int bExpressioN(int iDebut,int *piFin){
	//Expression->Somme || Produit || Puissance || Terme
	int nChoix2,bSucces,iFin;
	AppeL0("bExpressioN",-iDebut);
		if (bkEclaireuR){
			nChoix2=nAlexExpression(iDebut);//pré-vision instruction courante: 1 si Somme, 2 si Produit, 3 si Puissance, 4 si Terme et 0 si échec.
			if (bEclaireR) tee("iDebut et nChoix2 pr Terme",iDebut,nChoix2);
			switch(nChoix2){
				case  1:	bSucces=bSommE(iDebut,&iFin);break;
				case  2:	bSucces=bProduiT(iDebut,&iFin);break;
				case  3:	bSucces=bPuissancE(iDebut,&iFin);break;
				case  4:	bSucces=bTermE(iDebut,&iFin);break;
				default:	ErreR(eSyntExpression,iDebut);break;
			}
		}else bSucces=bSommE(iDebut,&iFin) || bProduiT(iDebut,&iFin) || bPuissancE(iDebut,&iFin) || bTermE(iDebut,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
		//d2(3,*piFin);
		//d(bSucces);
	AppeL1("bExpressioN",iDebut,*piFin);
	return(bSucces);
}//bExpressioN

/*
int bExpressioN(int iDebut,int *piFin){
	//Expression->"(" Chose ")"
	int bSucces,iFin,iX,iY;
	AppeL0("bExpressioN",iDebut);
		bSucces=bParentheseOuvrantE(iDebut,&iX) && bChosE(iX,&iY) && bParentheseFermantE(iY,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
		//d(bSucces);
	AppeL1("bExpressioN",iDebut,*piFin);
	return(bSucces);
}//bExpressioN
*/

int bFinFichieR(int iDebut,int *piFin){
	int bSucces;
	AppeL0("bFinFichieR",iDebut);
		bSucces=bAlexFinFichier(iDebut);
		*piFin=(bSucces)? iDebut+1:iDebut;
		//sd("bFinFichieR",iDebut);
	AppeL1("bFinFichieR",iDebut,*piFin);
	return(bSucces);
}//bFinFichieR

void GenerationTermineR(){
	if (nModeGeneratioN==1){
		for (int nAchat=nAchaT-1;nAchat>0;nAchat--)
			GenereR("ADD","calcul achat");
		GenererEnT("EMP",nCiblE,"");
		GenereR("STO","");
	}
	if (nModeGeneratioN==2){
		GenererEnT("EMP",nCiblE,"");
		GenereR("STO","");
	}
}//GenerationTermineR

void GenerE0(){
	printf("\n");
	if (bGenerE)
		fprintf(fGenerE,"\n");
}//GenerE0

void GenerE1(char *sA){
	printf("%s ",sA);
	if (bGenerE)
		fprintf(fGenerE,"%s ",sA);
}//GenerE1

void GenerE2(char *sA,char *sB){
	printf("%s %s ",sA,sB);
	if (bGenerE)
		fprintf(fGenerE,"%s %s ",sA,sB);
}//GenerE2

void GenerE3(char *sA,char *sB,char *sC){
	printf("%s %s %s ",sA,sB,sC);
	if (bGenerE)
		fprintf(fGenerE,"%s %s %s ",sA,sB,sC);
}//GenerE3

void GenerE4(char *sA,char *sB,char *sC,char *sD){
	printf("%s %s %s %s ",sA,sB,sC,sD);
	if (bGenerE)
		fprintf(fGenerE,"%s %s %s %s ",sA,sB,sC,sD);
}//GenerE4

void GenerE5(char *sA,char *sB,char *sC,char *sD,char *sE){
	printf("%s %s %s %s %s ",sA,sB,sC,sD,sE);
	if (bGenerE)
		fprintf(fGenerE,"%s %s %s %s %s ",sA,sB,sC,sD,sE);
}//GenerE5

void GenereR(char *sCodeOp,char *sCommentaire){
	printf("\t%s%9s%7s    ; %s\n",sMACHINE,sCodeOp," ",sCommentaire);
}//GenereR

void GenererChainE(char *sCodeOp,char *sChaine,char *sCommentaire){
	printf("\t%s%9s%7s    ; %s\n",sMACHINE,sCodeOp,sChaine,sCommentaire);
}//GenererChainE

void GenererEnT(char *sCodeOp,int nValeur,char *sCommentaire){
	printf("\t%s%9s%7d    ; %s\n",sMACHINE,sCodeOp,nValeur,sCommentaire);
}//GenererEnT

void GenererReeL(char *sCodeOp,float fValeur,char *sCommentaire){
	printf("\t%s%9s%7.2f    ; %s\n",sMACHINE,sCodeOp,fValeur,sCommentaire);
}//GenererReeL

void GenererSTX(char *sCodeOp,char *sChaine,char *sCommentaire){
	//empile STX après l'opérande 1 (adresse nCiblE), lui-même après l'opérande 2 (sChaine,tranche par tranche;la plus extrême a au moins un cButoir).
	int nCar,nTranche;
	char sQuad[1+4],cButoir='$';//cButoir='\0'
	int nTrancheEnTout=1+yArrondi(0.5+nChaineLg(sChaine))/4.0;
	//d(nTrancheEnTout);
	//opérande 2 à empiler en premier
		for (sQuad[4]='\0',nTranche=nTrancheEnTout-1;nTranche>=0;nTranche--){
			for (nCar=0;nCar<4;nCar++)
				sQuad[nCar]=(nTranche*4+nCar<nChaineLg(sChaine))? sChaine[nTranche*4+nCar] : cButoir;
			Assert1("GenererSTX",nTranche<nTrancheEnTout-1 || sQuad[3]==cButoir);
			printf("\t%s%9s%7s    ; %s\n",sMACHINE,sCodeOp,sQuad,sCommentaire);
		}
	//opérande 1 à empiler en second
		GenererEnT(sCodeOp,nCiblE,"");
	//code opération 1 à empiler en dernier
		GenereR("STX","");
	//actualiser nCiblE pour qu'il pointe sur un nouvel espace mémoire vierge, utilisable par exemple pour un autre STX
		nCiblE+=nTrancheEnTout*4;
}//GenererSTX

int bHorairE(int iDebut,int *piFin){
	int bSucces;
	AppeL0("bHorairE",iDebut);
	bSucces=bAlexReel(iDebut);
	if (bSucces && mAsyntAnalyseR==mAsyntSemantique){
		if (!bAlexHoraire(iDebut))
			ErreR(eSemHoraire,iDebut);
	}
	if (bSucces && mAsyntAnalyseR==mAsyntGeneration){
		if (!bAlexHoraire(iDebut))
			ErreR(eSemHoraire,iDebut);
	}
	*piFin=(bSucces)? iDebut+1:iDebut;
	AppeL1("bHorairE",iDebut,*piFin);
	return(bSucces);
}//bHoraire

int bIdentifianT(int iDebut,int *piFin){
	//Identifiant->Identificateur
	int bSucces,iFin,nItem;
	char *sNom;
	AppeL0("bIdentifianT",iDebut);
		bSucces=bIdentificateuR(iDebut,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
		if (bSucces && mAsyntAnalyseR==mAsyntSemantique){
			sNom=sAlexChaine(iDebut);//nom de la personne qui s'inscrit
			if (!bTdsContient(cNoM,sNom,&nItem))
				ErreR(eSemIdentifiant,iDebut);
			//d(nItem);
		}
		if (bSucces && mAsyntAnalyseR==mAsyntGeneration){
			sNom=sAlexChaine(iDebut);//nom de la personne qui s'inscrit
			if (!bTdsContient(cNoM,sNom,&nItem))
				ErreR(eSemIdentifiant,iDebut);
			//d(nItem);
		}
	AppeL1("bIdentifianT",iDebut,*piFin);
	return(bSucces);
}//bIdentifianT

int bIdentificateuR(int iDebut,int *piFin){
	int bSucces;
	AppeL0("bIdentificateuR",iDebut);
	bSucces=bAlexIdentificateur(iDebut);
	*piFin=(bSucces)? iDebut+1:iDebut;
	//sd("bIdentificateuR",iDebut);
	AppeL1("bIdentificateuR",iDebut,*piFin);
	return(bSucces);
}//bIdentificateuR

int bInferieurStricT(int iDebut,int *piFin){
	int bSucces;
	AppeL0("bInferieurStricT",iDebut);
	bSucces=bAlexOperateur(iDebut,'<');
	*piFin=(bSucces)? iDebut+1:iDebut;
	//sd("bInferieurStricT",iDebut);
	AppeL1("bInferieurStricT",iDebut,*piFin);
	return(bSucces);
}//bInferieurStricT

int bInscriptioN(int iDebut,int *piFin){
	//Inscription->Nom
	int bAjouter,nDate,bSucces,iFin,nItem;
	char *sNom;
	AppeL0("bInscriptioN",iDebut);
		bSucces=bNoM(iDebut,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
		if (bSucces && mAsyntAnalyseR==mAsyntSemantique){
			sNom=sAlexChaine(iDebut);//nom de la personne qui s'inscrit
			nDate=mAlexEntier(iDebut-1);//date d'inscription
			bAjouter=bTdsAjouter(cNoM,sNom);
			if (!bAjouter)//puisque la TDS ne déborde pas,il n'y a qu'une explication:le…
				ErreR(eSemInscrit,iDebut);//…sNom en question est déjà en TDS,donc la personne est déjà inscrite
			else if (bTdsContient(cNoM,sNom,&nItem))
				TdsValuer(cNoM,nItem,mAlexEntier(iDebut-1));
			if (!bAlexDate(iDebut-1))
				ErreR(eSemDate,iDebut-1);
			//d(nItem);
		}
		if (bSucces && mAsyntAnalyseR==mAsyntGeneration){
			sNom=sAlexChaine(iDebut);//nom de la personne qui s'inscrit
			nDate=mAlexEntier(iDebut-1);//date d'inscription
			bAjouter=bTdsAjouter(cNoM,sNom);
			if (!bAjouter)//puisque la TDS ne déborde pas,il n'y a qu'une explication:le…
				ErreR(eSemInscrit,iDebut);//…sNom en question est déjà en TDS,donc la personne est déjà inscrite
			else if (bTdsContient(cNoM,sNom,&nItem))
				TdsValuer(cNoM,nItem,mAlexEntier(iDebut-1));
			if (!bAlexDate(iDebut-1))
				ErreR(eSemDate,iDebut-1);
			//d(nItem);
		}
		//sd("bInscriptioN",iDebut);
	AppeL1("bInscriptioN",iDebut,*piFin);
	return(bSucces);
}//bInscriptioN

int bInstructioN(int iDebut,int *piFin){
	//Instruction->Emprunt || Reservation || Achat || Restitution || Radiation || Affectation || Affichage || Autre || Etat
	int nChoix9,bSucces=kF,iFin=0;
	Assert1("bInstructioN0",iDebut>0);
	AppeL0("bInstructioN",iDebut);
		if (bkEclaireuR){
			nChoix9=nAlexInstruction(iDebut);//pré-vision instruction courante: 1 si Emprunt, 2 si Reservation, etc, et 0 si échec.
			if (0 || bEclaireR) tee("iDebut et nChoix9 pr bInstructioN",iDebut,nChoix9);
			switch(nChoix9){
				case  1:	bSucces=bEmprunT(iDebut,&iFin);break;
				case  2:	bSucces=bReservatioN(iDebut,&iFin);break;
				case  3:	bSucces=bAchaT(iDebut,&iFin);break;
				case  4:	bSucces=bRestitutioN(iDebut,&iFin);break;
				case  5:	bSucces=bRadiatioN(iDebut,&iFin);break;
				case  6:	bSucces=bAffectatioN(iDebut,&iFin);break;
				case  7:	bSucces=bAffichagE(iDebut,&iFin);break;
				case  8:	bSucces=bAutrE(iDebut,&iFin);break;
				case  9:	bSucces=bEtaT(iDebut,&iFin);break;
				case 10: 	bSucces=bPuissancE(iDebut,&iFin);break;
				default:	ErreR(eSyntInstruction,iDebut);break;
			}
		}else bSucces=bEmprunT(iDebut,&iFin) || bReservatioN(iDebut,&iFin) || bAchaT(iDebut,&iFin) || bRestitutioN(iDebut,&iFin)
				|| bRadiatioN(iDebut,&iFin) || bAffectatioN(iDebut,&iFin) || bAffichagE(iDebut,&iFin) || bAutrE(iDebut,&iFin) || bEtaT(iDebut,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
		Assert1("bInstructioN1",*piFin>0);
	AppeL1("bInstructioN",iDebut,*piFin);
	return(bSucces);
}//bInstructioN

int bInstructionS(int iDebut,int *piFin){
	//Instructions->Instruction Separateur Programme
	int bSucces,iFin,iMilieu,iSuivant;
	AppeL0("bInstructionS",iDebut);
	bSucces=bInstructioN(iDebut,&iMilieu) && bSeparateuR(iMilieu,&iSuivant) && bProgrammE(iSuivant,&iFin);
	*piFin=(bSucces)? iFin:iDebut;
	AppeL1("bInstructionS",iDebut,*piFin);
	return(bSucces);
}//bInstructionS

int bInterdictioN(int iDebut,int *piFin){
	//Interdiction->Livre
	int nDuree,bSucces,iFin,nItem;
	char *sLivre;
	AppeL0("bInterdictioN",iDebut);
		bSucces=bLivrE(iDebut,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
		//sd("bInterdictioN",iDebut);
		if (bSucces && mAsyntAnalyseR==mAsyntSemantique){
			sLivre=sAlexChaine(iDebut);
			nDuree=mAlexEntier(iDebut-1);//durée en jours de l'interdiction (pour cause de réparation ou autre) de l'accès au livre
			if (nDuree>=100)
				ErreR(eSemDelaiTropLong,iDebut);
			else if (bTdsContient(cNoM,sLivre,&nItem))
				TdsValuer(cNoM,nItem,nDuree);
			else ErreR(eSemLivreInconnu,iDebut);
			//d(nItem);
			TdsVoir(cNoM,"après interdiction");
		}
		if (bSucces && mAsyntAnalyseR==mAsyntGeneration){
			sLivre=sAlexChaine(iDebut);
			nDuree=mAlexEntier(iDebut-1);//durée en jours de l'interdiction (pour cause de réparation ou autre) de l'accès au livre
			if (nDuree>=100)
				ErreR(eSemDelaiTropLong,iDebut);
			else if (bTdsContient(cNoM,sLivre,&nItem))
				TdsValuer(cNoM,nItem,nDuree);
			else ErreR(eSemLivreInconnu,iDebut);
			//d(nItem);
			TdsVoir(cNoM,"après interdiction");
		}
	AppeL1("bInterdictioN",iDebut,*piFin);
	return(bSucces);
}//bInterdictioN

int bInterrogatioN(int iDebut,int *piFin){
	int bSucces;
	AppeL0("bInterrogatioN",iDebut);
	bSucces=bAlexOperateur(iDebut,'?');
	*piFin=(bSucces)? iDebut+1:iDebut;
	//sd("bInterrogatioN",iDebut);
	AppeL1("bInterrogatioN",iDebut,*piFin);
	return(bSucces);
}//bInterrogatioN

int bLivrE(int iDebut,int *piFin){
	//Livre->Texte
	int bSucces,iFin;
	AppeL0("bLivrE",iDebut);
	bSucces=bTextE(iDebut,&iFin);
	*piFin=(bSucces)? iFin:iDebut;
	//Appel1("bLivrE");
	ErrerSouS(bSucces,eSyntLivre,iDebut);
	AppeL1("bLivrE",iDebut,*piFin);
	return(bSucces);
}//bLivrE

int bNoM(int iDebut,int *piFin){
	//Nom->Identificateur
	int bSucces,iFin;
	AppeL0("bNoM",iDebut);
		bSucces=bIdentificateuR(iDebut,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
		if (mAsyntAnalyseR==mAsyntSemantique)
			ErrerSouS(bSucces,eSyntNom,iDebut);
		if (mAsyntAnalyseR==mAsyntGeneration)
			ErrerSouS(bSucces,eSyntNom,iDebut);
	AppeL1("bNoM",iDebut,*piFin);
	return(bSucces);
}//bNoM

int bOperateuR(char cOperateur,int iDebut,int *piFin){
	//Operateur->cOperateur
	int bSucces;
	AppeL0("bOperateuR",iDebut);
		bSucces=bAlexOperateur(iDebut,cOperateur);
		*piFin=(bSucces)? iDebut+1:iDebut;
	AppeL1("bOperateuR",iDebut,*piFin);
	return(bSucces);
}//bOperateuR

int bParentheseFermantE(int iDebut,int *piFin){
	//sd("bParentheseFermantE",iDebut);
	//s(sAlexChaine(iDebut));
	int bSucces=bAlexParentheseur(iDebut,')');
	*piFin=(bSucces)? iDebut+1:iDebut;
	//sd("bParentheseFermantE",*piFin);
	return(bSucces);
}//bParentheseFermantE

int bParentheseOuvrantE(int iDebut,int *piFin){
	int bSucces=bAlexParentheseur(iDebut,'(');
	*piFin=(bSucces)? iDebut+1:iDebut;
	return(bSucces);
}//bParentheseOuvrantE

int bPriX(int iDebut,int *piFin){
	//Prix->Reel
	int bSucces,iFin;
	AppeL0("bPriX",iDebut);
		bSucces=bReeL(iDebut,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
		if (bSucces && mAsyntAnalyseR==mAsyntSemantique && fAlexReel(iDebut)<=0.0)
			ErreR(eSemPrix,iDebut);
		if (bSucces && mAsyntAnalyseR==mAsyntGeneration && fAlexReel(iDebut)<=0.0)
			ErreR(eSemPrix,iDebut);
	AppeL1("bPriX",iDebut,*piFin);
	return(bSucces);
}//bPriX

int bProduiT(int iDebut,int *piFin){
	//Produit->Terme "*" Expression  
	int bSucces,iFin,iX;
	AppeL0("bProduiT",iDebut);
		bSucces=bTermE(iDebut,&iX) && bAlexOperateur(iX,'*') && bExpressioN(iX+1,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
		//d(bSucces);
	AppeL1("bProduiT",iDebut,*piFin);
	return(bSucces);
}//bProduiT

int bProgrammE(int iDebut,int *piFin){
	//Programme->Instructions || FinFichier
	int bSucces,iFin,nChoix2;
	AppeL0("bProgrammE",-iDebut);
		//sd("bProgrammE:iDebut",iDebut);
		if (bkEclaireuR){
			if (bEclaireR) sd("iDebut pr bProgrammE",iDebut);
			nChoix2=nAlexProgramme(iDebut);
			if (bEclaireR) tee("iDebut et nChoix2 pr bProgrammE",iDebut,nChoix2);
			switch(nChoix2){
				case 1:	bSucces=bInstructionS(iDebut,&iFin);break;
				case 2:	bSucces=bFinFichieR(iDebut,&iFin);break;
				default:ErreR(eSyntProgramme,iDebut);break;
			};
		}else bSucces=bInstructionS(iDebut,&iFin) || bFinFichieR(iDebut,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
		//d4(77,iDebut,bSucces,*piFin);
	AppeL1("bProgrammE",iDebut,*piFin);
	return(bSucces);
}//bProgrammE

int bPuissancE(int iDebut,int *piFin){
	//Puissance->Terme "#" Expression
	int bSucces,iFin,iX;
	AppeL0("bPuissancE",iDebut);
		bSucces=bTermE(iDebut,&iX) && bAlexOperateur(iX,'#') && bExpressioN(iX+1,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
	AppeL1("bPuissancE",iDebut,*piFin);
	return(bSucces);
}//bPuissancE

int bRadiatioN(int iDebut,int *piFin){
	//Radiation->Nom
	int bSucces,iFin,nItem;
	char *sNom;
	AppeL0("bRadiatioN",iDebut);
		bSucces=bNoM(iDebut,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
		if (bSucces && mAsyntAnalyseR==mAsyntSemantique){
			//TdsVoir("avant radiation");
			sNom=sAlexChaine(iDebut);
			if (bTdsContient(cNoM,sNom,&nItem))
				TdsValuer(cNoM,nItem,0);
			else ErreR(eSemNonInscrit,iDebut);
			//TdsVoir("après radiation");
		}
		if (bSucces && mAsyntAnalyseR==mAsyntGeneration){
			//TdsVoir("avant radiation");
			sNom=sAlexChaine(iDebut);
			if (bTdsContient(cNoM,sNom,&nItem))
				TdsValuer(cNoM,nItem,0);
			else ErreR(eSemNonInscrit,iDebut);
			//TdsVoir("après radiation");
		}
	AppeL1("bRadiatioN",iDebut,*piFin);
	return(bSucces);
}//bRadiatioN

int bReeL(int iDebut,int *piFin){
	int bSucces;
	AppeL0("bReeL",iDebut);
	bSucces=bAlexReel(iDebut);
	*piFin=(bSucces)? iDebut+1:iDebut;
	//sd("bReeL",iDebut);
	AppeL1("bReeL",iDebut,*piFin);
	return(bSucces);
}//bReeL

int bReservatioN(int iDebut,int *piFin){
	//Reservation->"?" Livre Nom
	int bDefini1,bDefini2,bSucces,iFin,iMilieu,iSuivant,nValeur,nItem1,nItem2;
	char *sLivre,*sNom;
	AppeL0("bReservatioN",iDebut);
		bSucces=bInterrogatioN(iDebut,&iMilieu) && bLivrE(iMilieu,&iSuivant) && bNoM(iSuivant,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
		if (bSucces && mAsyntAnalyseR==mAsyntSemantique){
			//TdsVoir("avant réservation");
			sLivre=sAlexChaine(iSuivant);
			bDefini1=bTdsContient(cNoM,sLivre,&nItem1);
			if (!bDefini1)
				ErreR(eSemLivreInconnu,iDebut);
			else if ( bTdsPresent(cNoM,sLivre,&nValeur) )
					if (nValeur==0)
						ErreR(eSemLivreDispo,iDebut);
					else TdsValuer(cNoM,nItem1,0);
			sNom=sAlexChaine(iSuivant);
			bDefini2=bTdsContient(cNoM,sNom,&nItem2);
			if (!bDefini2)
				ErreR(eSemNonInscrit,iDebut);
			//TdsVoir("après réservation");
		}
		if (bSucces && mAsyntAnalyseR==mAsyntGeneration){
			//TdsVoir("avant réservation");
			sLivre=sAlexChaine(iSuivant);
			bDefini1=bTdsContient(cNoM,sLivre,&nItem1);
			if (!bDefini1)
				ErreR(eSemLivreInconnu,iDebut);
			else if ( bTdsPresent(cNoM,sLivre,&nValeur) )
					if (nValeur==0)
						ErreR(eSemLivreDispo,iDebut);
					else TdsValuer(cNoM,nItem1,0);
			sNom=sAlexChaine(iSuivant);
			bDefini2=bTdsContient(cNoM,sNom,&nItem2);
			if (!bDefini2)
				ErreR(eSemNonInscrit,iDebut);
			//TdsVoir("après réservation");
		}
	AppeL1("bReservatioN",iDebut,*piFin);
	return(bSucces);
}//bReservatioN

int bRestitutioN(int iDebut,int *piFin){
	//Restitution->">" Livre Nom
	int bDefini1,bDefini2,bSucces,iFin,iMilieu,iSuivant,nItem1,nItem2;
	char *sNom,*sLivre;
	AppeL0("bRestitutioN",iDebut);
		bSucces=bSuperieurStricT(iDebut,&iMilieu) && bLivrE(iMilieu,&iSuivant) && bNoM(iSuivant,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
		if (bSucces && mAsyntAnalyseR==mAsyntSemantique){
			//TdsVoir("avant restitution");
			sLivre=sAlexChaine(iMilieu);
			bDefini1=bTdsContient(cNoM,sLivre,&nItem1);
			if (!bDefini1)
				ErreR(eSemLivreInconnu,iDebut);
			else TdsValuer(cNoM,nItem1,0);
			sNom=sAlexChaine(iSuivant);
			bDefini2=bTdsContient(cNoM,sNom,&nItem2);
			if (!bDefini2)
				ErreR(eSemNonInscrit,iDebut);
			//TdsVoir("après restitution");
		}
		if (bSucces && mAsyntAnalyseR==mAsyntGeneration){
			//TdsVoir("avant restitution");
			sLivre=sAlexChaine(iMilieu);
			bDefini1=bTdsContient(cNoM,sLivre,&nItem1);
			if (!bDefini1)
				ErreR(eSemLivreInconnu,iDebut);
			else TdsValuer(cNoM,nItem1,0);
			sNom=sAlexChaine(iSuivant);
			bDefini2=bTdsContient(cNoM,sNom,&nItem2);
			if (!bDefini2)
				ErreR(eSemNonInscrit,iDebut);
			//TdsVoir("après restitution");
		}
	AppeL1("bRestitutioN",iDebut,*piFin);
	return(bSucces);
}//bRestitutioN

int bSeparateuR(int iDebut,int *piFin){
	int bSucces;
	AppeL0("bSeparateuR",iDebut);
	bSucces=bAlexSeparateur(iDebut);
	*piFin=(bSucces)? iDebut+1:iDebut;
	ErrerSouS(bSucces,eSyntSeparateur,iDebut);
	AppeL1("bSeparateuR",iDebut,*piFin);
	return(bSucces);
}//bSeparateuR

int bSommE(int iDebut,int *piFin){
	//Somme->TermE "+" Expression
	int bSucces,iFin,iX;
	AppeL0("bSommE",iDebut);
		bSucces=bTermE(iDebut,&iX) && bAlexOperateur(iX,'+') && bExpressioN(iX+1,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
	AppeL1("bSommE",iDebut,*piFin);
	return(bSucces);
}//bSommE

int bSuitE(int iDebut,int *piFin){
	//Suite->Interdiction || Inscription
	int bSucces=kF,iFin=0,nChoix2;
	AppeL0("bSuitE",-iDebut);
		if (bkEclaireuR){
			nChoix2=nAlexSuite(iDebut);//pré-vision instruction courante: 1 si Interdiction, 2 si Inscription, et 0 si échec.
			if (bEclaireR) tee("iDebut et nChoix2 pr bSuitE",iDebut,nChoix2);
			switch(nChoix2){
				case 1:	bSucces=bInterdictioN(iDebut,&iFin);break;
				case 2:	bSucces=bInscriptioN(iDebut,&iFin);break;
				default:ErreR(eSyntSuite,iDebut);break;
			};
		}else bSucces=bInterdictioN(iDebut,&iFin) || bInscriptioN(iDebut,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
		Assert1("bSuitE1",*piFin>0);
		//sd("bSuitE",iDebut);
	AppeL1("bSuitE",iDebut,*piFin);
	return(bSucces);
}//bSuitE

int bSuperieurStricT(int iDebut,int *piFin){
	int bSucces;
	AppeL0("bSuperieurStricT",iDebut);
	bSucces=bAlexOperateur(iDebut,'>');
	*piFin=(bSucces)? iDebut+1:iDebut;
	//sd("bSuperieurStricT",iDebut);
	AppeL1("bSuperieurStricT",iDebut,*piFin);
	return(bSucces);
}//bSuperieurStricT

int bTermE(int iDebut,int *piFin){
	//Terme->Entier || Identifiant
	int nChoix2,bSucces,iFin;
	AppeL0("bTermE",-iDebut);
		if (bkEclaireuR){
			nChoix2=nAlexTerme(iDebut);
			if (bEclaireR) tee("iDebut et nChoix2 pr Terme",iDebut,nChoix2);
			switch(nChoix2){
				case  1:	bSucces=bEntieR(iDebut,&iFin);break;
				case  2:	bSucces=bIdentifianT(iDebut,&iFin);break;
				default:	ErreR(eSyntTerme,iDebut);break;
			}
		}else bSucces=bEntieR(iDebut,&iFin) || bIdentifianT(iDebut,&iFin);
		*piFin=(bSucces)? iFin:iDebut;
		//d(bSucces);
	AppeL1("bTermE",iDebut,*piFin);
	return(bSucces);
}//bTermE

int bTextE(int iDebut,int *piFin){
	//Texte->lexème
	int bSucces;
	AppeL0("bTextE",iDebut);
	bSucces=bAlexTexte(iDebut);
	*piFin=(bSucces)? iDebut+1:iDebut;
	//sd("bTextE",iDebut);
	AppeL1("bTextE",iDebut,*piFin);
	return(bSucces);
}//bTextE

