#include <utility>
#include <string>

using namespace std;

class Esquema {
    private:
        int qtd_cols = 0;
        vector<string> cols_names;
        unordered_map<string, int> cols;
    public:
        Esquema() {};
        void set_index() {
            int i = 0;
            for(string s : cols_names) {
                cols[s] = i;
                ++i;
            }
        }
        void set_cols_names(vector<string> cn) { 
            qtd_cols = cn.size();
            for(string x : cn) cols_names.push_back(x);
        }

        int get_index_col(string key) { return cols[key]; }
        
        vector<string> read_line(string &s){
            vector<string> campos;
            string campo = "";
            for(char &ch:s){
                if(ch == ','){
                    campos.push_back(campo);
                    campo = "";
                }else{
                    campo += ch;
                }
            }
            campos.push_back(campo);
            return campos;
        }
        
};