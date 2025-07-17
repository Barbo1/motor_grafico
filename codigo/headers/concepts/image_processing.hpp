/* 
 * Here are some algorithms to manipulate images, like charging them, make 
 * set operations between them, and so more.
 * */

#include "./texture.hpp"

#include <string>

Texture chargePNG (const std::string& path);
Texture chargeJPEG (const std::string& path);
Texture chargeBMP (const std::string& path);
