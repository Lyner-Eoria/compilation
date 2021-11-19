#include "Kit.h"
#include "Alex.h"
#include "Erreur.h"
#include "Tds.h"
//module d'analyse lexicale
#define knAEFetatMaX 14
#define knByteLiM 256
//ci-dessous, ne toucher ni à "ccAlphA,ccButoiR", ni à "ccBlanC,ccDiverS,ccOmegA". Insérer les nouvelles classes dans ]ccButoiR,ccBlanC[
enum eCaractereClassE {ccAlphA,ccButoiR,ccDeciT,ccMajusculE,ccMinusculE,ccPoinT,ccSeparateuR,ccApostrophE,ccGuillemeT,ccOperateuR,ccParenthesE,ccBlanC,ccDiverS,ccOmegA};
#define knClasseCaractereMaX (ccOmegA-ccAlphA+1)
#define kuCodeSourceLgMaX 200
#define knLexemeMaX	511
#define knTrancheMaX 147
#define kbVoiR kF

int iAEF[1+knAEFetatMaX][1+knClasseCaractereMaX];
int bAlexAmorceR;
int ccCaractereClassE[knByteLiM];
char *sCodeSourcE;//alloué dans AlexAMORCER
//ci-dessous, ne toucher ni à "lcAlphA,lcBlanC,lcInconnU", ni à "lcFinDeFichieR,lcOmegA". Insérer les nouvelles classes dans ]lcInconnU,lcFinDeFichieR[
enum  eLexemeClassE   {lcAlphA,lcBlanC,lcInconnU,lcEntieR,lcReeL,lcIdentificateuR,lcOperateuR,lcParenthesE,lcSeparateuR,lcTextE,lcFinDeFichieR,lcMotCleF,lcOmegA};
char *sLexemeClassE  ="lcAlphA,lcBlanC,lcInconnU,lcEntieR,lcRéeL,lcIdentificateuR,lcOperateuR,lcParenthesE,lcSeparateuR,lcTextE,lcFinDeFichieR,lcMotCleF,lcOmegA";
int nLexemeEnTouT;
struct rLexemE{int classe;int indexdebut;int indexfin;int ligne;int valeur;} lxmChainE[1+knLexemeMaX];//la chaîne lexicale est générée dans dans lxmChainE
int Tds;

void AutomateAffecteR(int bVoir);
void ClasseCaractereDefiniR(int bVoir);
int bCodeSourceAnalyseR(int bVoiR);
void CodeSourceTrancheR(int nDebutInclus,int nFinExclue,int bVoir,char *sTranche);
char *sLexemE(int nLexeme);
void LexemeEmpileR(int uLxmIndex,int nLxmClasse,int nLxmDebutInclus,int nLxmFinExclue, int bVoir,int *pnErreurLexicale);
void LexemeVoiR();
int nMotCleF();

void AlexAfficher(){
	//affiche le code source ligne par ligne
	const int kbNumeroter = kV;
	int nC,nLg,iLigne,bSeparateur;
	//t("AlexAfficher");
	for (nC=0,iLigne=1,nLg=nChaineLg(sCodeSourcE);nC<nLg && (sCodeSourcE[nC]!='$');nC++){
		bSeparateur=(sCodeSourcE[nC]==';');
		if (bSeparateur){
			ErreurAfficher(abs(iLigne));
			printf("\n");
			iLigne=abs(iLigne)+1;//ie il faudra afficher iLigne avant d'afficher le prochain caractère du code source
		}else {
			if (kbNumeroter && iLigne>0){
				printf("Ligne %d: ",iLigne);
				iLigne= - iLigne;//négatif: le numéro iLigne a bien été affiché
			}
			printf("%c",sCodeSourcE[nC]);
		}
	}
	printf("\n");
}//AlexAfficher

void AlexAMORCER(){
	//amorce le présent module une fois pour toutes (donc sa complexité n'est pas critique)
	int kbVoir=kF;
	char Tds;
	AutomateAffecteR(kbVoir);
	ClasseCaractereDefiniR(kbVoir);
	sCodeSourcE=malloc(1+kuCodeSourceLgMaX);
	bBof = bTdsAllouer(1,&Tds);
	bTdsAjouter(Tds, "si");
	bTdsAjouter(Tds, "alors");
	bTdsAjouter(Tds, "sinon");
	TdsVoir(Tds, "");
	bAlexAmorceR=kV;
}//AlexAMORCER

