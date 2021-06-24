#include <iostream>
#include <string>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#define ALPH_SIZE 128 // Tamanho do alfabeto relativo a tabela ASCII

class Node{

    friend class DigitalTree;

    private:
        bool terminal; // Determina se um nó é terminal ou não
        Node* childs[ALPH_SIZE]= {nullptr}; // Array de nós
        int Nodes; // determina quantos nós estão preenchidos 
    
    public:
        // Constructor
        Node(const bool terminal);
};
class DigitalTree{

    private:
        Node* root; // Nó raíz
    
    public:
        // Constructor
        DigitalTree(); 

        //Métodos
        bool search(std::string*); // Retorna se uma palavra pertence ou não a arvore
        void insert(std::string*); // Chama primeira recursão da função de inserção de caracter
        bool remove(std::string*); // Retorna se a palavra foi removida ou se não existe na arvore
        void cleanTree(Node*); // Usada pelo destructor para limpeza antes do fim do programa
        void insertChar(Node*, std::string*, unsigned int); // Insere caracter recursivamente na arvore
        bool searchChar(Node*, std::string*, unsigned int); // Procura caracter recursivamente na arvore
        bool removeChar(Node*, std::string*, unsigned int, bool*); // Remove caracter recursivamente na arvore
        void printNode(Node*, std::string); // Imprime terminal 
        void printAll(); // Realiza a primeira chamada de impressao de arvore

        //Destructor
        ~DigitalTree();
};
int main(){

    std::string entry, word;
    DigitalTree* D= new DigitalTree();

    while(entry.compare("F") != 0){ // Menu principal do programa

        std::getline(std::cin, entry);

        if(entry.compare("I") == 0){ // opção para inserir nova palavra
            std::getline(std::cin, word);
            D->insert(&word);
        }
        else if(entry.compare("B") == 0){ // opção de busca de palavra

            std::getline(std::cin, word);

            if(D->search(&word)){
                std::cout << "Palavra Encontrada" << std::endl;
            }
            else{
                std::cout << "Palavra nao encontrada" << std::endl;
            }
        }
        else if(entry.compare("R") == 0){ // opção de remoção de palavra

            std::getline(std::cin, word);

            if(D->remove(&word)){
                std::cout << "Palavra removida" << std::endl;
            }
            else{
                std::cout << "Palavra inexistente" << std::endl;
            }
            
        }
        else if(entry.compare("E") == 0){ // opção de exibição de todas as palavras
            D->printAll();
        }
    }

    D->~DigitalTree(); // Limpeza da Arvore no fim do programa
}
// Constructor de nó
Node::Node(bool terminal){

    this->terminal= terminal;
    this->Nodes= 0;
}
// Constructor de Arvore Digital
DigitalTree::DigitalTree(){
    root= new Node(false);
}
// Métodos de Arvore Digital
void DigitalTree::insert(std::string* word){
    insertChar(root, word, 0);
}
bool DigitalTree::search(std::string* word){

    return searchChar(root, word, 0);
}
bool DigitalTree::remove(std::string* word){
    bool found;

    removeChar(root, word, 0, &found);

    return found;
}
void DigitalTree::insertChar(Node* a, std::string* word, unsigned int pos){

    if(pos < word->length()){ // Se ainda existem caracteres a serem percorridos

        char c= word->at(pos);
        int asciiPos= c;

        if(a->childs[asciiPos] == nullptr){ // Se não houver nó relativo a posição da palavra é criado

            a->childs[asciiPos]= new Node(false);
            a->Nodes++;
        }
        insertChar(a->childs[asciiPos], word, pos+1); // Recursividade até chegar ao fim da palavra
    }
    else{ // Última chamada para transformar em nó terminal
        a->terminal= true;
    }

}
bool DigitalTree::searchChar(Node* a, std::string* word, unsigned int pos){

    if(pos < word->length()){ // Se ainda existem caracteres a serem percorridos

        char c= word->at(pos);
        int asciiPos= c;

        if(a->childs[asciiPos] == nullptr){ // Se não houver no correspondente ao caracter a palavra nao existe na arvore
            return false;
        }
        searchChar(a->childs[asciiPos], word, pos+1); // Recursividade até atingir o fim da palavra
    }
    else if(a->terminal == true){ // Se o último nó relativo ao caracter for terminal a palavra existe
        return true;
    }
    else if(a->terminal == false && pos == word->length()){ // Se o último nó relativo ao caracter for nao terminal a palavra existe
        return false;
    }
}
bool DigitalTree::removeChar(Node* a, std::string* word, unsigned int pos, bool* found){

    if(pos < word->length()){ // Se ainda existem caracteres a serem percorridos

        char c= word->at(pos);
        int asciiPos= c;

        if(a->childs[asciiPos] == nullptr){ // Se não houver no correspondente ao caracter a palavra nao existe na arvore
            *found= false;
        }
        else{

            removeChar(a->childs[asciiPos], word, pos+1, found); // Recursividade até atingir o fim da palavra

            if(a->childs[asciiPos]->Nodes == 0 && a->childs[asciiPos]->terminal == false && *found == true){ // Se o nó filho nao tiver filhos e nao for terminal pode ser removido

                a->Nodes--;
                Node* aux=a->childs[asciiPos];
                a->childs[asciiPos]= nullptr;

                delete aux;
        }
        }
    }
    else if(a->terminal == false && pos == word->length()) // Se o último nó relativo ao caracter for nao terminal a palavra nao existe
        *found= false;
    
    else if(a->terminal == true && pos == word->length()){ // Se o último nó relativo ao caracter for terminal a palavra existe
        *found= true;
        a->terminal= false;
    }
}
void DigitalTree::cleanTree(Node* node){

    int index= 32;
    
    while(node->Nodes > 0 && index < ALPH_SIZE){ // Percorrendo array de ponteiros
        if(node->childs[index] != nullptr){
            cleanTree(node->childs[index]);
            delete node->childs[index];
        }
        index++;
    }
}
void DigitalTree::printAll(){

    printNode(root, "");
}
void DigitalTree::printNode(Node* p, std::string prefix){

    char c;
        if(p->terminal == true){ // Se o nó é terminal
            std::cout << prefix << std::endl;
        }

    for(int index = 32; index < ALPH_SIZE ; index++){ // Percorrendo Array de ponteiros

        if(p->childs[index] != nullptr){

            c= index;

            prefix.push_back(c);

            printNode(p->childs[index], prefix);

            prefix.pop_back();
        }
    }
}
//Destructor de Arvore Digital
DigitalTree::~DigitalTree(){
    cleanTree(root);
    
    delete root;
}