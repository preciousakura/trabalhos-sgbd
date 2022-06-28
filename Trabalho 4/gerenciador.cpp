#include "types.h"

class Gerenciador {
    public:
        fstream out;
        vector<Tr> transacoes;

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

        void classificar(Tr *transacao) {
            if(transacao->operacao == "w" || transacao->operacao == "r" || transacao->operacao == "a") 
                transacao->status = "ativa"; 
            
            else if(transacao->operacao == "c")
                transacao->status = "nao_ativa"; 
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

        void redo() {// não ativas
            out.open("out.txt", std::ios_base::app);

            for(Tr tr : transacoes){
                if(tr.status == "nao_ativa") {
                    out << "redo: " << tr.transacao << " -> " << tr.imgPosterior << endl;
                    // nome da transacao e imagem posterior dela
                    // [ 4 | 3 | T2 | c | x | 30 | 90 ] redo: T2 -> 90
                }
            }

            out.close();
        }

        void undo() { // ativas
            out.open("out.txt", std::ios_base::app);
            
            for(int i = transacoes.size() - 1; i >= 0; i--) {
                if(transacoes[i].status == "ativa") {
                    out << "undo: " << transacoes[i].transacao << " -> " << transacoes[i].imgAnterior << endl;
                    // ler do final do log pro início
                    // nome da transacao e imagem anterior dela
                    // [ 3 | 2 | T3 | r | x | 20 | 10 ] undo: T3 -> 20
                }
            }
            out.close();
        }
};