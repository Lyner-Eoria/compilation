#include "Kit.h"
#include "Tds.h"
//Table Des Symboles,ie table de hachage interne seule, donc sans index ni dico.
int bTdsAmorceR;//ie le module a effectivement été amorcé
int nTdsVoiR;//dédié TdsVoir();nombre courant d'appels de la routine TDdsVoir()
//#define kuItemLiM 8191
//#define kuItemLiM 97 //47
#define kuItemLiM 8191 //2**13-1
//#define kuItemLiM 131071 //2**17-1
#define kcTdsMaX 'Z' //ie la dernière TDS s'appelle Z
#define kuItemMaX (kuItemLiM-1)
#define kuSymboleLiM 24 
#define kuSymboleMax (kuSymboleLiM-1) //nombre total de symboles qui discrimine les identificateurs.
char cTdsAlloueR[1+kcTdsMaX];
//la TDS est un tableau de kuItemLiM items,dc elle peut être indexée dans [0..kuItemMaX]:
	struct item{
		int val;//valeur associée à l'identificateur
		char ident[kuSymboleLiM]; 
		char cNom;//nom de la Tds
	}*pitem;
	struct item TDS[kuItemLiM];

int nAjouT;
int nCOUT;//compteur destiné à estimer la complexité algorithmique

int nCollisioN(char *sIdentificateur);
int nDispersioN(char *sIdentificateur);
int bItemVidE(int nItem);

int nCollisioN(char *sIdentificateur){
	//rend la fonction de collision de sIdentificateur réputé non vide, à valeur dans [1..kuItemMaX].
	int nCollision=0;
	nCollision=nChaineLg(sIdentificateur);
	//d(nCollision);
	Assert1("nCollisioN",bCroit(1,nCollision,kuItemMaX));
	return(nCollision);
}//nCollisioN

int nDispersioN(char *sIdentificateur){
	//rend la fonction de dispersion de sIdentificateur réputé non vide, à valeur dans [0..kuItemMaX].
	int nDispersion=0;
	//printf("%s %c\n",sIdentificateur,sIdentificateur[0]);
	nDispersion=sIdentificateur[0]-'@';
	nDispersion=nModulo(nDispersion,kuItemLiM);
	//d(nDispersion);
	Assert1("nDispersioN",bCroit(0,nDispersion,kuItemMaX));
	return(nDispersion);
}//nDispersion

int bItemVidE(int nItem){
	//vrai ssi TDS[nItem] est une entrée vide.
	Assert1("bItemVidE",bCroit(0,nItem,kuItemMaX));
	int bVide=nChaineLg(TDS[nItem].ident)==0;
	return(bVide);
}//bItemVidE

int bTdsAjouter(char cTdsNom,char *sIdentificateur){
	//si sIdentificateur est absent et s'il reste de la place en TDS,insère (sIdentificateur,0) en TDS. Sinon,ne fait rien.
	Assert2("TdsAjouterValuer",bTdsAmorceR,bCroitStrict(0,nChaineLg(sIdentificateur),kuSymboleMax));
	int nPlace;
	int bAjouter=!bTdsContient(cTdsNom,sIdentificateur,&nPlace);
	//d(bAjouter);
	if (bAjouter){
		//d(nPlace);
		Assert1("bTdsAjouter débordt",nAjouT+1<kuItemMaX);
		TDS[nPlace].cNom=cTdsNom;
		strcpy(TDS[nPlace].ident,sIdentificateur);
		TDS[nPlace].val=0;
		nAjouT++;
		//printf("%s ajouté en TDS ligne %d.\n",sG(sIdentificateur),nPlace);
		ds(nPlace,sG(TDS[nPlace].ident));
		TdsVoir(cTdsNom,sC3b(sG(sIdentificateur),"ajouté ligne",sEnt(nPlace)));
	}
	return(bAjouter);
}//bTdsAjouter

int nTdsAjouter(char cTdsNom,char *sIdentificateur){
	//si sIdentificateur est absent et s'il reste de la place en TDS,insère (sIdentificateur,0) en TDS. Rend sa place.
	Assert2("nTdsAjouter",bTdsAmorceR,bCroitStrict(0,nChaineLg(sIdentificateur),kuSymboleMax));
	int nAjouter;
	int bAjouter=!bTdsContient(cTdsNom,sIdentificateur,&nAjouter);
	if (bAjouter){
		Assert1("nTdsAjouter débordt",nAjouT+1<kuItemMaX);
		TDS[nAjouter].cNom=cTdsNom;
		strcpy(TDS[nAjouter].ident,sIdentificateur);
		TDS[nAjouter].val=0;
		nAjouT++;
	}
	return(nAjouter);
}//nTdsAjouter

