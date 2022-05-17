#include <iostream>
#include <string>
#include "./tabela/tabela.cpp"

// VOCE DEVE INCLUIR SEUS HEADERS AQUI, CASO NECESSARIO!!!

using namespace std;

int main() {
    Tabela<Vinho> vinho {"vinho.csv"}; // cria estrutura necessaria para a tabela
    Tabela<Uva> uva {"uva.csv"};
    Tabela<Pais> pais {"pais.csv"};

    vinho.carregarDados(); // le os dados do csv e add na estrutura da tabela, caso necessario
    uva.carregarDados();
    pais.carregarDados();


    // //// DESCOMENTE A PROXIMA LINHA CASO SEU TRABALHO SEJA PROJECAO:
    // Operador op {vinho, new string[2] {"uva_id", "rotulo"}};
    // //// significa: SELECT uva_id, rotulo FROM Vinho
    // //// IMPORTANTE: isso eh so um exemplo, pode ser outra tabela e ter mais ou menos colunas.
    // //// genericamente: Operador(tabela, lista_colunas_proj):
    // //// significa: SELECT col_1, col_2, ..., col_n FROM tabela

    // op.executar(); // Realiza a operacao desejada

    // cout << "#Pags: " << op.numPagsGeradas(); // Retorna a quantidade de paginas geradas pela operacao
    // cout << "\n#IOss: " << op.numIOExecutados(); // Retorna a quantidade de IOs geradas pela operacao
    // cout << "\n#Tups: " << op.numTuplasGeradas(); // Retorna a quantidade de tuplas geradas pela operacao

    // op.salvarTuplasGeradas("selecao_vinho_ano_colheita_1990.csv"); // Retorna as tuplas geradas pela operacao e salva em um csv

    return 0;
}