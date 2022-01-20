#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <ctype.h>


#define MAXLENGTH 500
#define MAX_ARTIGOS 1000
#define MAX_CLIENTES 1000
#define MAX_ENCOMENDAS 1000

#define DEBUG

typedef struct Cliente {
	char nome_1[MAXLENGTH];
	char nome_2[MAXLENGTH];
	char morada[MAXLENGTH];
	char nascimento[MAXLENGTH];
	long int telefone;
	long int contribuinte;
} Cliente;

Cliente clientes[MAX_CLIENTES];
int num_clientes = 0;

typedef struct artigo {
	char nome[MAXLENGTH];
	int cod;
	char descricao[MAXLENGTH];
	float preco;
	float stock;
	int caracteristica[MAXLENGTH];
	int num_caracteristicas;
	float tm_preparo;
} Artigo;

Artigo artigos[MAX_ARTIGOS];
int num_artigos = 0;

typedef struct encomenda_artigos {
	char nome[MAXLENGTH];
	int quantidade;
} Encomenda_artigos;

typedef struct encomenda {
	int cliente_nif;
	Encomenda_artigos artigos_encomenda[MAXLENGTH];
	float total_encomenda;
	int encomenda_num;
	int qt_artigos;
} Encomenda;

Encomenda encomendas[MAX_ENCOMENDAS];
int num_encomendas = -1;
int num_artigos_encomenda = -1;


// ARTIGOS

void getsn(char str[], int len) {
	fgets(str, len, stdin);
	str[strcspn(str, "\n\r")] = '\0';
	fflush(stdin);
}
char* getfield(char* line, int coluna) {
    const char* tok;
    for (tok = strtok(line, ";"); tok && *tok; tok = strtok(NULL, ";\n"))
    {
        if (!--coluna)
            return (char *)tok;
    }
    return NULL;
}

int indice_artigo(char nome[]) {
	///Função que procura o indice de um artigo comparando o nome e devolve o indice
	for (int i = 0; i < num_artigos; i++) {
		if (strcmp(artigos[i].nome, nome) == 0) {
			return i; ///Devolve o indice do artigo comparado, utilizado na função alterar_artigo()
		}
	}

	return -1;///Se devolver o número -1, significa que não encontrou um artigo com o mesmo nome
}

void listar_artigos() {

	///Função que lista todos os artigos existentes
    printf("-----------------------------\n");

    //Ordenação do vetor artigos por ordem alfabética de nome
    for (int i = 0; i < num_artigos; i++) {
        for (int j = i + 1; j < num_artigos; j++) {
            if (strcmp(artigos[i].nome, artigos[j].nome) > 0) {
                Artigo aux = artigos[i];
                artigos[i] = artigos[j];
                artigos[j] = aux;
            }
        }
    }

	for (int i = 0; i < num_artigos; i++) {
		printf("Nome: %s\nDescrição: %s\nPreço: %.2f €\nCódigo: %d\nStock: %.2f\nTempo de Preparação: %.2f\n",
			artigos[i].nome,
			artigos[i].descricao,
			artigos[i].preco,
			artigos[i].cod,
			artigos[i].stock,
			artigos[i].tm_preparo);
		printf("-----------------------------\n");

	}



}
int indice_artigo_cod(int cod) {
	///Função que procura o indice de um artigo comparando o código do mesmo e devolve o indice
	for (int i = 0; i < num_artigos; i++) {
		if (artigos[i].cod == cod) {
			return i;///Devolve o indice do artigo comparado, utilizado na s_artigo()
		}
	}

	return -1;
}

