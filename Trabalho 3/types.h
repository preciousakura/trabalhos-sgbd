#include <bits/stdc++.h>

using namespace std;

typedef struct Tr {
    int timestamp;
    string id_tr;
    unordered_map<string, string> types_lock;
    bool waiting = false;
} Tr;

typedef struct Lock {
    string file, lock_type, id_lock;
} Lock;

typedef struct DataFile { 
    string type_op, file, id_tr;
} DataFile;
