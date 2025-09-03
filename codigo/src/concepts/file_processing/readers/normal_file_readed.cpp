#include <fstream>
#include <iostream>

/* This object is used to read files securely and effitiently in space. It 
 * is meant to read data by characters of 8 bits(bytes), hence the Normal 
 * part of the name. The NFR_BUFFER_LIMIT is the internal buffer size used
 * to read the data. 
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
    NormalFileReader (std::string path) {
      this->path = path;
      this->fil = std::ifstream (path, std::ios::in | std::ios::binary);

      if (!fil.is_open()) {
        std::cout 
          << "Error: cannot open the file " + path + "." 
          << std::endl;

        this->lim = this->pos;
        this->is_lim = true;
      } else {
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
    char reading () {
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
    void skipping (char stop) {
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
    std::string reading_word () {
      char let;
      std::string str = "";
      while((let = this->reading()) != ' ' && let != '\n' && let != EOF)
        str += let;
      return str;
    }

    /* Read a float in the file and return it. This is meant to be used 
     * when the data readed constitutes a word, which is entirely a float.
     * */
    float reading_float () {
      if (this->finished()) 
        return 0;

      std::string str = this->reading_word();
      char * last;
      float ret = strtof(str.c_str(), &last);
      if (*last != '\0') {
        std::cout 
          << "Error: the file " + this->path + " hasn't a proper structure(1)." 
          << std::endl;
        std::exit(-1);
      }
      return ret;
    }

    ~NormalFileReader () {
      this->fil.close();
    }
};