void inserir_artigo() {

	///Função insere um artigo e os seus dados no sistema
	int qt;
	printf("Quantos artigos quer introduzir?\n");
	scanf("%d%*c", &qt);
	fflush(stdin);

	for (int i = 0; i < qt; i++) {
		if (num_artigos < MAX_ARTIGOS) {
			printf("\nNome do %dº artigo:", i + 1);
			getsn(artigos[num_artigos].nome, MAXLENGTH);

				printf("\nCódigo numérico:");
				int cod;
				scanf("%d%*c", &cod);
				fflush(stdin);

			if (indice_artigo_cod(cod) < 0) { ///verificação se já existe um artigo com o codigo introduzido
				artigos[num_artigos].cod = cod;
			}
			else {
				printf("ERRO: Já existe um artigo com esse código\n");
				fflush(stdin);
				return;
			}

			printf("\nBreve descrição do artigo:");
			getsn(artigos[num_artigos].descricao, MAXLENGTH);

			printf("\nPreço:");
			scanf("%f%*c", &artigos[num_artigos].preco);

			printf("\nStock inicial do artigo:");
			scanf("%f%*c", &artigos[num_artigos].stock);

			printf("\nTempo de preparo do artigo:");
			scanf("%f%*c", &artigos[num_artigos].tm_preparo);

			printf("\n (1)Bebidas\n (2)Sandes\n (3)Acompanhamento\n");
			printf("\nNº de Categorias em que o produto se insere:");
			scanf("%d%*c", &artigos[num_artigos].num_caracteristicas);

			for (int n = 0; n < artigos[num_artigos].num_caracteristicas; n++) {
				printf("\n%dª categoria: ", n + 1);
				scanf("%d%*c", &artigos[num_artigos].caracteristica[n]);
			}

			printf("-----------------------------\n");

			num_artigos++;
			fflush(stdin);
		}
		else {
			printf("\nNão existe espaço para criar um artigo novo\n");
			fflush(stdin);
			return;
		}
	}
}

void alterar_artigo() {

	///Função que altera os dados de um artigo
	char nome[MAXLENGTH];
	printf("Qual o nome do artigo que quer alterar?\n");
	getsn(nome, MAXLENGTH);

	int indice_codigo_alterar = indice_artigo(nome);

	if (indice_codigo_alterar >= 0) {
		printf("Novo nome do artigo:");
		getsn(artigos[indice_codigo_alterar].nome, MAXLENGTH);

		printf("\nNovo código numérico:");
		scanf("%d%*c", &artigos[indice_codigo_alterar].cod);


		printf("\nBreve descrição do artigo:\n");
		getsn(artigos[indice_codigo_alterar].descricao, MAXLENGTH);

		printf("\nPreço por unidade:");
		scanf("%f%*c", &artigos[indice_codigo_alterar].preco);

		printf("\nStock do artigo");
		scanf("%f%*c", &artigos[indice_codigo_alterar].stock);

		printf("\nTempo de preparo do artigo:");
		scanf("%f%*c", &artigos[indice_codigo_alterar].tm_preparo);

		printf("\nArtigo alterado com sucesso!\n");
	}
	else {
		printf("\nERRO: Artigo não encontrado!\n");
	}
}

void remover_artigo() {

	//Função remove um artigo da memória
	char resposta_a[MAXLENGTH];

	printf("Qual o nome do artigo que quer remover?\n");
	getsn(resposta_a, MAXLENGTH);

	int indice_codigo_remover = indice_artigo(resposta_a);

	if (indice_codigo_remover >= 0) {
		for (int i = indice_codigo_remover; i < num_artigos; i++) {
			artigos[i] = artigos[i + 1];
		}

		num_artigos = num_artigos - 1;

		printf("\nArtigo removido com sucesso!\n");
	}
	else {
		printf("\nERRO: Artigo não encontrado!\n");
	}
}

void adicionar_stock_artigo() {

	///Função que adiciona stock a um determinado artigo
	char nome[MAXLENGTH];

	printf("Qual o nome do artigo que quer acrescentar stock?\n");
	getsn(nome, MAXLENGTH);

	int indice_nome_artigo = indice_artigo(nome);

	if (indice_nome_artigo >= 0) {
		float stock_adicional;
		printf("Stock a adicionar ao artigo %s:", artigos[indice_nome_artigo].nome);
		scanf("%f%*c", &stock_adicional);
		artigos[indice_nome_artigo].stock = artigos[indice_nome_artigo].stock + stock_adicional;
		printf("\nStock atualizado com sucesso!\n");
	}
	else {
		printf("\nERRO: Artigo não encontrado!\n");
	}
}


char imprimir_menu_artigos() {
	char op;

	//Menu dos artigos
	printf("\n\n########## Artigos ##########\n");

	printf("(L) Listar artigos\n");
	printf("(N) Inserir novo artigo\n");
	printf("(A) Alterar artigo\n");
	printf("(R) Remover artigo\n");
	printf("(I) Adicionar stock\n");
	printf("(S) Sair\n");

	printf("#############################\n");
	printf("Opção:");

	scanf("%c%*c", &op);

	return op;
}

