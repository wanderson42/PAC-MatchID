#include <stdio.h>
#include <stdbool.h>
#include <float.h>
// #include <windows.h>
#include <time.h>
#include <cmath>
#include <stdlib.h>
#include <dirent.h>

// novas modificações: 

// Tamanho do buffer
const int BUFSIZE = 256;
#include <iostream>
#include <cstdlib>
#include <cstring> // Adicionado para a função strcat
#include <unistd.h> // Adicionado para a função getcwd
// Para uso da função GetPrivateProfileString customizada 
#include <fstream>
#include <iostream>
#include <sstream>

#include <vector>
//#include <string>

// Substituições de tipos
using DWORD = unsigned int;
using TCHAR = char;

typedef struct
{
    char Canal[7];
    char Config[4];
} Configuracao;


void GetPrivateProfileString(const char* section, const char* key, const char* default_value, char* result, size_t size, const char* ini_path) {
    FILE* file = fopen(ini_path, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo INI");
        exit(EXIT_FAILURE);
    }

    char line[512];
    char search_key[256];
    snprintf(search_key, sizeof(search_key), "%s=", key);

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, section) != NULL) {
            while (fgets(line, sizeof(line), file) != NULL) {
                if (line[0] == '[') {
                    break;  // Fim da seção, a chave não foi encontrada
                }
                char* pos = strstr(line, search_key);
                if (pos != NULL) {
                    sscanf(pos, "%*[^=]=%s", result);
                    fclose(file);
                    return;
                }
            }
            break;  // A seção foi encontrada, mas a chave não foi
        }
    }

    fclose(file);
    strncpy(result, default_value, size - 1);
    result[size - 1] = '\0';
}
// ...

void fSubstituir(char *pTexto, char pAtual, char pNovo)
{
    int i=0;

    while (pTexto[i] != '\0')
    {
      if (pTexto[i] == pAtual) pTexto[i] = pNovo;
      i++;
    }
}
// ...

void fSplit (char sString[], char cSeparador[], char *Retorno[])
{
    int iAux = 0;
	char *newline = strchr(sString,'\n');
    if (newline) *newline = 0;

    char *cAux = strtok(sString,cSeparador);

    while(cAux!=NULL)
    {
        Retorno[iAux++]=cAux;
        cAux=strtok(NULL,cSeparador);
    }
}
// ...

void fGravarLog(const char* cMensagem, FILE* fLog)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fprintf(fLog,"%04d-%02d-%02d %02d:%02d:%02d - %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, cMensagem);
}
// ...

void fBuscaConfig(char * Retorno, char cCanal[7], Configuracao Config[])
{
    int iConfig;
    bool bAchou;

    iConfig = 0;

    bAchou= false;
    while(!bAchou)
    {
        //printf("fBuscaConfig  Config[%i].Canal:[%s] cCanal:[%s]\n",iConfig,Config[iConfig].Canal,cCanal);
        //getchar();
        if (strcmp(Config[iConfig].Canal, cCanal) == 0)
        {
            strcpy(Retorno,Config[iConfig].Config);
            bAchou = true;
        }
        iConfig++;
    }
}
// ...

// Protótipos
void sIdentificarCoincidencias();
void sAjustarCoincidencias();


int main()
{
    bool bOk;
    char cOpcao;

    bOk=false;
    while (!bOk)
    {
        if (system("clear") == -1) {
            perror("system");
            // Tratar o erro, se necessário
        }

        printf("╔════════════════════════════════════════════════════════╗\n");
        printf("║                MatchID ¹¹¹In (Gnu/Linux)               ║\n");
        printf("╚════════════════════════════════════════════════════════╝\n\n");

        printf("Escolha uma das opções abaixo:\n\n");
        printf(" Opção 1 - Para gerar e ajustar arquivo de coincidências\n");
        printf(" Opção 2 - Para somente ajustar arquivo de coincidências\n");
        printf(" Opção 0 - Para sair\n\n");

        printf("Digite uma opção: ");
        fflush(stdout);

        cOpcao=getchar();
        getchar(); // Consumir o caractere de nova linha

        if ((cOpcao == '0') || (cOpcao == '1') || (cOpcao == '2'))  bOk = true;
    }
    if (cOpcao == '1')
    {
        sIdentificarCoincidencias();
        sAjustarCoincidencias();
    }
    if (cOpcao == '2')
    {
        sAjustarCoincidencias();
    }
    //getchar();
    exit(0);
}

