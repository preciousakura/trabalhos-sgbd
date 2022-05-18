#include <iostream>
#include <string>
#include "tabela.cpp"
#include "operador.cpp"

using namespace std;

int main() {
    Tabela<Vinho> vinho {"vinho.csv"}; // cria estrutura necessaria para a tabela
    Tabela<Uva> uva {"uva.csv"};
    Tabela<Pais> pais {"pais.csv"};

    vinho.carregarDados(); // le os dados do csv e add na estrutura da tabela, caso necessario
    uva.carregarDados();
    pais.carregarDados();

    Operador<Vinho> op(vinho, new string[2] {"uva_id", "rotulo"});

    op.executar(); // Realiza a operacao desejada

    cout << "#Pags: " << op.numPagsGeradas(); // Retorna a quantidade de paginas geradas pela operacao
    cout << "\n#IOss: " << op.numIOExecutados(); // Retorna a quantidade de IOs geradas pela operacao
    cout << "\n#Tups: " << op.numTuplasGeradas(); // Retorna a quantidade de tuplas geradas pela operacao

    op.salvarTuplasGeradas("selecao_vinho_ano_colheita_1990.csv"); // Retorna as tuplas geradas pela operacao e salva em um csv

    return 0;
}