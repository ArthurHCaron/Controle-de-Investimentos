/*
    Projeto Controle de Investimentos

    Autores:
    Andr� Luiz Gon�alves da Silva Teixeira - RA: 2564289
    Arthur Henrique Caron                  - RA: 2564297
    Jo�o Pedro Trevisan Borghi             - RA: 2564424
    
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define MAXCUST 100 //Constante para m�ximo de consumidores.
#define MAXINVEST 30 //Constante para m�ximo de investimentos.
#define MAXTRADE 101 //Constante para m�ximo de transa��es.

typedef struct Date{
    int day;
    int month;
    int year;
} Date;

typedef struct Telephone{
    int DDD;
    long int number;
} Telephone;

typedef struct Customer{
    char name[50];
    char cpf[13];
    Telephone phone;
    Date birth;
} Customer;

typedef struct Investment{
    int applicationType;
    char emitter[52];
    float interestRate;
    char isActive;
} Investment;

typedef struct Trade{
    int tradeId;
    Customer customer;
    Investment investment;
    Date applicationDate;
    float applicationValue;
    Date withdrawDate;
    float withdrawValue;
} Trade;

int verifyDate(Date date);
int verifyTelephone(Telephone ph);
int verifyCPF(char cpf[12]);
int verifyName(char name[]);
int difDate(Date finalDate, Date startDate); //Fun��o que retorna diferen�a de dias entre duas datas
float calcInvest(Trade cTrade);
Investment registerInvestiment(void);
Customer registerCustomer(void);
Trade registerTrade(int Id);
Date scanDate(void);
void showInvestment(void);
void showExtract(char matchCPF[]);
void showSum(void);
void showCustumer(void);
int multipleCPF(char mCPF[]);
Date catchLocalDate(void);
char wContinue(void);

int contCust = 0, //N�mero de clientes cadastrados
    contTrade = 0, //N�mero de aplica��es cadastradas.
    contInvest = 0; //N�mero de investimentos cadastrados.
Trade trades[MAXTRADE];
Customer customers[MAXCUST];
Investment investments[MAXINVEST];

int main(){  
    setlocale(LC_ALL, "Portuguese");
    int match = -1, //Verifica se h� CPFs correspondentes.
        activeInvest = 0; //N�mero de investimentos ativos.
    char ask = 'x'; //Verifica se o usu�rio quer ou n�o continuar cadastrando clientes. Verifica, tamb�m, a op��o escolhida no menu.
    char resumeCPF[13];

    do{
        printf("********************Menu********************\n");
        printf("|(1) Cadastrar investimento                |\n");
        printf("|(2) Mostrar todos os clientes cadastrados |\n");
        printf("|(3) Apresentar extrato                    |\n");
        printf("|(4) Cadastrar cliente                     |\n");
        printf("|(5) Cadastrar aplica��o                   |\n");
        printf("|(6) Consultar valor em cust�dia           |\n");
        printf("|(7) Sair                                  |\n");
        printf("|------------------------------------------|\n");
        printf("Escolha uma op��o: ");
        scanf("%c", &ask);
        getchar();
        system("cls");

        switch(ask){
            case '1':
                do{
                    if(contInvest >= MAXINVEST){
                        printf("N�mero m�ximo de investimentos cadastrados j� foi atingido.\n");
                        printf("Pressione ENTER para continuar");
                        getchar();
                        system("cls");
                        break;
                    }

                    investments[contInvest] = registerInvestiment();
                    
                    if(investments[contInvest].isActive == 'S'){
                        activeInvest++;
                    }
                    
                    contInvest++; //Incrementa o n�mero de investimentos cadastrados
                    
                    if(contInvest < MAXINVEST){ //Verifica se o n�mero m�ximo n�o foi atingido
                        ask = wContinue();
                    }
                    else break;
                    
                } while(ask != 'n');
                
                break;

            case '2':
                showCustumer();
                
                printf("Pressione ENTER para continuar");
                getchar();
                
                system("cls");
                
                break;
                
            case '3':
                    printf("********Extrato********\n\n");
                    printf("CPF: ");
                    fgets(resumeCPF, 13, stdin);
                    resumeCPF[strlen(resumeCPF)-1] = '\0';
                    match = multipleCPF(resumeCPF);

                    if(match == -1){
                        printf("\nCPF n�o cadastrado.\n");
                        printf("Pressione qualquer tecla para continuar...");
                        getchar();
                        system("cls");
                    
                    } else{
                        showExtract(customers[match].cpf);
                    }

                break; 
            case '4':
                do{
                    if(contCust >= MAXCUST){ //Verifica se o n�mero m�ximo de cadastros j� n�o foi atingido.
                        printf("N�mero m�ximo de clientes cadastrados j� foi atingido.\n\n");
                        printf("Pressione ENTER para voltar ao menu");
                        getchar();
                        system("cls");
                        break;
                    }

                    customers[contCust] = registerCustomer();
                    contCust++; //Inteirando contCust para indicar novo cadastro.

                    if(contCust < MAXCUST){ //Verifica se o n�mero m�ximo de cadastros n�o foi atingido (de novo).
                        ask = wContinue();
        
                    } else {
                        ask = 'n';
                    }
                    system("cls");
               
                } while(ask != 'n');

                break;
                
            case '5':
                do{
                    if(contInvest==0 || activeInvest == 0){ //Verifica se h� investimentos dispon�veis
                            printf("Nenhuma op��o de investimento dispon�vel\n\n");
                            printf("Pressione ENTER para voltar ao menu");
                            getchar();
                            system("cls");
                            break;
                    }
                    else if(contTrade >= MAXTRADE){ //Verifica se o n�mero m�ximo de cadastros foi atingido.
                        printf("N�mero m�ximo de aplica��es cadastradas j� foi atingido.\n");
                        printf("Pressione ENTER para continuar");
                        getchar();
                        system("cls");
                        break;
                    }
                
                    printf("********Cadastrar aplica��o********\n\n");
                    printf("CPF: ");
                    fgets(trades[contTrade].customer.cpf, 13, stdin);
                    trades[contTrade].customer.cpf[strlen(trades[contTrade].customer.cpf)-1] = '\0';
                    match = multipleCPF(trades[contTrade].customer.cpf); // Verifica se o CPF est� cadastrado. se match != - 1, o nome est� cadastrado.
                    
                    if(match == -1){
                        printf("\nCPF n�o cadastrado");
                        printf("\n\nDeseja voltar ao menu? (y/n): ");
                        scanf("%c", &ask);

                        while(ask != 'y' && ask != 'n'){
                            printf("(Inv�lido) Digite novamente: ");
                            scanf(" %c", &ask);
                        }
                        getchar();
                        if(ask=='y'){
                            system("cls");
                            break;
                        }
                        else{
                            ask = 'y';
                        }
                    }
                    else{
                        trades[contTrade].customer = customers[match];
                        trades[contTrade] = registerTrade(contTrade+1);
                        contTrade++; //Inteirando contTrade para indicar nova aplica��o.

                        if(contTrade < MAXTRADE){ //Verifica se o n�mero m�ximo de cadastros foi atingido (de novo).
                           ask = wContinue();
                            
                        } else {
                            ask = 'n';
                        }
                    }
                    
                    system("cls");
               
                } while(ask != 'n');
                break;
                
            case '6':
                showSum();
                
                printf("\nPressione ENTER para continuar");
                getchar();
                system("cls");
                
                break;
            
            case '7':
                printf("Encerrando programa...\n");
                break;

            default:
                printf("(Inv�lido) - Pressione ENTER para continuar");
                getchar();
                system("cls");
        }

    } while(ask != '7');

    return 0;
}

int verifyDate(Date date){
    if(date.day<1 || date.day>30) return 1;
    else if(date.month<1 || date.month>12) return 2;
    else if(date.year<1900 || date.year>2023) return 3;
    else return 0;

    /* A fun��o retorna 0 para caso a data seja v�lida. Se h� um problema
       no dia, a fun��o retorna 1. Se h� um problema no m�s, retorna 2, e
       se h� no ano, retorna 3. o retorno de 1 n�o garante que o m�s e o
       ano estejam corretos, mas o retorno de 3 garante que dia e m�s est�o
       corretos. Analogamente para o retorno de 2. */
}

