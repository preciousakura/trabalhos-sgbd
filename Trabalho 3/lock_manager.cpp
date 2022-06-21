#include "types.h"
class Lock_Manager {  
    public:
        fstream lock_table_file, out;

        Lock_Manager() {
            lock_table_file.open("lock_table_file.txt", std::ios_base::out);
            lock_table_file.close();
        }

        vector<Wait_Q_Data> wait_q_data;
        vector<Lock> lock_table;
        int num_of_trs;
        Tr* first_element;

        vector<string> rollback_q;

        Tr* findTransaction(string id) {
            for(int i = 0; i < num_of_trs; i++) {
                if(first_element[i].id_tr == id) {
                    return &first_element[i];
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

        bool has_in_wait_data_q(string id_tr) {
            for(Wait_Q_Data wqd : wait_q_data) {
                if(id_tr == wqd.tr_id) return true;
            }
            return false;
        }

        void run_op(Tr *tr, string file, string type) {
            if(type == "S") {
                ls(tr, file, type);
            } else if(type == "X") {
                lx(tr, file, type);
            } else if(type == "C") {
                u(tr);
            }
        }

        void remove_rollback_q(string id_tr) {
            for(int i = 0; i < rollback_q.size(); i++) {
                if(id_tr == rollback_q[i]) {
                    rollback_q.erase(rollback_q.begin() + i);
                    return;
                }
            }
        }

        // void print_grafos() {
        //     for(int i = 0; i < num_of_trs; i++) {   
        //         for(string s:first_element[i].wait_for_list_ids) {
        //             cout << "T" << first_element[i].id_tr << "->T" << s << endl; 
        //         } 
        //     }
        // }

        void run_rollback_list() {
            cout << "Transacoes que sofreram rollback:" << endl;
            if(rollback_q.size() == 0) {
                cout << "Lista vazia" << endl;
            }

            for (string id_tr : rollback_q) {
                Tr *cur_tr = findTransaction(id_tr);
                cur_tr->state = "active";

                for(Op op:cur_tr->ops) {
                    cout << cur_tr->id_tr << ',' << op.file << ',' << op.type << endl;
                    run_op(cur_tr, op.file, op.type);
                }
            } 

            for (string id_tr : rollback_q) {
                remove_rollback_q(id_tr);
            }    
        }

        void run_waiting_list() {
            cout << "Agora considerando os comandos em espera:" << endl;
            
            if(wait_q_data.size() == 0) {
                cout << "Lista vazia" << endl;
            }

            // atualizando status de todas as transações antes de executar a wait q
            for (Wait_Q_Data wqd : wait_q_data) {
                Tr *cur_tr = findTransaction(wqd.tr_id);
                cur_tr->state = "active";
                cur_tr->ops.clear();
            }


            vector<Wait_Q_Data> wait_q_fake = wait_q_data;
            wait_q_data.clear();

            for (Wait_Q_Data wqd : wait_q_fake) {
                Tr *cur_tr = findTransaction(wqd.tr_id);
                cout << cur_tr->id_tr << ',' << wqd.file << ',' << wqd.type << endl;
                run_op(cur_tr, wqd.file, wqd.type);
            }  
        }

        vector<string> remove_element(string id_tr, vector<string> a) {
            vector<string> result = a;
            for(int i = 0; i < result.size(); i++) {
                if(id_tr == result[i]) {
                    result.erase(result.begin() + i);
                    return result;
                }
            }
            return result;
        }

        void clear_graph(string id_tr) {
            for(int i = 0; i < num_of_trs; i++) {
                first_element[i].wait_for_list_ids = remove_element(id_tr, first_element[i].wait_for_list_ids);
            }
        }
        

        void rollback(Tr *ti) {
            ti->state = "rollbacked";
            remove_tr_from_lock(ti->id_tr);  // limpa os bloqueios
            rollback_q.push_back(ti->id_tr); // coloca na wait_q
            clear_graph(ti->id_tr);
            ti->wait_for_list_ids.clear();

            cout << "vai ser cancelada (rollback)" << endl;
        }

        void realization(Tr* ti) {
            ti->state = "commited";
            remove_tr_from_lock(ti->id_tr);
            clear_graph(ti->id_tr);            
            ti->wait_for_list_ids.clear();

        }

        void wait_state(Tr *ti, string file, string type) {
            Wait_Q_Data n;
            n.file = file;
            n.tr_id = ti->id_tr;
            n.type = type;
        
            ti->state = "await";
            wait_q_data.push_back(n);
            cout << "vai esperar" << endl;
        }

        bool has_element(string id_tr, vector<string> a) {
            for(int i = 0; i < a.size(); i++) {
                if(id_tr == a[i]) return true;
            }
            return false;
        }

        // Quanto mais baixa a marca de tempo, mais alta a prioridade da transação;
        // Ti solicita um bloqueio e a transação Tj mantém um bloqueio conflitante 

        void protocol_wait_die(Tr *ti, vector<string> trs_conflitantes, string file, string type) {
            // Esperar-morrer: se Ti tem prioridade mais alta, ela pode esperar; caso contrário, é cancelada.
            Tr* highest_priority_tr = highest_priority(ti, trs_conflitantes);

            if(ti->id_tr == highest_priority_tr->id_tr) {
                wait_state(ti, file, type); 
                for(string tr_id : trs_conflitantes) {
                    if(!has_element(tr_id, ti->wait_for_list_ids)) ti->wait_for_list_ids.push_back(tr_id);
                }
            } else {
                rollback(ti);
            }
        }

        void protocol_wound_wait(Tr *ti, vector<string> trs_conflitantes, string file, string type) {
            // Ferir-esperar: se Ti tem prioridade mais alta, cancela Tj; caso contrário, Ti espera.
            for(string tr_id : trs_conflitantes) {
                Tr* highest_priority_tr = highest_priority(ti, trs_conflitantes);
                if(ti->id_tr == highest_priority_tr->id_tr) {
                    Tr *cur_tr = findTransaction(tr_id);
                    rollback(cur_tr);    
                } else {
                    wait_state(ti, file, type); 
                    for(string tr_id : trs_conflitantes) {
                        if(!has_element(tr_id, ti->wait_for_list_ids)) ti->wait_for_list_ids.push_back(tr_id);
                    }
                }
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
            if(tr->state != "active") {
                if(tr->state == "await") wait_state(tr, file, "S"); 
                else cout << "transacao sofreu rollback" << endl;
                return;
            }

            if(tr->ops.size() > 0 && tr->has_lock("S", file)) {
                cout << "ok!" << endl;
                return;
            }

            vector<string> trs_conflitantes = hasLock(file, "X", tr->id_tr);
            if(trs_conflitantes.size() > 0) {
                protocol_wait_die(tr, trs_conflitantes, file, "S");
                return;
            }

            createLock(tr, file, "S");
        }

        void lx(Tr *tr, string file, string type) {
            if(tr->state != "active") {
                if(tr->state == "await") {
                    wait_state(tr, file, type); 
                    cout << "operacao em espera" << endl;
                } else {
                    cout << "transacao sofreu rollback" << endl;
                }
                return;
            }

            vector<string> trs_conflitantes;
            if(tr->ops.size() > 0) {
                if(tr->has_lock("S", file)) { // editar a lock_table
                    trs_conflitantes = hasLock(file, "S", tr->id_tr);
                    if(trs_conflitantes.size() > 0) {
                        protocol_wait_die(tr, trs_conflitantes, file, "X");
                        return;
                    }
                } 
            }

            trs_conflitantes = hasLock(file, "X", tr->id_tr);
            if(trs_conflitantes.size() > 0) {
                protocol_wait_die(tr, trs_conflitantes, file, "X");
                return;
            }

            trs_conflitantes = hasLock(file, "S", tr->id_tr);
            if(trs_conflitantes.size() > 0) {
                protocol_wait_die(tr, trs_conflitantes, file, "X");
                return;
            }

            createLock(tr, file, "X");
            cout << "ok!" << endl;
        }

        void u(Tr *tr) {
            if(tr->state == "await") {
                wait_state(tr, "", "C");
            } 
            else if(tr->state == "rollbacked") {
                cout << "transacao sofreu rollback" << endl;
                return;
            }
            else {
                realization(tr);
                cout << "ok!" << endl;
            }
        }
};