int bAlexAnalyser(char *sCode){
	int bAnalyser;
	//Appel0("bAlexAnalyser");
		strcpy(sCodeSourcE,sCode);
		printf("   Code source analysé:\"%s\".\n",sCodeSourcE);
		//AlexAfficher();
		bAnalyser=bCodeSourceAnalyseR(1|| kbVoiR);
	//Appel1("bAlexAnalyser");
	return(bAnalyser);
}//bAlexAnalyser

char *sAlexChaine(int nLexeme){
	char *sChaine=sLexemE(nLexeme);
	//ds(nLexeme,sChaine);
	return(sChaine);
}//sAlexChaine

int nAlexChose(int uLexeme){
	//Chose->Somme || Produit || Terme 
	int nChose=0;
	if (bAlexLexeme(uLexeme)){
		if (bAlexOperateur(uLexeme+1,'+'))
			nChose=1;
		else if (bAlexOperateur(uLexeme+1,'*'))
			nChose=2;
		else if (nAlexTerme(uLexeme))
			nChose=3;
		else if (bAlexParentheseur(uLexeme,'('))
			nChose=4;
	};
	return(nChose);
}//nAlexChose

int bAlexDate(int uLexeme){
	//uLexeme est bien une date de l'année 2017 (l'année de l'ouverture de la bibliothèque) et au-delà donnée sous la forme JJMMAA.
	int bDate,nEntier,nJour,nMois,nAnnee;
	int nJourEnTout[1+12]={-1,    31,28,31,    30,31,30,    31,31,30,    31,30,31};
	//sd("bAlexDate",uLexeme);
	bDate=bAlexEntier(uLexeme);
	if (bDate){
		nEntier=mAlexEntier(uLexeme);
		nAnnee=nEntier%100;
		nEntier=(nEntier-nAnnee)/100;
		nMois=nEntier%100;
		nJour=(nEntier-nMois)/100;
		//teee("nJour,nMois,nAnnee",nJour,nMois,nAnnee);
		bDate=bCroit(1,nJour,31) && bCroit(1,nMois,12) && bCroit(17,nAnnee,99);
		bDate=bDate && nJour<=nJourEnTout[nMois] || (nJour==29 && nMois==2 && nAnnee%4==0);
	}
	return(bDate);
}//bAlexDate

void AlexDenoncer(int nLexeme){
	//affiche le numéro de la ligne de code source et la lexie à dénoncer suite à une erreur lexicale,syntaxique ou d'une autre nature.
	int nLexemeEnCause,uLxm,nLigne;
	char sLexeme[knByteLiM];
	//Appel0("AlexDenoncer");
		//nLigne:=rang1 de la ligne contenant le lexème de rang nLexeme
			//d2(nLexeme,nLexemeEnTouT);
			for (nLigne=1,uLxm=1;uLxm<nLexeme;uLxm++)
				if (lxmChainE[uLxm].classe==lcSeparateuR)
					nLigne++;
		nLexemeEnCause=iBorner(1,nLexeme,nLexemeEnTouT);//dc valide,quitte à choisir un voisin immédiat du lexème à dénoncer
		//d2(lxmChainE[nLexemeEnCause].indexdebut,lxmChainE[nLexemeEnCause].indexfin);
		CodeSourceTrancheR(lxmChainE[nLexemeEnCause].indexdebut,lxmChainE[nLexemeEnCause].indexfin,kF,/**/sLexeme);
		printf("   *** ligne %d,lexème n°%d %s",nLigne,nLexeme,sG(sLexeme));
	//Appel1("AlexDenoncer");
}//AlexDenoncer

int bAlexEntier(int uLexeme){
	int bEntier;
	bEntier=bAlexLexeme(uLexeme) && (lxmChainE[uLexeme].classe==lcEntieR);
	return(bEntier);
}//bAlexEntier

int nAlexEntier(int uLexeme) {
	int nEntier=0;
	if(bAlexLexeme(uLexeme)) {
		nEntier=nAlexSuite(uLexeme+1)?8:6;
	}
	return nEntier;
}

