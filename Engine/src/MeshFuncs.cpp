#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "3dMath.h"
#include "MeshVertex.h"
#include "Mesh.h"
#include "MeshFuncs.h"

namespace gfx {
  namespace {
    glm::vec4 makeCirclePos(float angleRad, float radius, float z = 0.0f) {
      return glm::vec4(
        glm::cos(angleRad) * radius,
        glm::sin(angleRad) * radius,
        z,
        1.0f
      );
    }

    glm::vec2 makeCircleCoord(float angleRad, float radius, float fullRadius) {
      return glm::vec2(
        (glm::cos(angleRad) * radius) / fullRadius,
        (glm::sin(angleRad) * radius) / fullRadius
      ) * 0.5f + glm::vec2(0.5f);
    }

    glm::vec4 makeSpherePos(float angleARad, float angleBRad, float radius) {
      return glm::vec4(
        glm::cos(angleARad) * glm::cos(angleBRad) * radius,
        glm::sin(angleARad) * glm::cos(angleBRad) * radius,
        glm::sin(angleBRad) * radius,
        1.0f
      );
    }

    glm::vec3 makeSphereNormal(float angleARad, float angleBRad) {
      return glm::normalize(glm::vec3(
        glm::cos(angleARad) * glm::cos(angleBRad),
        glm::sin(angleARad) * glm::cos(angleBRad),
        glm::sin(angleBRad)));
    }

    glm::vec2 makeSphereCoord(float angleARad, float angleBRad) {
      return glm::vec2(
        (glm::cos(angleARad / 2.0f) + 1.0f) * 0.5f,
        (glm::cos(angleBRad + glm::half_pi<float>()) + 1.0f) * 0.5f
      );
    }

    glm::vec3 makeCylinderNormal(float angleRad, float height, float bottomRadius, float topRadius) {
      auto b = topRadius - bottomRadius;
      auto a = height;
      auto c = pythagoreanTheorem(a, b);

      if (c == 0.0f)
        return glm::vec3(
          glm::cos(angleRad),
          glm::sin(angleRad),
          0.0f
        );

      auto bsin = a / c;
      auto bcos = b / c;

      return glm::normalize(glm::vec3(
        glm::cos(angleRad) * bcos,
        glm::sin(angleRad) * bcos,
        bsin
      ));
    }

    glm::vec2 makeCylinderCoord(float angleRad, float posZ, float height) {
      return glm::vec2(
        angleRad / glm::two_pi<float>(),
        height != 0.0f ? posZ / height : 0.0f
      );
    }
  }

  struct MeshBuilderContext::vertlist_t {
    VertListType type = VertListType::TRIANGLES;
    glm::vec3 normal;
    glm::vec2 tcoord;
    Mesh::vertices_t vertices;
  };

  void MeshBuilderContext::addMeshQuadAsTriangles(Mesh& mesh, const MeshVertex& v1, const MeshVertex& v2, const MeshVertex& v3, const MeshVertex& v4) {
    if (inverted) {
      mesh.add(v4);
      mesh.add(v2);
      mesh.add(v1);

      mesh.add(v4);
      mesh.add(v3);
      mesh.add(v2);
    }
    else {
      mesh.add(v1);
      mesh.add(v2);
      mesh.add(v4);

      mesh.add(v2);
      mesh.add(v3);
      mesh.add(v4);
    }
  }

  MeshBuilderContext::MeshBuilderContext() = default;

  MeshBuilderContext::~MeshBuilderContext() = default;

  void MeshBuilderContext::translate(glm::vec3 value) {
    matrixCurrent = glm::translate(matrixCurrent, value);
  }

  void MeshBuilderContext::translate(float x, float y, float z) {
    translate({ x, y, z });
  }

  void MeshBuilderContext::rotate(float angleDeg, glm::vec3 value) {
    matrixCurrent = glm::rotate(matrixCurrent, glm::radians(angleDeg), value);
  }

