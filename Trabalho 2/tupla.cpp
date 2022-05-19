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
};