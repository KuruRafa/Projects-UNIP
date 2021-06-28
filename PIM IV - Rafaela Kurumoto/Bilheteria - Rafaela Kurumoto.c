//Universidade Paulista EaD
//Projeto Integrado Multidisciplinar IV
//Curso: Analise e Desenvolvimento de Sistemas
//Aluna: Rafaela Sumie Andrade Kurumoto RA: 0500384
//Sistema de Bilheteria de Teatro

//bibliotecas de C
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

//Protótipos das funções
void statusSessao(void);
int escolhaPeca(void);
int dataHora(int peca);
int validaEscolha(int escolha, int *poltronaReservada);
void exibePoltronas(int *poltrona);
int escolhaPoltronas(int *poltrona, char *status, int *poltronaReservada);
float descontoPreco(int codigo, int qtd);
void pagamento(float valor);
void impressaoIngresso (int opcao, int quantidade, int *poltronaReservada);
void limpaReserva (int *poltronaReservada);
void  fechamento(void);

/*Variáveis globais
Declarei como globais para ficar mais fácil usar dentro de algumas funções não ficando uma lista enorme de parâmetros a serem passados.
E também para não zerar a cada vez que o programa volta ao menu principal. 
Usadas para status de cada sessão, poltronas de cada sessão, total vendido de cada peça, total vendido de ingressos,  total de descontos de 50%, 
total de descontos de 100%, total de ingressos pagos no valor integral, valor total passado em dinheiro, valor total passado no cartão. 
Inicializei estas variáveis para 'zerar' todas elas e não ocorrer erros nos cálculos. */
char status1[11]="Disponivel", status2[11]="Disponivel", status3[11]="Disponivel", status4[11]="Disponivel", status5[11]="Disponivel", status6[11]="Disponivel";
char status7[11]="Disponivel", status8[11]="Disponivel", status9[11]="Disponivel", status10[11]="Disponivel", status11[11]="Disponivel", status12[11]="Disponivel";
int poltronas1[20]={0}, poltronas2[20]={0}, poltronas3[20]={0}, poltronas4[20]={0}, poltronas5[20]={0}, poltronas6[20]={0};
int poltronas7[20]={0}, poltronas8[20]={0}, poltronas9[20]={0}, poltronas10[20]={0}, poltronas11[20]={0}, poltronas12[20]={0};
int cont1=0, cont2=0, cont3=0, cont4=0, cont5=0, cont6=0, cont7=0, cont8=0, cont9=0, cont10=0, cont11=0, cont12=0;
int totIng=0, tot50=0, tot100=0, totInt=0;
float caixa=100.00, cartao=0.0; //O caixa fui inicilizado com R$100 para troco, caso a primeira compra necessite. Este valor não altera o fechamento final do caixa, já que é removido na função de fechamento. 