int bTdsAjouterValuer(char cTdsNom,char *sIdentificateur,int iValeur){
	//comme TdsAjouter défini ci-dessus,plus l'affectation de iValeur.
	Assert2("TdsAjouterValuer",bTdsAmorceR,bCroitStrict(0,nChaineLg(sIdentificateur),kuSymboleMax));
	int bAjouter=bTdsAjouter(cTdsNom,sIdentificateur);
	int nPlace;
	if (bAjouter && bTdsContient(cTdsNom,sIdentificateur,&nPlace)){
		TDS[nPlace].val=iValeur;
		//d2(nPlace,iValeur);
	}
	return(bAjouter);
}//bTdsAjouterValuer

int bTdsAllouer(int bAllouer,char* pcTdsNom){
	// rend le nom d'une TDS disponible, ou rend '0' si échec
	char cTds;
	int nItem;
	int bTdsAllouer=kV;
	if (bAllouer){
		*pcTdsNom='0';
		for (bTdsAllouer=kF,cTds='A';cTds<=kcTdsMaX && !bTdsAllouer;cTds++)
			if (cTdsAlloueR[cTds]=='0'){
				*pcTdsNom=cTds;
				bTdsAllouer=kV;//au moins 1 entrée est disponible!
				//cTdsAlloueR[cTds]='1';
			}
	}else for (nItem=0;nItem<=kuItemMaX;nItem++)
		if (TDS[nItem].cNom==*pcTdsNom){
			//printf("%2d %20s %8d\n",nItem,TDS[nItem].ident,TDS[nItem].val);
			TDS[nItem].cNom='0';
			//TDS[nItem].val=-1;
			//TDS[nPlace].ident=0;
		}
	return(bTdsAllouer);
}//bTdsAllouer

void TdsAMORCER(){
	//amorce le présent module une fois pour toutes
	int cTds;
	//d(sizeof(struct item));
	Assert1("TdsAMORCER1",bFrontiere8(sizeof(struct item)));
	for (cTds='A';cTds<=kcTdsMaX;cTds++)
		cTdsAlloueR[cTds]='0';
	Assert1("TdsAMORCER2",bPremier(kuItemLiM));
	nTdsVoiR=0;
	bTdsAmorceR=kV;
}//TdsAMORCER

int bTdsContient(char cTdsNom,char *sIdentificateur,int *pnItem){//O(1)
	//vrai ssi sIdentificateur est en TDS. Si oui, *pnItem donne son emplacement;sinon, *pnItem indexe un item vide,dc libre. 
	int bContient;
	int bEureka,bPartir;
	Assert2("bTdsContient: identificateur de longueur nulle",bTdsAmorceR,bCroitStrict(0,nChaineLg(sIdentificateur),kuSymboleMax));
	bContient=nChaineLg(sIdentificateur)>0;
	if (bContient){
	//tt("bTdsContient$$$$$$",sG(sIdentificateur));
		*pnItem=nDispersioN(sIdentificateur);
	//t("bTdsContient$$$$$$ sIdentificateur");
		While(kuItemLiM);
		do{
			bEureka=(TDS[*pnItem].cNom==cTdsNom) && bChaineEgale(TDS[*pnItem].ident,sIdentificateur);
			bPartir=bEureka || bItemVidE(*pnItem);
			//tb("bEureka,bPartir",bEureka,bPartir);
			if (!bPartir)
				*pnItem=( *pnItem + nCollisioN(sC2(sC0(cTdsNom),sIdentificateur)) ) % kuItemLiM;
			nCOUT++;
		}while (bWhile("nIteM0:TDS saturée",!bPartir));//donc déroute si aucun item n'est vide.
		Assert1("bTdsContient1",bCroit(0,*pnItem,kuItemMaX));
		//d2(*pnItem,kuItemMaX);
		bContient=!bItemVidE(*pnItem);
		//b(bContient);
	}
	Assert1("bTdsContient2",bCroit(0,*pnItem,kuItemMaX));
	return(bContient);
}//bTdsContient

int nTdsCout(){
	Assert2("nTdsCout",bTdsAmorceR,nCOUT>=0);
	return(nCOUT);
}//nTdsCout