int mAlexEntier(int uLexeme){
	int nApostrophe,uBase,nC,nCarMax,nDecit,mEntier=0,bValide;
	if (bAlexEntier(uLexeme)){
		//s(sLexemE(uLexeme));
		//sscanf(sLexemE(uLexeme),"%d",&nEntier);
		//Titrer("##########");
		nApostrophe=mChainePos('\'',sLexemE(uLexeme));
		bValide=nApostrophe<0 || nApostrophe>0 && mChainePos('\'',sChainon(sLexemE(uLexeme),nApostrophe+1,100))<0;
		//sd2(sLexemE(uLexeme),nApostrophe,bValide);
		//nChainePos(sLexemE(uLexeme),'\');
		//d(nEntier);
		if (bValide){
			nCarMax=nChaineLg(sLexemE(uLexeme));
			//d(nCarMax);
			if (nApostrophe>0){
				uBase=sLexemE(uLexeme)[nCarMax-1]-'0';
				Assert1("nAlexEntier",bCroit(1,uBase,9));
				nCarMax=nApostrophe-1;
			}else uBase=10;
			for (mEntier=0,nC=0;nC<nCarMax+(nApostrophe>0);nC++){
				nDecit=sLexemE(uLexeme)[nC]-'0';
				bValide=bValide && bCroit(0,nDecit,uBase-1);
				mEntier=mEntier*uBase+nDecit;
				//d4(nDecit,uBase,bValide,mEntier);
			}
			//d(mEntier);
		};
		if (!bValide)
			mEntier=-1;
	}
	return(mEntier);
}//mAlexEntier

int nAlexEtat(int uLexeme){
	//rend la pré-vision de l'instruction courante:  1 si "achat", 2 si "stock", et 0 si échec
	//Etat->"achat" || "stock"
	int nSuite=0;
	if (bAlexLexeme(uLexeme)){
		switch (nMotCleF(sLexemE(uLexeme))){
			case  mcAchat:	nSuite=1;break;//"achat"
			case  mcStock:	nSuite=2;break;//"stock"
		}
	};
	return(nSuite);
}//nAlexEtat

int nAlexExpression(int uLexeme){
	//rend la pré-vision de l'instruction courante:  1 si somme, 2 si produit, 3 si puissance, 4 si terme et 0 si échec
	int nExpression=0;
	if (bAlexLexeme(uLexeme)){
		if (bAlexOperateur(uLexeme+1,'+')) {
			nExpression=1;
		}
		else if (bAlexOperateur(uLexeme+1,'*')) {
			nExpression=2;
		}
		else if (bAlexOperateur(uLexeme+1,'#')) {
			nExpression=3;
		}
		else if (nAlexTerme(uLexeme)) {
			nExpression=4;
		}
	};
	return(nExpression);
}//nAlexExpression

int bAlexFinFichier(int uLexeme){
	int bFinFichier;
	bFinFichier=(uLexeme>=nLexemeEnTouT);
	return(bFinFichier);
}//bAlexFinFichier

int bAlexHoraire(int uLexeme){
	#define kuHeureHeurePointMinuteMinuteLg 5
	int bHoraire;
	float rHoraire;
	int nMinute;
	char sLexeme[knByteLiM];
	bHoraire=bAlexReel(uLexeme);
	if (bHoraire){
		CodeSourceTrancheR(lxmChainE[uLexeme].indexdebut,lxmChainE[uLexeme].indexfin,kF,/**/sLexeme);
		//tt("sLexeme",sG(sLexeme));
		bHoraire=bHoraire && (nChaineLg(sLexeme)==kuHeureHeurePointMinuteMinuteLg);
		if (bHoraire){
			sscanf(sLexeme,"%f5.2",&rHoraire);
			//tr("rHoraire",rHoraire);
			nMinute=trunc(rHoraire*100);
			nMinute=nMinute%100;
			//d(nMinute);
			bHoraire=bHoraire && bCroit(0,nMinute,59);
			//compléter le code pour valider les heures
		}
	}
	return(bHoraire);
}//bAlexHoraire

int bAlexIdentificateur(int uLexeme){
	int bIdentificateur;
	bIdentificateur=bAlexLexeme(uLexeme) && lxmChainE[uLexeme].classe==lcIdentificateuR;
	return(bIdentificateur);
}//bAlexIdentificateur

