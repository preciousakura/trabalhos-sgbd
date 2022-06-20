#include "types.h"


class Lock_Manager {  
    public:
        fstream lock_table_file, out;

        Lock_Manager() {
            lock_table_file.open("lock_table_file.txt", std::ios_base::out);
            lock_table_file.close();
        }

        vector<Lock> lock_table;
        vector<Tr> trasactions;

        void restart(Tr *tr) {

        }

        Tr* findTransaction(string id) {
            for(int i = 0; i < trasactions.size(); i++) {
                if(trasactions[i].id_tr == id) {
                    return &trasactions[i];
                }
            }
            return nullptr;
        }

        Tr* highest_priority(Tr *ti, vector<string> trs_conflitantes) {
            int highest_priority_tr_index = -1;
            Tr* highest_priority_tr = ti;
            for(int i = 0; i < trs_conflitantes.size(); i++) {
                Tr *cur_tr = findTransaction(trs_conflitantes[i]);
                if(cur_tr->timestamp < highest_priority_tr->timestamp) {
                    highest_priority_tr = cur_tr;
                    highest_priority_tr_index = i;
                }
            } 
            return highest_priority_tr;
        }

        void rollback(Tr *ti) {
            remove_tr_from_lock(ti->id_tr);
            restart(ti);
        }

        // Quanto mais baixa a marca de tempo, mais alta a prioridade da transação;
        // Ti solicita um bloqueio e a transação Tj mantém um bloqueio conflitante 

        void protocol_wait_die(Tr *ti, vector<string> trs_conflitantes) {
            // Esperar-morrer: se Ti tem prioridade mais alta, ela pode esperar; caso contrário, é cancelada.
            Tr* highest_priority_tr = highest_priority(ti, trs_conflitantes);

            if(ti->id_tr == highest_priority_tr->id_tr) {
                ti->waiting = true;
                cout << "ti vai esperar" << endl;
                // ti vai esperar
            } else {
                rollback(ti);
                cout << "ti vai ser cancelada (rollback)" << endl;
                // ti vai ser cancelada (rollback)
            }
        }

        void protocol_wound_wait(Tr *ti, vector<string> trs_conflitantes) {
            // Ferir-esperar: se Ti tem prioridade mais alta, cancela Tj; caso contrário, Ti espera.
            Tr* highest_priority_tr = highest_priority(ti, trs_conflitantes);
            if(ti->id_tr == highest_priority_tr->id_tr) {
                for(string tr_id : trs_conflitantes) {
                    Tr *cur_tr = findTransaction(tr_id);
                    rollback(cur_tr);
                    cout << cur_tr->id_tr << " vai ser cancelada (rollback)" << endl;
                }
                // tj vai ser cancelada
            } else {
                ti->waiting = true;
                cout << "ti vai esperar" << endl;
            }
        }




        void createLock(Tr* tr, string file, string type_lock) {
            Lock new_lock;

            new_lock.id_lock = tr->id_tr;
            new_lock.lock_type = type_lock;
            new_lock.file = file;
            lock_table_file.open("lock_table_file.txt", std::ios_base::app);
            lock_table_file << new_lock.id_lock << ',' << type_lock << ',' << file << endl;
            lock_table_file.close();
        }

        vector<string> hasLock(string file, string type_lock, string id_tr) {
            vector<string> trs_conflitantes;

            lock_table_file.open("lock_table_file.txt",  std::ios_base::in);

            string tr_id_lock, type_lock_in_table, file_in_lock_table;
            
            while(lock_table_file.good()) {
                getline(lock_table_file, tr_id_lock, ',');
                getline(lock_table_file, type_lock_in_table, ',');
                getline(lock_table_file, file_in_lock_table);
                if(tr_id_lock == "" || type_lock_in_table == "" || file_in_lock_table == "") break;


                if(file == file_in_lock_table && type_lock_in_table == type_lock && tr_id_lock != id_tr) {
                    trs_conflitantes.push_back(tr_id_lock);
                }
            }
            
            lock_table_file.close();
            return trs_conflitantes;
        }