void gerir_artigos() {
	char op;

	///Opções possíveis de resposta do menu dos artigos
	do {
		op = imprimir_menu_artigos();

		switch (toupper(op)) {
			case 'L':
				listar_artigos();
				break;
			case 'N':
				inserir_artigo();
				break;
			case 'A':
				alterar_artigo();
				break;
			case 'R':
				remover_artigo();
				break;
			case 'I':
				adicionar_stock_artigo();
				break;
		}
	} while (toupper(op) != 'S');
}

/// CLIENTES

int indice_clientes(long int nif) {
	///Função que procura o indice de um cliente comparando o NIF e devolve o indice
	for (int i = 0; i < num_clientes; i++) {
		if (clientes[i].contribuinte == nif) {
			return i;///Devolve o indice do cliente comparado, utilizado na inserir cliente()
		}
	}

	return -1;///Se devolver o número -1, significa que não encontrou um cliente com o mesmo NIF
}

void listar_clientes() {

	///Função que lista todos os clientes guardados
	printf("-----------------------------\n");

	for (int i = 0; i < num_clientes; i++) {
		printf("Primeiro nome: %s\nÚltimo nome: %s\nNIF: %ld\nData nasc.: %s\nMorada: %s\nTelefone: %ld\n",
			clientes[i].nome_1,
			clientes[i].nome_2,
			clientes[i].contribuinte,
			clientes[i].nascimento,
			clientes[i].morada,
			clientes[i].telefone
			);
		printf("-----------------------------\n");
	}
}

void inserir_cliente() {

	///Função insere um cliente e os seus dados no sistema
    long int nif;

	if (num_clientes < MAX_CLIENTES) {
		printf("Primeiro Nome:");
		getsn(clientes[num_clientes].nome_1, MAXLENGTH);

		printf("\nÚltimo Nome:");
		getsn(clientes[num_clientes].nome_2, MAXLENGTH);

		printf("\nData de nascimento:");
		getsn(clientes[num_clientes].nascimento, MAXLENGTH);

		printf("\nMorada:");
		getsn(clientes[num_clientes].morada, MAXLENGTH);

		printf("\nNúmero de telefone:");
		scanf("%ld%*c", &clientes[num_clientes].telefone);


		do {
			printf("\nNúmero Contribuinte:");
    		scanf("%ld%*c", &nif);
		} while (nif < 99999999 || nif > 1000000000);

		if (indice_clientes(nif) < 0) { ///verificação se já existe um cliente com o NIF indroduzido
		clientes[num_clientes].contribuinte = nif;
		}
		else {
			printf("ERRO: Já existe um cliente com esse NIF!\n");
			return;
		}

		num_clientes++;
	}
	else {
		printf("Não existe espaço para criar uma ficha nova\n");
		return;
	}
}

void alterar_cliente() {

	///Função que altera os dados de um cliente
	long int nif;
	printf("Qual o NIF do cliente que quer alterar?\n");
	scanf("%ld%*c", &nif);

	int indice_codigo_alterar = indice_clientes(nif);

	if (indice_codigo_alterar >= 0) {
		printf("Primeiro Nome:\n");
		getsn(clientes[indice_codigo_alterar].nome_1, MAXLENGTH);

		printf("Último Nome:\n");
		getsn(clientes[indice_codigo_alterar].nome_2, MAXLENGTH);

		printf("Data de nascimento:\n");
		getsn(clientes[indice_codigo_alterar].nascimento, MAXLENGTH);

		printf("Morada:\n");
		getsn(clientes[indice_codigo_alterar].morada, MAXLENGTH);

		printf("Número de telefone:\n");
		scanf("%ld%*c", &clientes[indice_codigo_alterar].telefone);

		printf("Número Contribuinte:\n");
		scanf("%ld%*c", &clientes[indice_codigo_alterar].contribuinte);


	}
	else {
		printf("ERRO: Cliente não encontrado!\n");
	}
}

