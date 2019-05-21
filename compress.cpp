/*File:compress.cpp
 * Date: 5-20-19
 * Encodes the input file and uses the encoding to compress the file
 * the encoding tree is outputed to the outputFile becore the
 * compressed file in order to decompress.
 * This is all done using methods in HCTree.hpp
 *
 * */
#include "HCTree.hpp"

using namespace std;


int main(int argc, char** argv){
    
    //error check for not having 3 arguments   
    if(argc != 3){
        cout << "Invalid number of arguments." << endl;
        cout << "Usage: ./compress <infile filename> <outfile filename>." << endl;
        return -1;
    }
 
    //vector to hold frequencies
    vector<int> freq(256,0);
    //holds char that is gotten from reading 
    unsigned char nextchar;
    //open input file
    ifstream myFile;
    myFile.open(argv[1],ios::in);
  
    //handle if an empty file is passed in 
    if(myFile.peek() == ifstream::traits_type::eof()){
        ofstream outFile;
        outFile.open(argv[2], ofstream::out | ofstream::trunc );
        //close files
        outFile.close();
        myFile.close();
        return 1;
    } 

    //used to convert chars
    int asci = 0;
    //count how many bytes are read in
    unsigned int count = 0;
    //read file lines
    while(true){
        //get char from input file
        nextchar = (unsigned char)myFile.get();
        //dont convert end of file char
        if(myFile.eof()){
            break;
        }
        //convert char to ascii
        asci = nextchar;   
        //increase chars freq
        freq[asci]++;
        count++;
    }
    //close input file
    myFile.close();
    //build hauffman tree from frequencies
    HCTree* T = new HCTree();
    T->build(freq);

    //open file to write to
    ofstream outFile;
    outFile.open(argv[2],ios::out);
    //make bitStream to write bits to file
    BitOutputStream bitO(outFile);
    
    //WRITE THE COUNT TO THE FILE
    bitO.writeCount(count);

    //print out the tree on the header
    string myString;
    T->buildHC(bitO,myString);    
   
    //print out the headersize 
    cout << "Header size is: "<< bitO.byteC << " bytes"<<endl;

    //open file to read encoding from 
    ifstream read;
    read.open(argv[1],ios::in | ios::binary);
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
        //call BitOutputStream encode    
        T->encode(sym,bitO);
    }
    //if final buf was not flushed
    if(!(bitO.getbufCount() == 0)){
        bitO.flush();
    }
    
    //close files
    outFile.close();
    read.close();        
    //delete the tree
    delete(T);    
    
    return 0;
}

