/*************************************************************************
Descrição do Algoritmo: 

Nome da Disciplina:  Algoritmos II
Nome Professor: Leonair Neves Sousa
Nome Aluno: Mateus Augusto Silva
Turma: Sistemas de Informação 2021/3
RGA: 202111316026
Data Entrega: 12/08/2022
*************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<math.h>
#include <windows.h>

// Estruturas das tabelas:
typedef struct Genero{
	int codgenero;
	char nome[45];
	
	int excluido; // Varivel parametro para definir se o Genero foi Excluido;
    int usado; // Varivel parametro para definir se o Genero esta sendo usado em cadastro de filme;
    
};

typedef struct Filme{
	int codfilme;
	char nome[45];
	int codgenero;
	int ano;
	char classificaco[5];
	float valor;
	
	char nome_genero[45]; //Variavel para armazenar ,tambem, o nome do genero em que o filme esta vinculado;
	int excluido; // Varivel parametro para definir se o Genero foi Excluido;
	int locado; // Varivel parametro para definir se o Filme esta em locação;
	
};

typedef struct Cliente{
	int codcli;
	char nome[45];
	char rg[12];
	char cpf[14];
	char email[45];
	char cel[12];
	char endereco[30];
	char data_nasc[10];
	
	int excluido; // Varivel parametro para definir se o registro foi Excluido;
    int usado; // Varivel parametro para definir se o registro esta sendo usado em outra tabela;
	
};

typedef struct Funcionario{
	int codfunc;
	char nome[45];
	char rg[12];
	char cpf[14];
	char email[45];
	char cel[12];
	char data_nasc[10];
	char dataadm[10];
	
	int excluido; // Varivel parametro para definir se o registro foi Excluido;
    int usado; // Varivel parametro para definir se o registro esta sendo usado em outra tabela;
};

typedef struct Locacao{
	int codlocacao;
	char dataloca[11];
	char datadevo[11];
	float valortotal;
	float juro;
	float desc;
	int codcliente;
	int codfunc;
	char pagamento; //'A' = A vista || 'D' = Na Devolução;
	
	int qtd_fil; // Armazena a quantidade de filmes vinculados a locação;
	int excluido; // Varivel parametro para definir se o registro foi Excluido;
    int finalizada; // Parametro para definir se a locação já foi finalizada;
	int usado; // Varivel parametro para definir se o registro esta sendo usado em outra tabela;
};

typedef struct Locacao_Filme{
	int codloc;
	int codfilme;
};

typedef struct Devolucao{
	int codloc;
	char dataloca[11];
	char data_devolucao[11];
	float ValorOriginal;
	float ValorPago;
	int dias_atraso;
	
	char pagamento; // Registrar qual foi a forma de pagamento escolhida na Locação;
	char dataloca_devo[11]; //Registrar a data de devolução prevista na locação (para verificar atraso);
	int id_devo; //Importante cada tabela ter seu ID;
};

//Cadastro e Listagem de Generos:
void cadastro_genero(FILE *arq_gen){
	Genero reg;
	Genero dado;
	FILE *arq_aux;
	int aux;
	
	arq_gen = fopen("generos.txt","a+"); //abrindo um arquivo para ler e escrever no fim do arquivo
	printf("\n\tCADASTRO DE GENEROS:\n");
	if (arq_gen==NULL){ //testa abertura do arquivo     
	   	printf("Erro na abertura do Arquivo\n");       
    }else{
		
		// Registro do Codigo sequencial;	
		aux = 0;
		arq_aux = fopen("generos.txt","r+");
		fseek(arq_aux,-sizeof(struct Genero),SEEK_SET);
		while (!feof(arq_aux)){
			fread(&dado,sizeof(struct Genero),1,arq_aux); 
			if (!feof(arq_aux)) //verifica se chegou ao final do arquivo
	      		aux = dado.codgenero;   
		}
		fclose(arq_aux);        
	   	reg.codgenero = aux+1;
	   	
	   	//Registra Nome do Genero:
		printf("\nInforme Nome do novo Genero: ");          
	   	fflush(stdin);
		scanf("%[^\n]",&reg.nome); 
		
		//Registra como FAlSO os parametros de Excluido e Usado:       
		reg.excluido = 0;
		reg.usado = 0;
		
		//Salva o registro no arquivo:           
	   	fwrite(&reg,sizeof(struct Genero),1,arq_gen);//armazena o registro                 
	}  
	fclose(arq_gen);//fechamos o arquivo
}

void listar_genero(FILE *arq_gen){
	arq_gen = fopen("generos.txt","r+");  // a+ insere no final do arquivo binário 
	struct Genero dado;
	if (arq_gen==NULL){//testa abertura do arquivo    
		printf("Não há Clientes Cadastrados no Arquivo\n");     
	}else{  
		printf("\n\tGENEROS CADASTRADOS:\n");  
		fseek(arq_gen,-sizeof(struct Genero),SEEK_SET);
		while (!feof(arq_gen)){  //verifica se está no fim do arquivo
			fread(&dado,sizeof(struct Genero),1,arq_gen); 
			if ((!feof(arq_gen))&&(dado.excluido == 0)){
		      printf("Codigo: %i\tNome: %s",dado.codgenero,dado.nome);   
			  //printf("\tUSADO: %i",dado.usado); 
			  printf("\n");       		      
		  	}     
		}
	}
	fclose(arq_gen); //fechar arquivo
}

// Cadastro e Listagem de Filmes:
void cadastro_filme(FILE *arq_fil){
	Filme reg;
	FILE *arq_gen;
	Genero dado_gen;
	Filme dado;
	FILE *arq_aux;
	int flag1, aux;
	
	arq_fil = fopen("filmes.txt","a+"); //abrindo um arquivo para ler e escrever no fim do arquivo
	printf("CADASTRO DE FILME:");
	if (arq_fil==NULL){//testa abertura do arquivo     
	   	printf("Erro na abertura do Arquivo\n");    
		//system("pause");   
		exit(1);    
    }else{
		   	
	   	// Registro do Codigo sequencial;	
		aux = 0;
		arq_aux = fopen("filmes.txt","r+");
		fseek(arq_aux,-sizeof(struct Filme),SEEK_SET);
		while (!feof(arq_aux)){
			fread(&dado,sizeof(struct Filme),1,arq_aux); 
			if (!feof(arq_aux)) //verifica se chegou ao final do arquivo
	      		aux = dado.codfilme;   
		}
		fclose(arq_aux);        
	   	reg.codfilme = aux+1;
	   	
	   	//Registra o Nome do Filme:
	   	system("cls");
	   	printf("\nInforme Nome do Filme: ");          
	   	fflush(stdin);
		scanf("%[^\n]",&reg.nome); 
		
		
		//Registro do Codigo do Genero à ser vinculado:
		system("cls");
		flag1=0;aux=0; 
		long pos; 
		while(flag1==0){ 
			printf("\nSELECIONE O CODIGO DO GENERO DO FILME:\n\n ");  
			listar_genero(arq_gen);        
	   		scanf("\n%i",&aux);          
			fflush(stdin); 
			// Verificação se o codigo de Genero realmente existe cadastrado
			arq_gen = fopen("generos.txt","r+");
			//fseek(arq_gen,-sizeof(struct Genero),SEEK_SET);
			pos = ftell(arq_gen);
			while ((!feof(arq_gen))&&(flag1==0)){  //verifica se está no fim do arquivo
				fread(&dado_gen,sizeof(struct Genero),1,arq_gen); 
				if ((dado_gen.excluido == 0)&&(aux==dado_gen.codgenero)){
		      		flag1=1;   
					strcpy(reg.nome_genero,dado_gen.nome); // Copia o Nome do Genero;
					dado_gen.usado = 1; // Define TRUE no Registro do Genero o parametro de vinculo ativo;   
					fseek(arq_gen,pos,SEEK_SET);
					fwrite(&dado_gen,sizeof(struct Genero),1,arq_gen); 		   
				}
				pos = ftell(arq_gen);    
			}
			fclose(arq_gen);
			if(flag1!=1){
				printf("Codigo de Genero Não Encontrado!\n");
				Sleep(1200); 
				system("cls");
			}	
	   	}
	   	reg.codgenero = aux;
		
		//Demais registros do cadastro:        
		system("cls");
		printf("Informe o Ano de lançamento do Filme: ");
		//fflush(stdin);
		//scanf("%s",reg.ano);
		fflush(stdin);
		scanf("%i",&reg.ano);
		system("cls");
		printf("Informe a classificação indicativa do Filme: ");
		fflush(stdin);
		//scanf("%s",reg.classificaco);
		gets(reg.classificaco);
		system("cls");
		printf("Informe o valor de locação do Filme: ");
		fflush(stdin);
		scanf("%f",&reg.valor);

		
		reg.locado = 0;
		reg.excluido = 0;
	   	
		system("cls");
	   	
		// Armazena no arquivo;   
		fwrite(&reg,sizeof(struct Filme),1,arq_fil);                 
	}  
	fclose(arq_fil);//fechando o arquivo
}

void listar_filme(FILE *arq_fil){
	arq_fil = fopen("filmes.txt","r+");
	Filme dado;
	if (arq_fil==NULL){//testa abertura do arquivo    
		printf("Não há Filmes Cadastrados no Arquivo\n");       
	}else{  
		printf("\tFILMES CADASTRADOS:\n");  
		fseek(arq_fil,-sizeof(struct Filme),SEEK_SET);
		while (!feof(arq_fil)){  //verifica se está no fim do arquivo
			fread(&dado,sizeof(struct Filme),1,arq_fil); 
			if ((!feof(arq_fil))&&(dado.excluido == 0)&&(dado.locado==0)){ //verifica se chegou ao final do arquivo
		    	printf("Codigo: %2i\tNome: %s \tGenero: %10s \tAno de Lançamento: %4i",dado.codfilme,dado.nome,dado.nome_genero,dado.ano);           		      
				printf("\tClassificação: %2s \tValor: %.2f \n",dado.classificaco,dado.valor);		  	
			}     
		}
	}
	fclose(arq_fil); //fechar arquivo
}

// Cadastro e Listagem de Clientes:
void cadastro_cliente(FILE *arq_cli){
	Cliente reg;
	Cliente dado;
	FILE *arq_aux;
	int flag1, aux;
	arq_cli = fopen("clientes.txt","a+"); //abrindo um arquivo para ler e escrever no fim do arquivo
	printf("CADASTRO DE CLIENTES:");
	if (arq_cli==NULL){//testa abertura do arquivo     
	   	printf("Erro na aberutra do Arquivo\n");       
    }else{        
	   		   	
	   	// Registro do Codigo sequencial;	
		aux = 0;
		arq_aux = fopen("clientes.txt","r+");
		fseek(arq_aux,-sizeof(struct Cliente),SEEK_SET);
		while (!feof(arq_aux)){
			fread(&dado,sizeof(struct Cliente),1,arq_aux); 
			if (!feof(arq_aux)) //verifica se chegou ao final do arquivo
	      		aux = dado.codcli;   
		}
		fclose(arq_aux);        
	   	reg.codcli = aux+1;

	   	// Demais registros do cadastro:
	   	system("cls");
		printf("\tCADASTRO DE CLIENTES:\n");
		fflush(stdin);
		printf("Informe Nome do Cliente: ");          
		gets(reg.nome);   
		fflush(stdin);   
		system("cls");
		printf("\tCADASTRO DE CLIENTES:\n");
		printf("Informe o RG do Cliente: ");
		gets(reg.rg);
		fflush(stdin);
		system("cls");
		printf("\tCADASTRO DE CLIENTES:\n");
		printf("Informe o CPF do Cliente: ");
		gets(reg.cpf);
		fflush(stdin);
		system("cls");
		printf("\tCADASTRO DE CLIENTES:\n");
		printf("Informe o E-mail do Cliente: ");
		gets(reg.email);
		fflush(stdin);
		system("cls");
		printf("\tCADASTRO DE CLIENTES:\n");
		printf("Informe o número de Telefone/Celular do Cliente: ");
		gets(reg.cel);
		fflush(stdin);
		system("cls");
		printf("\tCADASTRO DE CLIENTES:\n");
		printf("Informe o Endereço do Cliente: ");
		gets(reg.endereco);
		fflush(stdin);
		system("cls");
		printf("\tCADASTRO DE CLIENTES:\n");
		printf("Informe a Data de Nascimento do Cliente (DD/MM/AAAA): ");
		gets(reg.data_nasc);
		fflush(stdin);
		
		reg.excluido = 0;
		reg.usado = 0;
		
		//armazena o registro
	   	fwrite(&reg,sizeof(struct Cliente),1,arq_cli);                 
	}  
	system("cls");
	fclose(arq_cli);//fecha o arquivo
}

void listar_cliente(FILE *arq_cli){
	arq_cli = fopen("clientes.txt","r+");
	Cliente dado;
	if (arq_cli==NULL){//testa abertura do arquivo    
		printf("Não há Clientes Cadastrados no Arquivo\n");       
	}else{  
		printf("\tCLIENTES CADASTRADOS:\n");  
		fseek(arq_cli,-sizeof(struct Cliente),SEEK_SET);
		while (!feof(arq_cli)){  //verifica se está no fim do arquivo
			fread(&dado,sizeof(struct Cliente),1,arq_cli); 
			if ((!feof(arq_cli))&&(dado.excluido == 0)){ //verifica se chegou ao final do arquivo
		      printf("Codigo: %i \tNome: %s \n",dado.codcli,dado.nome);           		      
		  	}     
		}
	}
	fclose(arq_cli); //fechar arquivo
}

// Cadastro e Listagem de Funcionarios:
void cadastro_funcionario(FILE *arq_fun){
	Funcionario reg;
	Funcionario dado;
	FILE *arq_aux;
	int flag1,aux;
	arq_fun = fopen("funcionarios.txt","a+"); //abrindo um arquivo para ler e escrever no fim do arquivo
	printf("CADASTRO DE FUNCIONARIO:");
	if (arq_fun==NULL){//testa abertura do arquivo     
	   	printf("Não há Funcionarios Cadastrados no Arquivo\n");       
    }else{
		// Registro do Codigo sequencial;	
		aux = 0;
		arq_aux = fopen("funcionarios.txt","r+");
		fseek(arq_aux,-sizeof(struct Funcionario),SEEK_SET);
		while (!feof(arq_aux)){
			fread(&dado,sizeof(struct Funcionario),1,arq_aux); 
			if (!feof(arq_aux)) //verifica se chegou ao final do arquivo
	      		aux = dado.codfunc;   
		}
		fclose(arq_aux);        
	   	reg.codfunc = aux+1;
		
		system("cls");
		printf("\tCADASTRO DE FUNCIONARIO:\n");
		fflush(stdin);           
	   	printf("Informe Nome do Funcionario: ");          
	   	gets(reg.nome);  
		fflush(stdin);    
		system("cls");
		printf("\tCADASTRO DE FUNCIONARIO:\n");
		printf("Informe o RG do Funcionario: ");
		gets(reg.rg);
		fflush(stdin);
		system("cls");
		printf("\tCADASTRO DE FUNCIONARIO:\n");
		printf("Informe o CPF do Funcionario: ");
		gets(reg.cpf);
		fflush(stdin);
		system("cls");
		printf("\tCADASTRO DE FUNCIONARIO:\n");
		printf("Informe o E-mail do Funcionario: ");
		gets(reg.email);
		fflush(stdin);
		system("cls");
		printf("\tCADASTRO DE FUNCIONARIO:\n");
		printf("Informe o número de Telefone do Funcionario: ");
		gets(reg.cel);
		fflush(stdin);
		system("cls");
		printf("\tCADASTRO DE FUNCIONARIO:\n");
		printf("Informe a Data de Nascimento do Funcionario (DD/MM/AAAA): ");
		gets(reg.data_nasc);
		fflush(stdin);
		system("cls");
		printf("\tCADASTRO DE FUNCIONARIO:\n");
		printf("Informe a Data de Admissão do Funcionario (DD/MM/AAAA): ");
		gets(reg.dataadm);
		fflush(stdin);
		
		reg.excluido = 0;
		reg.usado = 0;
		
	   	fwrite(&reg,sizeof(struct Funcionario),1,arq_fun);//armazena o registro                 
	}  
	fclose(arq_fun);//fechamos o arquivo
}

void listar_funcionario(FILE *arq_fun){
	arq_fun = fopen("funcionarios.txt","r+");
	Funcionario dado;
	if (arq_fun==NULL){//testa abertura do arquivo    
		printf("Não há Funcionarios Cadastrados no Arquivo\n");       
	}else{  
		printf("\tFUNCIONARIOS CADASTRADOS:\n");  
		fseek(arq_fun,-sizeof(struct Funcionario),SEEK_SET);
		while (!feof(arq_fun)){  //verifica se está no fim do arquivo
			fread(&dado,sizeof(struct Funcionario),1,arq_fun); 
			if ((!feof(arq_fun))&&(dado.excluido == 0)){ //verifica se chegou ao final do arquivo
		      printf("Codigo: %i \tNome: %s \n",dado.codfunc,dado.nome);           		      
		  	}     
		}
	}
	fclose(arq_fun); //fechar arquivo
}

//Procedimento para registrar a Locação do Filme;
void locafilme(int codloc, int codfil){
	FILE *arq_aux;
	Locacao_Filme dado;
	Filme dado_fil;
	
	arq_aux = fopen("loc_fil.txt","a+");
	if (arq_aux==NULL){//testa abertura do arquivo     
	   	printf("Erro na abertura do Arquivo 'loc_fil'\n");
		system("pause");      
    }else{
    	//define a o filme à locaçao na tabela e salva no arquivo:
    	dado.codfilme = codfil;
    	dado.codloc = codloc;
		fwrite(&dado,sizeof(struct Locacao_Filme),1,arq_aux);
		fclose(arq_aux);	
	}
	
	//Define o filme com vinculo ativo:
	arq_aux = fopen("filmes.txt","r+");
	//fseek(arq_gen,-sizeof(struct Genero),SEEK_SET);
	long pos = ftell(arq_aux);
	while (!feof(arq_aux)){  //verifica se está no fim do arquivo
		fread(&dado_fil,sizeof(struct Filme),1,arq_aux); 
		if (codfil==dado_fil.codfilme){
			dado_fil.locado = 1; // Define TRUE no Registro do Filme o parametro de locação;   
			fseek(arq_aux,pos,SEEK_SET);
			fwrite(&dado_fil,sizeof(struct Filme),1,arq_aux); 		   
		}
		pos = ftell(arq_aux);    
	}
	fclose(arq_aux);
	
}

// Procedimento de Locação:
void cadastro_locacao(FILE *arq_loc){
	Locacao reg;
	
	FILE *arq_aux;
	Filme dado_fil;
	Funcionario dado_fun;
	Cliente dado_cli;
	
	Locacao dado;
	FILE *arq_loc_aux;
	int flag1,flag2, aux,op,dias,meses;
	char dia[2], mes[2];
	
	arq_loc = fopen("locacoes.txt","a+"); //abrindo um arquivo para ler e escrever no fim do arquivo
	printf("\tCADASTRO DE LOCAÇAO:");
	if (arq_loc==NULL){//testa abertura do arquivo     
	   	printf("Erro na abertura do Arquivo\n");      
    }else{
    	
    	// Registro do Codigo sequencial da Locação;	
		aux = 0;
		arq_aux = fopen("locacoes.txt","r+");
		fseek(arq_aux,-sizeof(struct Locacao),SEEK_SET);
		while (!feof(arq_aux)){
			fread(&dado,sizeof(struct Locacao),1,arq_aux); 
			if (!feof(arq_aux)) //verifica se chegou ao final do arquivo
	      		aux = dado.codlocacao;   
		}
		fclose(arq_aux);        
	   	reg.codlocacao = aux+1;
	   	
	   	//Registro da Data em que esta sendo realizada a Locação:
	   	printf("\nInforme a Data em que esta sendo realizada a Locação (DD/MM/AAAA): ");          
	   	fflush(stdin);
		gets(reg.dataloca); 
		fflush(stdin);
		
		system("cls");
		printf("\tCADASTRO DE LOCAÇAO:\n");
		//Registro do Codigo do Funcionario:
		flag1=0;  
		while(flag1==0){ 
			printf("Selecione o Codigo do Funcionario:\n ");  
			listar_funcionario(arq_aux);        
	   		printf("\n");
			scanf("%i",&aux);          
			fflush(stdin); 
			// Verificação se o codigo de Funcionario realmente existe cadastrado
			arq_aux = fopen("funcionarios.txt","r+");
			fseek(arq_aux,-sizeof(struct Funcionario),SEEK_SET);
			while ((!feof(arq_aux))&&(flag1==0)){  //verifica se está no fim do arquivo
				fread(&dado_fun,sizeof(struct Funcionario),1,arq_aux); 
				if ((dado_fun.excluido == 0)&&(aux==dado_fun.codfunc)){
		      		flag1=1;         		       			
				}    
			}
			fclose(arq_aux);
			if(flag1!=1){
				printf("Codigo de Funcionario Não Encontrado!\n");
				Sleep(1200); 
				system("cls");
			}	
	   	}
	   	reg.codfunc = aux;
		
		system("cls");
		printf("\tCADASTRO DE LOCAÇAO:\n\n");
		//Registro do Codigo do Cliente:
		flag1=0;  
		while(flag1==0){ 
			printf("SELECIONE O CODIGO DO CLIENTE::\n ");  
			listar_cliente(arq_aux);        
	   		printf("\n");
			scanf("%i",&aux);          
			fflush(stdin); 
			// Verificação se o codigo de Funcionario realmente existe cadastrado
			arq_aux = fopen("clientes.txt","r+");
			fseek(arq_aux,-sizeof(struct Cliente),SEEK_SET);
			while ((!feof(arq_aux))&&(flag1==0)){  //verifica se está no fim do arquivo
				fread(&dado_cli,sizeof(struct Cliente),1,arq_aux); 
				if ((dado_cli.excluido == 0)&&(aux==dado_cli.codcli)){
		      		flag1=1;         		       			
				}    
			}
			fclose(arq_aux);
			if(flag1!=1){
				printf("Codigo de Cliente Não Encontrado!\n");
				Sleep(1200); 
				system("cls");
			}	
	   	}
	   	reg.codcliente = aux;
		
		//Registro de Filmes da Locação:
		system("cls");
		printf("\tCADASTRO DE LOCAÇAO:\n\n");
		reg.qtd_fil = 0;
		do{
			flag1=0;  
			while(flag1==0){ 
				printf("SELECIONE O CODIGO DO FILME:\n\n ");  
				listar_filme(arq_aux);        
		   		printf("\n");
				scanf("%i",&aux);          
				fflush(stdin); 
				// Verificação se o codigo de Funcionario realmente existe cadastrado
				arq_aux = fopen("filmes.txt","r+");
				fseek(arq_aux,-sizeof(struct Filme),SEEK_SET);
				while ((!feof(arq_aux))&&(flag1==0)){  //verifica se está no fim do arquivo
					fread(&dado_fil,sizeof(struct Filme),1,arq_aux); 
					if ((dado_fil.excluido == 0)&&(dado_fil.locado==0)&&(aux==dado_fil.codfilme)){
			      		flag1=1;
						aux = dado_fil.codfilme;         		       			
					}    
				}
				if(flag1!=1){
					printf("Codigo do Filme Não Encontrado!\n");
					Sleep(1200); 
					system("cls");
				}else{
					reg.qtd_fil++;
					reg.valortotal=reg.valortotal+dado_fil.valor;
				}	
				fclose(arq_aux);
	   		}
	   		
			//locafilme(reg.codlocacao,aux); // Faz a chamado da função para registrar a Locação do Filme;
			aux = 0;
	   		printf("\n1 - Selecionar mais um Filme \t2- Ir para pagamento.\n");
			scanf("%i",&op);
			fflush(stdin);
			system("cls");
		}while(op==1);
		
		system("cls");
		printf("\tCADASTRO DE LOCAÇAO:\n\n");
		printf("\tTELA DE PAGAMENTO:\n");
		//Registro do Pagamento:
		do{
			printf("Selecione a forma de Pagamento:\n1 - Pagamento a Vista\t2 - Pagamento na Devolução\n");
			scanf("%i",&op);
			fflush(stdin);
			system("cls");
			printf("\tCADASTRO DE LOCAÇAO:\n\n");
			printf("\tTELA DE PAGAMENTO:\n");
			switch(op){
				case 1:
					reg.pagamento = 'A';
					printf("Digite valor de desconto ('0' caso não tenha): ");
					scanf("%f",&reg.desc);
					fflush(stdin);
					reg.valortotal = reg.valortotal-(reg.valortotal*0.05)-reg.desc;
					printf("\nValor Total à pagar: R$ %.2f",reg.valortotal);
					break;
					
				case 2:
					reg.pagamento = 'D';
					printf("Digite o percentual do juros para atrasos (Sem '%%'): ");
					scanf("%f",&reg.juro);
					fflush(stdin);
					reg.juro = reg.juro/100;
					break;
				default:
					printf("Opção Invalida!");
				}
		}while(op!= 1 && op!=2);
		              
		//Essa parte verifica qual sera a data de devolução;
		int d,m,y,ne;
	 	char data[11];
	 	strcpy(data,reg.dataloca);
		char dia[11];
		char *str_aux;
		char mes[11];
		char ano[11];
		char dataf[11];
		
		strncpy(dia,data,2);
		d = atoi(dia);
	
		str_aux = &data[3];
		strncpy(mes,str_aux,2);
		m = atoi(mes);
		
		str_aux = &data[6];
		strncpy(ano,str_aux,4);
		y = atoi(ano);
	 
	 	if(reg.qtd_fil>3){
	 		ne = 4;
		}else{
		 	ne = 3;
		}
	 	
	 	int m2[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	 	int i,k=0,p,a;
	 	for(i=0;i<m;i++)
	  		k=k+m2[i];
	 	k=k+d+ne;
	 	if(k<=365)
	 	{
	  		for(i=0;i<13;i++)
	  		{
	   			p=k-m2[i];
	   			if(p<=m2[i+1])
	   			{
	    			a=i+1;
	    			break;
	   			}
	   			else
	    			k=p;
	  		}
	  		//printf("%i/%i/%i",p,a,y);
	  		itoa(p,dia,10);
	  		itoa(a,mes,10);
	  		itoa(y,ano,10);
			sprintf(dataf,"%s/%s/%s",dia,mes,ano);
	 	}
	 	else
	 	{
	  		k=k-365;
	   		for(i=0;i<13;i++)
	    	{
	     		p=k-m2[i];
	     		if(p<=m2[i+1])
	     		{
	       			a=i+1;
	       			break;
	     		}
	     		else
	      			k=p;
	    	}
	  		//printf("%i/%i/%i",p,a,y+1);
	  		itoa(p,dia,10);
	  		itoa(a,mes,10);
	  		itoa(y,ano,10);
			sprintf(dataf,"%s/%s/%s",dia,mes,ano);
	 	}
		
		strcpy(reg.datadevo,dataf);
		printf("\n\nData definida para devolução: %s\n",reg.datadevo);
		
		reg.excluido = 0;
		reg.usado = 0;
		reg.finalizada = 0;
		
	   	fwrite(&reg,sizeof(struct Locacao),1,arq_loc);//armazena o registro                 
	}  
	fclose(arq_loc);//fechamos o arquivo
}

void listar_locacao(FILE *arq_loc){
	Locacao dado;
	
	arq_loc = fopen("locacoes.txt","r+"); //abrindo um arquivo para ler e escrever no fim do arquivo
	
	if (arq_loc==NULL){//testa abertura do arquivo     
	   	printf("Não há Locações Cadastradas no Arquivo\n");      
    }else{
    	printf("\tLOCAÇÕES CADASTRADAS:\n");  
		fseek(arq_loc,-sizeof(struct Locacao),SEEK_SET);
		while (!feof(arq_loc)){  //verifica se está no fim do arquivo
			fread(&dado,sizeof(struct Locacao),1,arq_loc); 
			if ((!feof(arq_loc))&&(dado.excluido == 0)){ //verifica se chegou ao final do arquivo
		      printf("Codigo Locação: %i\tCodigo Cliente: %i\tCodigo Funcionario: %i\tValor: %.2f\tData da Locação: %s\tData para Devolução: %s \n",dado.codlocacao,dado.codcliente,dado.codfunc,dado.valortotal,dado.dataloca,dado.datadevo);           		      
		  	}     
		}
	}
	fclose(arq_loc); //fechar arquivo
}

//Função Simples para Converter as datas em dias totais (não é tão precisa mais funciona para o codigo aqui :D )
int conta_dias(int dia,int mes,int ano){
    int a=0,dias=0;
    dias = dia + 365.25*ano;
    switch(mes){
		case 12: dias=dias+30;
		case 11: dias=dias+31;
		case 10: dias=dias+30;
		case 9: dias=dias+31;
		case 8: dias=dias+31;
		case 7: dias=dias+30;
		case 6: dias=dias+31;
		case 5: dias=dias+30;
		case 4: dias=dias+31;
		case 3: dias=dias+28;
		case 2: dias=dias+31;
		case 1: dias=dias;
	}	
	return dias;
}

void devolucao(){
	/* O entendimento foi que o "cliente" deve realizar a Devolução de toda Locação.
	Ou seja, se no momento da Locação, esta foi feita com 3 filmes, no momento da devolução
	deverá ser irá devolver os 3 filmes. Sendo assim, o codigo não foi parametrizado para devolver 1 filme por vez.	
	*/
	
	FILE *arq_devo;
	FILE *arq_aux;
	FILE *arq_loc;
	
	Devolucao reg;
	Devolucao dado;
	
	int juros;
	
	arq_devo = fopen("devolucoes.txt","a+");
	printf("\tREGISTRO DE DEVOLUÇÃO:\n");
	if (arq_devo==NULL){//testa abertura do arquivo     
	   	printf("Erro na abertura do Arquivo\n");      
    }else{
    	
    	// Registro do ID da Devolução;	
		int aux = 0;
		arq_aux = fopen("devolucoes.txt","r+");
		fseek(arq_aux,-sizeof(struct Devolucao),SEEK_SET);
		while (!feof(arq_aux)){
			fread(&dado,sizeof(struct Devolucao),1,arq_aux); 
			if (!feof(arq_aux)) //verifica se chegou ao final do arquivo
	      		aux = dado.id_devo;   
		}
		fclose(arq_aux);        
	   	reg.id_devo = aux+1;
	   	
	   	//Registro do Codigo da Locação à ser devolvida:
	   	printf("Informe o Codigo da Locação à ser devolvida: ");          
	   	
		Locacao dado_loc;
		arq_loc = fopen("locacoes.txt","r+"); //abrindo um arquivo para ler e escrever no fim do arquivo
		if (arq_loc==NULL){//testa abertura do arquivo     
		   	printf("Não há Locações Cadastradas no Arquivo\n");      
	    }else{
	    	printf("\LOCAÇÕES CADASTRADAS:\n");  
			fseek(arq_loc,-sizeof(struct Locacao),SEEK_SET);
			while (!feof(arq_loc)){  //verifica se está no fim do arquivo
				fread(&dado_loc,sizeof(struct Locacao),1,arq_loc); 
				if ((!feof(arq_loc))&&(dado_loc.excluido == 0)&&(dado_loc.finalizada == 0)){ //verifica se chegou ao final do arquivo
			      printf("Codigo Locação: %i\tCodigo Cliente: %i\tCodigo Funcionario: %i\tValor: %.2f\tData da Locação: %s\tData para Devolução: %s \n",dado_loc.codlocacao,dado_loc.codcliente,dado_loc.codfunc,dado_loc.valortotal,dado_loc.dataloca,dado_loc.datadevo);           		      
			  	}     
			}
		}
		fclose(arq_loc); //fechar arquivo
	   	fflush(stdin);
		
		scanf("%i",&reg.codloc);
	   	fflush(stdin);
	   	
	   	// Usuario informa a data em que esta sendo realizada a devolução:
	   	printf("Informe a Data em que esta sendo realizada a Devolução (DD/MM/AAAA): ");          
	   	fflush(stdin);
		gets(reg.data_devolucao);
	   	fflush(stdin);
	   	
	   	//Abrindo o arquivo Locação para coletar os valores à registrar na tabela Devolucao:
	   	arq_loc = fopen("locacoes.txt","r+"); //abrindo um arquivo para ler e escrever no fim do arquivo
		if (arq_loc==NULL){//testa abertura do arquivo     
		   	printf("Não há Locações Cadastradas no Arquivo\n");      
	    }else{
	    	int flag1 = 0;
	    	long pos = ftell(arq_loc);
			while ((!feof(arq_loc))&&(flag1==0)){  //verifica se está no fim do arquivo
				fread(&dado_loc,sizeof(struct Locacao),1,arq_loc); 
				if (reg.codloc==dado_loc.codlocacao){
					flag1=1;
					strcpy(reg.dataloca,dado_loc.dataloca);
					strcpy(reg.dataloca_devo,dado_loc.datadevo);
					//strcpy(reg.pagamento,dado_loc.pagamento);
					reg.pagamento = dado_loc.pagamento;
					juros = dado_loc.juro;
					reg.ValorOriginal = dado_loc.valortotal;
					
					// Define TRUE no parametro de Finalizada a locação;
					dado_loc.finalizada = 1;    
					fseek(arq_loc,pos,SEEK_SET);
					fwrite(&dado_loc,sizeof(struct Locacao),1,arq_loc); 
						   
				}
				pos = ftell(arq_loc);    
			}
			printf("\nCHEGUEI AQUI\n");	
		}
		fclose(arq_loc); //fechar arquivo
		
		if(reg.pagamento == 'D'){
			
			//Converter a Data String em INT para realizar calculo;
			int d,m,y;
		 	char data[11];
		 	strcpy(data,reg.data_devolucao);
			char dia[11];
			char *str_aux;
			char mes[11];
			char ano[11];
			char dataf[11];
			strncpy(dia,data,2);
			d = atoi(dia);
			str_aux = &data[3];
			strncpy(mes,str_aux,2);
			m = atoi(mes);
			str_aux = &data[6];
			strncpy(ano,str_aux,4);
			y = atoi(ano);
			
			aux = conta_dias(d,m,y);
			
			strcpy(data,reg.dataloca_devo);
			strncpy(dia,data,2);
			d = atoi(dia);
			str_aux = &data[3];
			strncpy(mes,str_aux,2);
			m = atoi(mes);
			str_aux = &data[6];
			strncpy(ano,str_aux,4);
			y = atoi(ano);
			
			//calculo dos dias de atraso:
			reg.dias_atraso = aux - conta_dias(d,m,y);
			
			printf("\nForma de Pagamento Escolhida: 'Na Devolução'");
			//Caculo do valor, considerando juros em cima dos dias em atraso:
			if(reg.dias_atraso>0){
				printf("\nDias em Atraso: %i",reg.dias_atraso);
				reg.ValorPago = reg.ValorOriginal + reg.ValorOriginal*juros*reg.dias_atraso;
			}else{
				printf("\nDias em Atraso: 0");
				reg.ValorPago = reg.ValorOriginal;
			}
			printf("\nValor Devido: %.2f",reg.ValorPago);
			
		}else{
			printf("\nForma de Pagamento Efetuado: 'A Vista'");
			reg.ValorPago = reg.ValorOriginal;
			printf("\nValor Pago na Locação: %.2f",reg.ValorPago);
		}
	
		system("pause");
	}
	
	fwrite(&reg,sizeof(struct Devolucao),1,arq_devo);
	fclose(arq_devo);
	
}