void sIdentificarCoincidencias()
{
    DWORD dwRet;
    TCHAR ini[BUFSIZE];
    TCHAR diretorio[BUFSIZE];
    char Url0[500];
    char Url1[500];
    char Url2[500];
    char Url3[500];
    char UrlCoi[500];
    char UrlLog[500];
    char LinCabec[3];
    char SepCol[3];
    char sAux3[500];
    int Ch0Dec1Ini,Ch1Dec1Ini,Ch2Dec1Ini,Ch3Dec1Ini;
    int Ch0Dec1Fim,Ch1Dec1Fim,Ch2Dec1Fim,Ch3Dec1Fim;
    int Ch0Dec2Ini,Ch1Dec2Ini,Ch2Dec2Ini,Ch3Dec2Ini;
    int Ch0Dec2Fim,Ch1Dec2Fim,Ch2Dec2Fim,Ch3Dec2Fim;
    double IntervaloDe,IntervaloAte;
    time_t start_t, end_t;
    double TotCount0,TotCount1,TotCount2,TotCount3;
    char Ch0[500],Ch1[500],Ch2[500],Ch3[500];
    char MaxCh0[100],MaxCh1[100],MaxCh2[100],MaxCh3[100];
    FILE *Arq0,*Arq1,*Arq2,*Arq3,*ArqCoi,*ArqLog;
    bool Terminou;
    char *Campos0[500],*Campos1[500],*Campos2[500],*Campos3[500];
    char Canais[4];
    bool Ch0Ler,Ch1Ler,Ch2Ler,Ch3Ler;
    int Ch0End,Ch1End,Ch2End,Ch3End;
    const char *ChAtual,*ChProximo;
    const char *EnergiaAtual;
    const char *EnergiaProximo;
    const char *StartStopAtual,*StartStopProximo;
    double TimeStampAtual,TimeStampProximo;
    double ChTimeStamp[4];
    int IndMenor;
    double TotRegistros, TotCoincidencias;
    double Diferenca;
    double diff_t;
    char cLog;
    char cIntToChar[50];
    char cFloatToChar[100];
    char sAux4[500];
    int iLinhasCabec;
    int iAux;
    bool bECoincidencia;

    printf("\n\nInicio da identificacao das coincidencias");

    //dwRet = GetCurrentDirectory(BUFSIZE, ini); <= antes
    // Substitua GetCurrentDirectory por getcwd
    getcwd(ini, BUFSIZE);
    strcat(ini, "/MatchID_Indio.ini");
    fSubstituir(ini, '/', '/');

    //dwRet = GetCurrentDirectory(BUFSIZE, diretorio); <= antes
    // Substitua GetCurrentDirectory por getcwd
    getcwd(diretorio, BUFSIZE);
    DIR *dir;
    struct dirent *lsdir;
    dir = opendir(diretorio);
    /* print all the files and directories within directory */
    //strcat(diretorio,"\\"); <= antes
    strcat(diretorio, "/");



    while ( ( lsdir = readdir(dir) ) != NULL )
    {
        if ((lsdir->d_name[0] == '0') && (lsdir->d_name[1] == '@')) { strcpy(Url0,diretorio); strcat(Url0, lsdir->d_name); printf("\n%s",Url0);}
        if ((lsdir->d_name[0] == '1') && (lsdir->d_name[1] == '@')) { strcpy(Url1,diretorio); strcat(Url1, lsdir->d_name); printf("\n%s",Url1);}
        if ((lsdir->d_name[0] == '2') && (lsdir->d_name[1] == '@')) { strcpy(Url2,diretorio); strcat(Url2, lsdir->d_name); printf("\n%s",Url2);}
        if ((lsdir->d_name[0] == '3') && (lsdir->d_name[1] == '@')) { strcpy(Url3,diretorio); strcat(Url3, lsdir->d_name); printf("\n%s",Url3);}
    }
    closedir(dir);

    strcpy(UrlCoi,diretorio);
    strcpy(UrlLog,diretorio);
    strcat(UrlCoi,"MatchID_Indio_COI.txt");
    strcat(UrlLog,"MatchID_Indio_COI_LOG.txt");

    strcpy(LinCabec,"1");
    strcpy(SepCol,";");

    fSubstituir (Url0,  '\\','/');
    fSubstituir (Url1,  '\\','/');
    fSubstituir (Url2,  '\\','/');
    fSubstituir (Url3,  '\\','/');
    fSubstituir (UrlCoi,'\\','/');
    fSubstituir (UrlLog,'\\','/');

    IntervaloDe =-50000;
    IntervaloAte=973000;

    GetPrivateProfileString("ENERGIA FILTROS", "CH0Dec1INI",   0, sAux3, 500, ini); Ch0Dec1Ini  =atoi(sAux3);
    GetPrivateProfileString("ENERGIA FILTROS", "CH0Dec1FIM",   0, sAux3, 500, ini); Ch0Dec1Fim  =atoi(sAux3);
    GetPrivateProfileString("ENERGIA FILTROS", "CH0Dec2INI",   0, sAux3, 500, ini); Ch0Dec2Ini  =atoi(sAux3);
    GetPrivateProfileString("ENERGIA FILTROS", "CH0Dec2FIM",   0, sAux3, 500, ini); Ch0Dec2Fim  =atoi(sAux3);
    GetPrivateProfileString("ENERGIA FILTROS", "CH1Dec1INI",   0, sAux3, 500, ini); Ch1Dec1Ini  =atoi(sAux3);
    GetPrivateProfileString("ENERGIA FILTROS", "CH1Dec1FIM",   0, sAux3, 500, ini); Ch1Dec1Fim  =atoi(sAux3);
    GetPrivateProfileString("ENERGIA FILTROS", "CH1Dec2INI",   0, sAux3, 500, ini); Ch1Dec2Ini  =atoi(sAux3);
    GetPrivateProfileString("ENERGIA FILTROS", "CH1Dec2FIM",   0, sAux3, 500, ini); Ch1Dec2Fim  =atoi(sAux3);
    GetPrivateProfileString("ENERGIA FILTROS", "CH2Dec1INI",   0, sAux3, 500, ini); Ch2Dec1Ini  =atoi(sAux3);
    GetPrivateProfileString("ENERGIA FILTROS", "CH2Dec1FIM",   0, sAux3, 500, ini); Ch2Dec1Fim  =atoi(sAux3);
    GetPrivateProfileString("ENERGIA FILTROS", "CH2Dec2INI",   0, sAux3, 500, ini); Ch2Dec2Ini  =atoi(sAux3);
    GetPrivateProfileString("ENERGIA FILTROS", "CH2Dec2FIM",   0, sAux3, 500, ini); Ch2Dec2Fim  =atoi(sAux3);
    GetPrivateProfileString("ENERGIA FILTROS", "CH3Dec1INI",   0, sAux3, 500, ini); Ch3Dec1Ini  =atoi(sAux3);
    GetPrivateProfileString("ENERGIA FILTROS", "CH3Dec1FIM",   0, sAux3, 500, ini); Ch3Dec1Fim  =atoi(sAux3);
    GetPrivateProfileString("ENERGIA FILTROS", "CH3Dec2INI",   0, sAux3, 500, ini); Ch3Dec2Ini  =atoi(sAux3);
    GetPrivateProfileString("ENERGIA FILTROS", "CH3Dec2FIM",   0, sAux3, 500, ini); Ch3Dec2Fim  =atoi(sAux3);


    printf("\n\nInformacoes iniciais %s",ini);
    printf("\nArquivoINI=[%s]",ini);
    printf("\nArquivoCH0=[%s]",Url0);
    printf("\nArquivoCH1=[%s]",Url1);
    printf("\nArquivoCH2=[%s]",Url2);
    printf("\nArquivoCH3=[%s]",Url3);
    printf("\nLinhasCabec=[%s]",LinCabec);
    printf("\nSeparadorCol=[%s]",SepCol);
    printf("\nArquivoCoi=[%s]",UrlCoi);
    printf("\nArquivoLog=[%s]\n",UrlLog);
    printf("\nFiltro CH0:Dec1[%i>%i] Dec2[%i>%i]",Ch0Dec1Ini,Ch0Dec1Fim,Ch0Dec2Ini,Ch0Dec2Fim);
    printf("\nFiltro CH1:Dec1[%i>%i] Dec2[%i>%i]",Ch1Dec1Ini,Ch1Dec1Fim,Ch1Dec2Ini,Ch1Dec2Fim);
    printf("\nFiltro CH2:Dec1[%i>%i] Dec2[%i>%i]",Ch2Dec1Ini,Ch2Dec1Fim,Ch2Dec2Ini,Ch2Dec2Fim);
    printf("\nFiltro CH3:Dec1[%i>%i] Dec2[%i>%i]",Ch3Dec1Ini,Ch3Dec1Fim,Ch3Dec2Ini,Ch3Dec2Fim);
    printf("\nIntervaloDe:[%.0f]" ,IntervaloDe);
    printf("\nIntervaloAte:[%.0f]\n",IntervaloAte);

    printf("\nPressione uma tecla para iniciar (ou ctrl + c para sair)");
    getchar();
    printf("\nEm processamento...\n");
    time(&start_t);

	TotCount0 = TotCount1 = TotCount2 = TotCount3 = 0;
    Arq0 = Arq1 = Arq2 = Arq3 = ArqCoi = NULL;
    Ch0Ler = Ch1Ler = Ch2Ler = Ch3Ler = true;
    Ch0End = Ch1End = Ch2End = Ch3End = 0;
    ChAtual = ChProximo = "";
    EnergiaAtual = "";
    EnergiaProximo = "";
    StartStopAtual = StartStopProximo = "";
    TimeStampAtual=TimeStampProximo=0;
    for (int i=0; i<4; i++) ChTimeStamp[i]=DBL_MAX;
    strcpy(MaxCh0," ");
    strcpy(MaxCh1," ");
    strcpy(MaxCh2," ");
    strcpy(MaxCh3," ");

    ArqLog = fopen(UrlLog, "w");

    fGravarLog("Informacoes iniciais", ArqLog);
    strcpy(sAux4,"ArquivoINI=[");     strcat(sAux4,ini);      strcat(sAux4,"]"); fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"ArquivoCH0=[");     strcat(sAux4,Url0);     strcat(sAux4,"]"); fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"ArquivoCH1=[");     strcat(sAux4,Url1);     strcat(sAux4,"]"); fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"ArquivoCH2=[");     strcat(sAux4,Url2);     strcat(sAux4,"]"); fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"ArquivoCH3=[");     strcat(sAux4,Url3);     strcat(sAux4,"]"); fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"ArquivoSaida=[");   strcat(sAux4,UrlCoi);   strcat(sAux4,"]"); fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"ArquivoLog=[");     strcat(sAux4,UrlLog);   strcat(sAux4,"]"); fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"LinhasCabec=[");    strcat(sAux4,LinCabec); strcat(sAux4,"]"); fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"SeparadorCol=[");   strcat(sAux4,SepCol);   strcat(sAux4,"]"); fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"Filtro CH0:Dec1["); sprintf(cIntToChar,"%i", Ch0Dec1Ini); strcat(sAux4,cIntToChar); strcat(sAux4,">"); sprintf(cIntToChar,"%i", Ch0Dec1Fim); strcat(sAux4,cIntToChar); strcat(sAux4,"] Dec2["); sprintf(cIntToChar,"%i", Ch0Dec2Ini); strcat(sAux4,cIntToChar); strcat(sAux4,">"); sprintf(cIntToChar,"%i", Ch0Dec2Fim); strcat(sAux4,cIntToChar); strcat(sAux4,"]"); fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"Filtro CH1:Dec1["); sprintf(cIntToChar,"%i", Ch1Dec1Ini); strcat(sAux4,cIntToChar); strcat(sAux4,">"); sprintf(cIntToChar,"%i", Ch1Dec1Fim); strcat(sAux4,cIntToChar); strcat(sAux4,"] Dec2["); sprintf(cIntToChar,"%i", Ch1Dec2Ini); strcat(sAux4,cIntToChar); strcat(sAux4,">"); sprintf(cIntToChar,"%i", Ch1Dec2Fim); strcat(sAux4,cIntToChar); strcat(sAux4,"]"); fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"Filtro CH2:Dec1["); sprintf(cIntToChar,"%i", Ch2Dec1Ini); strcat(sAux4,cIntToChar); strcat(sAux4,">"); sprintf(cIntToChar,"%i", Ch2Dec1Fim); strcat(sAux4,cIntToChar); strcat(sAux4,"] Dec2["); sprintf(cIntToChar,"%i", Ch2Dec2Ini); strcat(sAux4,cIntToChar); strcat(sAux4,">"); sprintf(cIntToChar,"%i", Ch2Dec2Fim); strcat(sAux4,cIntToChar); strcat(sAux4,"]"); fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"Filtro CH3:Dec1["); sprintf(cIntToChar,"%i", Ch3Dec1Ini); strcat(sAux4,cIntToChar); strcat(sAux4,">"); sprintf(cIntToChar,"%i", Ch3Dec1Fim); strcat(sAux4,cIntToChar); strcat(sAux4,"] Dec2["); sprintf(cIntToChar,"%i", Ch3Dec2Ini); strcat(sAux4,cIntToChar); strcat(sAux4,">"); sprintf(cIntToChar,"%i", Ch3Dec2Fim); strcat(sAux4,cIntToChar); strcat(sAux4,"]"); fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"IntervaloDe:[");    sprintf(cFloatToChar,"%.0f", IntervaloDe);  strcat(sAux4,cFloatToChar); strcat(sAux4,"]"); fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"IntervaloAte:[");   sprintf(cFloatToChar,"%.0f", IntervaloAte); strcat(sAux4,cFloatToChar); strcat(sAux4,"]"); fGravarLog(sAux4,ArqLog);

    fGravarLog("Em processamento",ArqLog);
    fGravarLog("Abrindo arquivos de dados",ArqLog);
    printf("\nAbrindo arquivos de dados");

    Arq0 = (Url0 != nullptr) ? fopen(Url0, "r") : nullptr;
    Arq1 = (Url1 != nullptr) ? fopen(Url1, "r") : nullptr;
    Arq2 = (Url2 != nullptr) ? fopen(Url2, "r") : nullptr;
    Arq3 = (Url3 != nullptr) ? fopen(Url3, "r") : nullptr;


	if ((Arq0 == NULL) && (Arq1 == NULL) && (Arq2 == NULL) && (Arq3 == NULL))
    {
	    printf("\nErro, nao foi possivel abrir o(s) arquivo(s)\n");
        fGravarLog("Erro, nao foi possivel abrir o(s) arquivo(s)",ArqLog);
    }
	else
    {

        fGravarLog("Abrindo arquivos de saida",ArqLog);
        printf("\nAbrindo arquivo de saida");
        ArqCoi = fopen(UrlCoi, "w");

        fGravarLog("Analisando dados",ArqLog);
        printf("\n\nAnalisando dados...");

        Ch0Ler = (strcmp(Url0, "") != 0);
        Ch1Ler = (strcmp(Url1, "") != 0);
        Ch2Ler = (strcmp(Url2, "") != 0);
        Ch3Ler = (strcmp(Url3, "") != 0);


        //Remover linha de titulo
        iLinhasCabec = atoi(LinCabec);
        iAux=1;
        while (iAux<=iLinhasCabec)
        {
            if (Ch0Ler) {if (fgets(Ch0,sizeof(Ch0),Arq0) != NULL) {strcpy(MaxCh0,Ch0); Ch0End=0;} else {Ch0Ler=false; Ch0End=1;}};
            if (Ch1Ler) {if (fgets(Ch1,sizeof(Ch1),Arq1) != NULL) {strcpy(MaxCh1,Ch1); Ch1End=0;} else {Ch1Ler=false; Ch1End=1;}};
            if (Ch2Ler) {if (fgets(Ch2,sizeof(Ch2),Arq2) != NULL) {strcpy(MaxCh2,Ch2); Ch2End=0;} else {Ch2Ler=false; Ch2End=1;}};
            if (Ch3Ler) {if (fgets(Ch3,sizeof(Ch3),Arq3) != NULL) {strcpy(MaxCh3,Ch3); Ch3End=0;} else {Ch3Ler=false; Ch3End=1;}};
            iAux++;
        }

        TotRegistros=TotCoincidencias=0;

        Terminou=false;
        if ((Ch0End+Ch1End+Ch2End+Ch3End)>0) Terminou=true;

	    while(!Terminou)
        {
            if (Ch0Ler)
            {
                if (fgets(Ch0,sizeof(Ch0),Arq0) != NULL)
                {
                    strcpy(MaxCh0,Ch0);
                    Ch0End=0;
                    fSplit(Ch0,SepCol,&Campos0[0]);
                    TotCount0++;
                    ChTimeStamp[0]=atof(Campos0[0]);
                    Ch0Ler=false;
                }
                else
                {
                    Ch0End=1;
                    ChTimeStamp[0]=DBL_MAX;
                    printf("\nTerminou CH0");
                    fGravarLog("Terminou CH0",ArqLog);
                }
            };
            if (Ch1Ler) {if (fgets(Ch1,sizeof(Ch1),Arq1) != NULL) {strcpy(MaxCh1,Ch1); Ch1End=0; fSplit(Ch1,SepCol,&Campos1[0]); TotCount1++; ChTimeStamp[1]=atof(Campos1[0]); Ch1Ler=false;} else {Ch1End=1; ChTimeStamp[1]=DBL_MAX; printf("\nTerminou CH1"); fGravarLog("Terminou CH1",ArqLog);}};
            if (Ch2Ler) {if (fgets(Ch2,sizeof(Ch2),Arq2) != NULL) {strcpy(MaxCh2,Ch2); Ch2End=0; fSplit(Ch2,SepCol,&Campos2[0]); TotCount2++; ChTimeStamp[2]=atof(Campos2[0]); Ch2Ler=false;} else {Ch2End=1; ChTimeStamp[2]=DBL_MAX; printf("\nTerminou CH2"); fGravarLog("Terminou CH2",ArqLog);}};
            if (Ch3Ler) {if (fgets(Ch3,sizeof(Ch3),Arq3) != NULL) {strcpy(MaxCh3,Ch3); Ch3End=0; fSplit(Ch3,SepCol,&Campos3[0]); TotCount3++; ChTimeStamp[3]=atof(Campos3[0]); Ch3Ler=false;} else {Ch3End=1; ChTimeStamp[3]=DBL_MAX; printf("\nTerminou CH3"); fGravarLog("Terminou CH3",ArqLog);}};

            IndMenor=0;
            for (int i=1; i<4; i++) if (ChTimeStamp[IndMenor] > ChTimeStamp[i]) IndMenor=i;
            if (ChTimeStamp[IndMenor] == DBL_MAX) IndMenor=-1;

/*
            printf("\nIndMenor=[%i] ChAtual=[%s] ChProximo=[%s]",IndMenor,ChAtual,ChProximo);
            printf("\nEnergia0=[%i]",atoi(Campos0[1]));
            printf("\nEnergia1=[%i]",atoi(Campos1[1]));
            printf("\nEnergia2=[%i]",atoi(Campos2[1]));
            printf("\nEnergia3=[%i]",atoi(Campos3[1]));
            getch();
*/

            switch (IndMenor)
            {
                case 0:
                    Ch0Ler=true;
                    if ((strcmp(ChAtual, "") == 0) || (strcmp(ChAtual, "0") == 0))
                    {
                        if      ((atoi(Campos0[1]) >= Ch0Dec1Ini) && (atoi(Campos0[1]) <= Ch0Dec1Fim)) {ChAtual="0"; TimeStampAtual=atof(Campos0[0]); EnergiaAtual=Campos0[1]; StartStopAtual="START";}
                        else if ((atoi(Campos0[1]) >= Ch0Dec2Ini) && (atoi(Campos0[1]) <= Ch0Dec2Fim)) {ChAtual="0"; TimeStampAtual=atof(Campos0[0]); EnergiaAtual=Campos0[1]; StartStopAtual="STOP";}
                        else                                                                           {ChAtual="";  TimeStampAtual=0;                EnergiaAtual=0;          StartStopAtual="";}
                    }
                    else
                    {
                        if      ((atoi(Campos0[1]) >= Ch0Dec1Ini) && (atoi(Campos0[1]) <= Ch0Dec1Fim)) {ChProximo="0"; TimeStampProximo=atof(Campos0[0]); EnergiaProximo=Campos0[1]; StartStopProximo="START";}
                        else if ((atoi(Campos0[1]) >= Ch0Dec2Ini) && (atoi(Campos0[1]) <= Ch0Dec2Fim)) {ChProximo="0"; TimeStampProximo=atof(Campos0[0]); EnergiaProximo=Campos0[1]; StartStopProximo="STOP";}
                        else                                                                           {ChProximo="";  TimeStampProximo=0;                EnergiaProximo=0;          StartStopProximo="";}
                    }
                    break;
                case 1:
                    Ch1Ler=true;
                    if ((strcmp(ChAtual, "") == 0) || (strcmp(ChAtual, "1") == 0))
                    {
                        if      ((atoi(Campos1[1]) >= Ch1Dec1Ini) && (atoi(Campos1[1]) <= Ch1Dec1Fim)) {ChAtual="1"; TimeStampAtual=atof(Campos1[0]); EnergiaAtual=Campos1[1]; StartStopAtual="START";}
                        else if ((atoi(Campos1[1]) >= Ch1Dec2Ini) && (atoi(Campos1[1]) <= Ch1Dec2Fim)) {ChAtual="1"; TimeStampAtual=atof(Campos1[0]); EnergiaAtual=Campos1[1]; StartStopAtual="STOP";}
                        else                                                                           {ChAtual="";  TimeStampAtual=0;                EnergiaAtual=0;          StartStopAtual="";}
                    }
                    else
                    {
                        if      ((atoi(Campos1[1]) >= Ch1Dec1Ini) && (atoi(Campos1[1]) <= Ch1Dec1Fim)) {ChProximo="1"; TimeStampProximo=atof(Campos1[0]); EnergiaProximo=Campos1[1]; StartStopProximo="START";}
                        else if ((atoi(Campos1[1]) >= Ch1Dec2Ini) && (atoi(Campos1[1]) <= Ch1Dec2Fim)) {ChProximo="1"; TimeStampProximo=atof(Campos1[0]); EnergiaProximo=Campos1[1]; StartStopProximo="STOP";}
                        else                                                                           {ChProximo="";  TimeStampProximo=0;                EnergiaProximo=0;          StartStopProximo="";}
                    }
                    break;
                case 2:
                    Ch2Ler=true;
                    if ((strcmp(ChAtual, "") == 0) || (strcmp(ChAtual, "2") == 0))
                    {
                        if      ((atoi(Campos2[1]) >= Ch2Dec1Ini) && (atoi(Campos2[1]) <= Ch2Dec1Fim)) {ChAtual="2"; TimeStampAtual=atof(Campos2[0]); EnergiaAtual=Campos2[1]; StartStopAtual="START";}
                        else if ((atoi(Campos2[1]) >= Ch2Dec2Ini) && (atoi(Campos2[1]) <= Ch2Dec2Fim)) {ChAtual="2"; TimeStampAtual=atof(Campos2[0]); EnergiaAtual=Campos2[1]; StartStopAtual="STOP";}
                        else                                                                           {ChAtual="";  TimeStampAtual=0;                EnergiaAtual=0;          StartStopAtual="";}
                    }
                    else
                    {
                        if      ((atoi(Campos2[1]) >= Ch2Dec1Ini) && (atoi(Campos2[1]) <= Ch2Dec1Fim)) {ChProximo="2"; TimeStampProximo=atof(Campos2[0]); EnergiaProximo=Campos2[1]; StartStopProximo="START";}
                        else if ((atoi(Campos2[1]) >= Ch2Dec2Ini) && (atoi(Campos2[1]) <= Ch2Dec2Fim)) {ChProximo="2"; TimeStampProximo=atof(Campos2[0]); EnergiaProximo=Campos2[1]; StartStopProximo="STOP";}
                        else                                                                           {ChProximo="";  TimeStampProximo=0;                EnergiaProximo=0;          StartStopProximo="";}
                    }
                    break;
                case 3:
                    Ch3Ler=true;
                    if ((strcmp(ChAtual, "") == 0) || (strcmp(ChAtual, "3") == 0))
                    {
                        if      ((atoi(Campos3[1]) >= Ch3Dec1Ini) && (atoi(Campos3[1]) <= Ch3Dec1Fim)) {ChAtual="3"; TimeStampAtual=atof(Campos3[0]); EnergiaAtual=Campos3[1]; StartStopAtual="START"; }
                        else if ((atoi(Campos3[1]) >= Ch3Dec2Ini) && (atoi(Campos3[1]) <= Ch3Dec2Fim)) {ChAtual="3"; TimeStampAtual=atof(Campos3[0]); EnergiaAtual=Campos3[1]; StartStopAtual="STOP";}
                        else                                                                           {ChAtual="";  TimeStampAtual=0;                EnergiaAtual=0;          StartStopAtual="";}
                    }
                    else
                    {
                        if      ((atoi(Campos3[1]) >= Ch3Dec1Ini) && (atoi(Campos3[1]) <= Ch3Dec1Fim)) {ChProximo="3"; TimeStampProximo=atof(Campos3[0]); EnergiaProximo=Campos3[1]; StartStopProximo="START";}
                        else if ((atoi(Campos3[1]) >= Ch3Dec2Ini) && (atoi(Campos3[1]) <= Ch3Dec2Fim)) {ChProximo="3"; TimeStampProximo=atof(Campos3[0]); EnergiaProximo=Campos3[1]; StartStopProximo="STOP";}
                        else                                                                           {ChProximo="";  TimeStampProximo=0;                EnergiaProximo=0;          StartStopProximo="";}
                    }
                    break;
            }

            //printf("\n ChTimeStamp[0]:[%.0f]  ChTimeStamp[1]:[%.0f]  ChTimeStamp[2]:[%.0f]  ChTimeStamp[3]:[%.0f] -- ChAtual:[%s] ChProximo:[%s] ----> ",ChTimeStamp[0],ChTimeStamp[1],ChTimeStamp[2],ChTimeStamp[3],ChAtual, ChProximo);

            if ((strlen(ChAtual)>0) && (strlen(ChProximo)>0))
            {

                if (strcmp(StartStopAtual, "START") == 0) Diferenca = TimeStampProximo - TimeStampAtual;
                else Diferenca = TimeStampAtual - TimeStampProximo;

/*
                printf("[%.0f]<=[%.0f]<=[%.0f]",IntervaloDe,Diferenca,IntervaloAte);
                getch();

*/

                bECoincidencia = ((IntervaloDe<=Diferenca) && (Diferenca<=IntervaloAte)) ? true : false;

                if (bECoincidencia)
                {
                    if (strcmp(StartStopAtual, "START") == 0)
                    {
                        fprintf(ArqCoi,"Ch%sCh%s;%.0lf\n",ChAtual,ChProximo,Diferenca); fflush(ArqCoi); TotCoincidencias++;
                    }
                    else
                    {
                        fprintf(ArqCoi,"Ch%sCh%s;%.0lf\n",ChProximo,ChAtual,Diferenca); fflush(ArqCoi); TotCoincidencias++;
                    }

                    ChAtual=""; ChProximo=""; TimeStampAtual=0; TimeStampProximo=0; EnergiaAtual=0; EnergiaProximo=0; StartStopAtual=""; StartStopProximo="";
                }
                else
                {
                    ChAtual=ChProximo; ChProximo=""; TimeStampAtual=TimeStampProximo; TimeStampProximo=0; EnergiaAtual=EnergiaProximo; EnergiaProximo=0; StartStopAtual=StartStopProximo; StartStopProximo="";
                }
            }

            Ch0Ler = (Ch0End==1) ? false : Ch0Ler;
            Ch1Ler = (Ch1End==1) ? false : Ch1Ler;
            Ch2Ler = (Ch2End==1) ? false : Ch2Ler;
            Ch3Ler = (Ch3End==1) ? false : Ch3Ler;

            if ((Ch0End+Ch1End+Ch2End+Ch3End)>0) Terminou = true;

            TotRegistros=TotCount0+TotCount1+TotCount2+TotCount3;

            if (fmod(TotRegistros,1000000.0)==0)
            {
                printf("\nT:%.0lfM 0:%.0lf 1:%.0lf 2:%.0lf 3:%.0lf C:%.0lf",TotRegistros/1000000,TotCount0,TotCount1,TotCount2,TotCount3,TotCoincidencias);
                strcpy(sAux4,"TOT:");
                sprintf(cFloatToChar, "%.0f", TotRegistros);
                strcat(sAux4,cFloatToChar);
                strcat(sAux4," ");
                strcat(sAux4,"CH0:");
                sprintf(cFloatToChar, "%.0f", TotCount0);
                strcat(sAux4,cFloatToChar);
                strcat(sAux4," ");
                strcat(sAux4,"CH1:");
                sprintf(cFloatToChar, "%.0f", TotCount1);
                strcat(sAux4,cFloatToChar);
                strcat(sAux4," ");
                strcat(sAux4,"CH2:");
                sprintf(cFloatToChar, "%.0f", TotCount2);
                strcat(sAux4,cFloatToChar);
                strcat(sAux4," ");
                strcat(sAux4,"CH3:");
                sprintf(cFloatToChar, "%.0f", TotCount3);
                strcat(sAux4,cFloatToChar);
                strcat(sAux4," ");
                strcat(sAux4,"Coi:");
                sprintf(cFloatToChar, "%.0f", TotCoincidencias);
                strcat(sAux4,cFloatToChar);
                fGravarLog(sAux4,ArqLog);
            }
        }

        strcpy(sAux4,"TOT:");
        sprintf(cFloatToChar, "%.0f", TotRegistros);
        strcat(sAux4,cFloatToChar);
        strcat(sAux4," ");
        strcat(sAux4,"CH0:");
        sprintf(cFloatToChar, "%.0f", TotCount0);
        strcat(sAux4,cFloatToChar);
        strcat(sAux4," ");
        strcat(sAux4,"CH1:");
        sprintf(cFloatToChar, "%.0f", TotCount1);
        strcat(sAux4,cFloatToChar);
        strcat(sAux4," ");
        strcat(sAux4,"CH2:");
        sprintf(cFloatToChar, "%.0f", TotCount2);
        strcat(sAux4,cFloatToChar);
        strcat(sAux4," ");
        strcat(sAux4,"CH3:");
        sprintf(cFloatToChar, "%.0f", TotCount3);
        strcat(sAux4,cFloatToChar);
        strcat(sAux4," ");
        strcat(sAux4,"Coi:");
        sprintf(cFloatToChar, "%.0f", TotCoincidencias);
        strcat(sAux4,cFloatToChar);
        fGravarLog(sAux4,ArqLog);
        printf("\nT:%.0lf 0:%.0lf 1:%.0lf 2:%.0lf 3:%.0lf C:%.0lf\n",TotRegistros,TotCount0,TotCount1,TotCount2,TotCount3,TotCoincidencias);
        /*
        printf("\n\n�ltimos TimeStamp");
        printf("\nCh0:[%s]",MaxCh0);
        printf("\nCh1:[%s]",MaxCh1);
        printf("\nCh2:[%s]",MaxCh2);
        printf("\nCh3:[%s]",MaxCh3);
        */

        fGravarLog("Fechando arquivos",ArqLog);
        printf("\nFechando arquivos");
	    if (strcmp(Url0, "") != 0) fclose(Arq0);
        if (strcmp(Url1, "") != 0) fclose(Arq1);
        if (strcmp(Url2, "") != 0) fclose(Arq2);
        if (strcmp(Url3, "") != 0) fclose(Arq3);
        fclose(ArqCoi);
    }

    time(&end_t);
    diff_t = difftime(end_t, start_t);
    printf("\n\nExecution time(s):[%.0f]\n", diff_t);
    strcpy(sAux4,"Execution time(s):[");
    sprintf(cFloatToChar, "%.0f", diff_t);
    strcat(sAux4,cFloatToChar);
    strcat(sAux4,"]");
    fGravarLog(sAux4,ArqLog);

    printf("\n\nIdentificacao de coincidencias terminada");
    fGravarLog("Identificacao de coincidencias terminada",ArqLog);

    fclose(ArqLog);
}

