/* File: uncompress.cpp
 * Date: 5-20-19
 * This file holds the main to read in a compressed file and
 * recreate the original.
 * The resulting decompressed file gets created exactly as it was
 * using methods in HCTree.hpp
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

    //string to hold readings
    string line;
    //open the file to read from
    ifstream myFile;
    myFile.open(argv[1],ios::in | ios::binary);

    //handle if the file is empty
    if(myFile.peek() == ifstream::traits_type::eof()){ 
        ofstream out;
        out.open(argv[2], ofstream::out | ofstream::trunc);
        //close files that were open
        out.close();
        myFile.close();
        return 1;
    } 

    //open the outfile    
    ofstream out;
    out.open(argv[2], ios::out);

    //initialize Bitinput
    BitInputStream bitI(myFile);

    //read count of written bytes in order to ignore padding while decoding 
    unsigned int count = bitI.readCount();
     
    //build treee
    HCTree* T = new HCTree();
    T->buildBits(bitI);

    //read rest of file and decode
    for(unsigned int x = 0; x< count;x++){
        //encode the rest of the file
        int i = T->decode(bitI);
        //end of file was reached in decode 
        if(i == -1){
            //do nothing if end of file return
        }
        else{
            //print this returned Ascii code as unsigned char
            out << (unsigned char)i;
        }
    }
    //close files
    myFile.close();
    out.close();
    //delete the tree
    delete(T);

    return 0;
}
