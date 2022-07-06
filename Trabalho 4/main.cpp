#include "gerenciador.cpp"

string tratar_dado(string dado) {
    string campo = "";
    for(char &ch:dado)
        if(ch != ']' && ch != '[' && ch != ' ') 
            campo += ch;
    return campo;
}

int main() {
    Gerenciador gerenciador;

    fstream arquivo;
    arquivo.open("arquivo.txt", std::ios_base::in);
    string id, timestamp, transacao, operacao, objeto, imgAnterior, imgPosterior;
    while(arquivo.good()) {
        getline(arquivo, id, '|');
        getline(arquivo, timestamp, '|');
        getline(arquivo, transacao, '|');
        getline(arquivo, operacao, '|');
        getline(arquivo, objeto, '|');
        getline(arquivo, imgAnterior, '|');
        getline(arquivo, imgPosterior);

        if(id == "") break;
        gerenciador.readTr(tratar_dado(id), tratar_dado(timestamp), tratar_dado(transacao), tratar_dado(operacao), tratar_dado(objeto), tratar_dado(imgAnterior),  tratar_dado(imgPosterior));
    }

    arquivo.close();

    gerenciador.redo();
    gerenciador.undo();
    gerenciador.rollback();
    gerenciador.mostrar_objeto();

    return 0;
}