void remover_cliente() {

	///Função que remove um cliente da memória
	long int nif;

	printf("Qual o NIF do cliente que quer remover?\n");
	scanf("%ld%*c", &nif);

	int indice_codigo_remover = indice_clientes(nif);

	if (indice_codigo_remover >= 0) {
		for (int i = indice_codigo_remover; i < num_clientes; i++) {
			clientes[i] = clientes[i + 1];
		}

		num_clientes = num_clientes - 1;
	}
}

char imprimir_menu_clientes() {
	char op;

	// Menu do carrinho
	printf("\n\n########## Clientes ##########\n");

	printf("(L) Listar clientes\n");
	printf("(N) Inserir novo cliente\n");
	printf("(A) Alterar cliente\n");
	printf("(R) Remover cliente\n");
	printf("(S) Sair\n");

	printf("##############################\n");
	printf("Opção:");

	scanf("%c%*c", &op);

	return op;
}

void gerir_clientes() {

	///Opções possíveis de resposta do menu do carrinho
	char op;

	do {
		op = imprimir_menu_clientes();

		switch (toupper(op)) {
		case 'L':
			listar_clientes();
			break;
		case 'N':
			inserir_cliente();
			break;
		case 'A':
			alterar_cliente();
			break;
		case 'R':
			remover_cliente();
			break;
		}
	} while (toupper(op) != 'S');
}

// ENCOMENDAS

int indice_encomenda(char nome[]) {

	///Função semelhante a indice_artigo() aplicado às encomendas
	for (int i = 0; i <= num_artigos_encomenda; i++) {
		if (strcmp(encomendas[num_encomendas].artigos_encomenda[i].nome, nome) == 0) {
			return i; //Utilizado na função adicionar_artigo_carrinho()
		}
	}

	return -1; ///Se devolver o número -1, significa que não encontrou um artigo com o mesmo nome
}


int* percent_comparacao(int indice_pai) {

	///Função que devolve os indices dos artigos que têm maior percentagem de comparação (calculada a partir das características de um artigo)
	float percent_max[2] = { -1, -1 };
	static int indice_artigo[2] = { -1, -1 };

	for (int i = 0; i < num_artigos; i++) {
		if (i != indice_pai) {
			float percentagem = 0;
			int cont = 0;

			for (int n = 0; n < artigos[indice_pai].num_caracteristicas; n++) {
				for (int m = 0; m < artigos[i].num_caracteristicas; m++) {
					if (artigos[indice_pai].caracteristica[n] == artigos[i].caracteristica[m]) {
						cont++;
					}
				}
			}

			percentagem = (float)cont / artigos[indice_pai].num_caracteristicas;

			int percent_alterado = 0;

			for (int c = 0; c < 2; c++) {
				if (percentagem > percent_max[c] && percent_alterado == 0) {
					percent_max[c] = percentagem;
					indice_artigo[c] = i;
					percent_alterado++;
				}
			}
		}
	}

	return indice_artigo;  ///Devolve o vetor à função imprimir_recibo()
}


void adicionar_artigo_carrinho() {;

	///Função que adiciona um artigo ao "carrinho" do cliente
	num_artigos_encomenda++;

	printf("-----------------------------\n");
	printf("Que artigo quer comprar?\n");
	getsn(encomendas[num_encomendas].artigos_encomenda[num_artigos_encomenda].nome, MAXLENGTH);

	int indice_nome_artigo = indice_artigo(encomendas[num_encomendas].artigos_encomenda[num_artigos_encomenda].nome);

	for (int n = 0; n <= num_artigos_encomenda-1; n++) {
		if (strcmp(encomendas[num_encomendas].artigos_encomenda[num_artigos_encomenda].nome, encomendas[num_encomendas].artigos_encomenda[n].nome) == 0) {
			printf("O artigo já se encontra no carrinho!");
			num_artigos_encomenda--;

			return;
		}
	}

	if (indice_nome_artigo >= 0) {

		printf("\nQual a quantidade?\n");
		scanf("%d%*c", &encomendas[num_encomendas].artigos_encomenda[num_artigos_encomenda].quantidade);

		if (encomendas[num_encomendas].artigos_encomenda[num_artigos_encomenda].quantidade <= artigos[indice_nome_artigo].stock) {

			artigos[indice_nome_artigo].stock = artigos[indice_nome_artigo].stock - encomendas[num_encomendas].artigos_encomenda[num_artigos_encomenda].quantidade;

			printf("\nArtigo Adicionado!\n");




		}
		else {
			printf("\nERRO: Stock indisponível\n");

			num_artigos_encomenda--;
		}
	}
	else {
		printf("\nERRO: Artigo não encontrado!\n");

		num_artigos_encomenda--;
	}
}

