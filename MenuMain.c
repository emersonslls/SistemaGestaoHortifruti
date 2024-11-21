/* --- BIBLIOTECAS --- */
#include <stdio.h>   // Inclui a biblioteca padrão de entrada e saída para funções como printf e scanf
#include <stdlib.h>  // Inclui a biblioteca para funções utilitárias como system e exit
#include <string.h>  // Inclui a biblioteca para manipulação de strings
#include <windows.h> // Inclui a biblioteca Windows, necessária para usar a função Sleep()
#include <time.h>    // Biblioteca para manipulação de tempo
#include <conio.h>   // Necessária para usar getch() no Windows
#include <stdbool.h> // Inclui a biblioteca para tipos booleanos (true/false)
#include <locale.h>  // Inclui a biblioteca para manipulação de configurações locais

// Definições de constantes e macros
/* -- DEFINIÇÕES DO LOGIN --- */
#define FILENAMELOGIN "CadastrosUsuarios.txt" // Nome do arquivo para salvar os cadastros
#define MAX_USUARIOS 100                      // Limite máximo x'de usuários
#define MAX_NOME 80                           // Limite máximo de caracteres para o nome
#define MAX_LOGIN 20                          // Limite máximo de caracteres para o login

#define MAX_SENHA 9 // Limite de senha com 8 dígitos + '\0'

/* -- DEFINIÇÕES DO COLABORADOR --- */
#define FILENAMEPRODUTOS "produtosarq.txt" // Nome do arquivo para salvar os produtos
#define FILENAMEESTOQUE "estoque.txt"      // Nome do arquivo para controlar o estoque
#define MAX_PRODUTOSESTOQUE 300            // Limite máximo de produtos no estoque
#define MAX_PRODUTOS 350                   // Limite máximo de produtos
#define MAX_NOMEPRODUTO 50                 // Limite máximo de caracteres para o produto
#define MAX_QUANTIDADE 200                 // Limite máximo para a quantidade
#define MAX_NOMEPRODESTOQUE 80             // Limite máximo para o nome do produto no estoque
#define MAX_CATEGORIA 30                   // Limite máximo de caracteres para o nome da categoria

/* -- DEFINIÇÕES DO FINANCEIRO --- */
#define FILENAMEVENDAS "vendas.txt" // Nome do arquivo para controlar as vvendas

#define CONSOLE_WIDTH 130 // Define a largura do console como 100 caracteres

// Define uma macro para limpar a tela dependendo do sistema operacional
#ifdef _WIN32
#define LIMPAR_TELA "cls" /* Para Windows, utiliza o comando 'cls' para limpar a tela */
#else
#define LIMPAR_TELA "clear" /* Para outros sistemas, utiliza o comando 'clear' */
#endif

// Armazena dados
typedef struct Usuarios // Estrutura para armazenar dados de um usuário
{
    int id;                // ID do usuário
    char nome[MAX_NOME];   // Nome do usuário
    char login[MAX_LOGIN]; // Campo para login
    char senha[MAX_SENHA]; // Senha do usuário
    char cargo[15];        // Cargo do usuário (ex: Admin, Colaborador)
    // Campos para data de cadastro e último login
    int diaCadastro, mesCadastro, anoCadastro;
    int diaUltimoLogin, mesUltimoLogin, anoUltimoLogin;
    int horaUltimoLogin, minutoUltimoLogin; // Campos para hora
} Usuarios;
typedef struct ListadeProdutos // Estrutura para armazenar dados de um produto
{
    int id;                        // ID do produto
    char nome[MAX_NOMEPRODUTO];    // Nome do produto
    char categoria[MAX_CATEGORIA]; // Categoria do produto
    float preco;                   // Preço do produto
    float precoporkg;              // Preço por kg do produto
    float precodoFardo;            // Para "Líquidos", representa o valor do fardo
    float precoCaixa;              // Para "Outros", representa o valor da caixa
    int quantidade;                // Quantidade em estoque
} ListadeProdutos;
typedef struct ControleEstoque // Estrutura para controle de estoque
{
    int id;                         // ID do produto
    char nome[MAX_NOMEPRODESTOQUE]; // Nome do produto
    char categoria[MAX_CATEGORIA];  // Categoria do produto
    int quantidades;                // Quantidade do produto
    struct tm dataEntrada;          // Data em que o produto entrou
    int diasValidade;               // Dias de validade após a data de entrada
} ControleEstoque;
typedef struct Venda // Estrutura para armazenar dados de uma venda
{
    int idVenda;           // ID da venda
    char nomeProduto[50];  // Nome do produto vendido
    float precoPorUnidade; // Preço unitário do produto
    int quantidade;        // Quantidade do produto vendida
    float subtotal;        // Subtotal do item (preço x quantidade)

    int quantidadeItens;     // Quantidade total de itens na venda
    float totalCompra;       // Total da compra
    char formaPagamento[20]; // Forma de pagamento (ex: Cartão, Dinheiro)
    float valorPago;         // Valor pago pelo cliente
    float troco;             // Troco dado ao cliente

    struct tm dataVenda;                            // Data e hora da venda
    Usuarios usuarioCaixa;                          // Dados do caixa que fez a venda
    ListadeProdutos produtosVendidos[MAX_PRODUTOS]; // Produtos vendidos
} Venda;

// Declarção das funções utilizadas
void sair();                                                                   // Função para encerrar o sistema
void defaultMessage();                                                         // Exibe uma mensagem padrão para indicações gerais
void trim(char *str);                                                          // Função para remover espaços extras
void centralizarTexto(char *texto);                                            // Centraliza o texto no console
int validarsenha(char *senha);                                                 // Valida a senha para garantir requisitos específicos
void salvarUsuarios(Usuarios usuarios[], int qtdUsuarios);                     // Salva dados dos usuários no arquivo
void entradaSenha(char *senha);                                                // Mascarar a entrada da senha durante a digitação
void obterDataHoraAtual(int *dia, int *mes, int *ano, int *hora, int *minuto); // Obtém a data e a hora atual do sistema

// Funções Login
void ListarUsuarios(Usuarios usuario[], int qtdUsuarios); // Lista todos os usuários registrados
int carregarUsuarios(Usuarios usuarios[]);                // Carrega dados dos usuários de um arquivo
int gerarNovoId(Usuarios usuarios[], int qtdUsuarios);    // Gera um novo ID único para um usuário
void atualizarHoraLogin(Usuarios *usuario);
int removerUsuario(Usuarios usuarios[], int *qtdUsuarios);

// Funções Produtos
void ListaProdutos(ListadeProdutos produtos[], int qtdProdutos);
void CadastrarProdutos(ListadeProdutos produtos[], int *qtdProdutos);
void RemoverProduto(ListadeProdutos produtos[], int *qtdProdutos);
void BuscarProdutoPorID(ListadeProdutos produtos[], int qtdProdutos);
void BuscarPorNome(ListadeProdutos produtos[], int qtdProdutos);
void AtualizarProduto(ListadeProdutos produtos[], int qtdProdutos);
int GerarNovoIdProduto(ListadeProdutos produtos[], int qtdProdutos);
void SalvarProdutos(ListadeProdutos produtos[], int qtdProdutos);
int CarregarProdutos(ListadeProdutos produtos[], int *qtdProdutos);

// Funções Estoque
void ListadeEstoque(ControleEstoque estoque[], int qtdEstoque);          // Lista os produtos no estoque
void AdicionarnoEstoque(ControleEstoque estoque[], int *qtdEstoque);     // Adiciona produtos ao estoque
void RemoverdoEstoque(ControleEstoque estoque[], int *qtdEstoque);       // Remove produtos do estoque
void BuscarPorID(ControleEstoque estoque[], int qtdEstoque);             // Busca produto por ID
void AtualizarProdutoEstoque(ControleEstoque estoque[], int qtdEstoque); // Atualiza a quantidade de produtos no estoque
void SalvarEstoque(ControleEstoque estoque[], int qtdEstoque);           // Salva os dados de estoque
int CarregarEstoque(ControleEstoque estoque[], int *qtdEstoque);         // Carrega os dados de estoque
int GerarNovoIdEstoque(ControleEstoque estoque[], int qtdEstoque);       // Gera um novo ID para o estoque

// Funções Vendas
void RegistrarVenda(Venda venda);
void ListadasVendas();

// Telas
int Menu();               // Função para exibir o menu inicial
int MenuLogin();          // Função para exibir o menu de login e cadastro
int CadastrarUsuario();   // Função para cadastrar um novo usuário
int LoginUsuario();       // Função para login de usuário
int Caixa();              // Função para exibir o menu do caixa
int Estoque();            // Função para exibir o menu de estoque
int MenuAdmin();          // Função para exibir o menu do administrador
int MenuAtualizarLogin(); // Função para exibir o menu de atualização de dados do usuário
int GerenciarProdutos();  // Função de gerenciar produtos

