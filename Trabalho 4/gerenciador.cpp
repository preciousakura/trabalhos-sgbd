#include "types.h"

class Gerenciador {
    public:
        fstream out;
        vector<Tr> transacoes;
        vector<Object> objetos;

        Gerenciador() {
            out.open("out.txt", std::ios_base::out);
            out.close();
        }

        void readTr(string id, string timestamp, string transacao, string operacao, string objeto, string imgAnterior, string imgPosterior) {
            Tr trs;

            trs.id = id;
            trs.timestamp = timestamp;
            trs.transacao = transacao;
            trs.operacao = operacao;
            trs.objeto = objeto;
            trs.imgAnterior = imgAnterior;
            trs.imgPosterior = imgPosterior;

            criarTr(trs);
        }

        int verificar_existe(string object_name) {
            int index = -1;

            for(int i = 0; i < objetos.size(); i++)
                if(objetos[i].objeto_nome == object_name)
                    return i;                
            return index; 
        }

        vector<int> objects_by_transactions(string transaction_name) {
            vector<int> indexs;
            for(int i = 0; i < objetos.size(); i++)
                if(objetos[i].transacao_nome == transaction_name)
                    indexs.push_back(i);                
            return indexs; 
        }

        void classificar(Tr *transacao) {
            if(transacao->operacao == "w" || transacao->operacao == "r") 
                transacao->status = "ativa"; 
            else if(transacao->operacao == "c")
                transacao->status = "nao_ativa"; 
            else if(transacao->operacao == "a")
                transacao->status = "rollback";

            if(transacao->operacao == "c") {
                vector<int> indexs = objects_by_transactions(transacao->transacao);
                if(indexs.size() > 0) {
                    for(int i : indexs) {
                        objetos[i].valor = objetos[i].valores[transacao->transacao][objetos[i].valores[transacao->transacao].size() - 1][1];
                    }
                }  
            }  
            else if(transacao->operacao == "a") {
                vector<int> indexs = objects_by_transactions(transacao->transacao);
                if(indexs.size() > 0) {
                    for(int i : indexs) {
                        objetos[i].valor = objetos[i].valores[transacao->transacao][0][0];
                    }
                }  
            } else {
                int index = verificar_existe(transacao->objeto);
        
                if(index == -1) {
                    Object no;
                    no.objeto_nome = transacao->objeto;
                    no.transacao_nome = transacao->transacao;

                    
                    vector<string> valor;
                    valor.push_back(transacao->imgAnterior);
                    valor.push_back(transacao->imgPosterior);

                    no.valores[transacao->transacao].push_back(valor);

                    no.tr_type = transacao->status;

                    if(transacao->status == "ativa") no.valor = transacao->imgAnterior;
                    else if(transacao->status == "nao_ativa") no.valor = transacao->imgPosterior;

                    objetos.push_back(no);
                } else {
                    objetos[index].transacao_nome = transacao->transacao;

                    vector<string> valor;
                    valor.push_back(transacao->imgAnterior);
                    valor.push_back(transacao->imgPosterior);


                    objetos[index].valores[transacao->transacao].push_back(valor);

                    if(transacao->status == "ativa") objetos[index].valor = objetos[index].valores[transacao->transacao][0][0];
                    else if(transacao->status == "nao_ativa") objetos[index].valor = objetos[index].valores[transacao->transacao][objetos[index].valores[transacao->transacao].size() - 1][1];
                }
            }
        }

        void criarTr(Tr transacao) {
            for(int i = 0; i < transacoes.size(); i++) {
                if(transacoes[i].transacao == transacao.transacao) {
                    transacoes[i] = transacao;
                    classificar(&transacoes[i]);
                    return;
                };
            }
            classificar(&transacao);
            transacoes.push_back(transacao);
        }

        void rollback() { // que sofreram rollback
            out.open("out.txt", std::ios_base::app);
            out << "rollback: [";
            for(Tr tr : transacoes)
                if(tr.status == "rollback") out << tr.transacao << ", ";
            out << "]" << endl;
            out.close();
        }

        void redo() { // não ativas
            out.open("out.txt", std::ios_base::app);
            out << "redo: [";
            for(Tr tr : transacoes)
                if(tr.status == "nao_ativa") out << tr.transacao << ", ";
            out << "]" << endl;
            out.close();
        }

        void undo() { // ativas
            out.open("out.txt", std::ios_base::app);
            out<< "undo: ["; 
            for(int i = transacoes.size() - 1; i >= 0; i--) 
                if(transacoes[i].status == "ativa") out << transacoes[i].transacao << ", ";
            out << "]" << endl;
            out.close();
        }

        void mostrar_objeto() {
            out.open("out.txt", std::ios_base::app);
            out << endl; 
            for(int i = 0; i < objetos.size(); i++) 
                out << objetos[i].objeto_nome << " = " << objetos[i].valor << endl;
            out.close();
        }
};