void AlexINITIALISER(){//O(?)
	//relance le présent module
	Assert1("AlexINITIALISER",bAlexAmorceR);
}//AlexINITIALISER

int nAlexInstruction(int uLexeme){
	//rend la pré-vision de l'instruction courante:  1 si "?", 2 si "!", 3 si identificateur, et 0 si échec
	//Instruction->Emprunt || Reservation || Achat || Retour || Radiation || Autre 
	int nInstruction=0;
	if (bAlexLexeme(uLexeme)){
		switch (lxmChainE[uLexeme].classe){
			case  lcOperateuR:		switch (sCodeSourcE[lxmChainE[uLexeme].indexdebut]){
										case '<':	nInstruction=1;break;//Emprunt
										case '>':	nInstruction=4;break;//Retour
										case '?':	nInstruction=2;break;//Reservation
										case '#': 	nInstruction=10;break;
									};break;
			case  lcParenthesE:		switch (sCodeSourcE[lxmChainE[uLexeme].indexdebut]){
										case '(':	nInstruction=6;break;//Affectation
										case '[':	nInstruction=7;break;//Affichage
									};break;
	//Programme->Instructions || FinFichier
	//		Instructions->Instruction Programme
	//Instruction->Emprunt || Reservation || Achat || Retour || Radiation || Affectation || Affichage || Autre 
	//		Emprunt->"<" Livre Nom
	//		Reservation->"?" Livre Nom
	//		Achat->Prix Livre
	//		Retour->">" Livre Nom
	//		Radiation->Nom
	//		Autre->Entier Suite
	//Suite->Interdiction || Inscription
	//		Interdiction->Livre
	//		Inscription->Nom
			case  lcEntieR:	nInstruction=nAlexEntier(uLexeme);break;//Autre
			case  lcReeL:	nInstruction=3;break;//Achat
			case  lcIdentificateuR:	nInstruction=5;break;//Radiation
			case  lcMotCleF: nInstruction=9;break;//Etat
		}
	};
	//d(nInstruction);
	return(nInstruction);
}//nAlexInstruction

int bAlexLexeme(int uLexeme){
	int bLexeme=bCroit(1,uLexeme,nLexemeEnTouT);
	return(bLexeme);
}//bAlexLexeme

int nAlexLigne(int uLexeme){
	//rend le rang1 de la ligne qui contient le lexème d'index uLexeme
	int nLigne;
	Assert1("nAlexLigne",bCroit(1,uLexeme,nLexemeEnTouT));
	nLigne=lxmChainE[uLexeme].ligne;
	return(nLigne);
}//nAlexLigne

int bAlexOperateur(int uLexeme,char cOperateur){
	int bOperateur=bAlexLexeme(uLexeme) && lxmChainE[uLexeme].classe==lcOperateuR && sCodeSourcE[lxmChainE[uLexeme].indexdebut]==cOperateur;
	return(bOperateur);
}//bAlexOperateur

int bAlexParentheseur(int uLexeme,char cParenthese){
	int bParentheseur=bAlexLexeme(uLexeme) && lxmChainE[uLexeme].classe==lcParenthesE && sCodeSourcE[lxmChainE[uLexeme].indexdebut]==cParenthese;
	return(bParentheseur);
}//bAlexParentheseur

int nAlexProgramme(int uLexeme){
	int nProgramme=0;
	if (nAlexInstruction(uLexeme)>0)
		nProgramme=1;
	else if (bAlexFinFichier(uLexeme))
		nProgramme=2;
	return(nProgramme);
}//nAlexProgramme

int bAlexReel(int uLexeme){
	int bReel;
	bReel=bAlexLexeme(uLexeme) && lxmChainE[uLexeme].classe==lcReeL;
	return(bReel);
}//bAlexReel

float fAlexReel(int uLexeme){
	float fReel;
	Assert1("fAlexReel",bAlexReel(uLexeme));
	sscanf(sLexemE(uLexeme),"%f\n",&fReel);
	//tr("fReel",fReel);
	return(fReel);
}//fAlexReel