int main(){
	UINT CPAGE_UTF8 = 65001; SetConsoleOutputCP(CPAGE_UTF8);
	
	//definição dos arquivos para cada tabela:
	FILE *arq_gen;
	FILE *arq_fil;
	FILE *arq_cli;
	FILE *arq_fun;
	FILE *arq_loc;
	
	//Demais declarações:
	int op, flag1, flag2;
	
	flag1 = 0;
	do{
		system("cls");
		//exibição do Menu:
		printf("\tMENU\n");
		printf(" 1.Cadastro e Listar Gênero\n 2.Cadastro e Listar Filmes\n 3.Cadastro e Listar Cliente\n");
		printf(" 4.Cadastro e Listar funcionário\n 5.Cadastro e Listar Locação\n 6.Fazer Devolução\n");
		printf(" 7.Excluir gênero\n 8.Excluir Filmes\n 9.Excluir Cliente\n");
		printf("10.Excluir Funcionário\n11.Excluir Locação\n12.Listar locação data especifica\n");
		printf("13.Listar filmes em atraso\n14.Listar devolução\n15.Sair\n");
		
		//leitura e execução da opção escolhida:
		scanf("\n%i",&op);
	
		switch(op){
			case 1:
				flag2 = 0;
				system("cls");
				do{
					printf("1. Listar Generos Cadastrados.\t\t2. Cadastro de Novo Genero.\t\t3.Sair\n");
					scanf("%i",&op);
					system("cls");
					switch (op){
					case 1:
						listar_genero(arq_gen);	
						break;
					case 2:
						cadastro_genero(arq_gen);
						break;
					case 3:
						flag2=1;
						break;
					}
					printf("\n");	
				}while (flag2!=1);
				break;
			
			case 2:
				flag2 = 0;
				system("cls");
				do{
					printf("\n1. Listar Filmes Cadastrados.\t\t2. Cadastro de Novo Filme.\t\t3.Sair\n");
					scanf("%i",&op);
					system("cls");
					switch (op){
					case 1:
						listar_filme(arq_fil);	
						break;
					case 2:
						cadastro_filme(arq_fil);
						break;
					case 3:
						flag2=1;
						break;
					}	
				}while (flag2!=1);
				system("cls");
				break;	
				
				case 3:
				flag2 = 0;
				system("cls");
				do{
					printf("\n\n1. Listar Clientes Cadastrados.\t\t2. Cadastro de Novo Cliente.\t\t3.Sair\n");
					scanf("%i",&op);
					system("cls");
					switch (op){
					case 1:
						listar_cliente(arq_cli);	
						break;
					case 2:
						cadastro_cliente(arq_cli);
						break;
					case 3:
						flag2=1;
						break;
					}	
				}while (flag2!=1);
				system("cls");
				break;	
			
			case 4:
				flag2 = 0;
				system("cls");
				do{
					printf("1. Listar Funcionarios Cadastrados.\t\t2. Cadastro de Novo Funcionario.\t\t3.Sair\n");
					scanf("%i",&op);
					system("cls");
					switch (op){
					case 1:
						listar_funcionario(arq_fun);	
						break;
					case 2:
						cadastro_funcionario(arq_fun);
						break;
					case 3:
						flag2=1;
						break;
					}
					printf("\n");	
				}while (flag2!=1);
				break;
			case 5:
				flag2 = 0;
				system("cls");
				do{
					printf("1. Listar Locações Cadastradas.\t\t2. Cadastro de Nova Locação.\t\t3.Sair\n");
					scanf("%i",&op);
					system("cls");
					switch (op){
					case 1:
						listar_locacao(arq_loc);	
						break;
					case 2:
						cadastro_locacao(arq_loc);
						break;
					case 3:
						flag2=1;
						break;
					}
					printf("\n");	
				}while (flag2!=1);
				break;
			
			case 6: 
				devolucao();
				break;
			
			case 15:
				flag1=1;
				break;
				
			default:
				system("cls");
				printf("Opção Invalidada! Retornando ao Menu...");
				Sleep(1500);
		}
	}while (flag1!=1);
	
	return 0;
}