#include "lock_manager.cpp"

class Tr_Manager {
    public:
        fstream out;
        vector<Tr> trasaction;
        Lock_Manager lock_manager;

        Tr_Manager() {
            out.open("out.txt", std::ios_base::out);
            out.close();
        }

        Tr* findTransaction(string id) {
            for(int i = 0; i < trasaction.size(); i++) {
                if(trasaction[i].id_tr == id) {
                    return &trasaction[i];
                }
            }
            return nullptr;
        } 

        void readType(string type, string file, string id_tr) {
            if(type == "BT") {
                createTrasaction(id_tr);
                return;
            }

            Tr* tr = findTransaction(id_tr);
            if(tr == nullptr) return;

            if(type == "C") {
                lock_manager.u(tr);
            }
            else if(type == "r") {
                lock_manager.ls(tr, file, type);
            } 
            else if(type == "w") {
                lock_manager.lx(tr, file, type);
            }
        }

        void createTrasaction(string id_tr) {
            int index = trasaction.size() + 1;
            Tr new_tr;
            new_tr.id_tr = id_tr;
            new_tr.timestamp = index;

            trasaction.push_back(new_tr);
            lock_manager.trasactions = trasaction;
        }
};