int verifyTelephone(Telephone ph){
  if(ph.DDD <= 11 || ph.DDD >= 92) return 1;
  else if(ph.number>=10000000 && ph.number<=99999999) return 0;
  else if(ph.number>=900000000 && ph.number<=999999999) return 0;
  else return 2;

  /* A fun��o funciona de modo an�logo � verifyDate(). O retorno ser� 0 caso
     o n�mero esteja correto. O primeiro if verifica se o DDD est� correto. Os
     outros dois ifs verificam se o n�mero � correto. O primeiro verifica para 
     casos de n�meros com 8 d�gitos num�ricos e o segundo para n�meros com 9 
     d�gitos caso o primeiro seja um 9. Caso o n�mero esteja incorreto, o re-
     torno � de 2. */
}

int verifyCPF(char cpf[12]){
    int score = 0, //Guarda o n�mero de erros no CPF
        value = 0; //Valor da multiplica��o dos n�meros do CPF
    
    if(strlen(cpf)!=11) return 12; // Tamanho inv�lido
    else{
        for(int i = 0; i<11; i++){ // Verifica se o CPF cont�m apenas n�meros
            if(!(cpf[i]>=48 && cpf[i]<=57)){
                score++;
            }
        }

        if(score==0){
            for(int i = 0; i<9; i++){
                value += (cpf[i] - 48) * (10 - i);
            }

            if(value%11 < 2){
                if((cpf[9] - 48) != 0) return 13; // Pen�ltimo n�mero inv�lido
            }
            else if((11 - value%11) != (cpf[9] - 48)) return 13; // Pen�ltimo n�mero inv�lido

            value = 0;
            for(int i = 0; i<10; i++){
                value += (cpf[i] - 48) * (11 - i);
            }

            if(value%11 < 2){
                if((cpf[10] - 48) != 0) return 14; // �ltimo n�mero inv�lido
            }
            else if((11 - value%11) != (cpf[10] - 48)) return 14; // �ltimo n�mero inv�lido
            else return 0; //CPF v�lido.
        }
        else return score;
    }
}  