// Funções utilizadas
void sair() // Função para sair do sistema
{
    system(LIMPAR_TELA);                         // Limpa a tela ao selecionar a opção de sair
    centralizarTexto("Encerrando o sistema..."); // Exibe mensagem centralizada de encerramento
    exit(0);                                     // Encerra o programa com código de status 0 (saída bem-sucedida)
}
void defaultMessage() // Função para exibir mensagem padrão de conclusão
{
    system(LIMPAR_TELA);
    centralizarTexto("Operação concluída."); // Exibe uma mensagem padrão de conclusão
}
void centralizarTexto(char *texto) // Função para centralizar os textos
{
    int comprimento = strlen(texto);                 // Calcula o comprimento do texto
    int espacos = (CONSOLE_WIDTH - comprimento) / 2; // Calcula a quantidade de espaços para centralizar
    for (int i = 0; i < espacos; i++)                // Imprime espaços antes do texto
        printf(" ");
    printf("%s\n", texto); // Exibe o texto centralizado
}
void entradaSenha(char *senha) // Função para entrada de senha com caracteres mascarados
{
    int i = 0;
    char ch;
    printf("Digite a senha: "); // Solicita a senha ao usuário

    while (1)
    {
        ch = getch();     // Captura o caractere sem exibir na tela
        if (ch == 13)     // Verifica se Enter foi pressionado
            break;        // Finaliza a entrada da senha
        else if (ch == 8) // Verifica se Backspace foi pressionado
        {
            if (i > 0) // Remove o último caractere, se houver
            {
                i--;
                printf("\b \b"); // Move o cursor para trás, apaga o caractere e ajusta a posição
            }
        }
        else if (i < MAX_SENHA - 1) // Verifica se o limite de caracteres não foi excedido
        {
            senha[i++] = ch; // Armazena o caractere na senha
            printf("*");     // Exibe um asterisco para mascarar o caractere
        }
    }
    senha[i] = '\0'; // Finaliza a string com caractere nulo
    printf("\n");    // Pula para a próxima linha
}
int validarsenha(char *senha) // Função de validação da senha
{
    // Verifica se a senha contém apenas dígitos
    for (int i = 0; i < strlen(senha); i++)
    {
        if (!isdigit(senha[i])) // Verifica se o caractere não é um dígito
        {
            return 0; // Retorna 0 se a senha contiver caracteres não numéricos
        }
    }
    return 1; // Retorna 1 se a senha for válida (apenas dígitos)
}
void obterDataHoraAtual(int *dia, int *mes, int *ano, int *hora, int *minuto) // Função para obter a data e hora atual
{
    time_t t = time(NULL);         // Obtém a hora atual em segundos desde o Epoch
    struct tm tm = *localtime(&t); // Converte para data e hora local

    *dia = tm.tm_mday;        // Define o dia atual
    *mes = tm.tm_mon + 1;     // Define o mês (ajustando, pois tm_mon começa em 0)
    *ano = tm.tm_year + 1900; // Define o ano (ajustando para o ano correto)
    *hora = tm.tm_hour;       // Define a hora atual
    *minuto = tm.tm_min;      // Define o minuto atual
}
void trim(char *str) // Função para remover espaços extras
{
    int i, j;
    // Remove leading spaces
    for (i = 0; str[i] == ' ' || str[i] == '\t'; i++)
        ;
    // Shift characters to the left
    for (j = 0; str[i]; i++, j++)
    {
        str[j] = str[i];
    }
    str[j] = '\0';

    // Remove trailing spaces
    for (i = j - 1; i >= 0 && (str[i] == ' ' || str[i] == '\t'); i--)
        ;
    str[i + 1] = '\0';
}

// MENUS
int Menu() // Função do Menu Inicial
{
    system("cls");
    system("color 30"); // Fundo azul e letras brancas

    int opcao;

    // Header do sistema
    printf("\n");
    centralizarTexto("---------------------------------------------------------------");
    centralizarTexto("| BOAS-VINDAS AO SISTEMA DE GESTÃO DO HORTIFRUTI - ALPHADEV'S |");
    centralizarTexto("---------------------------------------------------------------");
    printf("\n");
    centralizarTexto("ADS - HORTFRUTI");
    printf("\n");

    // Opções de escolha
    centralizarTexto("1. Entrar no Sistema");
    centralizarTexto("0. Fechar o Sistema");
    printf("\n");

    centralizarTexto("Por favor, digite a opção desejada (PRESS ENTER): ");
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 1:
        system(LIMPAR_TELA); // Limpa a tela ao entrar no Menu de Login
        MenuLogin();
        break;
    case 0:
        centralizarTexto("Encerrando o sistema... Volte sempre!");
        sair();
        break;
    default:
        centralizarTexto("A opção selecionada é inválida. Por favor, digite uma opção válida.");
        Menu();
    }
}

