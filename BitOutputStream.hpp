/* File: BitOutputSteam.hpp
 *
 * Sources: The idea of how to structure this code came from lecture slides
 * on how to do the bit streams some of the code is directly copied.
 *
 * This file helps to output information bit by bit to the outputfile
 *  It is used in compress.cpp to write a tree and a compressed file 
 * */

#ifndef BITOUTPUTSTREAM_HPP
#define BITOUTPUTSTREAM_HPP

#include <iostream>
#include <fstream>

using namespace std;


class BitOutputStream{

private:
    //to keep track of the current byte being encoded
    char buf;
    //to know which bit to modify
    int bufCount;
    //stream to output bit information to
    std::ostream & out;

public:
    //keeps track of bytes written
    int byteC = 0;
    
    //constructor for object
    BitOutputStream(std::ostream & os) : out(os), buf(0), bufCount(0){

    }

    //returns the bufCount to see if the byte should be written
    //one last time before ending 
    int getbufCount(){
        return bufCount;
    }

    //method to call to write to file
    void writeBit(string c){
         
        while(!(c.empty())){
            //only fill the buffer a max of 8 bits
            for(int i = 0; i < 8; i++){

                //check what this string is equal to
                if(c[0] == '0'){ 
                    //increase buffer count
                    bufCount++;
                    //dont do anything
                    c = c.substr(1,c.size());
                }
                else{
                    //update the buffer
                    buf = buf | (1 << (7-bufCount));
                    bufCount++;
                    //update the string if 1 = size returns empty
                    c = c.substr(1,c.size()); 
                }

                //empty buffer it it reaches bufSize before returning 
                if(bufCount == 8){
                    flush();
                }
                //make sure string is not empty    
                if(c.size() == 0){
                    //if encoding is less than 8 bits 
                    return;
                }
            }
        }    
    }
    //writes a ecoded symbol to the outfile
    void writeByte(byte sym){
        int ascii = sym;
        //translates int to binary
        string binary = bitset<8>(ascii).to_string();
        writeBit(binary);
    }
    
    //writes the count of read in chars as 4 bytes
    void writeCount(unsigned int c){
        //translates a unsigned int to binary
        string size = bitset<32>(c).to_string();
        writeBit(size);
    }
    
    void flush(){
        //use methods from ostream to output to file
        out.put(buf);
        out.flush();
        //reset for next byte
        buf = bufCount = 0;
        //keep track of how many bytes have been written
        byteC++;
    }

};

#endif 