int bAlexSeparateur(int uLexeme){
	int bSeparateur;
	bSeparateur=bAlexLexeme(uLexeme) && lxmChainE[uLexeme].classe==lcSeparateuR;
	return(bSeparateur);
}//bAlexSeparateur

int nAlexSuite(int uLexeme){
	//rend la pré-vision de l'instruction courante:  1 si Interdiction, 2 si Inscription, et 0 si échec
	//Suite->Interdiction || Inscription
	//		Interdiction->Livre->Texte
	//		Inscription->Nom->Identificateur
	int nSuite=0;
	if (bAlexLexeme(uLexeme)){
		switch (lxmChainE[uLexeme].classe){
			case  lcTextE:			nSuite=1;break;//Interdiction
			case  lcIdentificateuR:	nSuite=2;break;//Inscription
		}
	};
	return(nSuite);
}//nAlexSuite

int nAlexTerme(int uLexeme){//pré-vision instruction courante: 1 si Entier 2 si Expression, et 0 si échec.
	//Terme->Entier || Identifiant   
	int nTerme=0;
	if (bAlexLexeme(uLexeme))
		switch (lxmChainE[uLexeme].classe){
			case  lcEntieR:			nTerme=1;break;
			case  lcIdentificateuR:	nTerme=2;break;
			/*case  lcParenthesE:		if (bAlexParentheseur(uLexeme,'('))
										nTerme=3;
									break;*/
		}
	return(nTerme);
}//nAlexTerme

void AlexTESTER(int iTest){
	//teste le présent module
	int bSucces;
	Appel0(sC2("AlexTESTER,test n°",sEnt(iTest)));
		sCodeSourcE="27+3.14;AB=5*8$";
		switch (iTest) {
			case  1:	sCodeSourcE="27 + 3.14 ; CD = 5 * 8 ' $";break;
			case  5:	sCodeSourcE="27+3.14;EF      =  5*8$";break;
			case  8:	sCodeSourcE="27+3.14;EF      =  5*8$$";break;
			case  4:	sCodeSourcE="$";break;
			case  3:	sCodeSourcE="";break;
			case  2:	sCodeSourcE="24 ; 5 ; 3...2 ; 4$";break;
			case  6:    sCodeSourcE="ALPHA1BETA22GAMMA333DELTA4444$$";break;
			default:	Assert1("AlexTESTER: entrée de case inconnue",0); break;
		}
		printf("sCodeSourcE=\"%s\".\n",sCodeSourcE);
		bSucces=bCodeSourceAnalyseR(kbVoiR);
		if (!bSucces)
			AlexDenoncer(0);
	Appel1(sC2("AlexTESTER,test n°",sEnt(iTest)));
}//AlexTESTER	

int bAlexTexte(int uLexeme){
	int bTexte;
	bTexte=bAlexLexeme(uLexeme) && lxmChainE[uLexeme].classe==lcTextE;
	return(bTexte);
}//bAlexTexte

