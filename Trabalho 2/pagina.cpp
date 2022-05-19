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

        vector<Tupla> get_tuplas() {
            return tuplas;
        }
};