  void MeshBuilderContext::rotate(float angleDeg, float x, float y, float z) {
    rotate(angleDeg, { x, y, z });
  }

  void MeshBuilderContext::scale(glm::vec3 value) {
    matrixCurrent = glm::scale(matrixCurrent, value);
  }

  void MeshBuilderContext::scale(float x, float y, float z) {
    scale({ x, y, z });
  }

  void MeshBuilderContext::loadIdentity() {
    matrixCurrent = glm::mat4(1.0f);
  }

  void MeshBuilderContext::pushMatrix() {
    matrixStack.push_back(matrixCurrent);
  }

  void MeshBuilderContext::popMatrix() {
    if (matrixStack.empty())
      return;
    matrixCurrent = *matrixStack.rbegin();
    matrixStack.pop_back();
  }

  void MeshBuilderContext::setVerticesInverted(bool value) {
    inverted = value;
  }

  void MeshBuilderContext::addDisk(Mesh& mesh, float innerRadius, float outerRadius, cb::u32 slices, cb::u32 loops) {
    if (slices < 3 || loops < 1)
      return;

    auto full = glm::two_pi<float>();
    auto step = full / static_cast<float>(slices);

    auto normal = glm::vec3(0.0f, 0.0f, -1.0f);
    auto texCoord = glm::vec2();

    auto loopRadiusStep = (outerRadius - innerRadius) / loops;
    for (auto loop = 0u; loop < loops; loop++) {
      for (auto i = 0u; i < slices; i++) {
        auto innerR = innerRadius + loopRadiusStep * loop;
        auto outerR = innerRadius + loopRadiusStep * (loop + 1);

        auto angleBeg = step * i;
        auto angleEnd = step * (i + 1);

        auto innerBegPos = matrixCurrent * makeCirclePos(angleBeg, innerR);
        auto outerBegPos = matrixCurrent * makeCirclePos(angleBeg, outerR);

        auto innerEndPos = matrixCurrent * makeCirclePos(angleEnd, innerR);
        auto outerEndPos = matrixCurrent * makeCirclePos(angleEnd, outerR);

        auto innerBegCoord = makeCircleCoord(angleBeg, innerR, outerRadius);
        auto outerBegCoord = makeCircleCoord(angleBeg, outerR, outerRadius);

        auto innerEndCoord = makeCircleCoord(angleEnd, innerR, outerRadius);
        auto outerEndCoord = makeCircleCoord(angleEnd, outerR, outerRadius);

        auto v1 = MeshVertex{ innerBegPos, normal, innerBegCoord };
        auto v2 = MeshVertex{ outerBegPos, normal, outerBegCoord };
        auto v3 = MeshVertex{ outerEndPos, normal, outerEndCoord };
        auto v4 = MeshVertex{ innerEndPos, normal, innerEndCoord };

        addMeshQuadAsTriangles(mesh, v1, v2, v3, v4);
      }
    }
  }

