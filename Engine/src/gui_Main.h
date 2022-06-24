#pragma once

#include "gfx_Texture.h"
#include "gfx_TextureRepository.h"

#include "core_object.h"

#include "event_MappedObserver.h"

#include "logic_ScriptParser.h"

#include "gui_MainMenu.h"
#include "gui_Screen.h"

#include "GUITextPrinter.h"
#include "GUIConsole.h"

namespace gui {
  class Main 
    : public core::Object
    , public event::MappedObserver
  {
  public:
    using screenptr_t = std::shared_ptr<Screen>;
    using screens_t = std::vector<screenptr_t>;

    Main(gfx::TextureRepository& texManager, std::shared_ptr<logic::ScriptParser> scriptParser, glm::vec2 screenSize);
    ~Main() override = default;

    void	update(float timeDelta);
    void	render() const;

    void	Print(float x, float y, std::string text, float ScaleX = 1.0f, float ScaleY = 1.0f);
    void	PrintConsole(const std::string& text);

  private:
    void eventPointerMoveX(float posX);
    void eventPointerMoveY(float posY);
    void eventMouseMove(const glm::vec2& pos);
    void eventProcess(GuiEventType type);

    glm::vec2 getScreenSize() const;
    float getAspectRatio() const;

    std::shared_ptr<TextPrinter> textPrinter;
    std::shared_ptr<logic::ScriptParser>	ScriptParser;

    std::shared_ptr<gfx::Texture> CH;
    std::shared_ptr<gfx::Texture> Cursor;

    std::shared_ptr<MenuMain> menu;

    screens_t screenStack;

    glm::vec2 screenSize;

    unsigned	cursorX, cursorY;

    glm::vec2 pointerPos;
  };
}
