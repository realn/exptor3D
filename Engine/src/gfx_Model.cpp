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

#include "core_FileParser.h"

#include "gfx_Frame.h"
#include "gfx_Texture.h"
#include "gfx_TextureRepository.h"
#include "gfx_Material.h"
#include "gfx_Mesh.h"
#include "gfx_BufferedMesh.h"
#include "gfx_MeshFuncs.h"
#include "gfx_Model.h"

#include "StrEx.h"

namespace gfx {
  struct Model::Mesh {
    cb::string materialName;
    std::shared_ptr<gfx::BufferedMesh> mesh;
  };

  struct Model::Object {
    using meshes_t = std::vector<Model::Mesh>;

    cb::string name;
    meshes_t meshes;
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

  Model::Model() : core::Object(L"Model") {}

  Model::~Model() = default;

  bool Model::loadMaterial(core::FileParser& parser, gfx::TextureRepository& texManager) {
    auto matCtx = std::make_shared<Material>(parser.getArg(0));

    while (parser.readLine()) {
      if (parser.getCmd() == L"ENDMATERIAL") {
        materials.push_back(std::move(matCtx));
        return true;
      }

      if (parser.getCmd() == L"TEXTURE") {
        auto texture = texManager.Get(cb::toUtf8(parser.getArg(0)));
        matCtx->setTexture(texture);
      }
      else if (parser.getCmd() == L"COLOR") {
        matCtx->setColor(parser.getVec4FromArgs(0));
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
          error(L"Failed to load mesh");
          return false;
        }
      }
    }

    return false;
  }

  bool Model::loadMesh(core::FileParser& parser, Object& obj) {
    gfx::MeshBuilderContext ctx;
    Mesh result;

    gfx::Mesh mesh;

    result.materialName = parser.getArg(0);

    while (parser.readLine()) {
      auto cmd = parser.getCmd();
      if (cmd == L"ENDMESH") {
        result.mesh = std::make_shared<BufferedMesh>(mesh);
        obj.meshes.push_back(std::move(result));
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
        ctx.commitVertexList(mesh);
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
        ctx.addSphere(mesh, parser.getFloat(0), parser.getUInt(1), parser.getUInt(2));
      }
      else if (cmd == L"gluCylinder") {
        ctx.addCylinder(mesh, parser.getFloat(0), parser.getFloat(1), parser.getFloat(2), parser.getUInt(3), parser.getUInt(4));
      }
      else if (cmd == L"gluDisk") {
        ctx.addDisk(mesh, parser.getFloat(0), parser.getFloat(1), parser.getUInt(2), parser.getUInt(3));
      }
      else if (cmd == L"gluPartialDisk") {
        ctx.addPartialDisk(mesh, parser.getFloat(0), parser.getFloat(1), parser.getUInt(2), parser.getUInt(3), parser.getFloat(4), parser.getFloat(5));
      }
      else if (cmd == L"glPlane") {
        ctx.addPlane(mesh, parser.getVec2FromArgs(0), parser.getVec2FromArgs(2, glm::vec2(1.0f)));
      }
      else {
        warning(L"unrecognized file command: " + cmd);
      }
    }

    return false;
  }

  std::shared_ptr<Material> Model::findMaterial(cb::string name) const {
    auto it = std::find_if(materials.begin(), materials.end(), [&](std::shared_ptr<Material> m) { return m->getName() == name; });
    if (it == materials.end())
      return std::shared_ptr<Material>();
    return *it;
  }


  /*=====METODA RenderObject=====
  Wywo³uje dan¹ listê wyœwietlania
  */
  void Model::queueRender(Frame& frame, const cb::string objName) const {
    if (objects.empty())
      return;

    auto it = std::find_if(objects.begin(), objects.end(), [&](const Object& o) {return o.name == objName; });
    if (it == objects.end())
      return;

    for (auto& mesh : it->meshes) {
      auto material = findMaterial(mesh.materialName);
      frame.addMesh(mesh.mesh, material);
    }
  }

  const cb::strvector& Model::getLoadingLog() const {
    return loadingLog;
  }

  bool Model::load(gfx::TextureRepository& texManager, const cb::string& filename) {

    core::FileParser parser(filename);

    if (!parser) {
      error(L"Invalid file " + filename);
      return false;
    }

    parser.readLine();
    if (parser.getCmd() != L"GLM" || parser.getArg(0) != L"100") {
      error(L"Invalid header for file " + filename);
      return false;
    }

    while (parser.readLine()) {
      if (parser.getCmd() == L"MATERIAL") {
        if (!loadMaterial(parser, texManager)) {
          loadingLog = parser.getLogLines();
          error(L"Failed to load material");
          return false;
        }
      }
      else if (parser.getCmd() == L"OBJECT") {
        if (!loadObject(parser)) {
          loadingLog = parser.getLogLines();
          error(L"Failed to load object");
          return false;
        }
      }
    }

    loadingLog = parser.getLogLines();
    writeLogLines(core::LogType::Info, loadingLog);
    report(L"Loaded new model.");
    return true;
  }

  const cb::string Model::GetFile() const {
    return file;
  }

  const cb::strvector Model::getObjectNames() const {
    auto result = cb::strvector();
    for (auto& obj : objects)
      result.push_back(obj.name);
    return result;
  }

  cb::string Model::getLogName() const {
    return getClassName() + L"(" + file + L"):";
  }
}