//Função principal
int main()
{
	//Variáveis locais
	int mainMenu, escolha, quantidade, peca;	
	int poltronaReservada[20]={0}; //inicializei todos os index do vetor com 0, pois 0 representa uma poltrona vaga
	float totalPreco;
	
	//primeiro loop para validar o comando de saída do programa, que acontece quando o usuário digita '0'. 
	//Usei o DO loop porque assim o bloco de comandos executa uma vez antes de validar a escolha.
	do
	{
		//Menu Principal
		system("cls"); //comando para limpar a tela, só funciona no windows. Foi aplicado diversas vezes no programa para a interface ficar organizada.
		printf("***Bem-Vindo(a) a Bilheteria do Teatro***\n\n");
		printf("Selecione uma opcao no menu abaixo:\n\n");
		printf("Iniciar Venda         (1)\n");
		printf("Fechamento de Caixa   (2)\n");
		printf("Encerrar programa     (0)\n");
		printf("\nDigite o numero e aperte 'enter': ");
		scanf("%d", &mainMenu); //guarda a opçao do menu principal
		
		switch (mainMenu)
		{
			case 1:
				do //segundo loop para validar se o usuário escolheu uma quantidade > que 0 de igressos.
				{	
					system("cls");
					totalPreco=0.0;
					peca=escolhaPeca(); //chama a função de escolha de peça e retorna um numero inteiro equivalente a peca escolhida
					if (peca==0) //se o usuário digitar '0' no menu de escolha da peça, o programa retorna ao menu principal através do comando break, saindo do segundo loop
					{
						totalPreco=-1.0; //dentro do if, mudo a variável totalPreco para validar no if fora do loop
						break; //comando para sair do segundo loop
					}
					statusSessao(); //procedimento para atualizar o status das sessões (Disponível ou Esgotado)
					escolha = dataHora(peca); // função para a escolha de data e hora, recebe a variavel peça e retorna uma escolha
					if (escolha==0) //se o usuário digitar '0' no menu de escolha da peça, o programa retorna ao menu principal através do comando break, saindo do segundo loop
					{
						totalPreco=-1.0; //idem ao procedimento if acima
						break;
					} 
					quantidade = validaEscolha(escolha, poltronaReservada); // a função validaEscolha chama uma função para escolha de poltronas e retorna a quantidade de lugares reservados
				} while (quantidade==0); //se a quantidade for 0, retorna para o menu de escolha de peças. Se não, termina o segundo loop
				if (totalPreco==-1.0) //se a variável totalPreco foi mudada acima para -1.0, o if executa o comando break para sair do primeiro loop e voltar ao menu principal.
				{
					break;
				}
				totalPreco = descontoPreco(escolha, quantidade); //a função descontoPreco recebe a escolha e a quantidade para retornar o preço total a ser pago pelo comprador
				totIng+=quantidade; // a variável global de total de ingressos (totIng) acumula ao seu valor a quantidade de ingressos comprados
				pagamento(totalPreco); // a função de pagamento recebe o preço total e realiza o pagamento
				impressaoIngresso (escolha, quantidade, poltronaReservada); //chama-se a função de impressaoIngresso, recebendo como parâmetros a escolha, quantidade e poltronaReservada
				limpaReserva (poltronaReservada); // Função para limpar a reserva e preparar o sistema para a proxima reserva
				break;
				
			case 2:
				system("cls");
				fechamento(); // chama a função para exibir o fechamento de caixa
				system("pause");
				break;
		}
	} while (mainMenu!=0); // quando o usuário entrar o valor '0', o programa é encerrado.
	system("cls");
	printf("\nSistema Encerrado.\nAte Logo!!! \n");
	
	return 0;
}

//Função para Escolher a Peça, retorna um inteiro
int escolhaPeca()
{
	int escolha;
	do //loop para retornar caso o usuário não entre uma escolha válida
	{
			system("cls");
			printf("Escolha a Peca que o espectador deseja assistir:\n\n");
			printf("Peca                        Preco Inteira    Codigo\n\n");
			printf("O Rei Leao                  R$25,00          (1)\n");
			printf("Os Improvaveis              R$15,00          (2)\n");
			printf("Sai de Baixo                R$20,00          (3)\n\n");
			printf("Retornar ao menu principal                   (0)\n\n");
			printf("Escolha: ");
			scanf("%d", &escolha);
			
			if(escolha<0 || escolha>3) //if para validar a escolha
			{
				printf("\nEscolha invalida, tente novamente.\n");
				system("pause"); //comando que pausa o computador até o usuário apertar alguma tecla. só funciona em windows
			}
	} while (escolha<0 || escolha>3);
		
		
	return escolha; //retorna a opcao de escolha entre 0 e 3, sendo 0 para retornar ao menu principal 
}


//Função de Status
void statusSessao() 
{	
	//checa se a contagem de cada sessao, quando alcançar 20, muda-se o status para "Esgotado".
	if (cont1==20)
		strcpy(status1, "Esgotado  ");
		 
	if (cont2==20)
		strcpy(status2, "Esgotado  ");
		
	if (cont3==20)
		strcpy(status3, "Esgotado  ");
		
	if (cont4==20)
		strcpy(status4, "Esgotado  ");
	
	if (cont5==20)
		strcpy(status5, "Esgotado  ");
		 
	if (cont6==20)
		strcpy(status6, "Esgotado  ");
		
	if (cont7==20)
		strcpy(status7, "Esgotado  ");
		
	if (cont8==20)
		strcpy(status8, "Esgotado  ");
		
	if (cont9==20)
		strcpy(status9, "Esgotado  ");
		 
	if (cont10==20)
		strcpy(status10, "Esgotado  ");
		
	if (cont11==20)
		strcpy(status11, "Esgotado  ");
		
	if (cont12==20)
		strcpy(status12, "Esgotado  ");
		
	return;		
}

