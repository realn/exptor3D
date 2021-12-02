/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	gfx_Model.h
Autor:	Real_Noname (real_noname@coderulers.info)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.info
Opis:	Zawiera definicje klas do �adowania i zarz�dzania
    modelami z plik�w GLM.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#pragma once

#include <fstream>

#include <CBGL/COpenGL.h>

#include "core_object.h"

// Numer wersji loader'a
#define		GLM_FILE_VERSION		100

namespace core {
  class FileParser;
}
namespace gfx {
  class Mesh;
  class TextureRepository;

  class Model : public core::Object {

  public:
    Model();
    ~Model() override;

    Model(const Model&) = delete;
    Model(Model&&) = default;
    Model& operator=(const Model&) = delete;
    Model& operator=(Model&&) = default;

    bool load(gfx::TextureRepository& texManager, const std::string& filePath);

    void Free();
    const std::string GetFile() const;
    unsigned int GetObjCount();

    void render(unsigned int index);

  protected:
    virtual cb::string getLogName() const override;

  private:
    struct Material;
    struct Object;
    struct Mesh;

    using materials_t = std::vector<Material>;
    using objects_t = std::vector<Object>;

    bool loadMaterial(core::FileParser& parser, gfx::TextureRepository& texManager);
    bool loadObject(core::FileParser& parser);
    bool loadMesh(core::FileParser& parser, Object& obj);

    materials_t materials;
    objects_t objects;

    // Czy model jest za�adowany?
    bool loaded = false;
    // Ostatni za�adowany plik
    cb::string file = L"-";
  };
}
