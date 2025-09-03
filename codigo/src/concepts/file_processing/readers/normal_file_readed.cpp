#include <fstream>
#include <cstdint>

/* This object is used to read files securely and effitiently in space. It is meant to read data 
 * by characters(hence the Normal part of the name) and sequentialy. The NFR_BUFFER_LIMIT is the 
 * length of the internal buffer size used to read the data. The errors returned could be:
 *  ->  0: no error,
 *  -> -1: an EOF error: the folder is seted in finished state,
 *  -> -2: an reading error: inconsistent information between readed and expected.
 * */

const int NFR_BUFFER_LIMIT = 1024;

class NormalFileReader {
  private:
    std::ifstream fil;
    char buffer[NFR_BUFFER_LIMIT];
    int pos, lim;
    bool is_lim;
    std::string path;

  public:
    /* Opens the file due the path. */
    NormalFileReader (std::string path, int * error) {
      this->path = path;
      this->fil = std::ifstream (path, std::ios::in | std::ios::binary);

      if (!fil.is_open()) {
        *error = -1;
        this->lim = this->pos;
        this->is_lim = true;
      } else {
        *error = 0;
        this->pos = NFR_BUFFER_LIMIT - 1;
        this->is_lim = false;
      }
    }

    /* Tests if all the data of the file where readed. */
    bool finished () {
      return this->is_lim && this->pos == this->lim;
    }

    /* Set the file in finished mode manualy. */
    void finish () {
      this->is_lim = true;
      this->lim = this->pos;
    }
  
    /* Function that returns the next character of data. */
    char read_char () {
      if (this->finished()) 
        return (char)EOF;

      this->pos++;

      if (this->pos == NFR_BUFFER_LIMIT) {
        this->fil.read(this->buffer, NFR_BUFFER_LIMIT);
        this->pos = 0;
        if (fil.eof()) {
          this->lim = this->fil.gcount();
          this->is_lim = true;
        }
      }
      return this->buffer[this->pos];
    }

    /* Reads characters until the character passed by parameter is found. */
    void skip_until (char stop) {
      while (!this->finished() && this->buffer[this->pos] != stop) {
        this->pos++;
        if (this->pos == NFR_BUFFER_LIMIT) {
          this->fil.read(this->buffer, NFR_BUFFER_LIMIT);
          this->pos = 0;
          if (this->fil.eof()) {
            this->lim = this->fil.gcount();
            this->is_lim = true;
          }
        }
      }
    } 

    /* Read a word in the file and return it. */
    std::string read_word () {
      char let;
      std::string str = "";
      while((let = this->read_char()) != ' ' && let != '\n' && let != EOF)
        str += let;
      return str;
    }

    /* Read a float in the file and return it. This is meant to be used 
     * when the data readed constitutes a word, which is entirely a float.
     * */
    uint64_t read_int (int * error) {
      if (this->finished()) {
        *error = -1;
        return 0;
      } else *error = 0;

      std::string str = this->read_word();
      char * last;
      uint64_t ret = strtoull(str.c_str(), &last, 0);
      if (*last != '\0')
        *error = -2;
      return ret;
    }

    /* Read a int in the file and return it. This is meant to be used 
     * when the data readed constitutes a word, which is entirely an int.
     * */
    float read_float (int * error) {
      if (this->finished()) {
        *error = -1;
        return 0;
      } else *error = 0;

      std::string str = this->read_word();
      char * last;
      float ret = strtof(str.c_str(), &last);
      if (*last != '\0')
        *error = -2;
      return ret;
    }

    ~NormalFileReader () {
      this->fil.close();
    }
};