//Função de escolha da Peça, Horário, retorna um código
int dataHora(int peca)
{
    int codigo;
    
    //executa o código de acordo com o valor do parametro recebido
    if (peca==1)
    {
    	do //loop para retornar se o usuário entra com escolha inválida
    	{	
				system("cls");
		    	printf("Selecione a Data e o Horario para [O Rei Leao] no menu abaixo\n\n");
		    	printf("Data              Horario          Status             Codigo\n\n");
		    	printf("20/11/18          4:30pm           %s         (1)\n", status1); //mostra o status de cada peça, sendo uma variável global, não precisa ser passada por parâmetro
		    	printf("25/11/18          7:30pm           %s         (2)\n", status2);
		    	printf("01/12/18          5:10pm           %s         (3)\n", status3);
		    	printf("01/12/18          6:00pm           %s         (4)\n\n", status4);
		    	printf("Retornar ao menu principal (0)\n");
		    	printf("\nCodigo: ");
		    	scanf("%d", &codigo);
			
		    if ((codigo<1 || codigo>4) && (codigo!=0)) //valida escolha
	    	{
	    		printf("\nEscolha invalida. Tente novamente\n");
	    		system("pause");
			}
		} while ((codigo<1 || codigo>4) && (codigo!=0));
	}
	
	else if (peca==2)
	{
		do
		{
			system("cls");
			printf("Selecione a Data e o Horario para [Os Improvaveis] no menu abaixo\n\n");
		    printf("Data              Horario          Status             Codigo\n\n");
		    printf("01/12/18          1:30pm           %s         (5)\n", status5);
		    printf("10/12/18          7:30pm           %s         (6)\n", status6);
		    printf("10/12/18          3:10pm           %s         (7)\n", status7);
		    printf("15/12/18          5:00pm           %s         (8)\n\n", status8);
		    printf("Retornar ao menu principal (0)\n");
		    printf("\nCodigo: ");
		    scanf("%d", &codigo);
		    
		    if ((codigo<5 || codigo>8) && (codigo!=0))
	    	{
	    		printf("\nEscolha invalida. Tente novamente\n");
	    		system("pause");
			}
		} while ((codigo<5 || codigo>8) && (codigo!=0));
		
	}
	else if (peca==3)
	{
		do
		{
			system("cls");
			printf("Selecione a Data e o Horario para [Sai de Baixo] no menu abaixo\n\n");
		    printf("Data              Horario          Status             Codigo\n\n");
		    printf("12/12/18          2:00pm           %s         (9)\n", status9);
		    printf("12/12/18          6:00pm           %s         (10)\n", status10);
		    printf("19/12/18          7:30pm           %s         (11)\n", status11);
		    printf("20/12/18          8:30pm           %s         (12)\n\n", status12);
		    printf("Retornar ao menu principal (0)\n");
		    printf("\nCodigo: ");
		    scanf("%d", &codigo);
		    
		    if ((codigo<9 || codigo>12) && (codigo!=0))
	    	{
	    		printf("\nEscolha invalida. Tente novamente.");
	    		system("pause");
			}
		} while ((codigo<9 || codigo>12) && (codigo!=0));
		
	}
    return codigo; //retorna o codigo da sessão
}

