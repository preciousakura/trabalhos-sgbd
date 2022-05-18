#include <string>
#include <bits/stdc++.h>
#include <vector>

using namespace std;

typedef struct Vinho
{
    int chave;
    string vinho_id, rotulo, uva_id, pais_producao_id;
    vector<string> cols = {"vinho_id", "rotulo", "ano_producao", "uva_id", "pais_origem_id"};

    void create(string v_id, string r, int ano, string u_id, string p_id)
    {
        vinho_id = v_id;
        rotulo = r;
        chave = ano;
        uva_id = u_id;
        pais_producao_id = p_id;
    }

    vector<int> get_index(vector<string> proj_cols) {
        vector<int> index;
        for (int i = 0; i < proj_cols.size(); i++) {
           for (int j = 0; j < cols.size(); j++) {
               if(proj_cols[i] == cols[j]) {
                   index.push_back(i);
                   continue;
               }
           }
        }
        return index;
    }

    vector<string> get_values_projection(vector<string> proj_cols, string fn) {
        fstream arquivo;

        string value;

        vector<string> proj_values;
        vector<int> ind = get_index(proj_cols);

        arquivo.open(fn, ios_base::in);
        string vinho_id,rotulo,ano_producao,uva_id,pais_producao_id;
        while (arquivo.good())
        {
            getline(arquivo, vinho_id, ',');
            if(vinho_id == "") break;

            getline(arquivo, rotulo, ',');
            getline(arquivo, ano_producao, ',');
            getline(arquivo, uva_id, ',');
            getline(arquivo, pais_producao_id);

            for(int i = 0; i < ind.size(); i++) {
                if(ind[i] == 0) value =  vinho_id;
                else if(ind[i] == 1) value += rotulo;
                else if(ind[i] == 2) value += ano_producao;
                else if(ind[i] == 3) value += uva_id;
                else value += pais_producao_id;

                if(i != ind.size() - 1) value += ",";
            }
            
            proj_values.push_back(value);
        }

        arquivo.close();
        return proj_values;
    }

    Vinho read(string fn, int i, string *str_change) {
		Vinho novo;
        
        fstream arquivo;
        arquivo.open(fn, ios_base::in);

        string vinho_id,rotulo,ano_producao,uva_id,pais_producao_id;

        int j = 0;
        while (arquivo.good())
        {
            getline(arquivo, vinho_id, ',');
            getline(arquivo, rotulo, ',');
            getline(arquivo, ano_producao, ',');
            getline(arquivo, uva_id, ',');
            getline(arquivo, pais_producao_id);

            if(ano_producao == "") {
                novo.chave = -1;
                break;
            }
            else if (j == i)
            {   
                *(str_change) = vinho_id + ',' + rotulo + ',' + ano_producao + ',' + uva_id + ',' + pais_producao_id + '\n';
                novo.create(vinho_id,rotulo,stoi(ano_producao),uva_id,pais_producao_id);
                break;
            } 
            j++;
        }

        arquivo.close();
        return novo;
    }

    string get_cols(Vinho v) {
        string text(v.vinho_id + "," + v.rotulo + "," + to_string(v.chave) + "," + v.uva_id + ',' + v.pais_producao_id);
        return text;
    }

    Vinho read_csv(istream & csv_file) {
        Vinho novo;
        string vinho_id,rotulo,ano_producao,uva_id,pais_producao_id;

        getline(csv_file, vinho_id, ',');
        getline(csv_file, rotulo, ',');
        getline(csv_file, ano_producao, ',');
        getline(csv_file, uva_id, ',');
        getline(csv_file, pais_producao_id);

        if (ano_producao != "") {
            novo.create(vinho_id, rotulo, stoi(ano_producao), uva_id, pais_producao_id);
            return novo;
        }
        novo.chave = -1;
        return novo;
    }
} Vinho;

