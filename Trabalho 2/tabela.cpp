#include<direct.h>

#include "pagina.cpp"
#include "esquema.cpp"

#define MAXPAGS 12
#define NUM_PARTICOES 10
class Tabela {
    private:
        vector<Pagina> pags;
        Esquema esquema;
        string type_table_name;
        int qtd_pags = 0, io = 0;

        typedef struct Hash {
            int qtd_paginas = 0;
            int qtd_elementos = 0;
        } Hash;

        Hash h[NUM_PARTICOES];

    public:

        Tabela() {};
        Tabela(string typeEsquema) {
            string campo = "";
            for(char &ch:typeEsquema){
                if(ch == '.'){
                    type_table_name = campo;
                    break;
                }else{
                    campo += ch;
                }
            }
            mkdir(type_table_name.c_str());
        };

        void carregarDados() {
            fstream arquivo, atual_page;
            arquivo.open(type_table_name+".csv", ios_base::in);

            string line;
		    getline(arquivo, line); // pegar primeira linha do arquivo
            esquema.set_cols_names(esquema.read_line(line));
            esquema.set_index();

            int num_tuplas = 0;

            atual_page.open(type_table_name+"/pagina"+to_string(qtd_pags)+".txt", ios_base::out);
            qtd_pags++;

            while (getline(arquivo, line))
            {
                atual_page << line << endl;
                num_tuplas++;

                if(num_tuplas == MAXPAGS) {
                    atual_page.close();
                    atual_page.open(type_table_name+"/pagina"+to_string(qtd_pags)+".txt", ios_base::out);
                    num_tuplas = 0, qtd_pags++;
                }
            }
            if(atual_page.is_open()) atual_page.close();
            arquivo.close();
        };

        int f_hash(vector<string> cols) {
            hash<string> str_hash;
            int key = 0;
            for(string c_proj : cols) key += str_hash(c_proj);
            return abs(key%NUM_PARTICOES); 
        }

        void particionamento(vector<string> cols) {
            mkdir(("buckets_"+type_table_name).c_str());

            fstream arquivo, atual_page;
            vector<string> col_values;
            string line;
            for(int i = 0; i < qtd_pags; i++) { // primeira fase de particionamento
                arquivo.open(type_table_name+"/pagina"+to_string(i)+".txt", ios_base::in);
                Pagina page_atual;
                while(getline(arquivo, line)) {
                    Tupla t;
                    col_values = esquema.read_line(line);
                    for(string c_proj : cols) t.set_tupla(col_values[esquema.get_index_col(c_proj)]);
                    page_atual.set_tupla(t);
                }
                vector<Tupla> tuplas = page_atual.get_tuplas();
                for(Tupla t : tuplas) {
                    vector<string> colunas = t.get_cols();

                    int h_index = f_hash(colunas);
                    if(h[h_index].qtd_elementos == MAXTUPLAS) {
                        h[h_index].qtd_elementos = 0;
                        h[h_index].qtd_paginas++;
                    }
                    if(h[h_index].qtd_elementos != 0) atual_page.open("buckets_"+type_table_name+"/bucket_"+to_string(h_index)+"_pagina_"+to_string(h[h_index].qtd_paginas)+".txt", std::ios_base::app);
                    else atual_page.open("buckets_"+type_table_name+"/bucket_"+to_string(h_index)+"_pagina_"+to_string(h[h_index].qtd_paginas)+".txt", std::ios_base::out);
                    
                    string text;
                    for(int k = 0; k < colunas.size(); k++) {
                        text += colunas[k];
                        if(k != colunas.size() - 1) text += ',';
                    }
                    atual_page << text << endl;
                    atual_page.close();
                    h[h_index].qtd_elementos++;
                } 
                ++io;
                arquivo.close();
            }
        }

        void remover_duplicatas() {
            fstream arquivo, atual_page;

            int i_page_entrada = 0,
                i_page_checagem = 0,
                i_page_saida = 0,
                num_elementos_saida = 0;

            string line;
            vector<string> col_values;

            for(int i = 0; i < NUM_PARTICOES; i++) { // percorre partições
                if(h[i].qtd_elementos == 0 && h[i].qtd_paginas == 0) // partição vazia
                    continue;
                
                if(num_elementos_saida == MAXTUPLAS) // entrar no modo checagem
                arquivo.open("buckets_"+type_table_name+"/bucket_"+to_string(i)+"_pagina_"+to_string(i_page_entrada)+".txt"); // arquivo de entrada
                while(getline(arquivo, line)) {
                    Tupla t;
                    col_values = esquema.read_line(line);
                    pags[0].set_tupla(t); // página de entrada
                }
                ++io;
                arquivo.close();
            }
        }

        int get_io() { return io; }
};