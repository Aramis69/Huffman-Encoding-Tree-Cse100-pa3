#include "HCTree.hpp"

using namespace std;


int main(int argc, char** argv){
   
    if(argc != 3){
        cout << "Invalid number of arguments." << endl;
        cout << "Usage: ./compress <infile filename> <outfile filename>." << endl;
        return -1;
    }
 
    vector<int> freq(256,0);

    unsigned char nextchar;
    ifstream myFile;
    myFile.open(argv[1],ios::in);
  
    //handle if an empty file is passed in 
    if(myFile.peek() == ifstream::traits_type::eof()){
        ofstream outFile;
        outFile.open(argv[2], ofstream::out | ofstream::trunc );
        
        outFile.close();
        myFile.close();
        return 1;
    } 

    int asci = 0;

    //read file lines
    while(true){
        
        nextchar = (unsigned char)myFile.get();

        if(myFile.eof()){
            break;
        }

        //convert char to ascii
        asci = nextchar;   
        //increase chars freq
        freq[asci]++;
        
    }
    
    myFile.close();
    
    //build hauffman tree
    HCTree* T = new HCTree();
    T->build(freq);

    ofstream outFile;
    outFile.open(argv[2],ios::out);

    //print out all the frequences on header
    for(int i =0; i<256; i++){
        outFile << freq[i] << endl;
    }    

    ifstream read;
    read.open(argv[1],ios::in);

    //read file lines
    while(true){
        //getchar
        nextchar = (unsigned char)read.get();

        if(read.eof()){
            break;
        }
        byte sym;
        //encode each char
        sym = nextchar;    
        T->encode(sym,outFile);
    }
    //close files
    outFile.close();
    read.close();        
    
    delete(T);    

    return 1;
}

