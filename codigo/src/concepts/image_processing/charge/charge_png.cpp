#include "../../../../headers/concepts/image_processing.hpp"
#include "../low_level_functions/zlib_algorithm.cpp"
#include "../low_level_functions/read_bytes.cpp"

#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <array>

const std::size_t SHIFTING_BUFFER_SIZE = 12;
const std::size_t BUFFER_SIZE = 1 << SHIFTING_BUFFER_SIZE;

Texture chargePNG (const std::string& path) {
  std::fstream fil(path);
  fil.open (path);
  if (!fil.is_open ()) {
    fil.close ();
    return Texture ();
  }

  fil.seekg (0, std::ios::end);
  std::size_t count = fil.tellg();
  if (count == -1) {
    fil.close ();
    return Texture ();
  }
  fil.seekg (0, std::ios::beg);

  std::vector<uint8_t> buff;
  buff.reserve (count);
  fil.read ((char* )buff.data (), count);
  fil.close ();

  uint64_t pos = 8;
  
  /* Verify signature. */
  if (buff[pos] != 0x89 || buff[pos + 1] != 0x50 || buff[pos + 2] != 0x4E || buff[pos + 3] != 0x47 || 
      buff[pos + 4] != 0x0D || buff[pos + 5] != 0x0A || buff[pos + 6] != 0x1A || buff[pos + 7] != 0x0A) {
    return Texture ();
  }

  /* Definition of values. */
  int width = 0;
  int height = 0;
  int bitdepth = 0;
  int colortype = 0;
  int compmethod = 0;
  int filtmethod = 0;
  int intemethod = 0;

  int sampledepth = 0;

  int plet_chnk_count = 0;

  std::array<SDL_Color, 256> palette;
  int palette_count = 0;
  std::vector<uint16_t> grayscale;
  struct Color16{uint16_t r; uint16_t g; uint16_t b;};
  std::vector<Color16> truecolor;

  std::vector<uint8_t> idat_data;
    
  /* Handling of IHDR chunk. */
  uint32_t info_lenght = read_bytes(&buff[0], pos, 4);
  if (read_bytes(&buff[0], pos, 4) != 0x49484452) return Texture ();

  /* Reading image parameters. */
  width = read_bytes(&buff[0], pos, 4);
  height = read_bytes(&buff[0], pos, 4);
  bitdepth = read_bytes(&buff[0], pos, 1);
  colortype  = read_bytes(&buff[0], pos, 1);
  compmethod = read_bytes(&buff[0], pos, 1);
  filtmethod = read_bytes(&buff[0], pos, 1);
  intemethod = read_bytes(&buff[0], pos, 1);

  if (width == 0 || height == 0) {
    std::cout 
      << "PNG reading error: size of the total image must not be zero(width="
      << width << ", height=" << height << ")." << std::endl;
    return Texture();
  }

  if (((bitdepth - 1) & bitdepth) != 0) {
    std::cout 
      << "PNG reading error: bit depth(" << bitdepth << ") unsupported."
      << std::endl;
    return Texture();
  }

  sampledepth = bitdepth;
  switch (colortype) {
    case 2: case 4: case 6:
      if ((bitdepth & 0x18) == 0) {
        std::cout 
          << "PNG reading error: bit depth(" << bitdepth << ") unsupported."
          << std::endl;
        return Texture();
      }
      break;
    case 3:
      if ((bitdepth & 0x1F) == 0) {
        std::cout 
          << "PNG reading error: bit depth(" << bitdepth << ") unsupported."
          << std::endl;
        return Texture();
      }
      sampledepth = 8;
      break;
    case 0:
      break;
    default: 
      return Texture();
  }

  if (compmethod != 0) {
    std::cout 
      << "PNG reading error: descompression method(code: " << compmethod << ") not recognized."
      << std::endl;
    return Texture();
  }

  if (filtmethod != 0) {
    std::cout 
      << "PNG reading error: filter method(code: " << filtmethod << ") not recognized."
      << std::endl;
    return Texture();
  }

  if (intemethod > 2) {
    std::cout 
      << "PNG reading error: interlace method(code: " << intemethod << ") not recognized."
      << std::endl;
    return Texture();
  }

  idat_data.reserve (height * width);

  /* Reading the rest of the file. */
  bool looping = true;
  while (pos < buff.size() && looping) {
    info_lenght = read_bytes(&buff[0], pos, 4);
    switch (read_bytes(&buff[0], pos, 4)) {

      /* PLET */
      case 0x504C5445:
        if (colortype == 4 || colortype == 0) { 
          std::cout 
            << "PNG reading error: PLET chunk present when it must not."
            << std::endl;
          return Texture ();
        }
        palette_count = info_lenght / 3;
        if (plet_chnk_count > 0 || info_lenght % 3 != 0 || palette_count > 1 << bitdepth) { 
          std::cout 
            << "PNG reading error: PLET chunk has an unreadable format or gives contradictory"
            << " information due the currently obtained."
            << std::endl;
          return Texture ();
        }
        plet_chnk_count++;

        for (int i = 0; i < palette_count; i++) {
          palette[i] = {
            .r = static_cast<Uint8>(read_bytes(&buff[0], pos, 1)),
            .g = static_cast<Uint8>(read_bytes(&buff[0], pos, 1)),
            .b = static_cast<Uint8>(read_bytes(&buff[0], pos, 1)),
            .a = 255,
          };
        }
        break;
  
      /* IDAT */
      case 0x49444154:
        for (int i = 0; i < info_lenght; i++)
          idat_data.push_back (read_bytes(&buff[0], pos, 1));

      /* tRNS */
      case 0x74524E53:
        if (colortype == 3) {
          if (palette_count < info_lenght) {
            std::cout 
              << "PNG reading error: number of alpha entries in tRNS exceeded PLET colors."
              << std::endl;
            return Texture ();
          }
          for (int i = 0; i < info_lenght; i++)
            palette[i].a = read_bytes(&buff[0], pos, 1);
        } else if (colortype == 0) {
          if (height * width < info_lenght) {
            std::cout 
              << "PNG reading error: number of grayscale entries in tRNS exceeded the number of pixels."
              << std::endl;
            return Texture ();
          }
          for (int i = 0; i < info_lenght; i++)
            grayscale[i] = read_bytes(&buff[0], pos, 2);
        } else if (colortype == 0) {
          if (height * width < info_lenght) {
            std::cout 
              << "PNG reading error: number of truecolor entries in tRNS exceeded the number of pixels."
              << std::endl;
            return Texture ();
          }
          for (int i = 0; i < info_lenght; i++)
            truecolor[i] = {
            .r = static_cast<Uint8>(read_bytes(&buff[0], pos, 2)),
            .g = static_cast<Uint8>(read_bytes(&buff[0], pos, 2)),
            .b = static_cast<Uint8>(read_bytes(&buff[0], pos, 2)),
            };
        } else {
          std::cout 
            << "PNG reading error: tRNS is not defined for color type " << colortype << "."
            << std::endl;
          return Texture ();
        }

      /* IEND */
      case 0x49454E44:
        pos += 4;
        looping = false;

      /* The unused chunks are disregarded. */
      default:
        pos += info_lenght;
    }
    /* CRC no usado actualmente(lo desprecio). */
    pos += 4;
  }

  /* Descompression of the image. */
  

  return Texture();
}
