enum eMotClef {mcAlpha,mcAchat,mcStock,mcSi,mcOmega};

void AlexAfficher();
void AlexAMORCER();
int bAlexAnalyser(char *sCode);
char *sAlexChaine(int nLexeme);
int bAlexDate(int uLexeme);
int nAlexChose(int nLexeme);
void AlexDenoncer(int nLexeme);
int bAlexEntier(int uLexeme);
int mAlexEntier(int uLexeme);
int nAlexEtat(int uLexeme);
int nAlexExpression(int uLexeme);
int bAlexFinFichier(int uLexeme);
int bAlexHoraire(int uLexeme);
int bAlexIdentificateur(int uLexeme);
void AlexINITIALISER();
int nAlexInstruction(int uLexeme);
int bAlexLexeme(int uLexeme);
int nAlexLigne(int uLexeme);
int bAlexOperateur(int uLexeme,char cOperateur);
int bAlexParentheseur(int uLexeme,char cParenthese);
int nAlexProgramme(int uLexeme);
int bAlexReel(int uLexeme);
float fAlexReel(int uLexeme);
int bAlexSeparateur(int uLexeme);
int nAlexSuite(int uLexeme);
int nAlexTerme(int uLexeme);
void AlexTESTER(int iTest);
int bAlexTexte(int uLexeme);
int bAlexMotClef(int uLexeme, int nMotClefNumero);