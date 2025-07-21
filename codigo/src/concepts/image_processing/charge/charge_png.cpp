#include "../../../../headers/concepts/texture.hpp"
#include "../low_level_functions/zlib_algorithm.cpp"
#include "../low_level_functions/read_bytes.cpp"

#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <bitset>

const std::size_t SHIFTING_BUFFER_SIZE = 12;
const std::size_t BUFFER_SIZE = 1 << SHIFTING_BUFFER_SIZE;

uint8_t paeth_predictor (int16_t left, int16_t above, int16_t diagonal) {
  int16_t p = left + above - diagonal;
  int16_t pa = std::abs (p - left);
  int16_t pb = std::abs (p - above);
  int16_t pc = std::abs (p - diagonal);

  if (pa <= pb && pa <= pc) 
    return static_cast<uint8_t>(left);
  else if (pb <= pc) 
    return static_cast<uint8_t>(above);
  else 
    return static_cast<uint8_t>(diagonal);
}

Texture chargePNG (SDL_Renderer* render, const std::string& path) {
  std::fstream fil(path, std::ios::binary);
  fil.open (path);
  if (!fil.is_open ()) {
    std::cout << "The file was not found." << std::endl;
    return Texture ();
  }

  fil.seekg (0, std::ios::end);
  std::size_t count = fil.tellg();
  if (count < 0) {
    fil.close ();
    std::cout << "Final position not found." << std::endl;
    return Texture ();
  }
  fil.seekg (0, std::ios::beg);

  std::vector<uint8_t> buff;
  buff.resize (count);
  if (!fil.read ((char* )buff.data (), count)) {
    std::cout << "Error while reading data." << std::endl;
    return Texture ();
  }
  fil.close ();

  uint64_t pos = 0;
  
  /* Verify signature. */
  if (buff[pos] != 0x89 || buff[pos + 1] != 0x50 || buff[pos + 2] != 0x4E || buff[pos + 3] != 0x47 || 
      buff[pos + 4] != 0x0D || buff[pos + 5] != 0x0A || buff[pos + 6] != 0x1A || buff[pos + 7] != 0x0A) {
    std::cout << "Failure in signature recognition." << std::endl;
    return Texture ();
  }
  pos += 8;

  /* Handling of IHDR chunk. */
  if (read_bytes (&buff[0], pos, 4) != 13 ||
      static_cast<uint32_t> (read_bytes (&buff[0], pos, 4)) != 0x49484452) { 
    std::cout << "IHDR chunk not founded." << std::endl;
    return Texture ();
  }

  /* Reading image parameters. */
  int width = read_bytes(&buff[0], pos, 4);
  int height = read_bytes(&buff[0], pos, 4);
  int bitdepth = read_bytes(&buff[0], pos, 1);
  int colortype  = read_bytes(&buff[0], pos, 1);
  int compmethod = read_bytes(&buff[0], pos, 1);
  int filtmethod = read_bytes(&buff[0], pos, 1);
  int intemethod = read_bytes(&buff[0], pos, 1);
  pos += 4;

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

  int sampledepth = bitdepth;
  int channels = 0;
  switch (colortype) {
    case 6:
      channels++;
    case 2: 
      channels++;
    case 4: 
      channels += 2;
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
    case 0:
      channels = 1;
      break;
    default: 
      std::cout 
        << "PNG reading error: color type(" << colortype << ") unsupported."
        << std::endl;
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
  } else if (intemethod == 1) {
    std::cout << "interlace method equal to 1." << std::endl;
  }

  /* Palette information. */
  std::vector<SDL_Color> palette;
  int palette_count = 0;
  int plet_chnk_count = 0;

  /* grayscale information. */
  std::vector<uint16_t> grayscale;

  /* true color information. */
  struct Color16{uint16_t r; uint16_t g; uint16_t b;};
  std::vector<Color16> truecolor;

  /* IDAT information. */
  std::vector<uint8_t> idat_data;
  idat_data.reserve (height * width);

  /* Reading the rest of the file. */
  bool looping = true;
  uint32_t info_lenght;
  while (pos < buff.size() && looping) {
    info_lenght = read_bytes(&buff[0], pos, 4);
    switch (read_bytes(&buff[0], pos, 4)) {

      /* PLET */
      case 0x504C5445:
        std::cout << "reading PLET." << std::endl;
        if (colortype == 4 || colortype == 0) { 
          std::cout 
            << "PNG reading error: PLTE chunk present when it must not."
            << std::endl;
          return Texture ();
        }
        palette_count = info_lenght / 3;
        if (plet_chnk_count > 0 || info_lenght % 3 != 0 || palette_count > 1 << sampledepth) { 
          std::cout 
            << "PNG reading error: PLTE chunk has an unreadable format or gives contradictory"
            << " information due the currently obtained."
            << std::endl;
          return Texture ();
        }
        plet_chnk_count++;

        palette.resize (palette_count);
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
        std::cout << "reading IDAT." << std::endl;
        idat_data.insert (idat_data.end (), buff.begin() + pos, buff.begin() + pos + info_lenght);
        pos += info_lenght;
        break;

      /* tRNS */
      case 0x74524E53:
        std::cout << "reading tRNS." << std::endl;
        if (colortype > 3) {
          std::cout 
            << "PNG reading error: tRNS must not be defined for color type " 
            << colortype << "."
            << std::endl;
          return Texture ();
        } else if (colortype == 3) {
          if (palette_count < info_lenght) {
            std::cout 
              << "PNG reading error: number of alpha entries in tRNS exceeded PLTE colors."
              << std::endl;
            return Texture ();
          }
          for (int i = 0; i < info_lenght; i++)
            palette[i].a = read_bytes(&buff[0], pos, 1);
        } else if (colortype == 0) {
          if (height * width < info_lenght) {
            std::cout 
              << "PNG reading error: number of grayscale entries in tRNS exceeded the "
              << "number of pixels."
              << std::endl;
            return Texture ();
          }
          grayscale.resize (info_lenght);
          uint64_t aux = (1 << sampledepth) - 1;
          for (int i = 0; i < info_lenght; i++)
            grayscale[i] = ((float)read_bytes(&buff[0], pos, 2) / aux) * 255;
        } else if (colortype == 2) {
          if (height * width < info_lenght) {
            std::cout 
              << "PNG reading error: number of truecolor entries in tRNS exceeded " 
              << "the number of pixels."
              << std::endl;
            return Texture ();
          }
          truecolor.resize (info_lenght);
          for (int i = 0; i < info_lenght; i++)
            truecolor.push_back({
              .r = static_cast<Uint8>(read_bytes(&buff[0], pos, 2)),
              .g = static_cast<Uint8>(read_bytes(&buff[0], pos, 2)),
              .b = static_cast<Uint8>(read_bytes(&buff[0], pos, 2)),
            });
        }
        break;

      /* IEND */
      case 0x49454E44:
        std::cout << "reading IEND." << std::endl;
        pos += 4;
        looping = false;
        break;

      /* The unused chunks are disregarded. */
      default:
        std::cout << "reading unknown chunk." << std::endl;
        pos += info_lenght;
        break;
    }
    /* CRC no usado actualmente(lo desprecio). */
    pos += 4;
  }

  for (int i = 0; i < 20; i++) {
    std::cout << (int)idat_data[i] << std::endl;
  }

  /* Descompression of IDAT field. */
  std::vector<uint8_t> output;
  if (!zlib_discompression (idat_data, output))
    return Texture();

  std::cout << "esto" << std::endl;

  /* Apply reverse-filtering algorithm. */
  uint64_t many = 0;
  uint64_t bpp = (channels * sampledepth >> 3);
  uint64_t scanline = 1 + ((width * channels * sampledepth - 1) >> 3);

  while (many < output.size()) {
    switch (output[many++]) {
      case 0: break;
      case 1:
        for (int i = bpp + many, j = many; i < scanline + many; i++, j++)
          output[i] += output[j];
        break;
      case 2:
        if (many > scanline)
          for (int i = many, j = many - scanline - 1; i < scanline + many; i++, j++)
            output[i] += output[j];
        break;
      case 3:
        /* evaluo si es la primera linea. */
        if (many > scanline) {
          int i = many, j = many - scanline - 1;
          for (; i < many + bpp; i++, j++)
            output[i] += output[j] / 2;
          for (int k = many; i < scanline + many; i++, j++, k++)
            output[i] += ((uint16_t)output[j] + (uint16_t)output[k]) / 2;
        } else {
          /* mismo caso que en el 1 */
          for (int i = bpp + many, j = many; i < scanline + many; i++, j++)
            output[i] += output[j] / 2;
        }
        break;
      case 4:
        /* evaluo si es la primera linea. */
        if (many > scanline) {
          int i = many, j = many - scanline - 1;
          for (; i < many + bpp; i++, j++)
            output[i] += output[j];
          for (int k = many, l = many - scanline; i < scanline + many; i++, j++, k++, l++)
            output[i] += paeth_predictor (output[k], output[j], output[l]);
        } else {
          /* mismo caso que en el 1. */
          for (int i = bpp + many, j = many; i < scanline + many; i++, j++)
            output[i] += output[j];
        }

        break;
      default:
        std::cout 
          << "PNG reading error: filter type unknown."
          << std::endl;
        return Texture ();
    }
    many += scanline;
  }

  /* Interlace changing. */
  
  /* returning texture. */
  Uint32* pixels = new Uint32[height * width];
  uint64_t pixels_pos = 0;
  SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  pos = 8;

  switch (colortype) {
    case 6:
      std::cout << "color 6." << std::endl;
      for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
          pixels[pixels_pos++] = SDL_MapRGBA(
            format,
            access_bit (&output[0], pos, sampledepth),
            access_bit (&output[0], pos, sampledepth),
            access_bit (&output[0], pos, sampledepth),
            access_bit (&output[0], pos, sampledepth)
          );
        }
        pos += ((pos & 7) > 0) * (8 - (pos & 7));
        pos += 8;
      }
      break;
    case 4:
      std::cout << "color 4." << std::endl;
      for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
          Uint8 color = access_bit (&output[0], pos, sampledepth);
          pixels[pixels_pos++] = SDL_MapRGBA(
            format, color, color, color, access_bit (&output[0], pos, sampledepth)
          );
        }
        pos += ((pos & 7) > 0) * (8 - (pos & 7));
        pos += 8;
      }
      break;
    case 3:
      std::cout << "color 3." << std::endl;
      for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
          uint64_t plet_pos = access_bit (&output[0], pos, sampledepth);
          pixels[pixels_pos++] = SDL_MapRGBA(
            format,
            palette[plet_pos].r,
            palette[plet_pos].g,
            palette[plet_pos].b,
            palette[plet_pos].a
          );
        }
        pos += ((pos & 7) > 0) * (8 - (pos & 7));
        pos++;
      }
      break;
    case 2:
      std::cout << "color 2." << std::endl;
      for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
          pixels[pixels_pos++] = SDL_MapRGBA(
            format,
            access_bit (&output[0], pos, sampledepth),
            access_bit (&output[0], pos, sampledepth),
            access_bit (&output[0], pos, sampledepth),
            255
          );
        }
        pos += ((pos & 7) > 0) * (8 - (pos & 7));
        pos += 8;
      }
      break;
    case 0:
      std::cout << "color 0." << std::endl;
      for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
          Uint8 color = access_bit (&output[0], pos, sampledepth);
          pixels[pixels_pos++] = SDL_MapRGBA (
            format, color, color, color, 
            grayscale[i*(width + 1) + j]
          );
        }
        pos += ((pos & 7) > 0) * (8 - (pos & 7));
        pos += 8;
      }
      break;
    default:
      std::cout << "color type not found." << std::endl;
      return Texture();
  }
  
  SDL_FreeFormat(format);

  SDL_Point center = SDL_Point {.x = width / 2, .y = height / 2}; 
  Texture tex = Texture (render, height, width, center, pixels);
  delete [] pixels;

  return tex;
}