float fTdsDensiteh(){
	float fDensiteh=(nAjouT*1.0)/kuItemMaX;
	//fDensiteh=nAjouT;
	return(fDensiteh);
}//fTdsDensiteh

void TdsINITIALISER(){//O(?)
	//raze la TDS et le compteur nCOUT
	int nItem;
	Assert1("TdsINITIALISER",bTdsAmorceR);
	for (nItem=0;nItem<=kuItemMaX;nItem++){
		TDS[nItem].ident[0]='\0';
		TDS[nItem].val=0;
	}
	nCOUT=0;
	nAjouT=0;
}//TdsINITIALISER

int nTdsItem(){
	//rend le nombre d'items stockés en TDS
	int nItem=nAjouT;
	return(nItem);
}//nTdsItem

int bTdsPresent(char cTdsNom,char *sIdentificateur,int *piValeur){
	int nItem;
	int bPresent=bTdsContient(cTdsNom,sIdentificateur,&nItem);
	//d3(nItem,TDS[nItem].val,bPresent);
	*piValeur=(bPresent)? TDS[nItem].val : 0;
	return(bPresent);
}//bTdsPresent

char *sTdsSymbole(char cTdsNom,int nItem){
	//rend le nom du symbole stocké en TDS à l'index nItem
	return(TDS[nItem].ident);
}//sTdsSymbole

void TdsTESTER(int iTest){
	//teste le présent module
	Appel0(sC2("TdsTESTER,test n°",sEnt(iTest)));
		char cTdsNom;
		switch (iTest) {
		case 1:
			if (bTdsAllouer(kV,&cTdsNom)){
				TdsVoir(cTdsNom,"vide");
				tb("bTdsContient Charlot",bTdsContient(cTdsNom,"Charlot",&nBof));
				bBof=bTdsAjouterValuer(cTdsNom,"Charlot",5);
				TdsVoir(cTdsNom,"après charlot");
				tb("bTdsContient Charlot",bTdsContient(cTdsNom,"Charlot",&nBof));
				Assert1("TdsTESTER",bTdsAllouer(kF,&cTdsNom));
			}
			break;
		case 2:
			if (bTdsAllouer(kV,&cTdsNom)){
				TdsVoir(cTdsNom,"vide");
				bTdsAjouter(cTdsNom, "oui");
				bTdsAjouter(cTdsNom, "non");
				bTdsAjouter(cTdsNom, "ptet");
				int item=0;
				while(bTdsSymbole(cTdsNom, &item)) {
					printf("SYMBOLE: %s\n", sTdsSymbole(cTdsNom,item));
				}
			}
			break;
		default:
			Assert1("TdsTESTER",0);
			break;
	}
	Appel1(sC2("TdsTESTER,test n°",sEnt(iTest)));
}//TdsTESTER	

void TdsValuer(char cTdsNom,int nItem,int iValeur){
	Assert1("TdsValuer",bCroit(0,nItem,kuItemMaX));
	TDS[nItem].val=iValeur;
}//TdsValuer

void TdsVoir(char cTdsNom,char *sMessage){//O(nCardMaX)
	int nItem,bVide;
	Assert2("TdsVoir1",bTdsAmorceR,sMessage!=0);
	nTdsVoiR++;
	printf("%s: (sous-table '%c',%s)\n",sMessage,cTdsNom,sPluriel(nTdsItem(),"item"));
	if (0) printf("    S  y  m  b  o  l  e   valeur\n");
	for (bVide=kV,nItem=0;nItem<=kuItemMaX;nItem++){
		if (TDS[nItem].cNom==cTdsNom){
			printf("%2d %20s %8d\n",nItem,TDS[nItem].ident,TDS[nItem].val);
			bVide=kF;
		}
		if (kF)//si vous mettez kV la Pause ci-dessous vous permettra de voir la TDS courante (il faudra presser une touche pour poursuivre).
			Pause();
	}
	if (bVide)
		printf("\tLa TDS '%c' est vide.\n",cTdsNom);
}//TdsVoir

int bTdsSymbole(char cTdsNom, int *pnItemAvant) {
	int nItem,bVide;
	for (nItem=(*pnItemAvant)+1;nItem<=kuItemMaX;nItem++){
		if (TDS[nItem].cNom==cTdsNom){
			*pnItemAvant=nItem;
			return kV;
		}
	}
	return kF;
}