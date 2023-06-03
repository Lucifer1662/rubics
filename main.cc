#include "glm/gtc/matrix_transform.hpp"
#include <array>
#include <cmath>
#include <iostream>

struct RubicsCube {
  using Face = unsigned char;
  std::array<Face, 5 * 5 * 5> mData;

  Face &at(int x, int y, int z) { return mData[x + y * 5 + z * 5 * 5]; }
  const Face &at(int x, int y, int z) const {
    return mData[x + y * 5 + z * 5 * 5];
  }

  void rotX(RubicsCube &newCube, int x, int y, int z) {
    newCube.at(x, y, z) = at(x, (-(z - 2)) + 2, y);
  }

  Face swap(int x, int y, int z, Face t) {
    auto &f = at(x, y, z);
    auto t1 = f;
    f = t;
    return t1;
  }

  void rotXCol(int x) {
    Face t;

    t = at(x, 0, 1);
    t = swap(x, 1, 4, t);
    t = swap(x, 4, 1, t);
    t = swap(x, 1, 0, t);
    t = swap(x, 0, 1, t);

    t = at(x, 0, 2);
    t = swap(x, 2, 4, t);
    t = swap(x, 4, 2, t);
    t = swap(x, 2, 0, t);
    t = swap(x, 0, 2, t);

    // bottom
    t = at(x, 0, 3);
    t = swap(x, 3, 4, t);
    t = swap(x, 4, 3, t);
    t = swap(x, 3, 0, t);
    t = swap(x, 0, 3, t);
  }

  void rotX(int x) {
    RubicsCube newCube = *this;

    for (int z = 0; z < 5; z++)
      for (int y = 0; y < 5; y++)
        rotX(newCube, x, y, z);
    *this = newCube;
  }

  void rotY(int y) {
    RubicsCube newCube = *this;
    for (int z = 0; z < 5; z++)
      for (int x = 0; x < 5; x++) {
        newCube.at(x, y, z) = at(z, y, (-(x - 2)) + 2);
      }

    *this = newCube;
  }

  void rotZ(int z) {
    RubicsCube newCube = *this;
    for (int y = 0; y < 5; y++)
      for (int x = 0; x < 5; x++) {
        newCube.at(x, y, z) = at((-(y - 2)) + 2, x, z);
      }

    *this = newCube;
  }

  void rotateX(int x) {
    rotX(x);
    if (x == 1) {
      rotX(0);
    } else if (x == 3) {
      rotX(4);
    }
  }

  void rotateY(int x) {
    rotY(x);
    if (x == 1) {
      rotY(0);
    } else if (x == 3) {
      rotY(4);
    }
  }

  void rotateZ(int x) {
    rotZ(x);
    if (x == 1) {
      rotZ(0);
    } else if (x == 3) {
      rotZ(4);
    }
  }

  void indices() {
    for (size_t i = 0; i < 5 * 5 * 5; i++) {
      mData[i] = i;
    }
  }

  bool solved() {
    Face f;

    // front
    f = at(1, 1, 0);
    for (size_t i = 1; i < 4; i++)
      for (size_t j = 1; j < 4; j++) {
        if (at(i, j, 0) != f) {
          return false;
        }
      }

    // back
    f = at(1, 1, 4);
    for (size_t i = 1; i < 4; i++)
      for (size_t j = 1; j < 4; j++) {
        if (at(i, j, 4) != f) {
          return false;
        }
      }

    // right
    f = at(4, 1, 1);
    for (size_t i = 1; i < 4; i++)
      for (size_t j = 1; j < 4; j++) {
        if (at(4, i, j) != f) {
          return false;
        }
      }

    // left
    f = at(0, 1, 1);
    for (size_t i = 1; i < 4; i++)
      for (size_t j = 1; j < 4; j++) {
        if (at(0, i, j) != f) {
          return false;
        }
      }

    // top
    f = at(1, 4, 1);
    for (size_t i = 1; i < 4; i++)
      for (size_t j = 1; j < 4; j++) {
        if (at(i, 4, j) != f) {
          return false;
        }
      }

    // bottom
    f = at(1, 0, 1);
    for (size_t i = 1; i < 4; i++)
      for (size_t j = 1; j < 4; j++) {
        if (at(i, 0, j) != f) {
          return false;
        }
      }

    return true;
  }

  void identity() {
    // front
    for (size_t i = 1; i < 4; i++)
      for (size_t j = 1; j < 4; j++) {
        at(i, j, 0) = 'f';
      }

    // back
    for (size_t i = 1; i < 4; i++)
      for (size_t j = 1; j < 4; j++) {
        at(i, j, 4) = 'b';
      }

    // right
    for (size_t i = 1; i < 4; i++)
      for (size_t j = 1; j < 4; j++) {
        at(4, i, j) = 'r';
      }

    // left
    for (size_t i = 1; i < 4; i++)
      for (size_t j = 1; j < 4; j++) {
        at(0, i, j) = 'l';
      }

    // top
    for (size_t i = 1; i < 4; i++)
      for (size_t j = 1; j < 4; j++) {
        at(i, 4, j) = 't';
      }

    // floor
    for (size_t i = 1; i < 4; i++)
      for (size_t j = 1; j < 4; j++) {
        at(i, 0, j) = 'p';
      }
  }
};

std::ostream &operator<<(std::ostream &os, const RubicsCube &cube) {
  os << "front" << std::endl;
  for (size_t i = 1; i < 4; i++) {
    for (size_t j = 1; j < 4; j++) {
      os << cube.at(i, j, 0) << ", ";
    }
    os << std::endl;
  }
  os << std::endl;

  os << "back" << std::endl;
  for (size_t i = 1; i < 4; i++) {
    for (size_t j = 1; j < 4; j++) {
      os << cube.at(i, j, 4) << ", ";
    }
    os << std::endl;
  }
  os << std::endl;

  os << "right" << std::endl;
  for (size_t i = 1; i < 4; i++) {
    for (size_t j = 1; j < 4; j++) {
      os << cube.at(4, i, j) << ", ";
    }
    os << std::endl;
  }
  os << std::endl;

  os << "left" << std::endl;
  for (size_t i = 1; i < 4; i++) {
    for (size_t j = 1; j < 4; j++) {
      os << cube.at(0, i, j) << ", ";
    }
    os << std::endl;
  }
  os << std::endl;

  os << "top" << std::endl;
  for (size_t i = 1; i < 4; i++) {
    for (size_t j = 1; j < 4; j++) {
      os << cube.at(i, 4, j) << ", ";
    }
    os << std::endl;
  }
  os << std::endl;

  os << "bottom" << std::endl;
  for (size_t i = 1; i < 4; i++) {
    for (size_t j = 1; j < 4; j++) {
      os << cube.at(i, 0, j) << ", ";
    }
    os << std::endl;
  }
  os << std::endl;

  return os;
}

int main() {
  RubicsCube cube;
  // cube.indices();
  cube.identity();

  std::cout << cube.solved() << std::endl;

  std::cout << cube << std::endl;

  cube.rotateZ(2);

  std::cout << cube << std::endl;

  return 0;
}