//Funcao de validar escolha, retorna quantidade
int validaEscolha(int escolha, int *poltronaReservada)
{
	int quantidade;
	
	//abre o menu de cada sessão de acordo com a escolha da sessão na função anterior
	if (escolha==1)
	{
		quantidade = escolhaPoltronas(poltronas1, status1, poltronaReservada); //chama a função de escolha de poltronas de cada sessão e retorna a quantidade de lugares reservados
		cont1+=quantidade; // acrescenta a quantidade de poltronas reservadas à contagem de vendas de cada sessão para validar quando estiverem esgotadas
	}
	else if (escolha==2)
	{
		quantidade = escolhaPoltronas(poltronas2, status2, poltronaReservada);
		cont2+=quantidade;
	}
	else if (escolha==3)
	{
		quantidade = escolhaPoltronas(poltronas3, status3, poltronaReservada);
		cont3+=quantidade;
	}
	else if (escolha==4)
	{
		quantidade = escolhaPoltronas(poltronas4, status4, poltronaReservada);
		cont4+=quantidade;
	}
	else if (escolha==5)
	{
		quantidade = escolhaPoltronas(poltronas5, status5, poltronaReservada);
		cont5+=quantidade;
	}
	else if (escolha==6)
	{
		quantidade = escolhaPoltronas(poltronas6, status6, poltronaReservada);
		cont6+=quantidade;
	}
	else if (escolha==7)
	{
		quantidade = escolhaPoltronas(poltronas7, status7, poltronaReservada);
		cont7+=quantidade;
	}
	else if (escolha==8)
	{
		quantidade = escolhaPoltronas(poltronas8, status8, poltronaReservada);
		cont8+=quantidade;
	}
	else if (escolha==9)
	{
		quantidade = escolhaPoltronas(poltronas9, status9, poltronaReservada);
		cont9+=quantidade;
	}
	else if (escolha==10)
	{
		quantidade = escolhaPoltronas(poltronas10, status10, poltronaReservada);
		cont10+=quantidade;
	}
	else if (escolha==11)
	{
		quantidade = escolhaPoltronas(poltronas11, status11, poltronaReservada);
		cont11+=quantidade;
	}
	else if (escolha==12)
	{
		quantidade = escolhaPoltronas(poltronas12, status12, poltronaReservada);
		cont12+=quantidade;
	}
						
	return quantidade; //retorna a quantidade de ingressos reservados
}


	 
//Função de exibição de poltronas disponíveis (chamada pela função de escolhaPoltronas)
void exibePoltronas(int *poltrona)
{
	//função para mostrar na tela as poltronas disponiveis e reservadas
	int i;

	printf("Selecione uma poltrona disponivel\n\n");
	
	printf("------------------PALCO------------------\n\n");
	
	//for loop para automatizar a exibição
	//eu poderia ter usado matriz no lugar de um vetor, mas na hora de altomatizar o endereço, tive dificuldades e optei por usar um vetor e dividí-lo em fileiras
	for (i=0; i<5; i++)
        {
            if (poltrona[i]==0)
                printf(" [P %2d] ", i+1);  //se a poltrona estiver livre, mostrar o número dela como i+1, pois o primeiro endereço do vetor é 0.
            else 
                printf(" [xxxx] "); //se a poltorna estiver ocupda, mostrar xxxx no lugar
        }
        printf("\n");
    for (i=5; i<10; i++)
        {
            if (poltrona[i]==0)
                printf(" [P %2d] ", i+1);
            else 
                printf(" [xxxx] ");
        }
        printf("\n");
    for (i=10; i<15; i++)
	    {
	        if (poltrona[i]==0)
	            printf(" [P %2d] ", i+1);
	        else 
	            printf(" [xxxx] ");
	    }
        printf("\n");
    for (i=15; i<20; i++)
	    {
	        if (poltrona[i]==0)
	            printf(" [P %2d] ", i+1);
	        else 
	            printf(" [xxxx] ");
	    }
	
	return;
}