void sAjustarCoincidencias()
{
	FILE *ArqCoi,*ArqCoiSxn,*ArqAju,*ArqLog;
    TCHAR ini[BUFSIZE];
    TCHAR diretorio[BUFSIZE];
    DWORD dwRet;
    double TotRegistros;
	char Reg[100];
    char *Campos[4];
    int iTotIntervalos;
    char sAux1[50];
    char sAux2[50];
    char sAux3[500];
    char sAux4[500];
    int iTotCanais;
    int iCanal;
    int iAux1, iAux2, iAux3;
    int iCoincidencia;
    double IntervaloDe;
    double IntervaloAte;
    double Unidade;
    double Chave;
    time_t start_t, end_t;
    double diff_t;
    bool Terminou,bAchou;
    char cIntToChar[50];
    char cFloatToChar[100];
    int iTamanhoArray;
    int iLimiteBkGround;
    int i,j,k;
    double MatrizAju[4][4][5000];
    double MatrizBkG[4][4][2];

    printf("\nInicio do ajuste das coincidencias (sAjustarCoincidencias)");

    //dwRet = GetCurrentDirectory(BUFSIZE, ini); <= antes
    // Substitua GetCurrentDirectory por getcwd
    getcwd(ini, BUFSIZE);
    strcat(ini, "/MatchID_Indio.ini");
    fSubstituir(ini, '/', '/');

    //dwRet = GetCurrentDirectory(BUFSIZE, diretorio); <= antes
    // Substitua GetCurrentDirectory por getcwd
    getcwd(diretorio, BUFSIZE);
    DIR *dir;
    struct dirent *lsdir;
    dir = opendir(diretorio);
    /* print all the files and directories within directory */
    //strcat(diretorio,"\\"); <= antes
    strcat(diretorio, "/");    

    char SepCol[3];
    char UrlCoi[500];
    char UrlSxn[500];
    char UrlAju[500];
    char UrlLog[500];

    strcpy(UrlCoi,diretorio);
    strcpy(UrlAju,diretorio);
    strcpy(UrlSxn,diretorio);
    strcpy(UrlLog,diretorio);
    strcat(UrlCoi,"MatchID_Indio_COI.txt");
    strcat(UrlAju,"MatchID_Indio_AJU.txt");
    strcat(UrlSxn,"MatchID_Indio_SXN.txt");
    strcat(UrlLog,"MatchID_Indio_AJU_LOG.txt");
    fSubstituir (UrlCoi,'\\','/');
    fSubstituir (UrlAju,'\\','/');
    fSubstituir (UrlSxn,'\\','/');
    fSubstituir (UrlLog,'\\','/');

    strcpy(SepCol,";");
    iTotCanais =4;
    IntervaloDe=-50000;
    IntervaloAte=973000;
    Unidade=1000;

    ArqLog = fopen(UrlLog, "w");

    fGravarLog("Informacoes iniciais",ArqLog);
    strcpy(sAux4,"ArquivoINI=[");     strcat(sAux4,ini);      strcat(sAux4,"]");  fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"ArquivoEntrada=["); strcat(sAux4,UrlCoi);   strcat(sAux4,"]");  fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"ArquivoSaida=[");   strcat(sAux4,UrlAju);   strcat(sAux4,"]");  fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"ArquivoSxn=[");     strcat(sAux4,UrlSxn);   strcat(sAux4,"]");  fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"ArquivoLog=[");     strcat(sAux4,UrlLog);   strcat(sAux4,"]");  fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"SeparadorCol=[");   strcat(sAux4,SepCol);   strcat(sAux4,"]");  fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"IntervaloDe:[");    sprintf(cFloatToChar,"%.0f", IntervaloDe);  strcat(sAux4,cFloatToChar); strcat(sAux4,"]"); fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"IntervaloAte:[");   sprintf(cFloatToChar,"%.0f", IntervaloAte); strcat(sAux4,cFloatToChar); strcat(sAux4,"]"); fGravarLog(sAux4,ArqLog);
    strcpy(sAux4,"Unidade:[");        sprintf(cFloatToChar,"%.0f", Unidade);      strcat(sAux4,cFloatToChar); strcat(sAux4,"]"); fGravarLog(sAux4,ArqLog);

    printf("\nEm processamento...\n");
    fGravarLog("Em processamento",ArqLog);
    time(&start_t);

    fGravarLog("Abrindo arquivo de coincidencias",ArqLog);
    printf("\nAbrindo arquivo de coincidencias");

	ArqCoi = (strcmp(UrlCoi, "") != 0) ? fopen(UrlCoi, "r") : NULL;

	if (ArqCoi == NULL)
    {
	    printf("\nErro, nao foi possivel abrir o(s) arquivo(s)\n");
        fGravarLog("Erro, nao foi possivel abrir o(s) arquivo(s)",ArqLog);
    }
	else
    {
        fGravarLog("Abrindo arquivo de saida",ArqLog);
        printf("\nAbrindo arquivo de saida");

        ArqAju = fopen(UrlAju, "w");
        ArqCoiSxn = fopen(UrlSxn, "w");

        fGravarLog("Analisando dados",ArqLog);
        printf("\n\nAnalisando dados...");

        TotRegistros = 0;
        IntervaloDe = IntervaloDe / Unidade;
        IntervaloAte = IntervaloAte / Unidade;
        iTotIntervalos = IntervaloAte - IntervaloDe;
        iTotIntervalos = ((IntervaloDe <= 0.0) && (IntervaloAte >= 0.0)) ?  (iTotIntervalos + 1) : iTotIntervalos;
        iLimiteBkGround = IntervaloAte - (IntervaloAte/10);

        iTamanhoArray = iTotCanais * (iTotCanais-1) * iTotIntervalos;
        strcpy(sAux4,"Tamanho array intervalos=[");  sprintf(cIntToChar,"%i", iTamanhoArray); strcat(sAux4,cIntToChar); strcat(sAux4,"]"); fGravarLog(sAux4,ArqLog);
        printf("\niTamanhoArray:[%i]",iTamanhoArray);
        printf("\nAlocando CoincidenciaAju");
        iCoincidencia = 0;

        printf("\nInicializando contadores");
        fGravarLog("Inicializando contadores",ArqLog);
        i=0;
        while (i<iTotCanais)
        {
            j=0;
            while (j<iTotCanais)
            {
                if (i!=j)
                {
                    k=0;
                    while (k<iTotIntervalos)
                    {
                        MatrizAju[i][j][k]=0.0;
                        k++;
                    }
                    MatrizBkG[i][j][0]=0.0;
                    MatrizBkG[i][j][1]=0.0;
                }
                j++;
            }
            i++;
        }
        printf("\nContadores inicializados");
        fGravarLog("Contadores inicializados",ArqLog);


        printf("\nInicio consolidacao coincidencias");
        fGravarLog("Inicio consolidacao coincidencias",ArqLog);
        Terminou=false;
        int p1,p2,p3;
	    while(!Terminou)
        {
            if (fgets(Reg,sizeof(Reg),ArqCoi) != NULL)
            {
                TotRegistros++;
                fSplit(Reg,SepCol,&Campos[0]);
                p1=Campos[0][2]-'0';
                p2=Campos[0][5]-'0';
                p3=(int)((atoi(Campos[1])/Unidade)-IntervaloDe);
                MatrizAju[p1][p2][p3]++;

/*
                printf("\np1=%i",p1);
                printf("\np2=%i",p2);
                printf("\nCampos[1]=%.2f",atof(Campos[1]));
                printf("\nUnidade=%.2f",Unidade);
                printf("\nIntervaloDe=%.2f",IntervaloDe);
                printf("\np3=%i",p3);
                printf("\n%i = (%.2f / %.2f) - %.2f",p3,atof(Campos[1]),Unidade,IntervaloDe);
                getch();
*/

                if (fmod(TotRegistros,1000000)==0)
                {
                    printf("\nCoincidencias distribuidas:[%.0fM]",TotRegistros/1000000);
                    strcpy(sAux4,"Coincidencias distribuidas:[");
                    sprintf(cFloatToChar, "%.0f", TotRegistros);
                    strcat(sAux4,cFloatToChar);
                    strcat(sAux4,"]");
                    fGravarLog(sAux4,ArqLog);
                }
            }
            else
            {
                Terminou = true;
            }
        }

        printf("\nTotal Coincidencias distribuidas:[%.0f]",TotRegistros);
        strcpy(sAux4,"Total Coincidencias distribuidas:[");
        sprintf(cFloatToChar, "%.0f", TotRegistros);
        strcat(sAux4,cFloatToChar);
        strcat(sAux4,"]");
        fGravarLog(sAux4,ArqLog);

        printf("\nInicio tratamento arquivo Saxena");
        fGravarLog("Inicio tratamento arquivo Saxena",ArqLog);
        i=0;
        while (i<iTotCanais)
        {
            j=0;
            while (j<iTotCanais)
            {
                if (i!=j)
                {
                    k=0;
                    while (k<iTotIntervalos)
                    {
                        if (k >= iLimiteBkGround)
                        {
                            MatrizBkG[i][j][0]++;
                            MatrizBkG[i][j][1] += MatrizAju[i][j][k];
                        }
                        fprintf(ArqCoiSxn,"%.0f\n",MatrizAju[i][j][k]);
                        fflush(ArqCoiSxn);
                        k++;
                    }
                }
                j++;
            }
            i++;
        }
        printf("\nFim tratamento arquivo Saxena");
        fGravarLog("Fim tratamento arquivo Saxena",ArqLog);
        //fclose(ArqCoiSxn);

        printf("\nInicio calculo do background");
        fGravarLog("Inicio calculo do background",ArqLog);

        i=0;
        while (i<iTotCanais)
        {
            j=0;
            while (j<iTotCanais)
            {
                if (i!=j)
                {
                    MatrizBkG[i][j][1] = MatrizBkG[i][j][1] / MatrizBkG[i][j][0];
                    printf("\nBackground Ch%iCh%i=[%.0f]",i,j,MatrizBkG[i][j][1]);
                    strcpy(sAux4,"Background Ch");
                    sprintf(cIntToChar, "%i", i);
                    strcat(sAux4,cIntToChar);
                    strcat(sAux4,"Ch");
                    sprintf(cIntToChar, "%i", j);
                    strcat(sAux4,cIntToChar);
                    strcat(sAux4,"=[");
                    sprintf(cFloatToChar, "%.0f", MatrizBkG[i][j][1]);
                    strcat(sAux4,cFloatToChar);
                    strcat(sAux4,"]");
                    fGravarLog(sAux4,ArqLog);
                }
                j++;
            }
            i++;
        }
        printf("\nFim calculo do background");
        fGravarLog("Fim calculo do background",ArqLog);

        printf("\nAplicando background");
        fGravarLog("Aplicando background", ArqLog);

        i=0;
        while (i<iTotCanais)
        {
            j=0;
            while (j<iTotCanais)
            {
                if (i!=j)
                {
                    k=0;
                    while (k<iTotIntervalos)
                    {
                        MatrizAju[i][j][k] = MatrizAju[i][j][k] - MatrizBkG[i][j][1];
                        if (MatrizAju[i][j][k] < 0.0) MatrizAju[i][j][k] = 0.0;
                        fprintf(ArqAju,"Ch%iCh%i;%i;%.0f\n",i,j,k,MatrizAju[i][j][k]);
                        k++;
                    }
                }
                j++;
            }
            i++;
        }

        fGravarLog("Fechando arquivos", ArqLog);
        printf("\nFechando arquivos");
        fclose(ArqCoi);
        fclose(ArqCoiSxn);
        fclose(ArqAju);
    }

    time(&end_t);
    diff_t = difftime(end_t, start_t);
    printf("\n\nExecution time(s):[%.0f]\n", diff_t);
    strcpy(sAux4,"Execution time(s):[");
    sprintf(cFloatToChar, "%.0f", diff_t);
    strcat(sAux4,cFloatToChar);
    strcat(sAux4,"]");
    fGravarLog(sAux4,ArqLog);

    fGravarLog("Terminado!",ArqLog);

    fclose(ArqLog);
    printf("\nEncerrando o MatchID!\n");
}




