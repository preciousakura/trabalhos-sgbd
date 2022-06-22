#include "lock_manager.cpp"

class Tr_Manager {
    public:
        fstream out;
        vector<Tr> transactions;
        Lock_Manager lock_manager;

        Tr_Manager() {
            cout << "0. WAIT-DIE" << endl;
            cout << "1. WOUND-WAIT" << endl;
            cout << "Protocolo: ";
            cin >> lock_manager.protocol;

            out.open("out.txt", std::ios_base::out);
            out << "--- Output ---" << endl << endl;
            out << "Protocolo aplicado: ";
            if(!lock_manager.protocol) out << "WAIT-DIE";
            else out << "WOUND-WAIT";
            out << endl << endl;
            out.close();
        }

        Op create_op(bool efetivacao, string file, string type) {
            Op n_op;

            n_op.file = file;
            n_op.type = type;

            return n_op;
        }
        
        Tr* findTransaction(string id) {
            for(int i = 0; i < transactions.size(); i++) 
                if(transactions[i].id_tr == id) 
                    return &transactions[i];
            return nullptr;
        } 

        void readType(string type, string file, string id_tr) {
            if(type == "BT") {
                createtransactions(id_tr);
                return;
            }

            Tr* tr = findTransaction(id_tr);
            if(tr == nullptr) return;

            if(type == "C") {
                lock_manager.u(tr);
                tr->ops.push_back(create_op(true, "", "C"));
            }
            else if(type == "r") {
                lock_manager.ls(tr, file, type);
                tr->ops.push_back(create_op(true, file, "S"));
            } 
            else if(type == "w") {
                lock_manager.lx(tr, file, type);
                tr->ops.push_back(create_op(true, file, "X"));
            }
        }

        void createtransactions(string id_tr) {
            int index = transactions.size() + 1;
            Tr new_tr;
            new_tr.id_tr = id_tr;
            new_tr.timestamp = index;
            new_tr.state = "active";

            transactions.push_back(new_tr);

            lock_manager.first_element = transactions.data();
            lock_manager.num_of_trs = transactions.size();

            out.open("out.txt", std::ios_base::app);
            out << "BT(" << id_tr << "): Arestas do Grafo { }, OK" << endl;
            out.close();
        }
};