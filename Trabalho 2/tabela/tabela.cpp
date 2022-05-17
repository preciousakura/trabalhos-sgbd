#include <bits/stdc++.h>
#include <string>
#include "../hash_extensivel/hash.cpp"
using namespace std;

template <typename TypeTable>
class Tabela {
    private: 
        typedef struct Esquema
        {
            int qtd_cols;
            Hash<TypeTable> indice;
        } Esquema;


        typedef struct Pagina 
        {
            string *cols;
        } Pagina;

        Pagina *pags;
        int qtd_pags;
        Esquema esquema;
        string file_name;
        
    public:
    	Tabela(string arquivo)
	    {
            if(arquivo == "vinho.csv") 
            {
                Hash<Vinho> vinhos(2, "vinhos");
                string colunas[5] = {"vinho_id", "rotulo", "ano_producao", "uva_id", "pais_producao_id"};
                esquema.qtd_cols = 5;
                esquema.indice = vinhos;
                file_name = "../vinho.csv";
            }
            else if(arquivo == "uva.csv")
            {
                Hash<Vinho> uvas(2, "uvas");
                string colunas[5] = {"uva_id","nome","tipo","ano_colheita","pais_origem_id"};
                esquema.qtd_cols = 5;
                esquema.indice = uvas;
                file_name = "../uva.csv";
            }
            else if(arquivo == "pais.csv")
            {
                Hash<Vinho> pais(2, "pais");
                string colunas[3] = {"pais_id","nome","sigla"};
                esquema.qtd_cols = 3;
                esquema.indice = pais;
                file_name = "../pais.csv";
            }
	    }

        void carregarDados() {
            esquema.indice.insert(file_name);
        }
};