void remover_artigo_carrinho() {

	///Função que remove um artigo do "carrinho" do cliente
	char nome[MAXLENGTH];

	printf("Artigos no carrinho\n");
	for (int i = 0; i <= num_artigos_encomenda ; i++) {
		printf("->%s\n", encomendas[num_encomendas].artigos_encomenda[i].nome);
	}

	printf("\nQue artigo quer remover?\n");
	getsn(nome, MAXLENGTH);

	int indice_artigo_remover = indice_encomenda(nome);

	if (indice_artigo_remover >= 0) {
		for (int i = indice_artigo_remover; i < num_artigos_encomenda; i++) {
			encomendas[num_encomendas].artigos_encomenda[i] = encomendas[num_encomendas].artigos_encomenda[i + 1];
		}

		num_artigos_encomenda = num_artigos_encomenda - 1;
	}
	else {
		printf("\nERRO: Artigo não está no carrinho!\n");
	}
}

float preco_total() {

	///Função que permite calcular a soma do preço dos artigos que o ultilizador adicionou ao "carrinho"
	float preco = 0;

	for (int i = 0; i <= num_artigos_encomenda; i++) {
		int indice_artigo_preco = indice_artigo(encomendas[num_encomendas].artigos_encomenda[i].nome);

		preco = preco + (artigos[indice_artigo_preco].preco * encomendas[num_encomendas].artigos_encomenda[i].quantidade);
	}

	return preco;
}

void ver_carrinho() {

	///Função ver os artigos que estão a ser comprados e quanto vai custar os artigos todos
	encomendas[num_encomendas].total_encomenda = preco_total();

	printf("-----------------------------\n");
	printf("Carrinho\n");
	for (int i = 0; i <= num_artigos_encomenda; i++) {
		printf("Nome:%s   Quantidade:%d\n", encomendas[num_encomendas].artigos_encomenda[i].nome, encomendas[num_encomendas].artigos_encomenda[i].quantidade);
	}

	printf("Total:%.2f", encomendas[num_encomendas].total_encomenda);
}


void imprimir_recibo() {

	///Função para imprimir o recibo após finalizar a encomenda
	int indice_artigo_preco;
	float tempo_total_preparacao = 0;

	int indice_cliente_nome = indice_clientes(encomendas[num_encomendas].cliente_nif);
    encomendas[num_encomendas].total_encomenda = preco_total();
	printf("\n|-------------------------------------------|");
	printf("\n|Cliente: %s %s", clientes[indice_cliente_nome].nome_1, clientes[indice_cliente_nome].nome_2);
	printf("\n|");
	printf("\n|");
	printf("\n|Artigos:                                   ");

	for (int i = 0; i <= num_artigos_encomenda; i++) {

		indice_artigo_preco = indice_artigo(encomendas[num_encomendas].artigos_encomenda[i].nome);

		printf("\n| %s", encomendas[num_encomendas].artigos_encomenda[i].nome);
		printf("\n| Quantidade:%d", encomendas[num_encomendas].artigos_encomenda[i].quantidade);
		printf("\n| Preço:%.2f", artigos[indice_artigo_preco].preco);
		tempo_total_preparacao = tempo_total_preparacao + artigos[indice_artigo_preco].tm_preparo;
		printf("\n|");
		printf("\n|");
	}

	printf("\n|");
	printf("\n|");
	printf("\n|Tempo de Preparação: %f", tempo_total_preparacao);
	printf("\n|Total: %.2f€" , encomendas[num_encomendas].total_encomenda);
	printf("\n|-------------------------------------------|\n\n ");
}
void finalizar_encomenda() {

	///Função para finalizar uma encomenda
	encomendas[num_encomendas].qt_artigos = num_artigos_encomenda;
	imprimir_recibo();
    FILE *fencomenda;
    char anoencomenda[MAXLENGTH];
    time_t t;
    t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(anoencomenda, "encomendas_%i.csv", tm.tm_year);
    fencomenda = fopen(anoencomenda, "a");
    fprintf(fencomenda, "%i;%.2f\n", tm.tm_yday+1, encomendas[num_encomendas].total_encomenda);
	num_artigos_encomenda = -1;
    fclose(fencomenda);
}


