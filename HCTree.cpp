#include "HCTree.hpp"

/*destructor for tree*/
HCTree::~HCTree()
{
    deleteAll(root);
}

//helper method to recursively delete all nodes
void HCTree::deleteAll(HCNode* n){
    if(n == nullptr){
        return;
    }

    deleteAll(n->c0);

    deleteAll(n->c1);

    
    delete(n);

}

/**/
void HCTree::build(const vector<int>& freqs){
    //count fre 
    //frquencies
    for(int i =0; i<freqs.size();i++){
        if(freqs[i] == 0){
            //dont do anything
        }
        else{
            //create a leaf for this ascii value
            leaves[i] = new HCNode(freqs[i],(unsigned char)i,nullptr,
                                    nullptr,nullptr);
            forestQue.push(leaves[i]);  
        }
    }
    
    //handle for single repeated character
    if(forestQue.size() == 1){
        //get char
        HCNode* one = forestQue.top();
        forestQue.pop();

        //create parent node
        HCNode* myHCNode = new HCNode(one->count,one->symbol,nullptr,
                                    one,nullptr); 
        //set parent
        one->p = myHCNode;
        //push new node into tre
        forestQue.push(myHCNode);
    }
    

    //loop until only one node is left
    while(forestQue.size() != 1){
        
        //get the two smallest nodes in the que
        HCNode* one = forestQue.top();
        forestQue.pop();
        HCNode* two = forestQue.top();
        forestQue.pop();
        
        //get value that will be used in new node
        int myCount = one->count + two->count;
        //create a new node with combined count val and 2 children   
        HCNode* myHCNode = new HCNode(myCount,two->symbol,one,
                                    two,nullptr); 
        //set parents of two nodes
        one->p = myHCNode;
        two->p = myHCNode;

        forestQue.push(myHCNode);
    }

    //set the root
    root = forestQue.top();
    forestQue.pop();
    
}

/**/
void HCTree::encode(byte symbol, ofstream& out) const{
    //get this leaf node
    HCNode* move = leaves[(int)symbol];   
    //to build string of 1 and 0 
    string code;
    //to hold the symbol you came from
    byte temp = symbol;

    while( move != root){
        //go to moves parent
        move = move->p;
        //if move is coming from right add 1 to beggining of string
        if(move->c1->symbol == temp){
            code.insert(code.begin(),'1');
        }
        else{
            //if it came from the left add 0 to beggining of string
            code.insert(code.begin(),'0');

        }
        temp = move->symbol;
    }

    out << code;
}

/**/
int HCTree::decode(ifstream& in) const{
    HCNode* move = root;
    unsigned char c;
    int bin;
    while(true){
        c = in.get();
        //check for end of file
        if(in.eof()){
            return -1;
        }
        //get 1 or 0
        bin = c - '0';
    
        //if read 1
        if(bin == 1){
            move = move->c1;
        }
        //if read 0
        else if(bin == 0){
            move = move->c0;
        }   
        //if at leaf node
        if(move->c1 == nullptr && move->c0 == nullptr){
            //return the sybol of leaf
            return move->symbol;
        }
    }
}


