#pragma once
#include <string>
#include <cstdint>
#include <ml_exception.h>
#include <matrix.h>

using namespace std;

class dataset_utils
{
private:
    dataset_utils() {} 
    // private constructor: dataset_utils cannot be instantiated or extended

public:
    static void split(string dataset_name, uint8_t matrix_1_percentage, uint32_t skip_lines, matrix &matrix_1, matrix &matrix_2); 
    static void split(string dataset_name, uint8_t file_1_percentage, uint32_t number_of_lines_in_header, const string &file_1, const string &file_2);
};