  void MeshBuilderContext::addPartialDisk(Mesh& mesh, float innerRadius, float outerRadius, cb::u32 slices, cb::u32 loops, float startAngleDeg, float sweepAngleDeg) {
    if (slices < 3 || loops < 1)
      return;

    float startAngle = glm::radians(startAngleDeg);
    float sweepAngle = glm::radians(sweepAngleDeg);
    float endAngle = startAngle + sweepAngle;

    auto full = glm::two_pi<float>();
    auto step = full / static_cast<float>(slices);

    auto normal = glm::vec3(0.0f, 0.0f, -1.0f);
    auto texCoord = glm::vec2();

    auto loopRadiusStep = (outerRadius - innerRadius) / loops;
    for (auto loop = 0u; loop < loops; loop++) {
      for (auto slice = 0u; slice < slices; slice++) {
        auto innerR = innerRadius + loopRadiusStep * loop;
        auto outerR = innerRadius + loopRadiusStep * (loop + 1);

        auto sliceAngleBeg = step * slice;
        auto sliceAngleEnd = step * (slice + 1);

        if (sliceAngleBeg < startAngle && sliceAngleEnd < startAngle)
          continue;
        if (sliceAngleBeg > endAngle && sliceAngleEnd > endAngle)
          continue;

        sliceAngleBeg = glm::max(sliceAngleBeg, startAngle);
        sliceAngleEnd = glm::min(sliceAngleEnd, endAngle);

        auto innerBegPos = matrixCurrent * makeCirclePos(sliceAngleBeg, innerR);
        auto outerBegPos = matrixCurrent * makeCirclePos(sliceAngleBeg, outerR);

        auto innerEndPos = matrixCurrent * makeCirclePos(sliceAngleEnd, innerR);
        auto outerEndPos = matrixCurrent * makeCirclePos(sliceAngleEnd, outerR);

        auto innerBegCoord = makeCircleCoord(sliceAngleBeg, innerR, outerRadius);
        auto outerBegCoord = makeCircleCoord(sliceAngleBeg, outerR, outerRadius);

        auto innerEndCoord = makeCircleCoord(sliceAngleEnd, innerR, outerRadius);
        auto outerEndCoord = makeCircleCoord(sliceAngleEnd, outerR, outerRadius);

        auto v1 = MeshVertex{ innerBegPos, normal, innerBegCoord };
        auto v2 = MeshVertex{ outerBegPos, normal, outerBegCoord };
        auto v3 = MeshVertex{ outerEndPos, normal, outerEndCoord };
        auto v4 = MeshVertex{ innerEndPos, normal, innerEndCoord };

        addMeshQuadAsTriangles(mesh, v1, v2, v3, v4);
      }
    }
  }

  void MeshBuilderContext::addSphere(Mesh& mesh, float radius, cb::u32 slices, cb::u32 stacks) {
    if (slices < 3 || stacks < 3)
      return;

    auto sliceStep = glm::two_pi<float>() / slices;
    auto stackStep = glm::pi<float>() / stacks;

    auto matrixRot = glm::mat3(matrixCurrent);
    for (auto stack = 0u; stack < stacks; stack++) {
      auto angleBBeg = -glm::half_pi<float>() + stackStep * stack;
      auto angleBEnd = -glm::half_pi<float>() + stackStep * (stack + 1);
      for (auto slice = 0u; slice < slices; slice++) {
        auto angleABeg = sliceStep * slice;
        auto angleAEnd = sliceStep * (slice + 1);

        auto AAPos = matrixCurrent * makeSpherePos(angleABeg, angleBBeg, radius);
        auto BAPos = matrixCurrent * makeSpherePos(angleAEnd, angleBBeg, radius);

        auto ABPos = matrixCurrent * makeSpherePos(angleABeg, angleBEnd, radius);
        auto BBPos = matrixCurrent * makeSpherePos(angleAEnd, angleBEnd, radius);

        auto AANor = matrixRot * makeSphereNormal(angleABeg, angleBBeg);
        auto BANor = matrixRot * makeSphereNormal(angleAEnd, angleBBeg);

        auto ABNor = matrixRot * makeSphereNormal(angleABeg, angleBEnd);
        auto BBNor = matrixRot * makeSphereNormal(angleAEnd, angleBEnd);

        auto AACrd = makeSphereCoord(angleABeg, angleBBeg);
        auto BACrd = makeSphereCoord(angleAEnd, angleBBeg);

        auto ABCrd = makeSphereCoord(angleABeg, angleBEnd);
        auto BBCrd = makeSphereCoord(angleAEnd, angleBEnd);

        auto v1 = MeshVertex{ AAPos, AANor, AACrd };
        auto v2 = MeshVertex{ BAPos, BANor, BACrd };
        auto v3 = MeshVertex{ BBPos, BBNor, BBCrd };
        auto v4 = MeshVertex{ ABPos, ABNor, ABCrd };

        addMeshQuadAsTriangles(mesh, v1, v2, v3, v4);
      }
    }
  }