//Função de escolha de poltronas, retorna quantidade, chamada pela função validaEscolha
//A função validaEscolha, utiliza o if para enviar para a função escolhaPoltronas parâmetros individuais de acordo com a sessão da peça escolhida pelo usuário
int escolhaPoltronas(int *poltrona, char *status, int *poltronaReservada)
{
	//inicializei a qtd com 0 para evitar envio errado de informações caso o usuário desista de reservar poltronas, já que eu uso o 0 para validar menus na função principal
    int i, opcao, qtd=0, repete;
			
    if (strcmp(status, "Disponivel")==0) //se o status da sessão estiver "Disponível"
    {
    	do //loop para promover ao usuário a opção de repetir a reserva até obter todos os lugares desejados
		{
	        system("cls");
	        exibePoltronas(poltrona); //chama o procedimento exibePoltronas
	        printf("\n\nNumero da poltrona: ");
	        scanf("%d", &opcao); //lê a opção que o usuário entrou
	        
	        if (opcao>0 && opcao<=20) //if para validar a opção do usuário, nao podendo entrar com um número de poltrona que que não existe
	        {
		        	if (poltrona[(opcao-1)] == 0) //if para checar se a poltrona escolhida não está ocupada. 0 equivale a desocupada e 1 ocupada.
		        {
		            poltrona[(opcao-1)]=1; // muda o valor da poltrona vaga '0', para poltrona ocupada '1'. Este valor permanece o mesmo, já que se refere a uma variável individual de cada sessão, recebida por parâmetro. Quando aberta a mesma sessão, os lugares já reservados permanecerão ocupados.
		            qtd++; //acrescenta 1 na quantidade de lugares reservados.
		            system("cls");
		        	exibePoltronas(poltrona); //chama o procedimento para exibir as poltronas novamente, desta vez alterando a poltrona escolhida que estava disponivel para uma poltrona ocupada.
		        	printf("\n\nPoltrona reservada com sucesso!\n");
		        	poltronaReservada[(opcao-1)]=5; //altera a poltrona que é impressa no ingresso para ocupada(5). Este valor zera a cada nova venda. 
		        }
		        
		        else // se a poltrona não conter o numero 0 dentro, ela esta ocupada
		        {
		        	system("cls");
		        	exibePoltronas(poltrona);
		            printf("\n\nPoltrona ocupada!\n");
		        }	
			}
			
			else //else para validar se a opcao entrada existe
				{
					printf("\nOpcao invalida.\n\n");
					system("pause");
				}
				
			printf("\n\nGostaria de reservar mais um lugar? (1)Sim   ou  (0)Nao: ");
        	scanf("%d", &repete);	
    	} while (repete != 0);	//repete a escolha de poltronas até que o usuário entre 0.

		printf("\nQuantidade de lugares reservados: %d\n\n", qtd); //mostra quantidade de lugares reservados
    	system("pause");		
	}
	else //se o status da sessão não estiver como "Disponível", não será possível mais acessá-la.
	{
		printf("\nSessao Esgotada. Por Favor, escolher outra.\n\n");
		system("pause");
	}

    return qtd; //retorna quantidade de lugares reservados. se o usuário não reservar nada, retorna 0.
} 

//Função de descontos e valor total do pagamento retorna valor total
float descontoPreco(int codigo, int qtd)
{
    int i=1, info, semana;
    float valor, preco, desconto, total=0.0; // inicializei o total com 0.0 para não haver erros de calculos.
    struct tm *local; //estrutura para validar o dia da semana. A princípio eu havia usado apenas a pergunta, hoje é terça feira? e validava a resposta sim ou não. Com esta estrutura, o programa se torna mais automatizado.
	time_t t;
	t= time(NULL);
	local=localtime(&t);
	semana=local->tm_wday;
	
 	switch (codigo) //codigo recebido da função principal para determinar o preço do ingresso de acordo com a peça escolhida.
    {
    	case 1:
    	case 2:
    	case 3:
    	case 4:
    		valor=25.00;
    		break;
    	case 5:
    	case 6:
    	case 7:
    	case 8:
    		valor=15.00;
    		break;
    	case 9:
    	case 10:
    	case 11:
    	case 12:
    		valor=20.00;
    		break;
    	default:
    		printf("invalido");
	}
		
    
	do //do loop para perguntar especificações para cada um dos ingressos reservados
	{
		system("cls");
    	printf("Informacao sobre o espectador[%d] de [%d]: \n\n", i, qtd);
	    printf("Estudante                                   (1) \n");
	    printf("Idade entre 2 a 12 anos                     (2) \n");
	    printf("Idade a partir de 60 anos                   (3) \n");
	    printf("Professor de rede publica de ensino         (4) \n");
	    printf("Crianca carente da rede publica de ensino   (5) \n");
	    printf("Nenhuma das alternativas                    (6) \n\n");
	    printf("Opcao escolhida: ");
	    scanf("%d",&info);
	    
		if (info>0 && info<=6) //if para o usuário não entrar escolha inexistente
		{
			switch (info)
		    {
		        case 1:
		        case 2:
		        case 3:
		        case 4:
		            printf("\nDesconto de 50 porcento.\n\n");
		            desconto = 0.5;
		            tot50++;
		            printf("\nValor do ingresso: R$%.2f \n\n", preco=valor-(valor*desconto)); //calcula e exibe o valor a pagar pelo ingresso após aplicação do desconto
		            system("pause");
		            break;
		            
		        case 5:
		            if (semana==2) //if para validar o dia da semana, sendo 2 equivalente a terça feira
		            {
						printf("\nParabens, hoje e terca-feira e voce tera desconto de 100 porcento!!!\n\n");
		                desconto=1;
						tot100++;
						printf("\nValor do ingresso: R$%.2f \n\n", preco=valor-(valor*desconto));
					}
		            	
		            else //se não for terça feira, a criança terá desconto normal de estudante
		            {
		            	printf("\nHoje nao e terca feira, portanto voce tera apenas o desconto normal de estudante de 50 porcento.\n\n");
		            	desconto=0.5;
						tot50++;
						printf("\nValor do ingresso: R$%.2f \n\n", preco=valor-(valor*desconto));	
					}
					system("pause");
		            break;
		            
		        case 6:
		        	printf("\nQue pena, voce nao tem direito a desconto.\n\n");
		        	desconto=0.0;
		        	totInt++;
		        	printf("\nValor do ingresso: R$%.2f \n\n", preco=valor-(valor*desconto));
		        	system("pause");
		        	break;
			}
			
			i++; //incremento do contador do loop
    		total+=preco; // acrescenta o valor do ingresso no preço total a pagar
	    }
	    
	    else
	    {
	    	printf("\nEscolha invalida\n\n");
	    	system("pause");
		} 
	} while (i<=qtd); // loop encerra quando contador alcança a quantidade
		
    return total; //retorna o valor total a pagar pelos ingressos
}