int verifyName(char name[]){ //A fun��o verifica se o nome � v�lido (apenas letras no nome, primeiro caractere n�o pode ser um espa�o)
    int nameLength = strlen(name);

    if(name[0] == ' '){
        return 1;
    }

    for(int i = 0; i < nameLength; i++){
        if(((name[i] < 'A' || name[i] > 'Z') && (name[i] < 'a' || name[i] > 'z')) && name[i] != ' '){
            return 1;
        }
    }

    return 0;
}

void showCustumer(void){
    Customer aux;
    
    for(int i = 0; i < contCust - 1; i++){
        for(int j = i + 1; j < contCust; j++){
            if(strcmp(customers[i].name, customers[j].name) > 0){
                aux = customers[i];
                customers[i] = customers[j];
                customers[j] = aux;
            }
        }
    }
    
    printf("********Clientes cadastrados********\n\n");
    
    printf("--------------------------------------------------------------\n\n");
    
    for(int i = 0; i < contCust; i++){
        printf("Nome: %s \n", customers[i].name);
        printf("Telefone: (%d) %ld\n", customers[i].phone.DDD, customers[i].phone.number);
        printf("\n--------------------------------------------------------------\n\n");
    }
}

int difDate(Date finalDate, Date startDate){ //A fun��o considera um m�s com 30 dias, e, portanto, um ano com 360 dias
    return (finalDate.year - startDate.year)*360 + (finalDate.month - startDate.month)*30 + finalDate.day - startDate.day;
}

Investment registerInvestiment(void){
    char option = '\0';
    Investment rInvestiment;
    
        printf("********Cadastrar investimento********\n");
        printf("|Tipo de investimento:               |\n");
        printf("|(1) LCI/LCA                         |\n");
        printf("|(2) CDB                             |\n");
        printf("|(3) Fundos                          |\n");
        printf("|------------------------------------|\n");
        printf("\nEscolha uma op��o: ");
        scanf(" %c", &option);

        while(option < '1' || option > '3'){
            printf("(Inv�lido) Escolha uma op��o: ");
            scanf(" %c", &option);
        }
        

    switch (option){
    case '1': //Tipo 1: LCI/LCA
        rInvestiment.applicationType = 1; //Tipo 1: LCI/LCA

        break;
    case '2':
        rInvestiment.applicationType = 2; //Tipo 2: CDB
        
        break;
    case '3':
        rInvestiment.applicationType = 3; //Tipo 3: Fundos

        break;
    }
    getchar();
    printf("�rg�o emissor: ");
    fgets(rInvestiment.emitter, 50, stdin);
    printf("Taxa de rendimento: ");
    scanf("%f", &rInvestiment.interestRate);
    getchar();
    printf("Ativar investimento? (S/N) ");
    scanf("%c", &rInvestiment.isActive);
    getchar();

    while(rInvestiment.isActive != 'S' && rInvestiment.isActive != 'N'){
        printf("(Inv�lido) ativar investimento? (S/N) ");
        scanf("%c", &rInvestiment.isActive);
        getchar();
    }

    return rInvestiment;
}

