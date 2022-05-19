#include<direct.h>

#include "pagina.cpp"
#include "esquema.cpp"

#define MAXPAGS 12
#define NUM_PARTICOES 10
class Tabela {
    private:
        Esquema esquema;
        string type_table_name;
        int qtd_pags = 0, io = 0, qtd_pags_saida = 0;

        typedef struct Hash {
            vector<int> h_id;
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

        int f_hash2(vector<string> cols) {
            hash<string> str_hash;
            int key = 0;
            for(string c_proj : cols) key += str_hash(c_proj)*3;
            return abs(key%7); 
        }

        void remover_duplicatas() {
            fstream arquivo, atual_page, check_page;

            string line;
            vector<string> col_values;
            int page_bucket = 0;

            for(int i = 0; i < NUM_PARTICOES; i++) { // percorre partições
                if(h[i].qtd_elementos == 0 && h[i].qtd_paginas == 0) // partição vazia
                    continue;

                page_bucket = 0;
                Pagina pagina_saida; // pags[1]
                for(int j = 0; j <= h[i].qtd_paginas; j++) { // percorre as páginas do bucket
                    Pagina pagina_entrada; // pags[0]

                    arquivo.open("buckets_"+type_table_name+"/bucket_"+to_string(i)+"_pagina_"+to_string(j)+".txt", ios_base::in); // arquivo de entrada
                    while(getline(arquivo, line)) {
                        Tupla t;
                        col_values = esquema.read_line(line);
                        for(string col : col_values) t.set_tupla(col); 
                        pagina_entrada.set_tupla(t);
                        
                    }

                    vector<Tupla> tuplas = pagina_entrada.get_tuplas(); // tuplas página de entrada

                    // criar novo hash
                    Hash hash_to_bucket[7];

                    int p = 0; // índice do elemento na página de entrada que está sendo lida
                    for(Tupla t : tuplas) {
                        vector<string> colunas = t.get_cols();
                        int h_index = f_hash2(colunas);

                        if(!pagina_saida.has_tupla(t)) {
                            pagina_saida.set_tupla(t);
                            hash_to_bucket[h_index].h_id.push_back(p);
                            hash_to_bucket[h_index].qtd_elementos++;
                        }

                        if(page_bucket > 0) { // comparar com páginas de saída já salvas em disco

                        }

                        if(pagina_saida.page_full())  {
                            atual_page.open("buckets_"+type_table_name+"/bucket_"+to_string(i)+"_pagina_"+to_string(page_bucket)+".txt", std::ios_base::out);
                            vector<Tupla> pagina_saida_tuplas = pagina_saida.get_tuplas();
                            for(Tupla pagina_saida_t : pagina_saida_tuplas) {
                                vector<string> pagina_saida_colunas = pagina_saida_t.get_cols();
                                string text;
                                for(int k = 0; k < pagina_saida_colunas.size(); k++) {
                                    text += pagina_saida_colunas[k];
                                    if(k != pagina_saida_colunas.size() - 1) text += ',';
                                }
                                atual_page << text << endl;
                                
                            }
                            atual_page.close();
                            ++io;
                            ++qtd_pags_saida;
                            ++page_bucket;
                            pagina_saida.zerar_pagina(); // pags[1] zera a página de saída;
                        }
                            
                        p++;
                    } 
                    ++io;
                    arquivo.close();
                }
                pagina_saida.ver_tuplas();
                cout << endl;
            }
        }

        int get_io() { return io; }
};