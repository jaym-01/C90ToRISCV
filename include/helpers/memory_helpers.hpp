#ifndef MEM_HELPERS_HPP
#define MEM_HELPERS_HPP

// brings the offset to the start of the word
inline int align_word(int offset){
    int rem = (offset)%4;

    if(rem == 0) return offset;
    else return offset - rem - 4;
}

#endif
