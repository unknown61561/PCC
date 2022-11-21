#include <bits/stdc++.h>
using namespace std;
class node
{
public:
    string identifier, scope, type;
    int lineNo;
    node *next;
    node(string identifier, string scope, string type, int lineNo){
        this->identifier = identifier;
        this->scope = scope;
        this->type = type;
        this->lineNo = lineNo;
    }
    ~node(){
        if (next != NULL)
        {
            delete next;
        }
    }
    void print(){
        cout << "Identifier's Name:" << identifier
             << endl << "Type:" << type
             << endl <<"Scope: " << scope
             << endl <<"Line Number: " << lineNo << endl;
    }
};

class symboltable{
    node **table;
    int table_size;
    int hashFn(string key){
        int index = 0;
        int p = 1;
        for (int i = 0; i < key.length(); i++){
            index = index + (key[i] * p) % table_size;
            index = index % table_size;
            p = (p * 27) % table_size;
        }
        return index;
    }

public:
    symboltable(int size = 7){
        table_size = size;
        table = new node *[table_size];
        for (int i = 0; i < table_size; i++){
            table[i] = NULL;
        }
    }
    void insert(string id, string scope, string type, int lineno){
        int index = hashFn(id);
        node *n = new node(id, scope, type, lineno);
        n->next = table[index];
        table[index] = n;
    }
    node *find(string key){
        int index = hashFn(key);
        node *ptr = table[index];
        while (ptr != NULL){
            if (ptr->identifier == key)
            {
                return ptr;
            }
            ptr = ptr->next;
        }
        return NULL;
    }
    bool erase(string key){
        int index = hashFn(key);
        node *ptr = table[index];
        if (ptr != NULL){
            if (ptr->identifier == key)
            {
                table[index] = ptr->next;
                return true;
            }
            node *prev = ptr;
            ptr = ptr->next;
            while (ptr != NULL)
            {
                if (ptr->identifier == key)
                {
                    prev->next = ptr->next;
                    ptr->next = NULL;
                    delete ptr;
                    return true;
                }
                prev = ptr;
                ptr = ptr->next;
            }
        }
        return false;
    }
    node *modify(string id, string scope, string type, int lineno){
        int index = hashFn(id);
        node *ptr = table[index];
        while (ptr != NULL){
            if (ptr->identifier == id){
                ptr->scope = scope;
                ptr->type = type;
                ptr->lineNo = lineno;
                return ptr;
            }
            ptr = ptr->next;
        }
        return NULL;
    }
    void print(){
        for (int i = 0; i < table_size; i++){
            cout << "Bucket " << i << " ->";
            node *ptr = table[i];
            while (ptr != NULL){
                cout << ptr->identifier << "->";
                ptr = ptr->next;
            }
            cout << endl;
        }
    }
};

int main(){
    symboltable s;
    s.insert("if", "local", "keyword", 4);
    s.insert("number", "global", "variable", 2);
    s.insert("add", "global", "function", 1);
    s.insert("sum", "local", "int", 3);
    s.insert("a", "function parameter", "int", 1);
    node *ptr = s.find("if");
    if (ptr != NULL){
        cout << "if Identifier is present\n";
        ptr->print();
    }
    else{
        cout << "if Identifier not present\n";
    }
    if (s.erase("if") == true){
        cout << endl <<"if Identifier is deleted" << endl;
    }
    else{
        cout << endl <<"Failed to delete if identifier" << endl;
    }
    ptr = s.modify("if", "global", "variable", 3);
    if (ptr != NULL){
        cout << endl <<"if Identifier updated" << endl;
        ptr->print();
    }
    else{
        cout << endl <<"Failed to update if identifer" << endl;
    }
    ptr = s.find("if");
    if (ptr != NULL){
        cout << endl <<"if Identifier is present" << endl;
        ptr->print();
    }
    else{
        cout << endl <<"if Identifier not present" << endl;
    }
    ptr = s.modify("number", "global", "variable", 3);
    if (ptr != NULL){
        cout << endl <<"number Identifier updated" << endl;
        ptr->print();
    }
    else{
        cout << endl <<"Failed to update number identifer" << endl;
    }
    ptr = s.find("number");
    if (ptr != NULL){
        cout << endl <<"number Identifier is present" << endl;
        ptr->print();
    }
    else{
        cout << endl <<"number Identifier not present" << endl;
    }
    cout << endl <<"---- SYMBOL_TABLE ----" << endl;
    s.print();
    return 0;
}