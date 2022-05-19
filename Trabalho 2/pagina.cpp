#include "tupla.cpp"
#define MAXTUPLAS 12

class Pagina {
    private:
        vector<Tupla> tuplas;
        int qtd_tuplas_ocup = 0;
    public:
        Pagina() {};
        void set_tupla(Tupla tupla) { 
            tuplas.push_back(tupla);
            ++qtd_tuplas_ocup;
        }

        void zerar_pagina() {
            tuplas.clear();
            qtd_tuplas_ocup = 0;
        }

        bool page_full() {
            return qtd_tuplas_ocup == MAXTUPLAS;
        }

        vector<Tupla> get_tuplas() {
            return tuplas;
        }

        Tupla get_tupla_id(int key) {
            return tuplas[key];
        }

        bool has_tupla(Tupla f_tupla) {
            for(Tupla t : tuplas) {
                if(t.comparar_tupla(f_tupla)) return true;
            } return false;
        }

        void ver_tuplas() {
            for(Tupla t : tuplas) {
                vector<string> colunas = t.get_cols();
                for(string str : colunas) cout << str << ' ';
                cout << '/';
            }
        }
};