  void MeshBuilderContext::addPartialSphere(Mesh& mesh, float radius, cb::u32 slices, cb::u32 stacks, float startAngleSlicesDeg, float sweepAngleSlicesDeg, float startAngleStackDeg, float sweepAngleStackDeg) {
    if (slices < 3 || stacks < 3)
      return;

    auto startAngleSlices = glm::radians(startAngleSlicesDeg);
    auto endAngleSlices = glm::radians(startAngleSlicesDeg + sweepAngleSlicesDeg);

    auto startAngleStack = glm::radians(startAngleStackDeg);
    auto endAngleStack = glm::radians(startAngleStackDeg + sweepAngleStackDeg);

    auto sliceStep = glm::two_pi<float>() / slices;
    auto stackStep = glm::pi<float>() / stacks;

    auto matrixRot = glm::mat3(matrixCurrent);
    for (auto stack = 0u; stack < stacks; stack++) {
      auto angleBBeg = -glm::half_pi<float>() + stackStep * stack;
      auto angleBEnd = -glm::half_pi<float>() + stackStep * (stack + 1);

      if (angleBBeg < startAngleStack && angleBEnd < startAngleStack)
        continue;
      if (angleBBeg > endAngleStack && angleBEnd > endAngleStack)
        continue;

      angleBBeg = glm::max(angleBBeg, startAngleStack);
      angleBEnd = glm::min(angleBEnd, endAngleStack);

      for (auto slice = 0u; slice < slices; slice++) {
        auto angleABeg = sliceStep * slice;
        auto angleAEnd = sliceStep * (slice + 1);

        if (angleABeg < startAngleSlices && angleAEnd < startAngleSlices)
          continue;
        if (angleABeg > endAngleSlices && angleAEnd > endAngleSlices)
          continue;

        angleABeg = glm::max(angleABeg, startAngleSlices);
        angleAEnd = glm::min(angleAEnd, endAngleSlices);

        auto AAPos = matrixCurrent * makeSpherePos(angleABeg, angleBBeg, radius);
        auto BAPos = matrixCurrent * makeSpherePos(angleAEnd, angleBBeg, radius);

        auto ABPos = matrixCurrent * makeSpherePos(angleABeg, angleBEnd, radius);
        auto BBPos = matrixCurrent * makeSpherePos(angleAEnd, angleBEnd, radius);

        auto AANor = matrixRot * makeSphereNormal(angleABeg, angleBBeg);
        auto BANor = matrixRot * makeSphereNormal(angleAEnd, angleBBeg);

        auto ABNor = matrixRot * makeSphereNormal(angleABeg, angleBEnd);
        auto BBNor = matrixRot * makeSphereNormal(angleAEnd, angleBEnd);

        auto AACrd = makeSphereCoord(angleABeg, angleBBeg);
        auto BACrd = makeSphereCoord(angleAEnd, angleBBeg);

        auto ABCrd = makeSphereCoord(angleABeg, angleBEnd);
        auto BBCrd = makeSphereCoord(angleAEnd, angleBEnd);

        auto v1 = MeshVertex{ AAPos, AANor, AACrd };
        auto v2 = MeshVertex{ BAPos, BANor, BACrd };
        auto v3 = MeshVertex{ BBPos, BBNor, BBCrd };
        auto v4 = MeshVertex{ ABPos, ABNor, ABCrd };

        addMeshQuadAsTriangles(mesh, v1, v2, v3, v4);
      }
    }
  }