Customer registerCustomer(void){
    int customerNameL = 0; //Tamanho do nome do cliente
    Customer rCustomer;

    printf("********Cadastrar cliente********\n\n");
    printf("Informe o DDD: ");
    scanf("%d", &rCustomer.phone.DDD);

    while(verifyTelephone(rCustomer.phone)==1){
        printf("(Inv�lido) Informe o DDD: ");
        scanf("%d", &rCustomer.phone.DDD);
    }
    
    printf("Informe o n�mero: ");
    scanf("%li", &rCustomer.phone.number);

    while(verifyTelephone(rCustomer.phone)==2){
        printf("(Inv�lido) Informe o n�mero: ");
        scanf("%li", &rCustomer.phone.number);
    }
    
    rCustomer.birth = scanDate();

    while(verifyDate(rCustomer.birth)!=0){
        printf("Data inv�lida! Digite novamente!\n");
        rCustomer.birth = scanDate();
    }
    
    printf("Nome: ");
    fgets(rCustomer.name, 50, stdin);
    rCustomer.name[strlen(rCustomer.name)-1] = '\0';

    while(verifyName(rCustomer.name) != 0){
        printf("(Apenas letras e espa�o, primeiro caractere deve ser letra) nome: ");
        fgets(rCustomer.name, 50, stdin);
        rCustomer.name[strlen(rCustomer.name)-1] = '\0';
    }

    customerNameL = strlen(rCustomer.name);

    /* A se��o seguinte padroniza os nomes dos usu�rios para que as palavras sempre comecem com mai�sculas
       e tenham min�sculas nas demais letras*/

    if(rCustomer.name[0] >= 'a'){
        rCustomer.name[0] -= 32; //32 � a taxa de convers�o entre mai�sculas e min�sculas em ASCII
    }

    for(int i = 1; i < customerNameL; i++){
        if(rCustomer.name[i] < 'a' && rCustomer.name[i - 1] != ' '){ //Se a letra for mai�scula && n�o vir em in�cio de palavra
            rCustomer.name[i] += 32; //Torna-se min�scula
        } else if(rCustomer.name[i - 1] == ' '){ 
            rCustomer.name[i] -= 32;
        }
    }

    printf("(Apenas os 11 n�meros, sem pontos e h�fen) CPF: ");
    fgets(rCustomer.cpf, 13, stdin);
    
    rCustomer.cpf[strlen(rCustomer.cpf)-1] = '\0';
    
    while(multipleCPF(rCustomer.cpf) != -1){
        printf("CPF j� cadastrado. Digite outro: ");
        fgets(rCustomer.cpf, 13, stdin);
        rCustomer.cpf[strlen(rCustomer.cpf) - 1] = '\0';
    }

    while(verifyCPF(rCustomer.cpf) != 0){
        printf("(Apenas os 11 n�meros, sem pontos e h�fen) CPF: ");
        fgets(rCustomer.cpf, 13, stdin);
        rCustomer.cpf[strlen(rCustomer.cpf)-1] = '\0';
    }
    
    return rCustomer;
}

