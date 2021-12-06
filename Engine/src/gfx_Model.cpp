/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	glm.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> gfx_Model.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include <CBCore/StringConvert.h>

#include <CBGL/COpenGL.h>

#include "FileParser.h"

#include "gfx_Texture.h"
#include "gfx_TextureRepository.h"

#include "Mesh.h"
#include "MeshFuncs.h"

#include "gfx_Model.h"
#include "StrEx.h"

namespace gfx {

  struct Model::Material {
    cb::string name;
    glm::vec4 color = glm::vec4(1.0f);
    std::shared_ptr<gfx::Texture> texture;
  };

  struct Model::Mesh {
    cb::string materialName;
    gfx::Mesh mesh;

    Mesh() = default;
    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;
  };

  struct Model::Object {
    using meshes_t = std::vector<Model::Mesh>;

    cb::string name;
    meshes_t meshes;

    Object() = default;
    Object(Object&&) = default;
    Object& operator=(Object&&) = default;
  };

  gfx::MeshBuilderContext::VertListType getVertListFromStr(const cb::string& str) {
    if (str == L"GL_TRIANGLES")
      return gfx::MeshBuilderContext::VertListType::TRIANGLES;

    if (str == L"GL_TRIANGLE_STRIP")
      return gfx::MeshBuilderContext::VertListType::TRIANGLE_STRIP;

    if (str == L"GL_TRIANGLE_FAN")
      return gfx::MeshBuilderContext::VertListType::TRIANGLE_FAN;

    if (str == L"GL_QUADS")
      return gfx::MeshBuilderContext::VertListType::QUADS;

    if (str == L"GL_QUAD_STRIP")
      return gfx::MeshBuilderContext::VertListType::QUAD_STRIP;

    return gfx::MeshBuilderContext::VertListType::TRIANGLES;
  }

  /*=====KONSTRUKTOR=====*/
  Model::Model() : core::Object(L"Model") {
  }

  /*=====DESTRUKTOR=====*/
  Model::~Model() {
    // Zwolnienie pamiêci
    Free();
  }

  bool Model::loadMaterial(core::FileParser& parser, gfx::TextureRepository& texManager) {
    Material matCtx;

    matCtx.name = parser.getArg(0);

    while (parser.readLine()) {
      if (parser.getCmd() == L"ENDMATERIAL") {
        materials.push_back(std::move(matCtx));
        return true;
      }

      if (parser.getCmd() == L"TEXTURE") {
        matCtx.texture = texManager.Get(cb::toUtf8(parser.getArg(0)));
      }
      else if (parser.getCmd() == L"COLOR") {
        matCtx.color = parser.getVec4FromArgs(0);
      }
    }

    return false;
  }

  bool Model::loadObject(core::FileParser& parser) {
    Object obj;

    obj.name = parser.getArg(0);

    while (parser.readLine()) {
      if (parser.getCmd() == L"ENDOBJECT") {
        objects.push_back(std::move(obj));
        return true;
      }

      auto cmd = parser.getCmd();
      if (cmd == L"MESH") {
        if (!loadMesh(parser, obj)) {
          error("Failed to load mesh");
          return false;
        }
      }
    }

    return false;
  }