//Função de pagamento 
void pagamento(float valor)
{
    float dinheiro, troco, recebido=0; //inicializei recebido para não ocorrer erro de validação
    int opcao;
    
    do //loop que repete o pagamento até que o mesmo seja realizado (quando a variável recebido mudar para 1)
    {
    	system("cls");
	    printf("*** Pagamento ***\n\n");
	    printf("Valor total a pagar: R$%.2f \n\n", valor);
	    printf("Como deseja realizar o pagamento: \n");
	    printf("Dinheiro (1)  Cartao (2)\n\n");
	    printf("Digite a opcao: ");
	    scanf("%d", &opcao);
	  
    	if (opcao==1) //opção para pagar em dinheiro
	    {
	        printf("\nValor recebido: R$");
	        scanf("%f", &dinheiro);
	        troco=dinheiro-valor; //calcula o troco
	        
	        while (dinheiro<valor || troco>caixa) //loop para validação de entradas erradas
	        {
	        	if (dinheiro<valor) //se o dinheiro recebido é menor do que o valor
		        {
		        	printf("\nDinheiro menor que valor\n");
		        	printf("\nValor recebido: R$");
		       		scanf("%f", &dinheiro);	
		       		troco=dinheiro-valor;
		       	}
		       	
		       	if (troco>caixa) //se o troco é maior do que o fundo de caixa
		        {
		            printf("\nNao ha troco disponivel\n");
		            printf("\nValor recebido: R$");
		            scanf("%f", &dinheiro);
		            troco=dinheiro-valor;
		        } 
			}
			
        	printf("\nTroco: R$%.2f\n", troco);
        	caixa+=valor; // adicionar o valor ao caixa
        	recebido = 1; //mudar a variável para 1 para validação do loop
        	system("pause");			    
	    } 
		       
	    else if (opcao==2)
	    {
	        printf("\nInsira o cartao e digite a senha...\n\n");
	        system("pause");
	        printf("\nAprovado\n\n");
	        cartao+= valor; //adicionar valor ao cartão
	        recebido = 1; //mudar a variável para 1 para validação do loop
	        system("pause");
	    }
	    
	    else
	    {
	    	printf("\nEscolha invalida!\n\n");
	    	system("pause");
		}
	} while (recebido!=1);
    
    return;
}


