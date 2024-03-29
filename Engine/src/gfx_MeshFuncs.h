#pragma once

#include <memory>
#include <glm/matrix.hpp>

#include <CBCore/Defines.h>

namespace gfx {
  class Mesh;
  class MeshVertex;

  class MeshBuilderContext {
  public:
    enum class VertListType {
      TRIANGLES = 0,
      TRIANGLE_STRIP,
      TRIANGLE_FAN,
      QUADS,
      QUAD_STRIP
    };

    MeshBuilderContext();
    ~MeshBuilderContext();

    void translate(glm::vec3 value);
    void translate(float x, float y, float z);
    void rotate(float angleDeg, glm::vec3 value);
    void rotate(float angleDeg, float x, float y, float z);
    void scale(glm::vec3 value);
    void scale(float x, float y, float z);
    void loadIdentity();

    void pushMatrix();
    void popMatrix();

    void setVerticesInverted(bool value);

    void addPlane(Mesh& mesh, glm::vec2 size, glm::vec2 texCoordSize);
    void addDisk(Mesh& mesh, float innerRadius, float outerRadius, cb::u32 slices, cb::u32 loops);
    void addPartialDisk(Mesh& mesh, float innerRadius, float outerRadius, cb::u32 slices, cb::u32 loops, float startAngleDeg, float sweepAngleDeg);
    void addSphere(Mesh& mesh, float radius, cb::u32 slices, cb::u32 stacks);
    void addPartialSphere(Mesh& mesh, float radius, cb::u32 slices, cb::u32 stacks, float startAngleSlicesDeg, float sweepAngleSlicesDeg, float startAngleStackDeg, float sweepAngleStackDeg);
    void addCylinder(Mesh& mesh, float baseRadius, float topRadius, float height, cb::u32 slices, cb::u32 stacks);

    void beginVertexList(VertListType type);
    void setVertexNormal(glm::vec3 value);
    void setVertexNormal(float x, float y, float z);
    void setVertexTCoord(glm::vec2 value);
    void setVertexTCoord(float x, float y);
    void addVertex(glm::vec3 value);
    void addVertex(float x, float y, float z);
    void commitVertexList(Mesh& mesh);

  private:
    using matstack_t = std::vector<glm::mat4>;
    struct vertlist_t;

    void addMeshQuadAsTriangles(Mesh& mesh, const MeshVertex& v1, const MeshVertex& v2, const MeshVertex& v3, const MeshVertex& v4);

    glm::mat4 matrixCurrent = glm::mat4(1.0f);
    matstack_t matrixStack;
    bool inverted = false;
    std::unique_ptr<vertlist_t> vertList;
  };
}