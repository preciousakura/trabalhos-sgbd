#include <bits/stdc++.h>
using namespace std;

typedef struct Tr {
    
    string id; // LSN
    string timestamp;
    string transacao; // transação T1, T2, T3
    string operacao;
    string objeto;
    string imgAnterior;
    string imgPosterior;
    string status; // saber se é ativa ou não ativa

}Tr;