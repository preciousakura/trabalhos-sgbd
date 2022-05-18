#include "hash.cpp"

template <typename TypeTable>
class Tabela {
    private: 
        typedef struct Esquema
        {
            Hash<TypeTable> indice;
        } Esquema;

        int qtd_pags;
        Esquema esquema;
        
        string file_name;
        
    public:
        Tabela() {}

    	Tabela(string arquivo)
	    {
            if(arquivo == "vinho.csv") 
            {
                file_name = arquivo;
                esquema.indice.init(2, "vinhos");
            }
            else if(arquivo == "uva.csv")
            {
                file_name = arquivo;
                esquema.indice.init(2, "uvas");

            }
            else if(arquivo == "pais.csv")
            {
                file_name = arquivo;
                esquema.indice.init(2, "pais");
            }
	    };

    void carregarDados() {
        esquema.indice.insert(file_name);
    }

    string get_tuplas() {
        return esquema.indice.get_tuplas();
    }
};

