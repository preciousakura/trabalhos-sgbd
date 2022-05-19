#include "tabela.cpp"

class Operador {
    private:
        Tabela tabela;
        vector<string> proj_cols;
    public:
        Operador() {}
        Operador(Tabela tb, vector<string> cols) {
            tabela = tb;
            for(string c : cols) proj_cols.push_back(c);
        }
        void executar() {
            tabela.particionamento(proj_cols);
            tabela.remover_duplicatas();
        }
        int numPagsGeradas() { return 0; }
        int numIOExecutados() { return tabela.get_io(); }
        int numTuplasGeradas() { return 0; }
        void salvarTuplasGeradas(string fn) {}
};