void cancelar_encomenda() {

	///Função para cancelar uma encomenda
	for (int i = 0; i <= num_artigos_encomenda; i++) {
		int indice_nome_artigo = indice_artigo(encomendas[num_encomendas].artigos_encomenda[i].nome);

		artigos[indice_nome_artigo].stock = artigos[indice_nome_artigo].stock + encomendas[num_encomendas].artigos_encomenda[i].quantidade;
	}

	num_encomendas--;
	num_artigos_encomenda = -1;
}


char imprimir_menu_carrinho() {
	char op;

	// Menu do carrinho
	printf("\n\n########## Carrinho ##########\n");

	printf("(A) Adicionar artigo ao carrinho\n");
	printf("(R) Remover artigo do carrinho\n");
	printf("(V) Ver carrinho\n");
	printf("(F) Finalizar Compra\n");
	printf("(C) Cancelar Compra\n");

	printf("##############################\n");
	printf("Opção:");

	scanf("%c%*c", &op);

	return op;
}


void nova_encomenda() {

	///Opções possíveis de resposta do menu do carrinho
	char op;
	num_encomendas++;
	printf("\nNIF:");
	scanf("%d%*c", &encomendas[num_encomendas].cliente_nif);

	do {
		op = imprimir_menu_carrinho();

		switch (toupper(op)) {
		case 'A':
			adicionar_artigo_carrinho();
			break;
		case 'R':
			remover_artigo_carrinho();
			break;
		case 'V':
			ver_carrinho();
			break;
		case 'F':
			finalizar_encomenda();
			break;
		case 'C':
			cancelar_encomenda();
			break;
		}
	} while (toupper(op) != 'C' && toupper(op) != 'F');
}

char imprimir_menu_listar_encomendas() {
	char op;
	printf("(E) Ver encomenda expecífica\n");
	printf("(V) Voltar\n");
	printf("-----------------------------\n");

	printf("Opção:");

	scanf("%c%*c", &op);

	return op;
}

///Função para ver uma encomenda em expecífico
void ver_encomenda() {
	int num;
	printf("Nº de encomenda: ");
	scanf("%d%*c", &num);

	num = num - 1;

	int indice_cliente_nome = indice_clientes(encomendas[num].cliente_nif);
	printf("\n-----------------------------\n");
	printf("Cliente: %s %s\n", clientes[indice_cliente_nome].nome_1, clientes[indice_cliente_nome].nome_2);
	printf("\nArtigos comprados: ");

	for (int i = 0; i <= encomendas[num_encomendas].qt_artigos; i++) {

		int indice_artigo_preco = indice_artigo(encomendas[num].artigos_encomenda[i].nome);

		printf("\n %s", encomendas[num].artigos_encomenda[i].nome);
		printf("\n Quantidade:%d\n", encomendas[num].artigos_encomenda[i].quantidade);
		printf("\n Preço:%.2f", artigos[indice_artigo_preco].preco);
		printf("\n-----------------------------\n");
	}
		printf("\n-----------------------------\n");
}

///Função para listar todas as encomendas efetuadas
void listar_encomendas() {
	char op;

	for (int i = 0; i <= num_encomendas; i++) {
		int indice_cliente_nome = indice_clientes(encomendas[i].cliente_nif);



		printf("\n-----------------------------\n");
		printf("Encomenda número: %d\n", i + 1);
		printf("\nCliente: %s %s", clientes[indice_cliente_nome].nome_1, clientes[indice_cliente_nome].nome_2);
		printf("\nTotal da compra: %.2f", encomendas[i].total_encomenda);
		printf("\n-----------------------------\n");
	}
		do {
			op = imprimir_menu_listar_encomendas();

			switch (toupper(op)) {
			case 'E':
				ver_encomenda();
				break;
			}
		} while (toupper(op) != 'V');

}


char imprimir_menu_encomendas() {
	char op;

	// Menu das encomendas

	printf("\n\n########## Encomendas ##########\n");

	printf("(N) Nova encomenda\n");
	printf("(L) Listar encomendas\n");
	printf("(S) Sair\n");

	printf("#############################\n");
	printf("Opção:");

	scanf("%c%*c", &op);

	return op;
}


