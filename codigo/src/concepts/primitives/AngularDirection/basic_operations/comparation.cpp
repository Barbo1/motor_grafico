#include "../../../../../headers/concepts/primitives.hpp"

bool AngularDirection::operator== (const AngularDirection & adir) {
  return this->Direction::operator==(adir) && this->a == adir.a;
}
