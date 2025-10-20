#pragma once

#include <fstream>
#include <vector>
#include <cstdint>

/* This object is used to read files securely and effitiently in space. It is meant to read data 
 * by characters and sequentialy. The SQT_BUFF_LIMIT is the length of the internal buffer size 
 * used to store the data readed. The errors returned could be:
 *  ->  0: no error,
 *  -> -1: an EOF error: the folder is setted in finished state,
 *  -> -2: an reading error: inconsistent information between readed and expected.
 * */

const int SQT_BUFF_LIMIT = 1024;

class SequentialFileReader {
  private:
    std::ifstream fil;
    std::string path;
    std::size_t pos, lim, curr_file_pos, lim_file_pos;
    char buffer[SQT_BUFF_LIMIT];
    bool is_lim;

  public:
    SequentialFileReader (std::string path, int * error);
    ~SequentialFileReader ();

    /* Test or set the file in finished mode. */
    bool finished ();
    void finish ();

    /* Discards elements until found the stop character, or n exact positions. */
    void skip_until (std::size_t n);
    void skip_until (char stop);

    /* read the next char or word of data in the file. */
    char read_char ();
    std::string read_word ();

    /* Read a float or int in the file and return it. This is meant to be used 
     * when the data readed constitutes a word, which is entirely the element.
     * */
    uint64_t read_int (int * error);
    float read_float (int * error);

    /* Read n bytes. For each byte, if it is equal to EOF, returns 0. */
    uint8_t read8 ();
    uint16_t read16 ();
    uint32_t read32 ();
    uint64_t read64 ();

    /* Read bytes to fill the array from position s to position f. */
    void fill_buffer (uint8_t* arr, std::size_t s, std::size_t f);
    std::vector<uint8_t> get_vector (std::size_t f);

    /* Tests if there are at least n spare elements. */
    bool spare(std::size_t n);
};