void AutomateAffecteR(int bVoir){
	//affecte une fois pour toutes l'automate d'états finis iAEF[]
	char *sAEF[1+knAEFetatMaX];
	const int knPas=5;//nombre total de caractères disponibles pour spécifier l'état suivant ds la i-ième ligne de texte sAEF[ligne i]
	int nL,nC;
	if (bVoir) Appel0("AutomateAffecteR");
		//for (nL=0;nL<=knAEFetatMaX;nL++)
			//sAEF[nL]=malloc(knPas*(1+knClasseCaractereMaX));//pas de free plus bas, mais cette routine n'est appelée qu'une fois
	    //classes:         $  0..9  AZ   az    .    ;    '    "  operH ()  blc autre
		sAEF[ 0]="    0    9    3    5   14    2    7    2   12    6   10    1    2";//dispatcheur vers les sous-automates
		sAEF[ 1]="    0    0    0    0    0    0    0    0    0    0    0    1    0";//lcBlanC
		sAEF[ 2]="   -2   -2   -2   -2   -2   -2   -2   -2   -2   -2   -2   -2    2";//lcInconnU
		sAEF[ 3]="   -3   -3    3   -3   -3   11   -3   13   -3   -3   -3   -3   -3";//lcEntieR, ou pas
		sAEF[ 4]="   -4   -4    4   -4   -4   -4   -4   -4   -4   -4   -4   -4   -4";//lcReeL
		sAEF[ 5]="   -5   -5    5    5    5   -5   -5   -5   -5   -5   -5   -5   -5";//lcIdentificateuR
		sAEF[ 6]="   -6   -6   -6   -6   -6   -6   -6   -6   -6   -6   -6   -6   -6";//lcOperateuR
		sAEF[ 7]="   -8   -8   -8   -8   -8   -8   -8   -8   -8   -8   -8   -8   -8";//lcSeparateuR
		sAEF[ 8]="   -9   -9   -9   -9   -9   -9   -9   -9   -9   -9   -9   -9   -9";//lcTextE
		sAEF[ 9]="  -10  -10  -10  -10  -10  -10  -10  -10  -10  -10  -10  -10  -10";//lcFinDeFichieR
		sAEF[10]="   -7   -7   -7   -7  -7    -7   -7   -7   -7   -7   -7   -7   -7";//lcParenthesE
		sAEF[11]="    0   -1    4   -1   -1  -20   -2   -1   -1   -1   -1   -1   -1";//un point a été rencontré;constante réelle attendue
		sAEF[12]="    0   -3   12   12   12   12   12   12    8   12   12   12   12";//un guillemet a été rencontré;guillemet fermant attendu
		sAEF[13]="    0   -3    3   12   12   12   12   12    8   12   12   12   12";//une apostrophe a été rencontrée;décit attendu
		sAEF[14]="  -11  -11  -11  -11   14  -11  -11  -11  -11  -11  -11  -11  -11";//lcMotCleF
//enum eLexemeClassE{lcAlphA,lcBlanC,lcInconnU,lcEntieR,lcReeL,lcIdentificateuR,lcOperateuR,lcParenthesE,lcSeparateuR,lcTextE,lcFinDeFichieR,lcOmegA};
		for (nL=0;nL<=knAEFetatMaX;nL++){
			for (nC=0;nC<=knClasseCaractereMaX;nC++){
				sscanf(sAEF[nL]+knPas*nC,"%d\n",&iAEF[nL][nC]);
				if (bVoir)
					printf("[%2d,%2d]=%2d ",nL,nC,iAEF[nL][nC]);
			}
			if (bVoir)
				printf("\n");
		}
	if (bVoir) Appel1("AutomateAffecteR");
}//AutomateAffecteR

void ClasseCaractereDefiniR(int bVoir){
	//définit une fois pour toutes la classe ClasseDeC au caractère C en mémorisant ClasseDeC dans ccCaractereClassE[C] pour tout C.
	if (bVoir) Appel0("ClasseCaractereDefiniR");
		char *sOperateur = "<>?+-*/%%&#";
		int nC,nClasse;
		for (nC=0;nC<knByteLiM;nC++){
			nClasse=ccDiverS;
			if (isdigit(nC)) nClasse=ccDeciT;
			if (isupper(nC)) nClasse=ccMajusculE;
			if (islower(nC)) nClasse=ccMinusculE;
			if (nC=='.') nClasse=ccPoinT;
			if (nC=='\'') nClasse=ccApostrophE;
			if (nC==';') nClasse=ccSeparateuR;
			if (nC=='"') nClasse=ccGuillemeT;
			//if ( (nC=='<') || (nC=='>') || (nC=='?') || (nC=='+') || (nC=='*') ) nClasse=ccOperateuR;
			if(mChainePos(nC, sOperateur) != -1) nClasse = ccOperateuR;
			if ( (nC=='(') || (nC==')') || (nC=='[') || (nC==']') ) nClasse=ccParenthesE;
			if (nC==' ') nClasse=ccBlanC;//blanc ou assimilé
			if (nC=='$') nClasse=ccButoiR;
			ccCaractereClassE[nC]=nClasse;
			if (bVoir)
				printf("caractère %3d    classe %2d \n",nC,nClasse);
		}
	if (bVoir) Appel1("ClasseCaractereDefiniR");
}//ClasseCaractereDefiniR

