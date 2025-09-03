#include "../../../../../headers/concepts/texture.hpp"
#include "../../../file_processing/readers/normal_file_readed.cpp"
#include <cstdint>
#include <iostream>
#include <ranges>
#include <vector>
#include <cstdlib>

/* Constructs a 3D figure due the information given a .obj file referenced by the 
 * variable path. If it will be assosiated with an image, it is referenced by the 
 * varialbe img. Both are paths to the corresponding files. The uv_hiding variable 
 * tells the implementation to not read the uv mapping vectors, so that a plain 
 * object could be defined.
 * */
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

    /* the current line of the file corresponds to... */
    /* ... a positon vector(three dimentions). */
    if (str == "v") {
      this->info->vectors.push_back(
        Dir3 { file.reading_float(), file.reading_float(), file.reading_float() }
      );

    /* ... a normal vector(three dimentions). */
    } else if (str == "vn") {
      this->info->normals.push_back(
        Dir3 { file.reading_float(), file.reading_float(), file.reading_float() }
      );

    /* ... a uv mapping vector(two dimentions). */
    } else if (str == "vt") {
      if (uv_hiding) {
        file.reading_word();
        file.reading_word();
      } else {
        this->info->mapping.push_back(
          Dir2 { file.reading_float(), file.reading_float() }
        );
      }

    /* ... the information of a face. */
    } else if (str == "f") {
      Face ret;
      uint64_t arr[3];

      for (int i = 0; i < 3; i++) {
        str = file.reading_word();
        char * last;
        auto parts = std::views::split(str, '/');

        if (std::ranges::distance(parts) != 3) {
          std::cout 
            << "Error: the file " << path  << " has faces with more than 3 verteces." 
            << std::endl;
          std::exit(-1);
        }
        
        int many = 0;
        for (const auto str: parts) {
          uint64_t res = strtoull(str.data(), &last, 0);

          if (res == 0) {
            std::cout 
              << "Error: the file " << path << " hasn't a proper structure(2)." 
              << std::endl;
            std::exit(-1);
          }

          arr[many++] = res - 1;
        }

        if (this->info->vectors.size() <= arr[0]) {
          std::cout 
            << "Error: the file " << path << " hasn't a proper structure(3)."
            << std::endl;
          std::exit(-1);
        } else {
          ret.pos[i] = arr[0];
        }

        if (uv_hiding) ret.map[i] = 0;
        else {
          if (this->info->mapping.size() <= arr[1]) {
            std::cout 
              << "Error: the file " << path << " hasn't a proper structure(4)."
              << std::endl;
            std::exit(-1);
          } else {
            ret.map[i] = arr[1];
          }
        }

        if (i == 0) {
          if (this->info->normals.size() <= arr[2]) {
            std::cout 
              << "Error: the file " << path << " hasn't a proper structure(5)."
              << std::endl;
            std::exit(-1);
          } else {
            ret.nor = arr[2];
          }
        } else {
          if (ret.nor != arr[2]) {
            std::cout 
              << "Error: the file " << path << " hasn't a proper structure(6)."
              << std::endl;
            std::exit(-1);
          }
        }
      }

      this->info->indeces.push_back(ret);

    /* ... a new object(only one object accepted). */
    } else if (str == "o") {
      obj_count++;
      if (obj_count >= 2)
        file.finish();
    }

    /* skipping data until the end of the line. */
    file.skipping('\n');
  }

  /* Aggregating an image if present. */
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
          << path << "." << std::endl;
        std::exit(-1);
      }
    }
  }
}