Trade registerTrade(int Id){
    int tInvestment; // Armazena o n�mero do investimento escolhido.
    Date pDate = catchLocalDate(); // Armazena a data atual.
    Trade rTrade;
    
    rTrade.tradeId = Id;
    printf("\nEscolha um tipo de investimento:\n\n");
    showInvestment();
    printf("\nInvestimento n�mero: ");
    scanf("%d", &tInvestment);
    
    while(tInvestment<1 || tInvestment>contInvest){
        printf("Investimento inv�lido, digite outro: ");
        scanf("%d", &tInvestment);
    }
    
    while(investments[tInvestment - 1].isActive == 'N'){
        printf("Investimento inativo. Escolha outro: ");
        scanf("%d", &tInvestment);
    }

    rTrade.investment = investments[tInvestment-1];
    
    printf("\n--Data de aplica��o--\n");
    rTrade.applicationDate=scanDate();
    
    while(verifyDate(rTrade.applicationDate)!=0 || difDate(pDate, rTrade.applicationDate)<0){
        printf("Data inv�lida! Digite novamente: \n");
        printf("\n--Data de aplica��o--\n");
        rTrade.applicationDate=scanDate();
    }
    
    printf("\nValor da aplica��o: R$ ");
    scanf("%f", &rTrade.applicationValue);
    
    printf("\n--Data de retirada--\n");
    rTrade.withdrawDate=scanDate();
    
    while((verifyDate(rTrade.withdrawDate)!=0 && verifyDate(rTrade.withdrawDate)!=3) ||  difDate(rTrade.withdrawDate, rTrade.applicationDate)<0){
        printf("Data inv�lida! Digite novamente: \n");
        printf("\n--Data de retirada--\n");
        rTrade.withdrawDate=scanDate();
    }

    rTrade.withdrawValue = calcInvest(rTrade);
    printf("**********************\n");
    
    return rTrade;
}

Date scanDate(void){

    Date date;

    printf("Dia (1-30): ");
    scanf("%d", &date.day);
    printf("M�s (1-12): ");
    scanf("%d", &date.month);
    printf("Ano: ");
    scanf("%d", &date.year);
    getchar();

    return date;
}

float calcInvest(Trade cTrade){ //Fun��o que calcula valores relacionados a um investimento. Funciona em sistema de juros simples.
    int time = difDate(cTrade.withdrawDate, cTrade.applicationDate); //Vari�vel que guarda diferen�a de dias entre aplica��o e resgate.
    float profit = (time / 30) * (cTrade.investment.interestRate / 100) * cTrade.applicationValue, //Lucro sem redu��es
          taxes = 0,
          adm = 0; //Taxa de administra��o
    
    if(cTrade.investment.applicationType != 1){
      if(time <= 180){
        taxes = 0.225 * profit;
      } else if(time >= 181 && time <= 360){
        taxes = 0.2 * profit;
      } else if(time >= 361 && time <= 720){
        taxes = 0.175 * profit;
      } else {
        taxes = 0.15 * profit;
      }
      
      if(cTrade.investment.applicationType == 3){
        adm = 0.01 * time / 360 * cTrade.applicationValue;
      }
    }

    cTrade.withdrawValue = profit + cTrade.applicationValue - (taxes + adm);

    printf("\n**********************\n");
    printf("Transa��o n�: %d\n", cTrade.tradeId);
    printf("Tipo: ");
    
    switch(cTrade.investment.applicationType){
        case 1:
            printf("LCI/LCA\n");
            break;
        case 2:
            printf("CDB\n");
            break;
        case 3:
            printf("Fundos\n");
    }
    printf("Valor inicial: R$ %.2f\n", cTrade.applicationValue);
    printf("Taxa de rendimento: %.2f %%\n", cTrade.investment.interestRate);
    printf("Lucro (sem redu��o de impostos): %.2f\n", profit);
    printf("Impostos: %.2f\n", taxes);
    printf("Taxas adicionais: %.2f\n", adm);
    printf("Valor a ser resgatado: %.2f\n", cTrade.withdrawValue);
    printf("Lucro (com redu��es): %.2f\n", cTrade.withdrawValue - cTrade.applicationValue);

    return cTrade.withdrawValue;
}

void showInvestment(void){
    printf("--------------------------------------------------------------\n");
    
    for(int i=0; i< contInvest; i++){
        if(investments[i].isActive == 'S'){
            printf("Investimento %d", i+1);
            printf("\n\nTipo de investimento: ");
            if(investments[i].applicationType==1) printf("LCI/LCA.");
            else if(investments[i].applicationType==2) printf("CDB.");
            else if(investments[i].applicationType==3) printf("Fundos.");
            printf("\nTaxa de rendimento: %.2f", investments[i].interestRate);
            printf("\n�rg�o emissor: ");
            puts(investments[i].emitter);
            
            printf("--------------------------------------------------------------\n");
        }
    }
}