  bool Model::loadMesh(core::FileParser& parser, Object& obj) {
    gfx::MeshBuilderContext ctx;
    Mesh mesh;

    mesh.materialName = parser.getArg(0);

    while (parser.readLine()) {
      auto cmd = parser.getCmd();
      if (cmd == L"ENDMESH") {
        mesh.mesh.prepare();
        obj.meshes.push_back(std::move(mesh));
        return true;
      }
      else if (cmd == L"glRotate") {
        auto angle = parser.getFloat(0);
        auto value = parser.getVec3FromArgs(1);
        ctx.rotate(angle, value);
      }
      else if (cmd == L"glTranslate") {
        auto value = parser.getVec3FromArgs(0);
        ctx.translate(value);
      }
      else if (cmd == L"glScale") {
        auto value = parser.getVec3FromArgs(0);
        ctx.scale(value);
      }
      else if (cmd == L"glBegin") {
        auto value = getVertListFromStr(parser.getArg(0));
        ctx.beginVertexList(value);
      }
      else if (cmd == L"glEnd") {
        ctx.commitVertexList(mesh.mesh);
      }
      else if (cmd == L"glPushMatrix") {
        ctx.pushMatrix();
      }
      else if (cmd == L"glPopMatrix") {
        ctx.popMatrix();
      }
      else if (cmd == L"glNormal3") {
        ctx.setVertexNormal(parser.getVec3FromArgs(0));
      }
      else if (cmd == L"glTexCoord2") {
        ctx.setVertexTCoord(parser.getVec2FromArgs(0));
      }
      else if (cmd == L"glVertex3") {
        ctx.addVertex(parser.getVec3FromArgs(0));
      }
      else if (cmd == L"gluQuadricNormals") {
        // TODO
      }
      else if (cmd == L"gluQuadricOrientation") {
        ctx.setVerticesInverted(parser.getArg(0) == L"GLU_INSIDE");
      }
      else if (cmd == L"gluQuadricTexture") {
        // TODO
      }
      else if (cmd == L"gluSphere") {
        ctx.addSphere(mesh.mesh, parser.getFloat(0), parser.getUInt(1), parser.getUInt(2));
      }
      else if (cmd == L"gluCylinder") {
        ctx.addCylinder(mesh.mesh, parser.getFloat(0), parser.getFloat(1), parser.getFloat(2), parser.getUInt(3), parser.getUInt(4));
      }
      else if (cmd == L"gluDisk") {
        ctx.addDisk(mesh.mesh, parser.getFloat(0), parser.getFloat(1), parser.getUInt(2), parser.getUInt(3));
      }
      else if (cmd == L"gluPartialDisk") {
        ctx.addPartialDisk(mesh.mesh, parser.getFloat(0), parser.getFloat(1), parser.getUInt(2), parser.getUInt(3), parser.getFloat(4), parser.getFloat(5));
      }

    }

    return false;
  }


  /*=====METODA RenderObject=====
  Wywo³uje dan¹ listê wyœwietlania
  */
  void Model::render(unsigned int index) {
    if (!loaded)
      return;

    if (objects.empty())
      return;

    auto& object = *objects.begin();

    for (auto& mesh : object.meshes) {
      auto it = std::find_if(materials.begin(), materials.end(), [&](const Material& material) { return material.name == mesh.materialName; });
      if (it != materials.end()) {
        if (it->texture)
          it->texture->Activate();
      }
      mesh.mesh.render();
    }
  }

  bool Model::load(gfx::TextureRepository& texManager, const std::string& filename) {
    // Sprawdzamy czy ³añcuch nie jest pusty
    if (filename.empty()) {
      error("Pusty ci¹g nazwy pliku!");
      return false;
    }

    core::FileParser parser(cb::fromUtf8(filename));

    if (!parser.readLine()) {
      error("Invalid file " + filename);
      return false;
    }

    if (parser.getCmd() != L"GLM" || parser.getArg(0) != L"100") {
      error("Invalid header for file " + filename);
      return false;
    }

    Material matCtx;
    Object objCtx;

    while (parser.readLine()) {
      if (parser.getCmd() == L"MATERIAL") {
        if (!loadMaterial(parser, texManager)) {
          error("Failed to load material");
          return false;
        }
      }
      else if (parser.getCmd() == L"OBJECT") {
        if (!loadObject(parser)) {
          error("Failed to load object");
          return false;
        }
      }
    }

    loaded = true;
    report("Loaded new model.");
    return true;
  }

  void Model::Free() {
    file.clear();
    loaded = false;
  }

  const std::string Model::GetFile() const {
    return cb::toUtf8(file);
  }

  unsigned int Model::GetObjCount() {
    return static_cast<unsigned>(objects.size());
  }

  cb::string Model::getLogName() const {
    return getClassName() + L"(" + file + L"):";
  }
}