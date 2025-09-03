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

  int error;
  NormalFileReader file = NormalFileReader(path, &error);
  if (error != 0) {
    std::cout << "Error: cannot import 3D figure in the path " << path << std::endl;
    return;
  }

  auto erase_all_info = [&] () {
    this->info->vectors.clear();
    this->info->normals.clear();
    this->info->mapping.clear();
    this->info->indeces.clear();
    this->texture = SDL_Color {
      .r = 255,
      .g = 255,
      .b = 255,
      .a = 255,
    };
  };
        
  if (uv_hiding) {
    this->info->mapping.push_back(Dir2({0,0}));
  }

  float elem1, elem2, elem3;
  int obj_count = 0;
  std::string str;
  while (!file.finished()) {
    str = file.read_word();

    /* the current line of the file corresponds to... */
    /* ... a positon vector(three dimentions). */
    if (str == "v") {
      elem1 = file.read_float(&error);
      if (error) {
        std::cout 
          << "Error in position vector while reading " << path 
          << ": float number malformed(1)." << std::endl;
        erase_all_info();
        return;
      }

      elem2 = file.read_float(&error);
      if (error) {
        std::cout 
          << "Error in position vector while reading " << path 
          << ": float number malformed(2)." << std::endl;
        erase_all_info();
        return;
      }

      elem3 = file.read_float(&error);
      if (error) {
        std::cout 
          << "Error in position vector while reading " << path 
          << ": float number malformed(3)." << std::endl;
        erase_all_info();
        return;
      }

      this->info->vectors.push_back( Dir3 { elem1, elem2, elem3 } );

    /* ... a normal vector(three dimentions). */
    } else if (str == "vn") {
      elem1 = file.read_float(&error);
      if (error) {
        std::cout 
          << "Error in normal vector while reading " << path 
          << ": float number malformed(1)." << std::endl;
        erase_all_info();
        return;
      }

      elem2 = file.read_float(&error);
      if (error) {
        std::cout 
          << "Error in normal vector while reading " << path 
          << ": float number malformed(2)." << std::endl;
        erase_all_info();
        return;
      }

      elem3 = file.read_float(&error);
      if (error) {
        std::cout 
          << "Error in normal vector while reading " << path 
          << ": float number malformed(3)." << std::endl;
        erase_all_info();
        return;
      }

      this->info->normals.push_back( Dir3 { elem1, elem2, elem3 } );

    /* ... a uv mapping vector(two dimentions). */
    } else if (str == "vt") {
      elem1 = file.read_float(&error);
      if (error) {
        std::cout 
          << "Error in texture vector while reading " << path 
          << ": float number malformed(1)." << std::endl;
        erase_all_info();
        return;
      }

      elem2 = file.read_float(&error);
      if (error) {
        std::cout 
          << "Error in texture vector while reading " << path 
          << ": float number malformed(2)." << std::endl;
        erase_all_info();
        return;
      }

      if (!uv_hiding)
        this->info->mapping.push_back( Dir2 { elem1, elem2 } );

    /* ... the information of a face. */
    } else if (str == "f") {
      Face ret;
      uint64_t arr[3];

      for (int i = 0; i < 3; i++) {
        str = file.read_word();
        char * last;
        auto parts = std::views::split(str, '/');

        /* Allways n/m/p structure... */
        if (std::ranges::distance(parts) != 3) {
          std::cout 
            << "Error in face while reading " << path 
            << ": structure different from n/m/p for faces were found." << std::endl;
          erase_all_info();
          return;
        }
        
        /* read the ints. */
        int many = 0;
        for (const auto str: parts) {
          uint64_t res = strtoull(str.data(), &last, 0);

          if (res == 0) {
            std::cout 
              << "Error in face while reading " << path 
              << ": integer number malformed." << std::endl;
            erase_all_info();
            return;
          }

          arr[many++] = res - 1;
        }

        /* evaluating if represents correct positions. */

        if (this->info->vectors.size() <= arr[0]) {
          std::cout 
            << "Error in face while reading " << path 
            << ": integer number represents incorrect position(v)." << std::endl;
          erase_all_info();
          return;
        } else ret.pos[i] = arr[0];

        if (uv_hiding) ret.map[i] = 0;
        else {
          if (this->info->mapping.size() <= arr[1]) {
            std::cout 
              << "Error in face while reading " << path 
              << ": integer number represents incorrect position(vt)." << std::endl;
            erase_all_info();
            return;
          } else ret.map[i] = arr[1];
        }

        if (i == 0) {
          if (this->info->normals.size() <= arr[2]) {
            std::cout 
              << "Error in face while reading " << path 
              << ": integer number represents incorrect position(vn)." << std::endl;
            erase_all_info();
            return;
          } else ret.nor = arr[2];
        } else {
          if (ret.nor != arr[2]) {
            std::cout 
              << "Error in face while reading " << path 
              << ": found two different normals for the same face(vn)." << std::endl;
            erase_all_info();
            return;
          }
        }
      }

      this->info->indeces.push_back(ret);

    /* ... a new object(only one object accepted). */
    } else if (str == "o") {
      obj_count++;
      if (obj_count >= 2) {
        std::cout 
          << "Error in obj while reading " << path 
          << ": There were found more than one object in the file," 
          << "finishing reading." << std::endl;
        return;
      }
    }

    /* skipping data until the end of the line. */
    file.skip_until('\n');
  }

  /* Aggregating an image if present. */
  if (img != "") {
    int w, h;
    Uint32* pixels = charging_PNG_to_memory (img, w, h);
    if (pixels != nullptr) {
      SDL_Surface * readed_img = SDL_CreateRGBSurfaceFrom (
        pixels, w, h, 32, w * 4, 
        0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
      );
      if (readed_img == nullptr) {
        std::cout 
          << "Error while reading " << img
          << "error found while reading the image." << std::endl;
        delete [] pixels;
        this->texture = SDL_Color {.r = 255, .g = 0, .b = 255, .a = 255};
      } else this->texture = readed_img;
    }
  }
}
