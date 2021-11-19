#include "Kit.h"
#include "Tri.h"
int bTriAmorceR;
#define kuElementMaX 6
int T[1+kuElementMaX];

void TriReussI(int nGauche,int nDroit);

void TriAMORCER(){
	//amorce le présent module une fois pour toutes
	bTriAmorceR=kV;
}//TriAMORCER

void TriINITIALISER(){//O(?)
	//relance le présent module
	Assert1("TriINITIALISER",bTriAmorceR);
}//TriINITIALISER

int nPartitionneR(int nGauche,int nDroit){
	int bEchange=kF,mPartitionner=-1,bVoir=kF;
	int iPivot=T[nGauche],nI=nGauche-1,nJ=nDroit+1;
	if (bVoir) d(iPivot);
	do{//faire se rencontrer nI et nJ en échangeant les éléments qui se trouvent dans les partitions opposées
		do{//faire progresser nI dans sa partition, donc de gauche à droite
		}while(T[++nI]<iPivot);
		do{//faire progresser nJ dans sa partition, donc de droite à gauche
		}while(T[--nJ]>iPivot);
		if (nI<nJ){//nI n'a pas encore rencontré nJ,donc il y a un échange à effectuer avant de progresser à nouveau 
					if (bVoir && !bEchange) {d2(nGauche,nDroit);VecteurVoir("T0",T,0,kuElementMaX-1);bEchange=kV;};
					if (bVoir) printf("échange des éléments d'index %d et %d:\n",nI,nJ);
			EntierEchanger(&T[nI],&T[nJ]);
					if (bVoir) {d2(nI,nJ);VecteurVoir("T1",T,0,kuElementMaX-1);}
		};
	}while(nI<nJ);
	mPartitionner=nJ;
	Assert1("nPartitionneR",mPartitionner>=0);
	if (bVoir) d(mPartitionner);
	return(mPartitionner);
}//nPartitionneR

void TriRapidE1(int nGauche,int nDroit){
	int nCoupure,bVoir=kF;
	Assert1("TriRapidE1",bCroit(0,nGauche,nDroit));
	if (nGauche<nDroit){
		if (bVoir) VecteurVoir("TRI RAPIDE de T",T,nGauche,nDroit);
		nCoupure=nPartitionneR(nGauche,nDroit);
		if (bVoir) VecteurVoir("APRES partitionnement",T,nGauche,nDroit);
		TriRapidE1(nGauche,nCoupure);
		TriRapidE1(nCoupure+1,nDroit);
	}//else sous-tableau à 1 élément 
}//TriRapidE

void TriRapideSedgeWicK(int nGauche,int nDroit,int nTabulation){
	int bVoir=kF,nI,nJ,nCoupure;
	int iPivot;
	//d2(nGauche,nDroit);
	//Assert1("TriRapideSedgeWicK",bCroit(0,nGauche,nDroit));//ABUSIF car nGauche peut être au-delà de nDroit
	if (nGauche<nDroit){
		if (bVoir) VecteurVoir(sC2(sTab(nTabulation),"TRI RAPIDE de T"),T,nGauche,nDroit);
			if (nDroit>nGauche){
				//nCoupure=nPartitionneR(nGauche,nDroit);
					iPivot=T[nDroit];
					nI=nGauche-1;nJ=nDroit;
					do{
						while(T[++nI]<iPivot);
						while(T[--nJ]>iPivot);
						if (nI<nJ)
							EntierEchanger(&T[nI],&T[nJ]);
					}while(nI<nJ);
					EntierEchanger(&T[nI],&T[nDroit]);
					nCoupure=nI;
					if (bVoir) printf("%snCoupure en %d.\n",sTab(nTabulation+1),nCoupure);
				if (bVoir) VecteurVoir(sC2(sTab(nTabulation),"APRES partitionnement"),T,nGauche,nDroit);
				TriRapideSedgeWicK(nGauche,nCoupure-1,nTabulation+1);
				TriRapideSedgeWicK(nCoupure+1,nDroit,nTabulation+1);
			}
	}//else sous-tableau à 1 élément 
}//TriRapideSedgeWicK

void TriRapideR(){
	VecteurVoir("AVANT TRI RAPIDE",T,0,kuElementMaX-1);
	TriRapideSedgeWicK(0,kuElementMaX-1,1);
	VecteurVoir("APRES TRI RAPIDE",T,0,kuElementMaX-1);
	TriReussI(0,kuElementMaX-1);
	Assert1("TriRapideR",bVecteurCroit(T,0,kuElementMaX-1,!k1Strict));
}//TriRapideR

void TriReussI(int nGauche,int nDroit){
	int nIndex,bReussi;
	for (bReussi=kV,nIndex=nGauche;nIndex<nDroit;nIndex++)
		bReussi=bReussi && T[nIndex]<=T[nIndex+1];
	Assert1("TriReussI",bReussi);
}//TriReussI

void TriTESTER(int iTest){
	//teste le présent module
	const int kuTriEnTout=10;
	int uTri;
	Appel0(sC2("TriTESTER,test n°",sEnt(iTest)));
		iTest=2;
		switch (iTest) {
		case 1:
			VecteurHasarder(T,0,kuElementMaX-1,5);
			VecteurVoir("T",T,0,kuElementMaX-1);
			TriRapideR();
			break;
		case 2:
			for (uTri=1;uTri<=kuTriEnTout;uTri++){
				printf("Tri %d/%d:\n",uTri,kuTriEnTout);
				VecteurHasarder(T,0,kuElementMaX-1,5);
				//VecteurVoir("T",T,0,kuElementMaX-1);
				TriRapideR();
			}
			break;
		default:
			Assert1("TriTESTER",0);
			break;
	}
	Appel1(sC2("TriTESTER,test n°",sEnt(iTest)));
}//TriTESTER	

