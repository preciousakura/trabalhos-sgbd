#include <bits/stdc++.h>

using namespace std;

typedef struct Wait_Q_Data {
    string file, type, tr_id;
} Wait_Q_Data;

typedef struct Op {
    string file, type;
} Op;

typedef struct Tr {
    int timestamp;
    string id_tr;
    string state;
    vector<string> wait_for_list_ids;
    vector<Op> ops;

    bool has_lock(string type, string file) {
        for(Op op: ops) {
            if(op.type == type && op.file == file) return true;
        } return false;
    }
} Tr;

typedef struct Lock {
    string file, lock_type, id_lock;
} Lock;

typedef struct DataFile { 
    string type_op, file, id_tr;
} DataFile;
