/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	gfx_Model.h
Autor:	Real_Noname (real_noname@coderulers.info)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.info
Opis:	Zawiera definicje klas do ³adowania i zarz¹dzania
    modelami z plików GLM.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#pragma once

#include <fstream>

#include <CBGL/COpenGL.h>

#include "core_object.h"

namespace core {
  class FileParser;
}

namespace gfx {
  class Mesh;
  class Material;
  class TextureRepository;
  class Frame;

  class Model : public core::Object {
  public:
    Model();
    ~Model() override;

    Model(const Model&) = delete;
    Model(Model&&) = default;
    Model& operator=(const Model&) = delete;
    Model& operator=(Model&&) = default;

    const cb::strvector& getLoadingLog() const;
    const cb::string GetFile() const;

    bool load(gfx::TextureRepository& texManager, const cb::string& filePath);

    void queueRender(Frame& frame, const cb::string objName) const;

  protected:
    virtual cb::string getLogName() const override;

  private:
    struct Object;
    struct Mesh;

    using materials_t = std::vector<std::shared_ptr<Material>>;
    using objects_t = std::vector<Object>;

    bool loadMaterial(core::FileParser& parser, gfx::TextureRepository& texManager);
    bool loadObject(core::FileParser& parser);
    bool loadMesh(core::FileParser& parser, Object& obj);
    std::shared_ptr<Material> findMaterial(cb::string name) const;

    materials_t materials;
    objects_t objects;

    cb::string file = L"-";
    cb::strvector loadingLog;
  };
}