void gerir_encomendas() {
	char op;

	///Opções possíveis de resposta do menu das encomendas
	do {
		op = imprimir_menu_encomendas();

		switch (toupper(op)) {
		case 'N':
			nova_encomenda();
			break;
		case 'L':
			listar_encomendas();
			break;
		}
	} while (toupper(op) != 'S');
}

//MENU Infomação geral das ecomendas

void gerir_informacoes(){
	time_t t = time(NULL);
    struct tm tm = *localtime(&t);
	char anoencomenda[MAXLENGTH];
	sprintf(anoencomenda, "encomendas_%i.csv", tm.tm_year);
    FILE *fencomenda = fopen(anoencomenda, "r");
    char line[32];
    int cmp = -1;
    int numdia = 1;
	int encomendasdia = 0;
    int totalencomendas = 0;
	float totaldia = 0;
	int current;
    while (fgets(line, 32, fencomenda)){
        current = atoi(getfield(strdup(line),1));
        float valorencomenda = atof(getfield(strdup(line),2));
        if (current != cmp && cmp != -1){
            numdia++;
			printf("Valor do dia %i: %f\n", current, totaldia);
			printf("Encomendas do dia %i: %i\n", current, encomendasdia);
			totaldia = 0;
			totalencomendas++;
        } else {
			totaldia += valorencomenda;
			encomendasdia++;
		}
        cmp = current;
    }
	printf("Valor do dia %i: %f\n", current, totaldia);
	printf("Encomendas do dia %i: %i\n", current, encomendasdia);
	totalencomendas += encomendasdia;
    printf("Média de encomendas por dia: %i\n", totalencomendas/numdia);
	printf("Total de encomendas: %i\n", totalencomendas);
    fclose(fencomenda);
}

//Menu pré-feito

void preencher()
{
    strcpy(artigos[0].nome, "Coca cola");
	strcpy(artigos[0].descricao, "Abre a felicidade! Uma Coca-Cola bem fresca faz-nos gozar cada momento das nossas vidas de uma forma especial ");
	artigos[0].caracteristica[0] = 1;
	artigos[0].num_caracteristicas = 0;
	artigos[0].tm_preparo = 0;
    artigos[0].cod = 123456789;
	artigos[0].stock = 200;
    artigos[0].preco = 1.0;

    strcpy(artigos[1].nome, "McQueen");
    strcpy(artigos[1].descricao, "Sandes com bacon,queijo,duas carnes e batata frita incluida");
	artigos[1].caracteristica[0] = 1;
	artigos[1].num_caracteristicas = 1;
	artigos[1].tm_preparo = 15;
	artigos[1].cod = 123456788;
	artigos[1].stock = 100;
    artigos[1].preco = 10;

    strcpy(artigos[2].nome, "Queen nuggets");
	strcpy(artigos[2].descricao, "Irresistiveis pedaços de frango panados e dourados.");
	artigos[2].caracteristica[0] = 1;
	artigos[2].num_caracteristicas = 2;
	artigos[2].tm_preparo = 5;
	artigos[2].cod = 123456787;
    artigos[2].stock = 150;
    artigos[2].preco = 3.0;
	printf("Preenchido com sucesso!\n");
	num_artigos=3;
}
// MENU PRINCIPAL

char imprimir_menu() {
    char op;

	printf("\n#############################\n");

	printf("(A) Gerir artigos\n");
	printf("(C) Gerir clientes\n");
	printf("(E) Encomendas\n");
	printf("(I) Infomaçao geral das encomendas\n");
	printf("(S) Sair da aplicaçao\n");

	printf("#############################\n");
	printf("Opçao:");

	scanf("%c%*c", &op);

	return op;
}

int main() {
	setlocale(LC_ALL, "Portuguese");
	char op;

	//Opções possíveis de resposta do menu principal
	do {
		op = imprimir_menu();
		switch (toupper(op)) {
			case 'A':
				gerir_artigos();
				break;
			case 'C':
				gerir_clientes();
				break;
			case 'E':
				gerir_encomendas();
				break;
			case 'I':
			    gerir_informacoes();
				break;
			#ifdef DEBUG
			case 'P':
				preencher();
				break;
			#endif
		}
	} while (toupper(op) != 'S');

	return 0;
}