//Função de impressão de ingresso na tela
void impressaoIngresso (int opcao, int quantidade, int *poltronaReservada)
{
	int i=1, cont;
	char nome[30], data[9], hora[8];
	
	//Atribuicao de Nome, Data e Hora da Peça
	switch (opcao)
	{
		//Rei Leao
		case 1:
			strcpy(nome, "O Rei Leao");
			strcpy(data, "20/11/18");
			strcpy(hora, "4:30pm");
			break;
		case 2:
			strcpy(nome, "O Rei Leao");
			strcpy(data, "25/11/18");
			strcpy(hora, "7:30pm");
			break;
		case 3:
			strcpy(nome, "O Rei Leao");
			strcpy(data, "01/12/18");
			strcpy(hora, "5:10pm");
			break;
		case 4:
			strcpy(nome, "O Rei Leao");
			strcpy(data, "01/12/18");
			strcpy(hora, "6:00pm");
			break;
		//Os Improvaveis
		case 5:
			strcpy(nome, "Os Improvaveis");
			strcpy(data, "01/12/18");
			strcpy(hora, "1:30pm");
			break;
		case 6:
			strcpy(nome, "Os Improvaveis");
			strcpy(data, "10/12/18");
			strcpy(hora, "7:30pm");
			break;
		case 7:
			strcpy(nome, "Os Improvaveis");
			strcpy(data, "10/12/18");
			strcpy(hora, "3:10pm");
			break;
		case 8:
			strcpy(nome, "Os Improvaveis");
			strcpy(data, "15/12/18");
			strcpy(hora, "5:00pm");
			break;
		//Sai de Baixo	
		case 9:
			strcpy(nome, "Sai de Baixo");
			strcpy(data, "12/12/18");
			strcpy(hora, "2:00pm");
			break;
		case 10:
			strcpy(nome, "Sai de Baixo");
			strcpy(data, "12/12/18");
			strcpy(hora, "6:00pm");
			break;
		case 11:
			strcpy(nome, "Sai de Baixo");
			strcpy(data, "19/12/18");
			strcpy(hora, "7:30pm");
			break;
		case 12:
			strcpy(nome, "Sai de Baixo");
			strcpy(data, "20/12/18");
			strcpy(hora, "8:30pm");
			break;
	}
	
	//Imprime Ingresso na Tela
	do //loop para repetir até todos os ingressos serem impressos
	{	
		for (cont=0; cont<20; cont++) //loop para checar quais as poltronas foram reservadas
		{	
			if (poltronaReservada[cont]==5) //se a poltrona checada estiver reservada, exibir o ingresso referente
			{	
				system("cls");
				printf("Mostrando ingresso [%d] de [%d]\n\n", i, quantidade);
				printf(" ----------------------------------------- \n");
				printf("|Nome da Peca: %15s            |\n", nome);
				printf("|Data: %s                           |\n", data);
				printf("|Hora: %s                             |\n", hora);
				printf("|Poltrona: %2d                             |\n", cont+1);
				printf(" ----------------------------------------- \n");
				system("pause");
				i++;
			}	
		}
	} while (i<quantidade); //termina o loop quando todos os ingressos comprados forem mostrados na tela
	
	return;
}

//Funcao de limpar vetor de poltrona Reservada
void limpaReserva (int *poltronaReservada)
{
	int i;
	
	for (i=0; i<20; i++) //loop para esvaziar as poltronas para a impressão do ingresso.
	{
		poltronaReservada[i]=0;
	}
	return;
}

//Função de fechamento de caixa
void  fechamento()
{
	int totalP1, totalP2, totalP3;
	totalP1=cont1+cont2+cont3+cont4;
	totalP2=cont5+cont6+cont7+cont8;
	totalP3=cont9+cont10+cont11+cont12;
	
	//exibe na tela o relatório de caixa
	printf ("*** Fechamento do caixa ***\n\n");
	printf ("Ingressos vendidos no total:           %d\n", totIng);
	printf ("Total de vendas para [O Rei Leao]:     %d\n", totalP1);
	printf ("Total de vendas para [Os Improvaveis]: %d\n", totalP2);
	printf ("Total de vendas para [Sai de Baixo]:   %d\n\n", totalP3);
	printf ("Pagos no valor integral:               %d\n", totInt);
	printf ("Descontos de 50 porcento:              %d\n", tot50);
	printf ("Descontos de 100 porcento:             %d\n\n", tot100);
	printf ("Total arrecadado no dinheiro:          R$%.2f\n", caixa-100.0); //retiro o fundo de caixa para não influenciar na conta.
	printf ("Total arrecadado no cartao:            R$%.2f\n", cartao);
	printf ("Total arrecadado:                      R$%.2f\n\n", cartao+(caixa-100.0));

	return;
}