int bCodeSourceAnalyseR(int bVoiR){
	//analyse le code source censé figurer dans sCodeSourcE[] et empile dans lxmChainE la chaîne lexicale;rend faux ssi un lexème inconnu est empilé
	int bAnalyser,nC,nClasse,nClim,nErreurLexicale,nEtat,nEtat0,nDebut,nLxm,lcType,bDetailAfficher=kF;
	//Appel0("bCodeSourceAnalyseR");
		Assert1("bCodeSourceAnalyseR0",bAlexAmorceR);
		bAnalyser=kV;
		nLexemeEnTouT=0;
		nClim=nChaineLg(sCodeSourcE);
		for (nEtat=0,nC=0,nDebut=nC;nC<=nClim;nC++){// *** donc atteint et lit un caractère "\0"
			nClasse=ccCaractereClassE[sCodeSourcE[nC]];
			nEtat0=nEtat;
			nEtat=iAEF[nEtat0][nClasse];
			if (bDetailAfficher)
				printf("Itération n°%d: à l'état courant %d, '%c' est lu, la classe de ce caractère vaut %d, donc iAEF[%d][%d] rend l'état %d.\n",nC,nEtat0,sCodeSourcE[nC],nClasse,nEtat0,nClasse,nEtat);
			//d2(nEtat,bAnalyser);
			if (nEtat<=0){
				if (nEtat0!=lcBlanC){//empiler le lexème courant dans la chaîne lexicale de sommet nLexemeEnTouT
					lcType=-nEtat;
					nErreurLexicale=(nEtat==-20)? eAlexReel: ((lcType==lcInconnU)?eAlexInconnu:0);
					//d3(nEtat,nErreurLexicale,eAlexReel);
					if (nEtat<=-20)
						lcType=lcInconnU;
					//d3(nDebut,nC,lcType);
					Assert1("bCodeSourceAnalyseR1",bCroitStrict(lcAlphA,lcType,lcOmegA));
					LexemeEmpileR(++nLexemeEnTouT,lcType, nDebut, nC,bDetailAfficher,&nErreurLexicale);
					if (nErreurLexicale )
						ErreurEmpiler(nErreurLexicale,nLexemeEnTouT);
					bAnalyser=bAnalyser && (lcType!=lcInconnU);
				} else if (bDetailAfficher)
					printf("\t\t Suite de blancs: aucun lexème n'est empilé.\n");
				nDebut=nC;//saute donc aussi les blancs éventuels
				nEtat=iAEF[nEtat0=0][nClasse];//inutile de relire le caractère qui vient d'être lu puisque sa classe est nClasse.
			}
		}
		if ((nLexemeEnTouT<=0) || (lxmChainE[nLexemeEnTouT].classe!=lcFinDeFichieR))
			LexemeEmpileR(++nLexemeEnTouT,lcFinDeFichieR, nDebut, nDebut+1, !k1Afficher,&nErreurLexicale);//assimile donc la lexie '\0' au '$'.
		if (k1Afficher)
			LexemeVoiR();
		if (k1Afficher && bVoiR &&!bAnalyser)
			t(sC3b("      Il",sUn("y a",!bAnalyser),"lexème inconnu dans la chaîne lexicale"));
		//d(nClim);
		//d(lcFinDeFichieR);
		Assert2("bCodeSourceAnalyseR2",nLexemeEnTouT>0,(!bAnalyser || lxmChainE[nLexemeEnTouT].classe==lcFinDeFichieR));
		//tb("bAnalyser",bAnalyser);
	//Appel1("bCodeSourceAnalyseR");
	//bAnalyser:=aucun lexeme inconnu n'a été empilé
		for (bAnalyser=1,nLxm=1;nLxm<=nLexemeEnTouT;nLxm++)
			bAnalyser=bAnalyser && (lxmChainE[nLxm].classe!=lcInconnU);
	return bAnalyser;
}//bCodeSourceAnalyseR

void CodeSourceTrancheR(int nDebutInclus,int nFinExclue,int bVoir,char *sTranche){
	//copie dans sTranche le fragment sCodeSourcE[nDebutInclus..nFinExclue[ ou rend sFinDeFichier afin que sTranche ne soit pas vide
	char *sFinDeFichier="Fin de fichier";
	int nC,bValide;
	bValide=bCroit(0,nDebutInclus,nFinExclue-1) && (nFinExclue-nDebutInclus<knTrancheMaX);
	if (bValide){
		for (nC=nDebutInclus;nC<nFinExclue;nC++)
			sTranche[nC-nDebutInclus]=sCodeSourcE[nC];
		sTranche[nFinExclue-nDebutInclus]='\0';
		if (nChaineLg(sTranche)==0)//copie d'un caractère '\0' dès nDebutInclus, caractéristique du lexème "fin de fichier"
			strcpy(sTranche,sFinDeFichier);
	} 
	else Assert1(sC5p("CodeSourceTrancheR:tranche [",sEnt(nDebutInclus),"..",sEnt(nFinExclue),"[ invalide"),bValide);
}//CodeSourceTrancheR

