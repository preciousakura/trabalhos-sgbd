#include <bits/stdc++.h>

using namespace std;

class Tupla {
    private:
        vector<string> cols;
    public:
        Tupla() {};
        void set_tupla(string cols_cpy) {
            cols.push_back(cols_cpy);
        }

        vector<string> get_cols() { return cols; }
        
        bool comparar_tupla(Tupla t) {
            vector<string> t_str = t.get_cols();
            int i;
            for(i = 0; i < cols.size(); i++) 
                if(cols[i] != t_str[i]) return false;
            return true;
        }
};