        void update_lock_table(string str_change, string change_to) {
            lock_table_file.open("lock_table_file.txt", std::ios_base::in);
			stringstream lock;

			lock << lock_table_file.rdbuf();
			string str = lock.str();
			size_t pos = str.find(str_change);
			str.replace(pos, string(str_change).length(), change_to);
			lock_table_file.close();

			remove("lock_table_file.txt");

			lock_table_file.open("lock_table_file.txt", ios_base::out);
			lock_table_file << str;
			lock_table_file.close();
        }

        void change_lock(string file, string type_lock, string id_tr, string change_to) {
            string str_change;
            lock_table_file.open("lock_table_file.txt",  std::ios_base::in);

            string tr_id_lock, type_lock_in_table, file_in_lock_table;
            
            while(lock_table_file.good()) {
                getline(lock_table_file, tr_id_lock, ',');
                getline(lock_table_file, type_lock_in_table, ',');
                getline(lock_table_file, file_in_lock_table);
                if(tr_id_lock == "" || type_lock_in_table == "" || file_in_lock_table == "") break;

                if(file == file_in_lock_table && type_lock_in_table == type_lock && tr_id_lock == id_tr) {
                    str_change = tr_id_lock + ',' + type_lock_in_table + ',' + file_in_lock_table + '\n';
                    break;
                }
            }
            
            lock_table_file.close();

            string new_lock = tr_id_lock + ',' + change_to + ',' + file_in_lock_table + '\n';
            update_lock_table(str_change, new_lock);
        }

        void remove_tr_from_lock(string id_tr) {
            vector<string> str_change;
            lock_table_file.open("lock_table_file.txt",  std::ios_base::in);

            string tr_id_lock, type_lock_in_table, file_in_lock_table;
            
            while(lock_table_file.good()) {
                getline(lock_table_file, tr_id_lock, ',');
                getline(lock_table_file, type_lock_in_table, ',');
                getline(lock_table_file, file_in_lock_table);
                if(tr_id_lock == "" || type_lock_in_table == "" || file_in_lock_table == "") break;

                if(tr_id_lock == id_tr) {
                    str_change.push_back(tr_id_lock + ',' + type_lock_in_table + ',' + file_in_lock_table + '\n');
                }
            }

            lock_table_file.close();
            for(string s:str_change) {
                update_lock_table(s, "");
            }
        }   

  

        void ls(Tr *tr, string file, string type) {
            if(tr->types_lock[file] != "") return;
            
            vector<string> trs_conflitantes = hasLock(file, "X", tr->id_tr);
            if(trs_conflitantes.size() > 0) {
                protocol_wait_die(tr, trs_conflitantes);
                cout << "Teve conflito!" << endl;
                return;
            }

            createLock(tr, file, "S");
            tr->types_lock[file] = "S";
        }

        void lx(Tr *tr, string file, string type) {
            vector<string> trs_conflitantes;
            if(tr->types_lock[file] != "") {
                if(tr->types_lock[file] == "S") { // editar a lock_table
                    trs_conflitantes = hasLock(file, "S", tr->id_tr);
                    if(trs_conflitantes.size() > 0) {
                        protocol_wait_die(tr, trs_conflitantes);
                        cout << "Teve conflito!" << endl;
                        return;
                    }
                    else {
                        change_lock(file, tr->types_lock[file], tr->id_tr, "X");
                        tr->types_lock[file] = "X";
                    }
                } 
                return;
            }

            trs_conflitantes = hasLock(file, "X", tr->id_tr);
            if(trs_conflitantes.size() > 0) {
                protocol_wait_die(tr, trs_conflitantes);
                cout << "Teve conflito!" << endl;
                return;
            }

            trs_conflitantes = hasLock(file, "S", tr->id_tr);
            if(trs_conflitantes.size() > 0) {
                protocol_wait_die(tr, trs_conflitantes);
                cout << "Teve conflito!" << endl;
                return;
            }

            createLock(tr, file, "X");
            tr->types_lock[file] = "X";
        }

        void u(Tr *tr) {
            if(!tr->waiting) {
                remove_tr_from_lock(tr->id_tr);
                tr->types_lock.clear();
            } else {
                cout << "estah em espera" << endl;
            }
        }
};