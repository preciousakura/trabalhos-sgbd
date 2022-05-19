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
        int numPagsGeradas() { return tabela.get_num_pages(); }
        int numIOExecutados() { return tabela.get_io(); }
        int numTuplasGeradas() { return tabela.get_num_tuplas(); }
        void salvarTuplasGeradas(string fn) {
            fstream out_arquivo;
            string text;
            out_arquivo.open(fn, ios::out);
            for(int i = 0; i < proj_cols.size(); i++) {
                text+= proj_cols[i];
                if(i != proj_cols.size() - 1) text+= ',';
            }
            out_arquivo << text << endl;
            out_arquivo.close();
            tabela.save_on_file(fn);
        }
};