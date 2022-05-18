template <typename TypeTable>
class Operador {
    private:
        Tabela<TypeTable> *tabela;
        string* colunas;
    public:
    Operador(Tabela<TypeTable> & table, string *cols) 
    {
        tabela = &table;
    };

    void executar() {
        cout << tabela->get_tuplas();
    }

    void salvarTuplasGeradas(string file_name) {

    }

    int numPagsGeradas() {

    } 

    int numIOExecutados() {

    }

    int numTuplasGeradas() {

    }
};