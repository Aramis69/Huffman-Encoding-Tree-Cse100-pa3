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


/*Builds the HCNode tree*/
void HCTree::build(const vector<int>& freqs){
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

/* builds and writes the encoded huffman treee to file in binary*/
void HCTree::buildHC(BitOutputStream& out,string str){
    //handle single character case
    if(root->c0 == nullptr){
        //add a 1 for when a leaf is found
        str.append("1"); 
        str.append("1"); 
        //write the pathto get to the leaf
        out.writeBit(str);
        //write the symbol in binary
        out.writeByte(root->c1->symbol);
    }
    else{
        //call helper regularly
        buildHeader(root,out,str);
    }
}

/*helper method to buildHC which recursively traverses tree and encodes it*/
void HCTree::buildHeader(HCNode* mov, BitOutputStream& out, string str){
    
    //when you reach the leaf dont go any further down
    if((mov->c0 == nullptr) && (mov->c1 == nullptr)){
        //add a 1 for when a leaf is found
        str.append("1"); 
        //write the pathto get to the leaf
        out.writeBit(str);
        //to start a new path for the next         
        str.clear();
        //wrie the symbol in binary
        out.writeByte(mov->symbol);

        return;
    }
    else{
        //0 for node which is not leaf
        str.append("0"); 
    }

    //move to left node
    buildHeader(mov->c0, out, str);
    //to start a new path for the next         
    str.clear();
    //move to right node
    buildHeader(mov->c1,out,str);
}

/*Rebuilds the huffman tree from a binary representation*/
void HCTree::buildBits(BitInputStream& in){
    //read bit from file
    int bit =  in.readBit();
    root = new HCNode(0,0,nullptr,nullptr,nullptr); 
    //handle case with single char
    if(bit == 1){
        //for right tree
        root->c1 = bitHelp(in, root);
    }
    else{
        //for left tree
        root->c0 = bitHelp(in, root);
        //for right tree
        root->c1 = bitHelp(in, root);
    }
}
/*Helper method to build the tree using only binary info*/
HCNode* HCTree::bitHelp(BitInputStream& in, HCNode* par){

    HCNode* myHCNode;
    //read a bit
    int bin = in.readBit();

    //if eof is somehow reached
    if(bin == -1){
        return nullptr;
    }
    //if a 1 is read it means there a leaf following
    if(bin == 1){
        //read the nect 8 bits of ascii
        int sym =  in.readByte();
        //create a node with the ascii value
        myHCNode = new HCNode(1,(unsigned char)sym,nullptr,nullptr,par);
        //return to become a child
        return myHCNode; 
    }
    else{ 
        //create a filler node with no values
        myHCNode = new HCNode(0,0,nullptr,nullptr,par); 
    }
    
    //go to create left tree
    myHCNode->c0 = bitHelp(in, myHCNode);
    //to create right tree
    myHCNode->c1 = bitHelp(in, myHCNode);
    //return to become a child
    return myHCNode;
}

/*bit encoding */
void HCTree::encode(byte symbol, BitOutputStream& out) const{
    
    //get leaf node
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
        //hold symbol before moving on to parent
        temp = move->symbol;
    }
    //call the function to write bits
    out.writeBit(code);
}

/*encodes the symbol by moving through the tree*/
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
        //hold symbol before moving on to parent
        temp = move->symbol;
    }
    //print to the outfile
    out << code;
}

/*decode using the bitinput steeam*/
int HCTree::decode(BitInputStream& in) const{
    //used to traverse tree
    HCNode* move = root;
    //to convert char to int
    int bin;
    while(true){ 
        //get a char
        bin = in.readBit();
    
        //check for end of file
        if(bin == -1){
            return -1;
        }
        //if read 1
        if(bin == 1){
            move = move->c1;
        }
        //if read 0
        else if(bin == 0){
            move = move->c0;
        } 
         
        //if at leaf node
        if(move->count == 1){
            //return the sybol of leaf
            return move->symbol;
        }
    }
}

/*is used to read from the ifstream chars of 1 and 0*/
int HCTree::decode(ifstream& in) const{
    //used to traverse tree
    HCNode* move = root;
    //holds char being read
    unsigned char c;
    //to convert char to int
    int bin;
    while(true){
        //get a char
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


