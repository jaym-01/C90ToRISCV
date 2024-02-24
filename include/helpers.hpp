#ifndef HELPERS_HPP
#define HELPERS_HPP

inline int align_to_multiple_of_4(int offset)
{
    int remainder = offset % 4;
    if (remainder == 0) {
        return offset;
    }
    else {
        return offset + (4 - remainder);
    }
}

inline int calculate_var_offset(int cur_offset, std::string type) {
    if (type == "int") {
        return align_to_multiple_of_4(cur_offset - 4);
    }

    else if (type == "char") {
        return cur_offset - 1;
    }

    else if (type == "float") {
        return align_to_multiple_of_4(cur_offset - 4);
    }

    else if (type == "double") {
        // return cur_offset - 8;
    }

    else {
        return align_to_multiple_of_4(cur_offset - 4);
    }
}

inline int calculate_var_size(std::string type)
{
    if (type == "int")
        return 4;

    else if (type == "char")
        return 4;

    else if (type == "float")
        return 4;

    else if (type == "double")
        return 8;

    else return 4;
}

#endif
