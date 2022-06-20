#include "tr_manager.cpp"

DataFile read_file(string &line) {
    DataFile df;
    
   for(int i = 0; i < line.size(); i++) {
    if(line[i] == 'C') {
        df.type_op = "C";
        df.id_tr = line[i + 2];
        break;
    }
    else if(line[i] == 'B') {
        df.type_op = "BT";
        df.id_tr = line[i + 3];
        break;
    } 
    else if(line[i] == 'r' || line[i] == 'w') {
        if(line[i] == 'r') df.type_op = "r";
        else df.type_op = "w";

        df.id_tr = line[i + 1];
        df.file = line[i + 3];
        break;
    } 
   }
   return df;
}

int main() {
    Tr_Manager tr_manager;

    fstream arquivo;
    arquivo.open("in.txt", std::ios_base::in);

    string line;
    while(arquivo.good()) {
        getline(arquivo, line);
        if(line == "") break;
        DataFile df = read_file(line);
        cout << df.type_op << ',' << df.file << ',' << df.id_tr << endl;
        tr_manager.readType(df.type_op, df.file, df.id_tr);
    }
    arquivo.close();

    return 0;
}