void showExtract(char matchCPF[]){ //O par�metro matchCPF � o CPF do cliente do qual deseja-se ver o extrato. limitTrades � o m�ximo de transa��es cadastradas.
    for(int i = 0; i < contTrade; i++){
        float auxWithdrawValue = 0;
        Date auxDate;

        if(strcmp(trades[i].customer.cpf, matchCPF) == 0){
            auxDate = trades[i].withdrawDate;
            auxWithdrawValue = trades[i].withdrawValue;
            trades[i].withdrawDate = catchLocalDate();    
            trades[i].withdrawValue = calcInvest(trades[i]);
            trades[i].withdrawDate = auxDate;
            trades[i].withdrawValue = auxWithdrawValue;

            printf("\n");
        }
    }

    printf("****************\n");
    printf("Pressione qualquer tecla para continuar");
    getchar();
    system("cls");
}

void showSum(void){
    float sum = 0, // Registra o montante em cust�dia do banco.
          sumLC = 0, // Valor total aplicado em LCI/LCA.
          sumCDB = 0, // Valor total aplicado em CDB.
          sumFundos = 0; // Valor total aplicado em Fundos.
    
    for(int i = 0; i < contTrade; i++){
        sum += trades[i].applicationValue;
        
        if(trades[i].investment.applicationType==1){
            sumLC += trades[i].applicationValue;
            
        } else if (trades[i].investment.applicationType==2){
            sumCDB += trades[i].applicationValue;
            
        } else sumFundos += trades[i].applicationValue;
    }
    
    printf("Montante total em cust�dia do banco: R$ %.2f\n\n", sum);
    printf("Montante separado por tipo de aplica��o:\n");
    printf("___________________________________________\n");
    printf("|Total aplicado em LCI/LCA: R$ %11.2f|\n", sumLC);
    printf("|Total aplicado em CDB....: R$ %11.2f|\n", sumCDB);
    printf("|Total aplicado em Fundos.: R$ %11.2f|\n", sumFundos);
}

int multipleCPF(char mCPF[]){ //Fun��o que busca se h� ou n�o um CPF igual cadastrado. limitCust � o n�mero de clientes cadastrados.
    for(int i=0; i < contCust; i++){
        if(strcmp(mCPF, customers[i].cpf)==0){
            return i; //O retorno � o �ndice do CPF igual cadastrado.
        }
    }

    return (-1);
}

Date catchLocalDate(void){
    struct tm *dateCatcher;
    time_t localTime = time(NULL);
    Date localDate;

    /* dateCatcher � um ponteiro para a data atual do computador. localTime � uma vari�vel que
       recebe da fun��o time() a contagem atual de segundos. localDate ser� o retorno */

    dateCatcher = localtime(&localTime); //localtime() converte a contagem em segundos de localTime para o formato de data.
    localDate.day = dateCatcher->tm_mday + 1; //O dia do retorno recebe o dia de dateCatcher. O +1 � necess�rio porque a biblioteca considera dias de 0 a 30
    localDate.month = dateCatcher->tm_mon + 1; //Recebe o m�s. Os meses da biblioteca v�o de 0 a 11.
    localDate.year = dateCatcher->tm_year + 1900; //A biblioteca faz uma contagem considerando o dia 01/01/1900 como a data 00/00/0000

    /* Realizei diversos testes envolvendo a catchLocalDate() e a difDate(). Pela maneira como a
       difDate() funciona, n�o precisamos nos preocupar com a catchLocalDate() capturando os 31� dias
       de qualquer m~es. Para fins de c�lculo, a diferen�a de dias � sempre a mesma, convertendo o
       dia 31/x para 01/x+1 ou n�o. */

    return localDate;
}

char wContinue(void){ //Verifica se o usu�rio quer continuar a cadastrar algo
    char wcontinue = 'x';
    printf("\nDeseja continuar cadastrando? (y/n) ");
    scanf(" %c", &wcontinue);

    while(wcontinue != 'y' && wcontinue != 'n'){
        printf("(Inv�lido) Deseja continuar cadastrando? (y/n) ");
        scanf(" %c", &wcontinue);
    }
    getchar();
    system("cls");

    return wcontinue;
}