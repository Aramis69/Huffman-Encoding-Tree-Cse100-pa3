/* File: BitInputStream.hpp
 * Sources: The idea of how to structure this code came from lecture slides
 * on how to do the bit streams some of the code is directly copied.
 *
 * This file helps to get bytes back from bit values by doing bitwise
 * operations. It is used in uncompress.cpp
 *
 * */

#ifndef BITINPUTSTREAM_HPP
#define BITINPUTSTREAM_HPP

#include <iostream>
#include <fstream>

using namespace std;

/* Opject of this type are used to translate inputed Bit files to 
 * regular output streams
 * */
class BitInputStream{
private:
    //used to dump to output files when full
    char buf;
    //keeps track of which bit should be extracted
    int bufCount;
    //I stream that we output to
    std::istream & input;

public:
    //constructor for object
    BitInputStream(std::istream & i) : input(i), buf(0), bufCount(0){
    
    }

    //method to call to write to file
    int readBit(){
        //only get from input stream if not previusly set
        if(bufCount == 0){
            //set the read in char
            buf = input.get();
            //check for end of file
            if(input.eof()){
                return -1;   
            }
        }

        //extract the bit left off at last position and save to new char
        unsigned char masked = (buf & (1 << (7-bufCount)));
        //move back to see if bit was extracted
        masked = masked >> (7-bufCount);     
        //increase the count of bits moved through
        bufCount++;   
        //check if buf is full 
        if(bufCount == 8){
            bufCount = buf = 0;
        }
        
        //convert extracted bit to int
        int bit = masked;
        return bit;
    } 
    
    //reads a byte of information used for symbols
    int readByte(){
        //holds number this byte holds
        int sum = 0;
        for(int i = 0; i<8; i++){
            //when you encounter a value
            if(readBit() == 1){
                //add to sum
                sum = sum + (1 << (7-i));
            }
        }
        return sum;
    }

    //reads the first 4 bytes which represent the count
    unsigned int readCount(){
        //holds number this byte holds
        unsigned int sum = 0;
        for(unsigned int i = 0; i<32; i++){
            //when you encounter a value
            if(readBit() == 1){
                //add to sum
                sum = sum + (1 << (31-i));
            }
        }
        return sum;
    }

};

#endif 


