#include "shape.hpp"

Shape::Shape(char sym, const glm::vec3& pos, const glm::vec3& rot,
    const glm::vec3& scl, const Material& mat,
    const glm::vec3& x, const glm::vec3& z, bool door, bool chim, const char * cSide)
    : symbol(sym), position(pos), rotation(rot), scale(scl),
    material(mat), xAxis(x), zAxis(z), hasDoor(door), hasChim(chim), chimSide(cSide) {
}