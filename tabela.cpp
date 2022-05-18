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
                string colunas[5] = {"vinho_id", "rotulo", "ano_producao", "uva_id", "pais_producao_id"};
                esquema.qtd_cols = 5;
                file_name = arquivo;
                esquema.indice.init(2, "vinhos");
            }
            else if(arquivo == "uva.csv")
            {
                string colunas[5] = {"uva_id","nome","tipo","ano_colheita","pais_origem_id"};
                esquema.qtd_cols = 5;
                file_name = arquivo;
                esquema.indice.init(2, "uvas");
            }
            else if(arquivo == "pais.csv")
            {
                string colunas[3] = {"pais_id","nome","sigla"};
                esquema.qtd_cols = 3;
                file_name = arquivo;
                esquema.indice.init(2, "pais");
            }
	    };


};