// Login
int MenuLogin() // Função do Menu de Login e Cadastro
{
    system(LIMPAR_TELA); // Limpa a tela ao selecionar uma opção

    int opcaoLogin;

    do
    {
        // Header do sistema
        printf("\n");
        centralizarTexto("-------------------------------------------");
        centralizarTexto("| VOCE ESTA NO MENU DE LOGIN - ALPHADEV'S |");
        centralizarTexto("-------------------------------------------");
        centralizarTexto("ADS - HORTFRUTI");
        printf("\n");

        // Opções de escolha
        centralizarTexto("1. Cadastre-se");
        centralizarTexto("2. Login");
        centralizarTexto("3. Atualize seus dados");
        centralizarTexto("4. Voltar para o Menu Principal");
        centralizarTexto("0. Fechar sistema");
        printf("\n");

        // Lê a opção do usuário
        centralizarTexto("Por favor, digite a opção desejada (PRESS ENTER):");
        scanf("%d", &opcaoLogin);

        // Executa a ação de acordo com a opção
        switch (opcaoLogin)
        {
        case 1:
            CadastrarUsuario(); // Chama a função de cadastro
            break;
        case 2:
            LoginUsuario(); // Chama a função de login
            break;
        case 3:
            MenuAtualizarLogin(); // Chama a função de atualizar dados
            break;
        case 4:
            centralizarTexto("Voltando ao inicio...");
#ifdef _WIN32
            Sleep(2000); // Pausa 2 segundos no Windows
#else
            sleep(2); // Pausa 2 segundos em outros sistemas
#endif
            Menu(); // Volta ao menu principal
            break;
        case 0:
            centralizarTexto("Saindo do sistema...");
            sair(); // Sai do sistema
            break;
        default:
            centralizarTexto("Opção inválida! Por favor, escolha uma opção válida.");
        }

    } while (opcaoLogin != 0);

    return 0;
}
int CadastrarUsuario() // Função do Cadastro de usuário
{
    system(LIMPAR_TELA); // Limpa a tela ao selecionar uma opção

    Usuarios usuarios[MAX_USUARIOS];
    int qtdUsuarios = carregarUsuarios(usuarios); // Carrega usuários existentes

    // Verifica se o limite de usuários foi atingido
    if (qtdUsuarios >= MAX_USUARIOS)
    {
        centralizarTexto("Limite de usuários atingido.\n");
        return;
    }

    Usuarios novoUsuario;
    novoUsuario.id = gerarNovoId(usuarios, qtdUsuarios); // Gera novo ID para o usuário

    // Exibe cabeçalho de cadastro
    printf("\n");
    centralizarTexto("---------------------");
    centralizarTexto("| FAÇA SEU CADASTRO |");
    centralizarTexto("---------------------");
    centralizarTexto("ADS - HORTFRUTI");
    printf("\n");

    // Solicita dados do novo usu?rio
    printf("Nome: ");
    scanf(" %[^\n]", novoUsuario.nome);
    printf("Login (O nome que você deseja usar para entrar no sistema): ");
    scanf(" %[^\n]", novoUsuario.login);
    printf("Digite o cargo (Admin ou Colaborador): ");
    scanf(" %[^\n]", novoUsuario.cargo);

    // Caso o usuario coloque Colaborador, ele terá as opções de quais funções ele irá fazer
    if (strcmp(novoUsuario.cargo, "Colaborador") == 0)
    {
        int opcaoFuncao;

        // Opções de escolha
        printf("Escolha a função:\n");
        printf("1. CAIXA\n");
        printf("2. ESTOQUISTA\n");
        printf("Por favor, digite a opção desejada: (PRESS ENTER): ");
        scanf("%d", &opcaoFuncao);

        switch (opcaoFuncao)
        {
        case 1:
            strcpy(novoUsuario.cargo, "CAIXA");
            break;
        case 2:
            strcpy(novoUsuario.cargo, "ESTOQUISTA");
            break;
        default:
            printf("Opção inválida! Definindo cargo como Colaborador.\n");
            strcpy(novoUsuario.cargo, "Colaborador");
            break;
        }
    }

    // Validação de senha com até 8 dígitos numéricos
    do
    {
        printf("Senha (até 8 dígitos): ");
        scanf(" %8s", novoUsuario.senha);
        if (strlen(novoUsuario.senha) > 8)
        {
            printf("Senha inválida. Deve ter até 8 dígitos.\n");
        }
    } while (strlen(novoUsuario.senha) > 8);
    entradaSenha(novoUsuario.senha);

    // Obtendo a data e hora do cadastro
    obterDataHoraAtual(&novoUsuario.diaCadastro, &novoUsuario.mesCadastro, &novoUsuario.anoCadastro,
                       &novoUsuario.horaUltimoLogin, &novoUsuario.minutoUltimoLogin);

    // Salvando o novo usuário no array e no arquivo
    usuarios[qtdUsuarios] = novoUsuario;       // Adiciona o novo usuário ao array
    salvarUsuarios(usuarios, qtdUsuarios + 1); // Salva todos os usuários, incluindo o novo

    printf("Usuário cadastrado com sucesso.\n");

    return 0;
}
int LoginUsuario() // Função do Login
{
    system(LIMPAR_TELA); // Limpa a tela ao selecionar uma opção

    Usuarios usuarios[MAX_USUARIOS];
    int qtdUsuarios = carregarUsuarios(usuarios); // Carrega usuários existentes

    char login[MAX_LOGIN], senha[MAX_SENHA];

    printf("\n");
    centralizarTexto("------------------");
    centralizarTexto("| FAÇA SEU LOGIN |");
    centralizarTexto("------------------");
    centralizarTexto("ADS - HORTFRUTI");
    printf("\n");

    printf("Login: ");
    scanf("%s", login);
    trim(login); // Remover espaços extras no login

    entradaSenha(senha);
    trim(senha); // Remover espaços extras na senha

    int encontrado = 0;
    for (int i = 0; i < qtdUsuarios; i++)
    {
        trim(usuarios[i].login); // Remover espaços extras no login do arquivo
        trim(usuarios[i].senha); // Remover espaços extras na senha do arquivo

        if (strcmp(usuarios[i].login, login) == 0)
        {
            if (strcmp(usuarios[i].senha, senha) == 0)
            {
                printf("Login realizado com sucesso! Seja bem-vindo, %s.\n", usuarios[i].login);
                printf("Permissão: %s\n", usuarios[i].cargo);
                encontrado = 1;

                // Atualiza a hora do último login
                atualizarHoraLogin(&usuarios[i]);
                salvarUsuarios(usuarios, qtdUsuarios); // Salva os dados atualizados no arquivo

                // Verifica o cargo e redireciona para o menu apropriado
                if (strcmp(usuarios[i].cargo, "Admin") == 0)
                {
                    MenuAdmin(); // Acesso ao menu de administrador
                }
                else if (strcmp(usuarios[i].cargo, "CAIXA") == 0)
                {
                    Caixa(); // Acesso ao menu de Caixa
                }
                else if (strcmp(usuarios[i].cargo, "ESTOQUISTA") == 0)
                {
                    Estoque(); // Acesso ao menu de Estoquista
                }
                else
                {
                    printf("Cargo não reconhecido.\n");
                }
                break;
            }
            else
            {
                printf("Senha incorreta!\n");
                return 0; // Retorna se a senha estiver incorreta
            }
        }
    }

    if (!encontrado)
    {
        printf("Login não encontrado.\n");
    }

    return 0;
}
int MenuAtualizarLogin() // Função do Menu de Atualizar Dados do Login
{
    system(LIMPAR_TELA); // Limpa a tela ao selecionar uma opção

    Usuarios usuarios[MAX_USUARIOS];
    int qtdUsuarios = carregarUsuarios(usuarios);

    char login[MAX_LOGIN];
    int encontrado = -1;

    printf("\n");
    centralizarTexto("---------------------------");
    centralizarTexto("| ATUALIZAR DADOS DE LOGIN |");
    centralizarTexto("---------------------------");
    centralizarTexto("ADS - HORTIFRUTI");
    printf("\n");

    // Primeira coisa: o usuário informa o seu login
    printf("Digite o seu login: ");
    scanf("%s", login);

    // Procurando usuário
    for (int i = 0; i < qtdUsuarios; i++)
    {
        if (strcmp(usuarios[i].login, login) == 0)
        {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1)
    {
        printf("Usuário não encontrado.\n");
        return 0;
    }

    int opcao;
    if (strcmp(usuarios[encontrado].cargo, "Admin") == 0)
    {
        // Se for Admin, ele pode atualizar outros usuários ou o próprio
        printf("Você é um Admin.\n");
        printf("Escolha uma opção:\n");

        // Opções do Admin
        printf("1. Atualizar seus dados\n");
        printf("2. Atualizar dados de outro usuário\n");
        printf("3. Demitir colaborador\n");
        printf("0. Voltar\n");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            // Alterações no próprio perfil (Admin)
            {
                char novoLogin[MAX_LOGIN];
                char novaSenha[MAX_SENHA];

                printf("Novo login (caso não queira, aperte Enter): ");
                getchar(); // Limpa o buffer de entrada
                fgets(novoLogin, sizeof(novoLogin), stdin);

                if (novoLogin[0] != '\n')
                {
                    novoLogin[strcspn(novoLogin, "\n")] = '\0'; // Remove o '\n' do final da string
                    strcpy(usuarios[encontrado].login, novoLogin);
                }

                printf("Nova senha (caso não queira, aperte Enter): ");
                fgets(novaSenha, sizeof(novaSenha), stdin);

                if (novaSenha[0] != '\n')
                {
                    novaSenha[strcspn(novaSenha, "\n")] = '\0'; // Remove o '\n' do final da string
                    strcpy(usuarios[encontrado].senha, novaSenha);
                }

                printf("Dados alterados com sucesso.\n");
            }
            break;

        case 2:
            // Alterações no perfil de outro usuário
            {
                char outroLogin[MAX_LOGIN];
                printf("Digite o login do usuário que você deseja atualizar: ");
                scanf(" %[^\n]", outroLogin);

                int encontradoOutro = -1;
                for (int i = 0; i < qtdUsuarios; i++)
                {
                    if (strcmp(usuarios[i].login, outroLogin) == 0)
                    {
                        encontradoOutro = i;
                        break;
                    }
                }

                if (encontradoOutro == -1)
                {
                    printf("Usuário não encontrado.\n");
                }
                else
                {
                    // Atualiza o cargo do colaborador
                    int opcaoCargo;
                    printf("Novo cargo (Escolha uma opção):\n");
                    printf("1. CAIXA\n");
                    printf("2. ESTOQUISTA\n");
                    printf("3. Admin\n");
                    printf("0. Voltar\n");
                    scanf("%d", &opcaoCargo);

                    switch (opcaoCargo)
                    {
                    case 1:
                        strcpy(usuarios[encontradoOutro].cargo, "CAIXA");
                        printf("Cargo alterado para CAIXA.\n");
                        break;
                    case 2:
                        strcpy(usuarios[encontradoOutro].cargo, "ESTOQUISTA");
                        printf("Cargo alterado para ESTOQUISTA.\n");
                        break;
                    case 3:
                        strcpy(usuarios[encontradoOutro].cargo, "Admin");
                        printf("Cargo alterado para Admin.\n");
                        break;
                    case 0:
                        printf("Voltando...\n");
                        break;
                    default:
                        printf("Opção inválida!\n");
                        break;
                    }
                }
            }
            break;

        case 3:
            // Deletar colaborador
            {
                char loginDeletar[MAX_LOGIN];
                printf("Digite o login do colaborador que você deseja deletar: ");
                scanf(" %[^\n]", loginDeletar);

                int encontradoDeletar = -1;
                for (int i = 0; i < qtdUsuarios; i++)
                {
                    if (strcmp(usuarios[i].login, loginDeletar) == 0 && strcmp(usuarios[i].cargo, "CAIXA") == 0)
                    {
                        encontradoDeletar = i;
                        break;
                    }
                }

                if (encontradoDeletar == -1)
                {
                    printf("Colaborador não encontrado ou o login não corresponde a um colaborador.\n");
                }
                else
                {
                    // Remover o colaborador
                    for (int i = encontradoDeletar; i < qtdUsuarios - 1; i++)
                    {
                        usuarios[i] = usuarios[i + 1];       // Move os dados para a posição anterior
                        usuarios[i].id = usuarios[i].id - 1; // Atualiza o ID para manter a sequência
                    }

                    qtdUsuarios--; // Reduz a quantidade de usuários
                    printf("Colaborador deletado com sucesso.\n");
                }
            }
            break;

        case 0:
            return 0;
        default:
            printf("Opção inválida!\n");
            break;
        }
    }
    else
    {
        // Se for Colaborador, ele pode alterar seu próprio login e senha
        printf("Você é um Colaborador.\n");
        printf("Escolha uma opção:\n");
        printf("1. Alterar login e senha\n");
        printf("0. Voltar\n");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
        {
            char novoLogin[MAX_LOGIN];
            char novaSenha[MAX_SENHA];
            int loginExistente = 0;

            // Alterar login
            printf("Novo login (caso não queira, aperte Enter): ");
            getchar(); // Limpa o buffer de entrada
            fgets(novoLogin, sizeof(novoLogin), stdin);

            if (novoLogin[0] != '\n')
            {
                novoLogin[strcspn(novoLogin, "\n")] = '\0'; // Remove o '\n' do final da string
                // Verifica se o novo login já existe
                for (int i = 0; i < qtdUsuarios; i++)
                {
                    if (strcmp(usuarios[i].login, novoLogin) == 0)
                    {
                        loginExistente = 1;
                        break;
                    }
                }

                if (loginExistente)
                {
                    printf("O login informado já existe. Tente outro.\n");
                }
                else
                {
                    strcpy(usuarios[encontrado].login, novoLogin);
                    printf("Login alterado com sucesso.\n");
                }
            }

            // Alterar senha
            printf("Nova senha (caso não queira, aperte Enter): ");
            fgets(novaSenha, sizeof(novaSenha), stdin);

            if (novaSenha[0] != '\n')
            {
                novaSenha[strcspn(novaSenha, "\n")] = '\0'; // Remove o '\n' do final da string
                strcpy(usuarios[encontrado].senha, novaSenha);
                printf("Senha alterada com sucesso.\n");
            }
            else
            {
                printf("Nenhuma alteração de senha realizada.\n");
            }
            break;
        }
        case 0:
            return 0;
        default:
            printf("Opção inválida!\n");
            break;
        }
    }

    // Salva as alterações no arquivo
    salvarUsuarios(usuarios, qtdUsuarios);
    printf("Dados atualizados com sucesso.\n");

    return 0;
}

