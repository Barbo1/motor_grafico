#include "../../../../../headers/concepts/texture.hpp"
#include <cstdint>
#include <iostream>
#include <fstream>
#include <ranges>
#include <vector>
#include <cstdlib>

const int NFR_BUFFER_LIMIT = 1024;
class NormalFileReader {
  private:
    std::fstream fil;
    char buffer[NFR_BUFFER_LIMIT];
    int pos, lim;
    bool is_lim;
    std::string path;

  public:
    NormalFileReader (std::string path) {
      this->path = path;
      this->fil = std::fstream (path);

      if (!fil.is_open()) {
        std::cout 
          << "Error: cannot open the file while charging vizualizer from " + path + "." 
          << std::endl;

        this->lim = this->pos;
        this->is_lim = true;
      } else {
        this->pos = NFR_BUFFER_LIMIT - 1;
        this->is_lim = false;
      }
    }

    bool finished () {
      return this->is_lim && this->pos == this->lim;
    }

    void finish () {
      this->is_lim = true;
      this->lim = this->pos;
    }
  
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

    std::string reading_word () {
      char let;
      std::string str = "";
      while((let = this->reading()) != ' ' && let != '\n' && let != EOF)
        str += let;
      return str;
    }

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

    std::string get_path () {
      return this->path;
    }

    ~NormalFileReader () {
      this->fil.close();
    }
};

Visualizer<D3FIG>::Visualizer (std::string path, std::string img, bool uv_hiding) {
  this->info = new Info {
    .vectors = std::vector<Dir3>(), 
    .normals = std::vector<Dir3>(),
    .mapping = std::vector<Dir2>(),
    .indeces = std::vector<Face>()
  };
  this->use_count = new int(1);

  NormalFileReader file = NormalFileReader(path);
  std::string str;
        
  if (uv_hiding) {
    this->info->mapping.push_back(Dir2({0,0}));
  }

  int obj_count = 0;
  while (!file.finished()) {
    str = file.reading_word();

    if (str == "v") {
      this->info->vectors.push_back(
        Dir3 { file.reading_float(), file.reading_float(), file.reading_float() }
      );

    } else if (str == "vn") {
      this->info->normals.push_back(
        Dir3 { file.reading_float(), file.reading_float(), file.reading_float() }
      );

    } else if (str == "vt") {
      if (uv_hiding) {
        file.reading_word();
        file.reading_word();
      } else {
        this->info->mapping.push_back(
          Dir2 { file.reading_float(), file.reading_float() }
        );
      }

    } else if (str == "f") {
      Face ret;
      uint64_t arr[3];

      for (int i = 0; i < 3; i++) {
        str = file.reading_word();
        char * last;
        auto parts = std::views::split(str, '/');

        if (std::ranges::distance(parts) != 3) {
          std::cout 
            << "Error: the file " << file.get_path()  << " has faces with more than 3 verteces." 
            << std::endl;
          std::exit(-1);
        }
        
        int many = 0;
        for (const auto str: parts) {
          uint64_t res = strtoull(str.data(), &last, 0);

          if (res == 0) {
            std::cout 
              << "Error: the file " << file.get_path() << " hasn't a proper structure(2)." 
              << std::endl;
            std::exit(-1);
          }

          arr[many++] = res - 1;
        }

        if (this->info->vectors.size() <= arr[0]) {
          std::cout 
            << "Error: the file " << file.get_path() << " hasn't a proper structure(3)."
            << std::endl;
          std::exit(-1);
        } else {
          ret.pos[i] = arr[0];
        }

        if (uv_hiding) ret.map[i] = 0;
        else {
          if (this->info->mapping.size() <= arr[1]) {
            std::cout 
              << "Error: the file " << file.get_path() << " hasn't a proper structure(4)."
              << std::endl;
            std::exit(-1);
          } else {
            ret.map[i] = arr[1];
          }
        }

        if (i == 0) {
          if (this->info->normals.size() <= arr[2]) {
            std::cout 
              << "Error: the file " << file.get_path() << " hasn't a proper structure(5)."
              << std::endl;
            std::exit(-1);
          } else {
            ret.nor = arr[2];
          }
        } else {
          if (ret.nor != arr[2]) {
            std::cout 
              << "Error: the file " << file.get_path() << " hasn't a proper structure(6)."
              << std::endl;
            std::exit(-1);
          }
        }
      }

      this->info->indeces.push_back(ret);

    } else if (str == "o") {
      obj_count++;
      if (obj_count >= 2)
        file.finish();
    }

    file.skipping('\n');
  }

  /* Agregado de imagen. */
  if (img != "") {
    int w, h;
    Uint32* pixels = charging_PNG_to_memory (img, w, h);
    if (pixels != nullptr) {
      this->texture = SDL_CreateRGBSurfaceFrom (
        pixels, w, h, 32, w * 4, 
        0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
      );
      if (std::get<SDL_Surface*>(this->texture) == nullptr) {
        std::cout 
          << "Error: There were an error charging the image from the path" 
          << file.get_path() << "." << std::endl;
        std::exit(-1);
      }
    }
  }
}