typedef struct Uva
{
    int chave;
    string uva_id, nome, tipo, pais_origem_id;
    vector<string> cols = {"uva_id", "nome", "tipo", "ano_colheita", "pais_origem_id"};

    void create(string u_id, string nome_uva, string tipo_uva, int ano_colheita_uva, string pais_origem_id_uva)
    {
        uva_id = u_id;
        nome = nome_uva;
        tipo = tipo_uva;
        chave = ano_colheita_uva;
        pais_origem_id = pais_origem_id_uva;
    }

    vector<int> get_index(vector<string> proj_cols) {
        vector<int> index;
        for (int i = 0; i < proj_cols.size(); i++) {
           for (int j = 0; j < cols.size(); j++) {
               if(proj_cols[i] == cols[j]) {
                   index.push_back(i);
                   continue;
               }
           }
        }
        return index;
    }

    vector<string> get_values_projection(vector<string> proj_cols, string fn) {
        fstream arquivo;

        string value;

        vector<string> proj_values;
        vector<int> ind = get_index(proj_cols);

        arquivo.open(fn, ios_base::in);
        string u_id, nome_uva, tipo_uva, ano_colheita_uva, pais_origem_id_uva;

        while (arquivo.good())
        {
            getline(arquivo, u_id, ',');
            if(u_id == "") break;

            getline(arquivo, nome_uva, ',');
            getline(arquivo, tipo_uva, ',');
            getline(arquivo, ano_colheita_uva, ',');
            getline(arquivo, pais_origem_id_uva);

            for(int i = 0; i < ind.size(); i++) {
                if(ind[i] == 0) value =  u_id;
                else if(ind[i] == 1) value += nome_uva;
                else if(ind[i] == 2) value += tipo_uva;
                else if(ind[i] == 3) value += ano_colheita_uva;
                else value += pais_origem_id_uva;

                if(i != ind.size() - 1) value += ",";
            }
            proj_values.push_back(value);
        }

        arquivo.close();
        return proj_values;
    }

    Uva read(string fn, int i, string *str_change) {
        Uva novo;
        fstream arquivo;
        arquivo.open(fn, ios_base::in);

        string u_id, nome_uva, tipo_uva, ano_colheita_uva, pais_origem_id_uva;

        int j = 0;
        while (arquivo.good())
        {
            getline(arquivo, u_id, ',');
            getline(arquivo, nome_uva, ',');
            getline(arquivo, tipo_uva, ',');
            getline(arquivo, ano_colheita_uva, ',');
            getline(arquivo, pais_origem_id_uva);

            if(ano_colheita_uva == "") {
                novo.chave = -1;
                break;
            }
            else if (j == i)
            {
                *(str_change) = u_id + ',' + tipo_uva + ',' + ano_colheita_uva + ',' + ano_colheita_uva + ',' + pais_origem_id_uva + '\n';
                novo.create(u_id, nome_uva, tipo_uva, stoi(ano_colheita_uva),pais_origem_id_uva);
                break;
            }
            j++;
        }

        arquivo.close();
        return novo;
    }

    string get_cols(Uva u) {
        string text(u.uva_id + "," + u.nome + "," + u.tipo + "," + to_string(u.chave) + ',' + u.pais_origem_id);
        return text;
    }

    Uva read_csv(istream & csv_file) {
        Uva novo;
        string u_id, nome_uva, tipo_uva, ano_colheita_uva, pais_origem_id_uva;

        getline(csv_file, u_id, ',');
        getline(csv_file, nome_uva, ',');
        getline(csv_file, tipo_uva, ',');
        getline(csv_file, ano_colheita_uva, ',');
        getline(csv_file, pais_origem_id_uva);

        if (ano_colheita_uva != "") {
            novo.create(u_id, nome_uva, tipo_uva, stoi(ano_colheita_uva), pais_origem_id_uva);
            return novo;
        }
        novo.chave = -1;
        return novo;
    }
} Uva;

typedef struct Pais
{
    int chave;
    string nome, sigla;
    vector<string> cols = {"pais_id","nome","sigla"};

    void create(int pais, string nome_pais, string sigla_pais)
    {
        chave = pais;
        nome = nome_pais;
        sigla = sigla_pais;
    }

    vector<int> get_index(vector<string> proj_cols) {
        vector<int> index;
        for (int i = 0; i < proj_cols.size(); i++) {
           for (int j = 0; j < cols.size(); j++) {
               if(proj_cols[i] == cols[j]) {
                   index.push_back(i);
                   continue;
               }
           }
        }
        return index;
    }

    vector<string> get_values_projection(vector<string> proj_cols, string fn) {
        fstream arquivo;

        string value;

        vector<string> proj_values;
        vector<int> ind = get_index(proj_cols);

        arquivo.open(fn, ios_base::in);
        string id_pais, nome_pais, sigla_pais;

        while (arquivo.good())
        {
            getline(arquivo, id_pais, ',');
            if(id_pais == "") break;

            getline(arquivo, nome_pais, ',');
            getline(arquivo, sigla_pais);

            for(int i = 0; i < ind.size(); i++) {
                if(ind[i] == 0) value =  id_pais;
                else if(ind[i] == 1) value += nome_pais;
                else if(ind[i] == 2) value += sigla_pais;

                if(i != ind.size() - 1) value += ",";
            }

            proj_values.push_back(value);
        }

        arquivo.close();
        return proj_values;
    }

    Pais read(string fn, int i, string *str_change) {
        Pais novo;
        fstream arquivo;
        arquivo.open(fn, ios_base::in);

        string id_pais, nome_pais, sigla_pais;

        int j = 0;
        while (arquivo.good())
        {
            getline(arquivo, id_pais, ',');
            getline(arquivo, nome_pais, ',');
            getline(arquivo, sigla_pais);

            if(id_pais == "") {
                novo.chave = -1;
                break;
            }
            else if (j == i)
            {
                *(str_change) = id_pais + ',' + nome_pais + ',' + sigla_pais + '\n';
                novo.create(stoi(id_pais), nome_pais, sigla_pais);
                break;
            }
            j++;
        }

        arquivo.close();
        return novo;
    }

    string get_cols(Pais p) {
        string text(to_string(p.chave) + "," + p.nome + "," + p.sigla);
        return text;
    }

    Pais read_csv(istream & csv_file) {
        Pais novo;
        string id_pais, nome_pais, sigla_pais;

        getline(csv_file, id_pais, ',');
        getline(csv_file, nome_pais, ',');
        getline(csv_file, sigla_pais);

        if (id_pais != "") {
            novo.create(stoi(id_pais), nome_pais, sigla_pais);
            return novo;
        }
        novo.chave = -1;
        return novo;
    }
} Pais;

