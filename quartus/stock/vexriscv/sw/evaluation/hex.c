#include "hex.h"

uint32_t char_to_seg_binary(char x) {
    uint32_t ret;
    switch(x){
        case '0':
            ret = 0b1000000;
            break;        
        case '1':
            ret = 0b1111001;
            break;        
        case '2':
            ret = 0b0100100;
            break;        
        case '3':
            ret = 0b0110000;
            break;        
        case '4':
            ret = 0b0011001;
            break;        
        case '5':
            ret = 0b0010010;
            break;        
        case '6':
            ret = 0b0000010;
            break;
        case '7':
            ret = 0b1111000;
            break;        
        case '8':
            ret = 0b0000000;
            break;        
        case '9':
            ret = 0b0010000;
            break;        
        case 'a':
            ret = 0b0001000;
            break;        
        case 'b':
            ret = 0b0000011;
            break;        
        case 'c':
            ret = 0b1000110;
            break;        
        case 'd':
            ret = 0b0100001;
            break;        
        case 'e':
            ret = 0b0000110;
            break;        
        case 'f':
            ret = 0b0001110;
            break;
        default:
            ret = 0b1111111;
            break;
    } 
    return ret;
}
        
void hex_write_uint(uint32_t x, EightBitPio* hexPair) {
    uint16_t ones = x%10;
    uint16_t tens = (x%100 - ones)/10;
    char charOnes = ones + '0';
    char charTens = tens + '0';
    uint32_t writeVal = (char_to_seg_binary(charTens) << 7) | char_to_seg_binary(charOnes);
    hexPair->port = writeVal;
}