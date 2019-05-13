#include "HCTree.hpp"

using namespace std;

int main(int argc, char** argv){
    
    //for reconsructing huffman
    vector<int> freq(256,0);
    
    string line;
    ifstream myFile;
    myFile.open(argv[1],ios::in);

    //read 256 lines to fill vector of frequencies
    for(int i = 0 ; i<256;i++){
        getline(myFile,line);
        freq[i] = stoi(line);
    }
    
    //build treee
    HCTree* T = new HCTree();
    T->build(freq);
    
    ofstream out;
    out.open(argv[2],ios::out);
    //read rest of file and decode
    while(!myFile.eof()){
        //encode the rest of the file
        int i = T->decode(myFile);
        if(i == -1){

        }
        else{
            out << (unsigned char)i;
        }

    }

    myFile.close();
    out.close();
    
    delete(T);

}
