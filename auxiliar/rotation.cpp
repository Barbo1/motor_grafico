void Texture::rotate (float angle) {
  /*
  static float coeffitients[7][3] = {
    {1.570796327, -1, 1},
    {1.28700579, -3.f/4.f, 24.f/25},
    {1.17600285, -2.f/3, 12.f/13},
    {0.927293432, -1.f/2, 4.f/5},
    {0.643502895, -1.f/3, 3.f/5},
    {0.489913921, -1.f/4, 8.f/17},
    {0.394793477, -1.f/5, 5.f/13},
  };

  int i = 0;
  float * res = nullptr;
  while (i < 7 && angle < (res = coeffitients[i++])[0]);
*/
  float alpha = 1-tan(angle);
  float beta = sin (angle);
  float newX, newY;
  for (const auto con: this->_level) {
    for (SDL_Point * piter = con.second.points; piter < con.second.points + con.second.points_many; piter++) {
      newY = piter->y;
      newX = piter->x;
      newX += round(alpha * newY);
      newY += round(beta * newX);
      newX += round(alpha * newY);
      *piter = {
        static_cast<int>(lroundf(newX)),
        static_cast<int>(lroundf(newY)),
      };
    }
  }
  /*
  float sinA = std::sin (angle);
  float cosA = std::cos (angle);
  for (const auto con: this->_level) {
    for (SDL_Point * piter = con.second.points; piter < con.second.points + con.second.points_many; piter++) {
      *piter = {
        static_cast<int>(float (piter->x) * cosA + float (piter->y) * sinA),
        static_cast<int>(-float (piter->x) * sinA + float (piter->y) * cosA),
      };
    }
  }
  */
}