// Colaborador
int Caixa() // Função de atendimento do caixa
{
    system(LIMPAR_TELA); // Limpa a tela ao selecionar uma opção

    system("color 87");

    float total = 0.0;
    int id;
    float quantidade;

    // Carrega a lista de produtos uma vez
    ListadeProdutos produtos[MAX_PRODUTOS];
    int qtdProdutos = 0;

    CarregarProdutos(produtos, &qtdProdutos);

    printf("\n");
    centralizarTexto("-----------------------------------");
    centralizarTexto("| BEM-VINDO AO CAIXA - ALPHADEV'S |");
    centralizarTexto("-----------------------------------");
    centralizarTexto("ADS - HORTFRUTI");
    printf("\n");

    Venda venda;       // Estrutura para a venda
    venda.idVenda = 1; // Exemplo de ID de venda, pode ser incrementado com base no histórico de vendas
    venda.totalCompra = 0.0;
    venda.quantidadeItens = 0;

    while (true)
    {
        int opcao;
        printf("\n");
        centralizarTexto("Escolha uma opção:");
        centralizarTexto("1. Lista de produtos");
        centralizarTexto("2. Buscar produto(Nome)");
        centralizarTexto("3. Passar produtos");
        centralizarTexto("0. Finalizar compra");
        printf("\n");
        printf("Por favor, digite a opção desejada: (PRESS ENTER): ");
        scanf("%d", &opcao);
        printf("\n");

        if (opcao == 1)
        {
           ListaProdutos(produtos, qtdProdutos);
            continue;
        }
        if (opcao == 2)
        {
            BuscarPorNome(produtos, qtdProdutos); // Função para buscar produto por nome
            continue;                             // Retorna ao início para permitir outra escolha
        }
        if (opcao == 3)
        {
            // Código para passar os produtos
            printf("Digite o ID do produto (ou -1 para finalizar): ");
            scanf("%d", &id);
            if (id == -1)
                break;

            bool produtoEncontrado = false;

            // Procura pelo produto na lista
            for (int i = 0; i < qtdProdutos; i++)
            {
                if (produtos[i].id == id)
                {
                    produtoEncontrado = true;
                    char unidadePeso;
                    printf("O produto é por (u) unidade, (p) peso ou (f) fardo? ");
                    scanf(" %c", &unidadePeso);

                    if (unidadePeso == 'u')
                    {
                        printf("Digite a quantidade do produto: ");
                        scanf("%f", &quantidade);
                        total += produtos[i].preco * quantidade;                     // Atualiza o total com o preço por unidade
                        venda.produtosVendidos[venda.quantidadeItens] = produtos[i]; // Adiciona o produto à venda
                        venda.produtosVendidos[venda.quantidadeItens].quantidade = quantidade;
                        venda.quantidadeItens++;
                    }
                    else if (unidadePeso == 'p')
                    {
                        printf("Digite o peso em kg do produto: ");
                        scanf("%f", &quantidade);
                        total += produtos[i].precoporkg * quantidade;                // Atualiza o total com o preço por kg
                        venda.produtosVendidos[venda.quantidadeItens] = produtos[i]; // Adiciona o produto à venda
                        venda.produtosVendidos[venda.quantidadeItens].quantidade = quantidade;
                        venda.quantidadeItens++;
                    }
                    else if (unidadePeso == 'f' && strcmp(produtos[i].categoria, "Líquidos") == 0)
                    {
                        printf("Digite a quantidade de fardos: ");
                        scanf("%f", &quantidade);
                        total += produtos[i].precodoFardo * quantidade;              // Atualiza o total com o preço do fardo
                        venda.produtosVendidos[venda.quantidadeItens] = produtos[i]; // Adiciona o produto à venda
                        venda.produtosVendidos[venda.quantidadeItens].quantidade = quantidade;
                        venda.quantidadeItens++;
                    }
                    else
                    {
                        printf("Opção inválida!\n");
                        continue; // Retorna ao início se a opção for inválida
                    }

                    printf("Produto: %s | Preço por %s: R$ %.2f | Quantidade: %.2f\n",
                           produtos[i].nome,
                           (unidadePeso == 'u') ? "unidade" : (unidadePeso == 'p') ? "kg"
                                                                                   : "fardo",
                           (unidadePeso == 'u') ? produtos[i].preco : (unidadePeso == 'p') ? produtos[i].precoporkg
                                                                                           : produtos[i].precodoFardo,
                           quantidade);
                    break;
                }
            }

            if (!produtoEncontrado)
            {
                centralizarTexto("Produto não encontrado!");
            }
            continue; // Retorna ao início para passar mais produtos
        }
        if (opcao == 0)
        {
            break; // Finaliza a compra
        }

        printf("Opção inválida, por favor tente novamente.\n");
    }

    printf("\nTotal da compra: R$ %.2f\n", total);

    // Registra a forma de pagamento
    char formaPagamento[20];
    printf("Digite qual a forma de pagamento (ex: Cartão ou Dinheiro): ");
    scanf("%s", formaPagamento);
    strcpy(venda.formaPagamento, formaPagamento); // Armazena a forma de pagamento

    if (strcmp(formaPagamento, "Dinheiro") == 0)
    {
        float valorPago;
        float troco;
        printf("Digite o valor entregue pelo cliente: ");
        scanf("%f", &valorPago);

        if (valorPago >= total)
        {
            troco = valorPago - total;
            printf("Troco: R$ %.2f\n", troco);
            venda.valorPago = valorPago;
            venda.troco = troco;
        }
        else
        {
            printf("Valor insuficiente.\n");
            venda.valorPago = 0;
            venda.troco = 0;
        }
    }
    else if (strcmp(formaPagamento, "Cartao") == 0)
    {
        char tipoCartao[10];
        printf("Digite o tipo do cartão (Débito ou Crédito): ");
        scanf("%s", tipoCartao);

        if (strcmp(tipoCartao, "Debito") == 0)
        {
            printf("Pagamento realizado com Cartão de Débito.\n");
            venda.valorPago = total;
            venda.troco = 0.0;
        }
        else if (strcmp(tipoCartao, "Credito") == 0)
        {
            printf("Pagamento realizado com Cartão de Crédito.\n");
            venda.valorPago = total;
            venda.troco = 0.0;
        }
        else
        {
            printf("Tipo de cartão inválido.\n");
            venda.valorPago = 0.0;
            venda.troco = 0.0;
        }
    }
    else
    {
        printf("Forma de pagamento inválida.\n");
        venda.valorPago = 0.0;
        venda.troco = 0.0;
    }

    // Salva a venda
    time_t t = time(NULL);
    venda.dataVenda = *localtime(&t); // Atribui a data e hora atuais à venda
    RegistrarVenda(venda);            // Registra a venda no arquivo

    // Após finalizar a compra, oferece opções ao usuário
    while (true)
    {
        printf("\nEscolha uma opção:\n");
        printf("1. Fazer outra compra\n");
        printf("2. Sair do sistema\n");
        printf("\n");
        printf("Por favor, digite a opção desejada: (PRESS ENTER): ");
        int opcaoFinalizacao;
        scanf("%d", &opcaoFinalizacao);

        if (opcaoFinalizacao == 1)
        {
            total = 0.0; // Reinicia o total para uma nova compra
            Caixa();
            break; // Retorna ao loop principal
        }
        else if (opcaoFinalizacao == 2)
        {
            centralizarTexto("Obrigado pela sua compra! Volte sempre.");
            return 0; // Sai da função Caixa
        }
        else
        {
            printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}
int Estoque() // Função para exibir o estoque
{
    system(LIMPAR_TELA); // Limpa a tela ao selecionar uma op??o

    system("color F0");

    int opcaoEstoque;
    ControleEstoque estoque[MAX_QUANTIDADE];
    int qtdEstoque = 0;

    CarregarEstoque(estoque, &qtdEstoque);

    printf("\n");
    centralizarTexto("------------------------------------");
    centralizarTexto("| BEM-VINDO AO ESTOQUE - ALPHADEV'S |");
    centralizarTexto("------------------------------------");
    centralizarTexto("ADS - HORTFRUTI");
    printf("\n");

    while (1)
    {
        printf("\n");
        centralizarTexto("Escolha uma opção:");
        centralizarTexto("1. Adicionar produto");
        centralizarTexto("2. Lista do estoque");
        centralizarTexto("3. Buscar um produto por ID");
        centralizarTexto("4. Atualizar produto");
        centralizarTexto("5. Salvar estoque");
        centralizarTexto("6. Remover produto");
        centralizarTexto("0. Fechar sistema");
        printf("\n");
        printf("Por favor, digite a opção desejada (PRESS ENTER): ");
        scanf("%d", &opcaoEstoque);
        printf("\n");

        switch (opcaoEstoque)
        {
        case 1:
            AdicionarnoEstoque(estoque, &qtdEstoque);
            break;
        case 2:
            ListadeEstoque(estoque, qtdEstoque);
            break;
        case 3:
            BuscarPorID(estoque, qtdEstoque);
            break;
        case 4:
            AtualizarProdutoEstoque(estoque, qtdEstoque);
            break;
        case 5:
            SalvarEstoque(estoque, qtdEstoque);
            break;
        case 6:
            RemoverdoEstoque(estoque, &qtdEstoque);
            break;
        case 0:
            centralizarTexto("Saindo do sistema...");
            sair(); // Sai do sistema
        default:
            centralizarTexto("A opção selecionada é inválida. Por favor, digite uma opção válida.");
        }
    }

    return 0;
}

// Administrador
int MenuAdmin()
{
    system(LIMPAR_TELA); // Limpa a tela ao selecionar uma op??o

    int opcao;

    // Carregar usuarios uma vez
    Usuarios usuarios[MAX_USUARIOS];
    int qtdUsuarios = carregarUsuarios(usuarios);

    // Carregar estoque uma vez
    ControleEstoque estoque[MAX_PRODUTOS]; // Define o array de produtos
    int qtdEstoque = 0;                    // Variável para armazenar a quantidade de produtos carregados
    CarregarEstoque(estoque, &qtdEstoque); // Chamada correta da função com os argumentos necessários

    do
    {
        printf("\n");
        centralizarTexto("---------------------------------------------");
        centralizarTexto("| SEJA BEM-VINDO ADMINISTRADOR - ALPHADEV'S |");
        centralizarTexto("---------------------------------------------");
        centralizarTexto("ADS - HORTFRUTI");
        printf("\n");

        centralizarTexto("1. Controle de Colaboradores");
        centralizarTexto("2. Controle de vendas");
        centralizarTexto("3. Gerenciamneto de produtos");
        centralizarTexto("4. Gerenciamento do estoque");
        centralizarTexto("5. Voltar ao Inicio");
        centralizarTexto("0. Fechar sistema");
        printf("\n");

        centralizarTexto("Por favor, digite a opção desejada (PRESS ENTER): ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            ListarUsuarios(usuarios, qtdUsuarios);
            break; // Volta ao in?cio para permitir outra escolha
        case 2:
            ListadasVendas();
            break;
        case 3:
            GerenciarProdutos();
            break;
        case 4:
            ListadeEstoque(estoque, qtdEstoque);
            break;
        case 5:
            centralizarTexto("Voltando ao inicio...");
#ifdef _WIN32
            Sleep(2000); // Pausa 2 segundos no Windows
#else
            sleep(2); // Pausa 2 segundos em outros sistemas
#endif
            Menu(); // Volta ao menu principal
            break;
        case 0:
            sair();
            break;
        default:
            centralizarTexto("Opcao invalida! Por favor, escolha uma opcao valida.");
        }
    } while (opcao != 0);

    return 0;
}

// Funções Login
void salvarUsuarios(Usuarios usuarios[], int qtdUsuarios)
{
    FILE *sUsuarios = fopen(FILENAMELOGIN, "w"); // Abre o arquivo em modo escrita
    if (sUsuarios == NULL)
    {
        printf("Erro ao abrir o arquivo para salvar os usu?rios.\n");
        return;
    }

    for (int i = 0; i < qtdUsuarios; i++)
    {
        // Escreve os dados do usu?rio no arquivo
        fprintf(sUsuarios, "%d;%s;%s;%s;%s;%d/%d/%d;%02d:%02d\n",
                usuarios[i].id,
                usuarios[i].nome,
                usuarios[i].login,
                usuarios[i].senha,
                usuarios[i].cargo,
                usuarios[i].diaCadastro,
                usuarios[i].mesCadastro,
                usuarios[i].anoCadastro,
                usuarios[i].horaUltimoLogin,
                usuarios[i].minutoUltimoLogin);
    }

    fclose(sUsuarios); // Fecha o arquivo
}
int carregarUsuarios(Usuarios usuarios[])
{
    FILE *file = fopen(FILENAMELOGIN, "r"); // Abre o arquivo em modo leitura
    if (file == NULL)
    {
        return 0; // Retorna 0 se o arquivo n?o puder ser aberto
    }

    int count = 0;
    while (fscanf(file, "%d;%79[^;];%19[^;];%8[^;];%14[^;];%d/%d/%d;%02d:%02d\n",
                  &usuarios[count].id,
                  usuarios[count].nome,
                  usuarios[count].login,
                  usuarios[count].senha,
                  usuarios[count].cargo,
                  &usuarios[count].diaCadastro,
                  &usuarios[count].mesCadastro,
                  &usuarios[count].anoCadastro,
                  &usuarios[count].horaUltimoLogin,
                  &usuarios[count].minutoUltimoLogin) == 10)
    {
        count++; // Incrementa o contador de usuários
    }

    fclose(file); // Fecha o arquivo
    return count; // Retorna o número de usuários carregados
}
int gerarNovoId(Usuarios usuarios[], int qtdUsuarios)
{
    if (qtdUsuarios == 0)
        return 1;                            // Se não há usuários, o primeiro ID é 1
    return usuarios[qtdUsuarios - 1].id + 1; // Caso contrário, gera um ID sequencial
}
void ListarUsuarios(Usuarios usuarios[], int qtdUsuarios)
{
    system(LIMPAR_TELA);
    printf("\n");
    centralizarTexto("---------------------------------");
    centralizarTexto("| LISTA DE USUÁRIOS - ALPHADEV'S |");
    centralizarTexto("---------------------------------");
    centralizarTexto("ADS - HORTFRUTI");
    printf("\n");

    // Cabeçalho da tabela
    printf("Nome                  | Cargo        | Data de Cadastro | Último Login\n");
    printf("-----------------------------------------------------------------------\n");

    // Exibindo os dados dos usuários
    for (int i = 0; i < qtdUsuarios; i++)
    {
        printf("%-20s | %-12s | %02d/%02d/%04d     | %02d:%02d\n",
               usuarios[i].nome,
               usuarios[i].cargo, // Exibe o cargo do usuário
               usuarios[i].diaCadastro,
               usuarios[i].mesCadastro,
               usuarios[i].anoCadastro,
               usuarios[i].horaUltimoLogin,
               usuarios[i].minutoUltimoLogin);
    }
    printf("\n");
}
void atualizarHoraLogin(Usuarios *usuario)
{
    time_t agora = time(NULL);
    struct tm *tempoAtual = localtime(&agora);

    usuario->horaUltimoLogin = tempoAtual->tm_hour;
    usuario->minutoUltimoLogin = tempoAtual->tm_min;
}
int removerUsuario(Usuarios usuarios[], int *qtdUsuarios)
{
    int idRemover;
    printf("Digite o ID do usuário que deseja remover: ");
    scanf("%d", &idRemover);

    int encontrado = -1;

    // Procurar o usuário pelo ID
    for (int i = 0; i < *qtdUsuarios; i++)
    {
        if (usuarios[i].id == idRemover)
        {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1)
    {
        printf("Usuário não encontrado.\n");
        return 0;
    }

    // Remover o usuário (mover os elementos posteriores para a posição anterior)
    for (int i = encontrado; i < *qtdUsuarios - 1; i++)
    {
        usuarios[i] = usuarios[i + 1];
        // Atualizar o ID dos usuários seguintes
        usuarios[i].id = usuarios[i].id - 1; // Decrementa o ID para manter a sequência
    }

    // Reduzir a quantidade de usuários
    (*qtdUsuarios)--;

    // Salvar as alterações no arquivo
    salvarUsuarios(usuarios, *qtdUsuarios);
    printf("Usuário removido com sucesso.\n");
    return 1;
}

// Funções Produtos
int GerenciarProdutos()
{
    system(LIMPAR_TELA); // Limpa a tela ao selecionar uma opção

    int opcaoProduto;

    // Carrega a lista de produtos uma vez
    ListadeProdutos produtos[MAX_PRODUTOS];
    int qtdProdutos = 0;

    CarregarProdutos(produtos, &qtdProdutos);

    printf("\n");
    centralizarTexto("-----------------------------");
    centralizarTexto("| GERENCIAMENTO DE PRODUTOS |");
    centralizarTexto("-----------------------------");
    centralizarTexto("ADS - HORTFRUTI");
    printf("\n");

    while (1)
    {
        printf("\n");
        centralizarTexto("Escolha uma opção:");
        centralizarTexto("1. Cadastrar produto");
        centralizarTexto("2. Lista de produtos");
        centralizarTexto("3. Buscar produto(ID)");
        centralizarTexto("4. Atualizar produto");
        centralizarTexto("5. Salvar prosutos");
        centralizarTexto("6. Remover produto");
        centralizarTexto("7. Voltar para o menu anterior");
        centralizarTexto("0. Fechar sistema");
        printf("\n");
        printf("Por favor, digite a opção desejada (PRESS ENTER): ");
        scanf("%d", &opcaoProduto);
        printf("\n");

        switch (opcaoProduto)
        {
        case 1:
            CadastrarProdutos(produtos, &qtdProdutos);
            break;
        case 2:
            ListaProdutos(produtos, qtdProdutos);
            break;
        case 3:
            BuscarProdutoPorID(produtos, qtdProdutos);
            break;
        case 4:
            AtualizarProduto(produtos, qtdProdutos);
            break;
        case 5:
            SalvarProdutos(produtos, qtdProdutos);
            break;
        case 6:
            RemoverProduto(produtos, &qtdProdutos);
            break;
        case 7:
            centralizarTexto("Voltando ao inicio...");
#ifdef _WIN32
            Sleep(2000); // Pausa 2 segundos no Windows
#else
            sleep(2); // Pausa 2 segundos em outros sistemas
#endif
            MenuAdmin(); // Volta ao menu principal
            break;
        case 0:
            centralizarTexto("Saindo do sistema...");
            sair(); // Saí do programa de forma limpa
        default:
            centralizarTexto("A opção selecionada é inválida. Por favor, digite uma opção válida.");
        }
    }

    return 0;
}
void ListaProdutos(ListadeProdutos produtos[], int qtdProdutos) // Função para listar todos os produtos no estoque, separados por categoria
{
    system(LIMPAR_TELA);

    printf("\n");
    centralizarTexto("----------------------------------");
    centralizarTexto("| LISTA DE PRODUTOS - ALPHADEV'S |");
    centralizarTexto("----------------------------------");
    centralizarTexto("ADS - HORTFRUTI");
    printf("\n");

    char *categorias[] = {"Frutas", "Verduras", "Legumes", "Líquidos", "Grãos e Cereais", "Outros"};
    int numCategorias = sizeof(categorias) / sizeof(categorias[0]);

    // Declaaração de 'j' e 'i'
    int j, i; // Adiciona a declaração de 'j' aqui para que seja visível em todo o escopo da função

    for (j = 0; j < numCategorias; j++)
    {
        bool encontrouCategoria = false;
        printf("\n%s:\n", categorias[j]);

        // Títulos da tabela
        if (strcmp(categorias[j], "Líquidos") == 0)
        {
            printf("ID    | Nome            | Preço Unitário | Preço do Fardo\n");
            printf("------------------------------------------------------------\n");
        }
        else if (strcmp(categorias[j], "Outros") == 0)
        {
            printf("ID    | Nome            | Preço Unitário | Preço da Caixa\n");
            printf("------------------------------------------------------------\n");
        }
        else
        {
            printf("ID    | Nome            | Preço Unitário | Preço por Kg\n");
            printf("----------------------------------------------------------\n");
        }

        for (i = 0; i < qtdProdutos; i++)
        {
            if (strcmp(produtos[i].categoria, categorias[j]) == 0)
            {
                if (strcmp(produtos[i].categoria, "Líquidos") == 0)
                {
                    printf("%-5d | %-15s | R$ %.2f     | R$ %.2f\n",
                           produtos[i].id,
                           produtos[i].nome,
                           produtos[i].preco,
                           produtos[i].precodoFardo);
                }
                else if (strcmp(produtos[i].categoria, "Outros") == 0)
                {
                    printf("%-5d | %-15s | R$ %.2f     | R$ %.2f\n",
                           produtos[i].id,
                           produtos[i].nome,
                           produtos[i].preco,
                           produtos[i].precoCaixa);
                }
                else
                {
                    printf("%-5d | %-15s | R$ %.2f     | R$ %.2f\n",
                           produtos[i].id,
                           produtos[i].nome,
                           produtos[i].preco,
                           produtos[i].precoporkg);
                }
                encontrouCategoria = true;
            }
        }

        if (!encontrouCategoria)
        {
            printf("Nenhum produto encontrado nesta categoria.\n");
        }
    }
}
void CadastrarProdutos(ListadeProdutos produtos[], int *qtdProdutos) // Função para adicionar um produto ao estoque
{
    system(LIMPAR_TELA); // Limpa a tela ao selecionar uma opção

    if (*qtdProdutos >= MAX_PRODUTOS)
    {
        centralizarTexto("Limite de produtos atingido.\n");
        return; // Corrigido: simplesmente retorna da função
    }

    ListadeProdutos novoProduto;
    novoProduto.id = GerarNovoIdProduto(produtos, *qtdProdutos);

    printf("\n");
    centralizarTexto("--------------------");
    centralizarTexto("| CADASTRAR PRODUTO |");
    centralizarTexto("--------------------");
    centralizarTexto("ADS - HORTFRUTI");
    printf("\n");

    printf("Nome do produto: ");
    getchar();
    fgets(novoProduto.nome, MAX_NOMEPRODUTO, stdin);
    novoProduto.nome[strcspn(novoProduto.nome, "\n")] = 0;

    // Solicita a categoria do produto
    printf("Escolha a categoria do produto:\n");
    printf("1. Frutas\n");
    printf("2. Verduras\n");
    printf("3. Legumes\n");
    printf("4. Líquidos\n");
    printf("5. Grãos e Cereais\n");
    printf("6. Outros\n");
    printf("Por favor, digite a opção desejada(PRESS ENTER): ");
    int opcaoCategoria;
    scanf("%d", &opcaoCategoria);

    switch (opcaoCategoria)
    {
    case 1:
        strcpy(novoProduto.categoria, "Frutas");
        break;
    case 2:
        strcpy(novoProduto.categoria, "Verduras");
        break;
    case 3:
        strcpy(novoProduto.categoria, "Legumes");
        break;
    case 4:
        strcpy(novoProduto.categoria, "Líquidos");
        break;
    case 5:
        strcpy(novoProduto.categoria, "Grãos e Cereais");
        break;
    case 6:
        strcpy(novoProduto.categoria, "Outros");
        break;
    default:
        printf("Categoria inválida! Selecione uma categoria válida\n");
        break;
    }

    printf("Preço unitário (R$): ");
    scanf("%f", &novoProduto.preco);

    if (strcmp(novoProduto.categoria, "Líquidos") == 0)
    {
        printf("Preço do fardo (R$): ");
        scanf("%f", &novoProduto.precodoFardo);
        novoProduto.precoporkg = 0;
    }
    else if (strcmp(novoProduto.categoria, "Outros") == 0)
    {
        printf("Preço da caixa (R$): ");
        scanf("%f", &novoProduto.precoCaixa);
        novoProduto.precoporkg = 0;
        novoProduto.precodoFardo = 0;
    }
    else
    {
        printf("Preço por kg (R$): ");
        scanf("%f", &novoProduto.precoporkg);
        novoProduto.precodoFardo = 0;
    }

    produtos[*qtdProdutos] = novoProduto;
    (*qtdProdutos)++;

    printf("Produto cadastrado com sucesso!\n");
}
void RemoverProduto(ListadeProdutos produtos[], int *qtdProdutos) // Função para remover um produto pelo ID
{
    system(LIMPAR_TELA); // Limpa a tela ao selecionar uma opção

    int idBusca;
    printf("\n");
    centralizarTexto("------------------");
    centralizarTexto("| REMOVER PRODUTO |");
    centralizarTexto("------------------");
    centralizarTexto("ADS - HORTFRUTI");
    printf("\n");

    printf("Digite o ID do produto a ser removido: ");
    scanf("%d", &idBusca); // Lê o ID do produto

    for (int i = 0; i < *qtdProdutos; i++)
    {
        if (produtos[i].id == idBusca) // Verifica se o ID corresponde
        {
            printf("Produto encontrado: ID: %d | Nome: %s | Categoria: %s\n",
                   produtos[i].id, produtos[i].nome, produtos[i].categoria);

            // Pergunta se o usuário tem certeza que deseja remover o produto
            char confirmacao;
            printf("Deseja remover este produto? (s/n): ");
            scanf(" %c", &confirmacao); // Leitura da confirmação

            if (confirmacao == 's' || confirmacao == 'S') // Se o usuário confirmar a remoção
            {
                for (int j = i; j < *qtdProdutos - 1; j++)
                {
                    produtos[j] = produtos[j + 1]; // Move todos os produtos à frente
                    produtos[j].id = j + 1;        // Atualiza o ID dos produtos
                }
                (*qtdProdutos)--; // Decrementa a quantidade de produtos
                printf("Produto removido com sucesso!\n");
                return;
            }
            else
            {
                printf("Remoção cancelada.\n");
                return;
            }
        }
    }

    // Caso o produto com o ID não seja encontrado
    printf("Produto com ID %d não encontrado!\n", idBusca);
}
void BuscarProdutoPorID(ListadeProdutos produtos[], int qtdProdutos) // Função para buscar um produto pelo ID
{
    system(LIMPAR_TELA); // Limpa a tela ao selecionar uma opção

    int id;
    printf("\n");
    centralizarTexto("-----------------");
    centralizarTexto("| BUSCAR PRODUTO |");
    centralizarTexto("-----------------");
    centralizarTexto("ADS - HORTFRUTI");
    printf("\n");

    printf("Digite o ID do produto para buscar: ");
    scanf("%d", &id);

    for (int i = 0; i < qtdProdutos; i++)
    {
        if (produtos[i].id == id)
        {
            // Exibe o produto encontrado com base na categoria
            printf("Produto encontrado: ID: %d | Nome: %s | Categoria: %s | Preço Unitário: R$ %.2f\n",
                   produtos[i].id, produtos[i].nome, produtos[i].categoria, produtos[i].preco); // Exibe preço unitário

            if (strcmp(produtos[i].categoria, "Líquidos") == 0) // Caso o produto seja da categoria 'Líquidos'
            {
                printf("Preço(Fardo): R$ %.2f\n", produtos[i].precodoFardo); // Exibe o preço do fardo
            }
            else if (strcmp(produtos[i].categoria, "Outros") == 0) // Caso o produto seja da categoria 'Outros'
            {
                printf("Preço(Caixa): R$ %.2f\n", produtos[i].precoCaixa); // Exibe o preço da caixa
            }
            else // Para as outras categorias, exibe o preço por kg
            {
                printf("Preço(kg): R$ %.2f\n", produtos[i].precoporkg); // Exibe o preço por kg
            }

            return; // Produto encontrado, função termina
        }
    }

    // Caso o produto não seja encontrado
    printf("Produto com ID %d não encontrado!\n", id);
}
void BuscarPorNome(ListadeProdutos produtos[], int qtdProdutos) // Função para buscar um produto pelo nome
{
    system(LIMPAR_TELA); // Limpa a tela ao selecionar uma opção

    char nomeBusca[100];
    printf("\n");
    centralizarTexto("--------------------------");
    centralizarTexto("| BUSCAR PRODUTO POR NOME |");
    centralizarTexto("--------------------------");
    centralizarTexto("ADS - HORTFRUTI");
    printf("\n");

    printf("Digite o nome do produto para buscar: ");
    getchar();                               // Limpa o buffer do teclado
    fgets(nomeBusca, 100, stdin);            // Lê o nome do produto
    nomeBusca[strcspn(nomeBusca, "\n")] = 0; // Remove o '\n' final

    int encontrou = 0; // Flag para verificar se algum produto foi encontrado
    for (int i = 0; i < qtdProdutos; i++)
    {
        if (strstr(produtos[i].nome, nomeBusca) != NULL) // Verifica se o nome buscado está contido no nome do produto
        {
            // Exibe o produto encontrado com base na categoria
            printf("Produto encontrado: ID: %d | Nome: %s | Categoria: %s | Preço Unitário: R$ %.2f\n",
                   produtos[i].id, produtos[i].nome, produtos[i].categoria, produtos[i].preco); // Exibe preço unitário

            if (strcmp(produtos[i].categoria, "Líquidos") == 0) // Caso o produto seja da categoria 'Líquidos'
            {
                printf("Preço(Fardo): R$ %.2f\n", produtos[i].precodoFardo); // Exibe o preço do fardo
            }
            else if (strcmp(produtos[i].categoria, "Outros") == 0) // Caso o produto seja da categoria 'Outros'
            {
                printf("Preço(Caixa): R$ %.2f\n", produtos[i].precoCaixa); // Exibe o preço da caixa
            }
            else // Para as outras categorias, exibe o preço por kg
            {
                printf("Preço(kg): R$ %.2f\n", produtos[i].precoporkg); // Exibe o preço por kg
            }
            encontrou = 1; // Produto encontrado
        }
    }

    // Caso nenhum produto seja encontrado
    if (!encontrou)
    {
        printf("\nNenhum produto com o nome '%s' encontrado!\n", nomeBusca);
    }
}
void AtualizarProduto(ListadeProdutos produtos[], int qtdProdutos) // Função para atualizar um produto pelo ID
{
    system(LIMPAR_TELA); // Limpa a tela ao selecionar uma opção

    int id;
    printf("\n");
    centralizarTexto("--------------------");
    centralizarTexto("| ATUALIZAR PRODUTO |");
    centralizarTexto("--------------------");
    centralizarTexto("ADS - HORTFRUTI");
    printf("\n");

    printf("Digite o ID do produto a ser atualizado: ");
    scanf("%d", &id);

    for (int i = 0; i < qtdProdutos; i++)
    {
        if (produtos[i].id == id)
        {
            printf("\nProduto encontrado:\n");
            printf("ID: %d\n", produtos[i].id);
            printf("Nome: %s\n", produtos[i].nome);
            printf("Preço unitário: R$ %.2f\n", produtos[i].preco);
            printf("Preço por kg: R$ %.2f\n", produtos[i].precoporkg);
            printf("Preço do fardo: R$ %.2f\n", produtos[i].precodoFardo);
            printf("Preço da caixa: R$ %.2f\n", produtos[i].precoCaixa);
            printf("Categoria: %s\n", produtos[i].categoria);

            // Atualização dos dados do produto
            printf("\nDigite os novos dados para o produto:\n");

            // Atualiza nome do produto
            printf("Novo nome do produto (ou pressione Enter para manter o atual): ");
            char novoNome[50];
            getchar(); // Limpa o buffer
            fgets(novoNome, 50, stdin);
            if (novoNome[0] != '\n') // Se uma nova entrada foi feita
            {
                novoNome[strcspn(novoNome, "\n")] = 0; // Remove o '\n' final
                strcpy(produtos[i].nome, novoNome);
            }

            // Atualiza o preço unitário
            printf("Novo preço unitário (atual: R$ %.2f): ", produtos[i].preco);
            float novoPreco;
            if (scanf("%f", &novoPreco) == 1)
            {
                produtos[i].preco = novoPreco;
            }

            // Atualiza o preço por kg, preço do fardo ou preço do caixa dependendo da categoria
            if (strcmp(produtos[i].categoria, "Líquidos") == 0)
            {
                printf("Novo preço do fardo (atual: R$ %.2f): ", produtos[i].precodoFardo);
                float novoPrecoFardo;
                if (scanf("%f", &novoPrecoFardo) == 1)
                {
                    produtos[i].precodoFardo = novoPrecoFardo;
                }
            }
            else if (strcmp(produtos[i].categoria, "Outros") == 0)
            {
                printf("Novo preço da caixa (atual: R$ %.2f): ", produtos[i].precoCaixa);
                float novoPrecoCaixa;
                if (scanf("%f", &novoPrecoCaixa) == 1)
                {
                    produtos[i].precoCaixa = novoPrecoCaixa;
                }
            }
            else
            {
                printf("Novo preço por kg (atual: R$ %.2f): ", produtos[i].precoporkg);
                float novoPrecoPorKg;
                if (scanf("%f", &novoPrecoPorKg) == 1)
                {
                    produtos[i].precoporkg = novoPrecoPorKg;
                }
            }
            printf("\nProduto atualizado com sucesso!\n");
            return;
        }
    }
    printf("\nProduto com ID %d não encontrado!\n", id);
}
int GerarNovoIdProduto(ListadeProdutos produtos[], int qtdProdutos) // Função para gerar um novo ID sequencial para o produto
{
    if (qtdProdutos == 0)
        return 1;                            // Se o estoque estiver vazio, começa com o ID 1
    return produtos[qtdProdutos - 1].id + 1; // Gera um ID sequencial
}

void SalvarProdutos(ListadeProdutos produtos[], int qtdProdutos)
{
    FILE *arquivo = fopen("produtosarq.txt", "w");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo para salvar os produtos.\n");
        return;
    }

    for (int i = 0; i < qtdProdutos; i++)
    {
        fprintf(arquivo, "%d;%s;%.2f;%.2f;%.2f;%.2f;%s\n",
                produtos[i].id,
                produtos[i].nome,
                produtos[i].preco,
                produtos[i].precoporkg,
                produtos[i].precodoFardo,
                produtos[i].precoCaixa,
                produtos[i].categoria);
    }

    fclose(arquivo);
    printf("Produtos salvos com sucesso no arquivo 'produtos.txt'.\n");
}
int CarregarProdutos(ListadeProdutos produtos[], int *qtdProdutos)
{
    FILE *arquivo = fopen("produtosarq.txt", "r");
    if (arquivo == NULL)
    {
        printf("Nenhum arquivo de produtos encontrado. Será criado um novo.\n");
        *qtdProdutos = 0;
        return;
    }

    *qtdProdutos = 0;
    while (fscanf(arquivo, "%d;%[^;];%f;%f;%f;%f;%[^\n]\n",
                  &produtos[*qtdProdutos].id,
                  produtos[*qtdProdutos].nome,
                  &produtos[*qtdProdutos].preco,
                  &produtos[*qtdProdutos].precoporkg,
                  &produtos[*qtdProdutos].precodoFardo,
                  &produtos[*qtdProdutos].precoCaixa,
                  produtos[*qtdProdutos].categoria) == 7)
    {
        (*qtdProdutos)++;
    }

    fclose(arquivo);
  /*  printf("Produtos carregados do arquivo 'produtosarq.txt'.\n");*/
}

// Funções Estoque
void ListadeEstoque(ControleEstoque estoque[], int qtdEstoque) // Função para listar todos os produtos no estoque, separados por categoria
{
    system(LIMPAR_TELA);

    printf("\n");
    centralizarTexto("---------------------------------");
    centralizarTexto("| LISTA DO ESTOQUE - ALPHADEV'S |");
    centralizarTexto("---------------------------------");
    centralizarTexto("ADS - HORTFRUTI");
    printf("\n");

    char *categorias[] = {"Frutas", "Verduras", "Legumes", "Líquidos", "Grãos e Cereais", "Outros"};
    int numCategorias = sizeof(categorias) / sizeof(categorias[0]);

    for (int j = 0; j < numCategorias; j++)
    {
        bool encontrouCategoria = false;
        printf("\n%s:\n", categorias[j]);
        printf("ID    | Nome            | Quantidade | Data de Entrada | Validade (dias) | Categoria\n");
        printf("------------------------------------------------------------------------------------\n");

        for (int i = 0; i < qtdEstoque; i++)
        {
            if (strcmp(estoque[i].categoria, categorias[j]) == 0)
            {
                printf("%-5d | %-15s | %-10d | %02d/%02d/%04d | %-16d | %-10s\n",
                       estoque[i].id,
                       estoque[i].nome,
                       estoque[i].quantidades,
                       estoque[i].dataEntrada.tm_mday,
                       estoque[i].dataEntrada.tm_mon + 1,
                       estoque[i].dataEntrada.tm_year + 1900,
                       estoque[i].diasValidade,
                       estoque[i].categoria);
                encontrouCategoria = true;
            }
        }
        if (!encontrouCategoria)
        {
            printf("Nenhum produto encontrado nesta categoria.\n");
        }
    }
}
void AdicionarnoEstoque(ControleEstoque estoque[], int *qtdEstoque) // Função para adicionar um produto ao estoque
{
    system(LIMPAR_TELA);

    if (*qtdEstoque >= MAX_QUANTIDADE)
    {
        printf("Estoque cheio!\n");
        return;
    }

    ControleEstoque novoProduto;
    novoProduto.id = GerarNovoIdEstoque(estoque, *qtdEstoque);

    printf("\n");
    centralizarTexto("-------------------------");
    centralizarTexto("| ADICIONAR AO ESTOQUE  |");
    centralizarTexto("-------------------------");
    centralizarTexto("ADS - HORTFRUTI");
    printf("\n");

    printf("Nome do produto: ");
    getchar();
    fgets(novoProduto.nome, MAX_NOMEPRODESTOQUE, stdin);
    novoProduto.nome[strcspn(novoProduto.nome, "\n")] = 0;

    printf("Escolha a categoria do produto:\n");
    printf("1. Frutas\n2. Verduras\n3. Legumes\n4. Líquidos\n5. Grãos e Cereais\n6. Outros\n");
    printf("Digite a opção desejada: ");
    int opcaoCategoria;
    scanf("%d", &opcaoCategoria);

    switch (opcaoCategoria)
    {
    case 1:
        strcpy(novoProduto.categoria, "Frutas");
        break;
    case 2:
        strcpy(novoProduto.categoria, "Verduras");
        break;
    case 3:
        strcpy(novoProduto.categoria, "Legumes");
        break;
    case 4:
        strcpy(novoProduto.categoria, "Líquidos");
        break;
    case 5:
        strcpy(novoProduto.categoria, "Grãos e Cereais");
        break;
    case 6:
        strcpy(novoProduto.categoria, "Outros");
        break;
    default:
        printf("Categoria inválida. Atribuindo 'Outros'.\n");
        strcpy(novoProduto.categoria, "Outros");
        break;
    }

    printf("Quantidade do produto: ");
    scanf("%d", &novoProduto.quantidades);

    printf("Validade do produto em dias: ");
    scanf("%d", &novoProduto.diasValidade);

    time_t t = time(NULL);
    novoProduto.dataEntrada = *localtime(&t);

    struct tm dataValidade = novoProduto.dataEntrada;
    dataValidade.tm_mday += novoProduto.diasValidade;
    mktime(&dataValidade);

    estoque[*qtdEstoque] = novoProduto;
    (*qtdEstoque)++;

    printf("Produto adicionado ao estoque com sucesso!\n");
    printf("Data de validade: %02d/%02d/%04d\n",
           dataValidade.tm_mday,
           dataValidade.tm_mon + 1,
           dataValidade.tm_year + 1900);
}
void RemoverdoEstoque(ControleEstoque estoque[], int *qtdEstoque) // Função para remover um produto pelo ID
{
    system(LIMPAR_TELA);

    int id;
    printf("\n");
    centralizarTexto("-----------------------------");
    centralizarTexto("| REMOVER PRODUTO DO ESTOQUE |");
    centralizarTexto("-----------------------------");
    centralizarTexto("ADS - HORTFRUTI");
    printf("\n");

    printf("Digite o ID do produto a ser removido: ");
    scanf("%d", &id);

    for (int i = 0; i < *qtdEstoque; i++)
    {
        if (estoque[i].id == id)
        {
            for (int j = i; j < *qtdEstoque - 1; j++)
            {
                estoque[j] = estoque[j + 1];
                estoque[j].id = j + 1;
            }
            (*qtdEstoque)--;
            printf("Produto removido com sucesso!\n");
            return;
        }
    }
    printf("Produto com ID %d não encontrado!\n", id);
}
void BuscarPorID(ControleEstoque estoque[], int qtdEstoque) // Função para buscar um produto pelo ID
{
    system(LIMPAR_TELA); // Limpa a tela ao selecionar uma opção

    int id;
    printf("\n");
    centralizarTexto("----------------------------");
    centralizarTexto("| BUSCAR PRODUTO NO ESTOQUE |");
    centralizarTexto("----------------------------");
    centralizarTexto("ADS - HORTFRUTI");
    printf("\n");

    printf("Digite o ID do produto para buscar: ");
    scanf("%d", &id);

    for (int i = 0; i < qtdEstoque; i++)
    {
        if (estoque[i].id == id)
        {
            printf("Produto encontrado: ID: %d | Nome: %s    | Quantidade: %d | Data de Entrada: %02d/%02d/%04d | Validade: %d dias | Categoria: %s\n",
                   estoque[i].id, estoque[i].nome, estoque[i].quantidades,
                   estoque[i].dataEntrada.tm_mday, estoque[i].dataEntrada.tm_mon + 1, estoque[i].dataEntrada.tm_year + 1900,
                   estoque[i].diasValidade, estoque[i].categoria); // Exibe a categoria
            return;
        }
    }
    printf("Produto com ID %d nao encontrado!\n", id);
}
void AtualizarProdutoEstoque(ControleEstoque estoque[], int qtdEstoque) // Função para atualizar um produto pelo ID
{
    system(LIMPAR_TELA); // Limpa a tela ao selecionar uma opção

    int id;
    printf("\n");
    centralizarTexto("-----------------------------");
    centralizarTexto("| ATUALIZAR PRODUTO ESTOQUE |");
    centralizarTexto("-----------------------------");
    centralizarTexto("ADS - HORTFRUTI");
    printf("\n");

    printf("Digite o ID do produto a ser atualizado: ");
    scanf("%d", &id);

    for (int i = 0; i < qtdEstoque; i++)
    {
        if (estoque[i].id == id)
        {
            printf("\nProduto encontrado:\n");
            printf("ID: %d\n", estoque[i].id);
            printf("Nome: %s\n", estoque[i].nome);
            printf("Quantidade: %d\n", estoque[i].quantidades);
            printf("Validade: %d dias\n", estoque[i].diasValidade);

            // Atualização dos dados do produto
            printf("\nDigite os novos dados para o produto:\n");

            printf("Novo nome do produto: ");
            getchar(); // Limpa o buffer
            fgets(estoque[i].nome, MAX_NOMEPRODESTOQUE, stdin);
            estoque[i].nome[strcspn(estoque[i].nome, "\n")] = 0; // Remove o caractere de nova linha

            printf("Nova quantidade do produto: ");
            scanf("%d", &estoque[i].quantidades);

            printf("Nova validade (em dias): ");
            scanf("%d", &estoque[i].diasValidade);

            printf("\nProduto atualizado com sucesso!\n");
            return;
        }
    }
    printf("\nProduto com ID %d não encontrado!\n", id);
}
int GerarNovoIdEstoque(ControleEstoque estoque[], int qtdEstoque) // Função para gerar um novo ID sequencial para o produto
{
    if (qtdEstoque == 0)
        return 1;                          // Se o estoque estiver vazio, começa com o ID 1
    return estoque[qtdEstoque - 1].id + 1; // Gera um ID sequencial
}
void SalvarEstoque(ControleEstoque estoque[], int qtdEstoque) // Função para salvar o estoque em um arquivo
{
    FILE *file = fopen(FILENAMEESTOQUE, "w");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo de estoque para salvamento!\n");
        return;
    }

    for (int i = 0; i < qtdEstoque; i++)
    {
        fprintf(file, "ID: %d| NOME: %s| QUANTIDADE: %d| DATA DE ENTRADA: %02d/%02d/%04d| VALIDADE (EM DIAS): %d| CATEGORIA: %s\n",
                estoque[i].id,
                estoque[i].nome,
                estoque[i].quantidades,
                estoque[i].dataEntrada.tm_mday,
                estoque[i].dataEntrada.tm_mon + 1,
                estoque[i].dataEntrada.tm_year + 1900,
                estoque[i].diasValidade,
                estoque[i].categoria);
    }
    fclose(file);
    printf("Estoque salvo com sucesso!\n");
}
int CarregarEstoque(ControleEstoque estoque[], int *qtdEstoque) // Função para carregar o estoque do arquivo
{
    FILE *file = fopen(FILENAMEESTOQUE, "r");
    if (file == NULL)
    {
        printf("Não foi possível abrir o arquivo para leitura. Criando um novo arquivo.\n");
        return;
    }

    while (fscanf(file, "ID: %d| NOME: %[^|]| QUANTIDADE: %d| DATA DE ENTRADA: %d/%d/%d| VALIDADE (EM DIAS): %d| CATEGORIA: %[^\n]\n",
                  &estoque[*qtdEstoque].id,
                  estoque[*qtdEstoque].nome,
                  &estoque[*qtdEstoque].quantidades,
                  &estoque[*qtdEstoque].dataEntrada.tm_mday,
                  &estoque[*qtdEstoque].dataEntrada.tm_mon,
                  &estoque[*qtdEstoque].dataEntrada.tm_year,
                  &estoque[*qtdEstoque].diasValidade,
                  estoque[*qtdEstoque].categoria) == 8)
    {
        estoque[*qtdEstoque].dataEntrada.tm_year -= 1900;
        estoque[*qtdEstoque].dataEntrada.tm_mon -= 1;
        (*qtdEstoque)++;
    }

    fclose(file);
}

// Função para registrar a venda em um arquivo
void RegistrarVenda(Venda venda)
{
    FILE *arquivo = fopen(FILENAMEVENDAS, "a");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de registro de vendas.\n");
        return;
    }

    // Atribui um novo ID à venda
    venda.idVenda = GerarNovoIdVenda();

    // Calcula o total da venda
    float totalCompra = 0.0;
    for (int i = 0; i < venda.quantidadeItens; i++)
    {
        totalCompra += venda.produtosVendidos[i].preco * venda.produtosVendidos[i].quantidade;
    }
    venda.totalCompra = totalCompra;

    // Registro da venda no arquivo
    fprintf(arquivo, "ID da Venda: %d\n", venda.idVenda);
    fprintf(arquivo, "Data: %02d/%02d/%04d %02d:%02d\n",
            venda.dataVenda.tm_mday, venda.dataVenda.tm_mon + 1, venda.dataVenda.tm_year + 1900,
            venda.dataVenda.tm_hour, venda.dataVenda.tm_min);

    fprintf(arquivo, "Itens da venda:\n");
    for (int i = 0; i < venda.quantidadeItens; i++)
    {
        fprintf(arquivo, "Produto: %s | Quantidade: %d | Preço Unitário: R$ %.2f | Subtotal: R$ %.2f\n",
                venda.produtosVendidos[i].nome,
                venda.produtosVendidos[i].quantidade,
                venda.produtosVendidos[i].preco,
                venda.produtosVendidos[i].preco * venda.produtosVendidos[i].quantidade);
    }

    fprintf(arquivo, "Total da compra: R$ %.2f\n", totalCompra);
    fprintf(arquivo, "Forma de pagamento: %s | Valor pago: R$ %.2f | Troco: R$ %.2f\n",
            venda.formaPagamento, venda.valorPago, venda.troco);
    fprintf(arquivo, "----------------------------------------\n");

    fclose(arquivo);
    printf("Venda registrada com sucesso! ID da Venda: %d\n", venda.idVenda);
}
void ListadasVendas()
{
    system("cls"); // Use "clear" no Linux
    printf("\n");
    centralizarTexto("---------------------------------");
    centralizarTexto("| RELATÓRIO DE VENDAS - ALPHADEV'S |");
    centralizarTexto("---------------------------------");
    centralizarTexto("ADS - HORTFRUTI");
    printf("\n");

    FILE *arquivo = fopen(FILENAMEVENDAS, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de registro de vendas.\n");
        return;
    }

    struct tm dataVenda;
    float totalCompra;
    char linha[256], formaPagamento[30];
    char mesAno[8], mesAnoAnterior[8] = "";
    int idVenda;

    centralizarTexto("--------------------------------------------------------");
    centralizarTexto("| ID da Venda | Data     | Total   | Forma de Pagamento |");
    centralizarTexto("--------------------------------------------------------");

    while (fgets(linha, sizeof(linha), arquivo))
    {
        if (strstr(linha, "ID da Venda:"))
        {
            sscanf(linha, "ID da Venda: %d", &idVenda);
        }

        if (strstr(linha, "Data:"))
        {
            sscanf(linha, "Data: %02d/%02d/%04d %02d:%02d",
                   &dataVenda.tm_mday, &dataVenda.tm_mon, &dataVenda.tm_year,
                   &dataVenda.tm_hour, &dataVenda.tm_min);
            dataVenda.tm_mon -= 1;     // Ajustar mês
            dataVenda.tm_year -= 1900; // Ajustar ano

            // Exibe o mês e ano da venda
            strftime(mesAno, sizeof(mesAno), "%Y-%m", &dataVenda);

            if (strcmp(mesAno, mesAnoAnterior) != 0)
            {
                printf("\n--- Mês: %s ---\n", mesAno);
                strcpy(mesAnoAnterior, mesAno);
            }
        }

        if (strstr(linha, "Total da compra:"))
        {
            sscanf(linha, "Total da compra: R$ %f", &totalCompra);
        }

        if (strstr(linha, "Forma de pagamento:"))
        {
            sscanf(linha, "Forma de pagamento: %s", formaPagamento);
            printf("%-12d | %02d/%02d/%04d | R$ %.2f | %s\n",
                   idVenda,
                   dataVenda.tm_mday,
                   dataVenda.tm_mon + 1,
                   dataVenda.tm_year + 1900,
                   totalCompra,
                   formaPagamento);
        }
    }

    fclose(arquivo);
}
int GerarNovoIdVenda()
{
    int ultimoId = 0;
    char linha[256];

    FILE *arquivo = fopen(FILENAMEVENDAS, "r");
    if (arquivo != NULL)
    {
        while (fgets(linha, sizeof(linha), arquivo))
        {
            if (strstr(linha, "ID da Venda:"))
            {
                sscanf(linha, "ID da Venda: %d", &ultimoId);
            }
        }
        fclose(arquivo);
    }

    return ultimoId + 1; // Incrementa o último ID encontrado
}

// Função principal
int main()
{
    setlocale(LC_ALL, "Portuguese_Brazil.1252");

    Menu(); // Exibe o menu de boas-vindas antes do menu principal
    return 0;
}