char *sLexemE(int uLexeme){
	//rend le fragment sCodeSourcE[nDebutInclus..nFinExclue[
	char *sLexeme=malloc(100);
	sLexeme[0]='\0';
	CodeSourceTrancheR(lxmChainE[uLexeme].indexdebut,lxmChainE[uLexeme].indexfin,!k1Voir,sLexeme);
	return(sLexeme);
}//sLexemE

void LexemeEmpileR(int uLxmIndex,int nLxmClasse,int nLxmDebutInclus,int nLxmFinExclue, int bVoir,int *pnErreurLexicale){
	char sLexeme[knByteLiM];
	//Appel0("LexemeEmpileR");
		Assert3("LexemeEmpileR1",uLxmIndex>0,bCroitStrict(lcAlphA,nLxmClasse,lcOmegA),bCroit(0,nLxmDebutInclus,nLxmFinExclue));
		lxmChainE[uLxmIndex].classe=nLxmClasse;
		lxmChainE[uLxmIndex].indexdebut=nLxmDebutInclus;
		lxmChainE[uLxmIndex].indexfin=nLxmFinExclue;
		//d3(uLxmIndex,nLxmDebutInclus,nLxmFinExclue);
		lxmChainE[uLxmIndex].ligne=(uLxmIndex==1)? 1: lxmChainE[uLxmIndex-1].ligne;
		if (uLxmIndex>1 && lxmChainE[uLxmIndex-1].classe==lcSeparateuR)
			lxmChainE[uLxmIndex].ligne++;
		CodeSourceTrancheR(nLxmDebutInclus,nLxmFinExclue,bVoir,/**/sLexeme);
		//s(sLexeme);
		if (nLxmClasse==lcEntieR && mAlexEntier(uLxmIndex)<0){
			lxmChainE[uLxmIndex].classe=lcInconnU;
			*pnErreurLexicale=eAlexEntieR;
		}
		if (nLxmClasse==lcMotCleF && !nMotCleF(sLexeme)){
			lxmChainE[uLxmIndex].classe=lcInconnU;
			*pnErreurLexicale=eAlexMotClef;
		}
		if (bVoir)
			printf("\t\t État négatif, donc un nouveau lexème (d'index %d) est empilé: (classe %s, lexie %s).\n",uLxmIndex,sItem(sLexemeClassE,nLxmClasse+1),sG(sLexemE(uLxmIndex)));
	//Appel1("LexemeEmpileR");
}//LexemeEmpileR

void LexemeVoiR(){
	printf("Chaine lexicale (%s):\n",sPluriel(nLexemeEnTouT,"lexème"));
	for (int uLxm=1;uLxm<=nLexemeEnTouT;uLxm++)
		printf("   Lexème n°%2d: %16s %s ligne %d.\n",uLxm,sChaine1(sItem(sLexemeClassE,lxmChainE[uLxm].classe+1),16),sG(sLexemE(uLxm)),lxmChainE[uLxm].ligne);
}//LexemeVoiR

int nMotCleF(char *sM) {
	if(bChaineEgale("stock", sM))
		return mcStock;
	if(bChaineEgale("achat", sM))
		return mcAchat;
	if(bChaineEgale("si", sM))
		return mcSi;
	
	return 0;
}

int bAlexMotClef(int uLexeme, int nMotClefNumero) { //o(N), N mots clés
	if(bAlexLexeme(uLexeme)&&lxmChainE[uLexeme].classe==lcMotCleF)
		if(nMotClefNumero > mcAlpha && nMotClefNumero < mcOmega) {
			return nMotClefNumero == nMotCleF(sLexemE(uLexeme));
		}
		else if(!nMotClefNumero){
			return 1;
		}
	return 0;
}