  void MeshBuilderContext::addCylinder(Mesh& mesh, float baseRadius, float topRadius, float height, cb::u32 slices, cb::u32 stacks) {
    if (slices < 3 || stacks < 1)
      return;

    float sliceStep = glm::two_pi<float>() / slices;
    float stackStep = height / stacks;
    float stackRadiusStep = (topRadius - baseRadius) / stacks;

    auto matrixRot = glm::mat3(matrixCurrent);
    for (auto stack = 0u; stack < stacks; stack++) {
      auto zPosA = stackStep * stack;
      auto radiusA = baseRadius + stackRadiusStep * stack;

      auto zPosB = stackStep * (stack + 1);
      auto radiusB = baseRadius + stackRadiusStep * (stack + 1);

      for (auto slice = 0u; slice < slices; slice++) {
        auto angleA = sliceStep * slice;
        auto angleB = sliceStep * (slice + 1);

        auto AAPos = matrixCurrent * makeCirclePos(angleA, radiusA, zPosA);
        auto BAPos = matrixCurrent * makeCirclePos(angleB, radiusA, zPosA);

        auto ABPos = matrixCurrent * makeCirclePos(angleA, radiusB, zPosB);
        auto BBPos = matrixCurrent * makeCirclePos(angleB, radiusB, zPosB);

        auto ANor = matrixRot * makeCylinderNormal(angleA, height, baseRadius, topRadius);
        auto BNor = matrixRot * makeCylinderNormal(angleB, height, baseRadius, topRadius);

        auto AACrd = makeCylinderCoord(angleA, zPosA, height);
        auto BACrd = makeCylinderCoord(angleB, zPosA, height);

        auto ABCrd = makeCylinderCoord(angleA, zPosB, height);
        auto BBCrd = makeCylinderCoord(angleB, zPosB, height);

        auto v1 = MeshVertex{ AAPos, ANor, AACrd };
        auto v2 = MeshVertex{ BAPos, BNor, BACrd };
        auto v3 = MeshVertex{ BBPos, BNor, BBCrd };
        auto v4 = MeshVertex{ ABPos, ANor, ABCrd };

        addMeshQuadAsTriangles(mesh, v1, v2, v3, v4);
      }
    }
  }

  void MeshBuilderContext::beginVertexList(VertListType type) {
    vertList = std::make_unique<vertlist_t>();
    vertList->type = type;
  }

  void MeshBuilderContext::setVertexNormal(glm::vec3 value) {
    if (vertList) {
      vertList->normal = value;
    }
  }

  void MeshBuilderContext::setVertexNormal(float x, float y, float z) {
    setVertexNormal({ x, y, z });
  }

  void MeshBuilderContext::setVertexTCoord(glm::vec2 value) {
    if (vertList) {
      vertList->tcoord = value;
    }
  }

  void MeshBuilderContext::setVertexTCoord(float x, float y) {
    setVertexTCoord({ x, y });
  }

  void MeshBuilderContext::addVertex(glm::vec3 value) {
    if (vertList) {
      auto vertex = MeshVertex{
        value,
        vertList->normal,
        vertList->tcoord
      };
      vertList->vertices.push_back(vertex);
    }
  }

  void MeshBuilderContext::addVertex(float x, float y, float z) {
    addVertex({ x, y, z });
  }

  void MeshBuilderContext::commitVertexList(Mesh& mesh) {
    if (!vertList)
      return;

    if (vertList->type == VertListType::TRIANGLES) {
      auto triangleNum = vertList->vertices.size() / 3;
      for (size_t i = 0u; i < triangleNum; i++) {
        const auto& v1 = vertList->vertices[i * 3 + 0];
        const auto& v2 = vertList->vertices[i * 3 + 1];
        const auto& v3 = vertList->vertices[i * 3 + 2];

        mesh.add(v1);
        mesh.add(v2);
        mesh.add(v3);
      }
    }
    else if (vertList->type == VertListType::QUADS) {
      auto quadNum = vertList->vertices.size() / 4;
      for (size_t i = 0u; i < quadNum; i++) {
        const auto& v1 = vertList->vertices[i * 3 + 0];
        const auto& v2 = vertList->vertices[i * 3 + 1];
        const auto& v3 = vertList->vertices[i * 3 + 2];
        const auto& v4 = vertList->vertices[i * 3 + 3];

        mesh.add(v1);
        mesh.add(v2);
        mesh.add(v3);

        mesh.add(v1);
        mesh.add(v3);
        mesh.add(v4);
      }
    }

    vertList.reset();
  }
}