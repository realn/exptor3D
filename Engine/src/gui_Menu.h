#pragma once

#include <glm/vec2.hpp>

#include "core_FontInfo.h"
#include "core_Transformer.h"

#include "GUIRenderContext.h"

namespace gui {
  class RenderContext;
  class TextPrinter;
  class MenuItem;
  class Screen;
  class Element;
  class TextElement;
  class StackElement;

  enum class MenuState {
    HIDDEN = 0,
    VISIBLE,
    HIDING,
    REVEALING
  };

  class Menu {
  public:
    using menuitemptr_t = std::shared_ptr<MenuItem>;

    Menu(const cb::string& id, const core::FontInfo& fontInfo);
    ~Menu();

    void	update(const float fTD);
    RenderContext	makeRender(gui::TextPrinter& printer) const;

    void	eventMouseMove(const glm::vec2& pos);
    void	eventMoveUp();
    void	eventMoveDown();
    bool	eventEnter(cb::string& outScript);

    void	addMenuItem(menuitemptr_t item);

    void	setTitle(const cb::string& value);
    void	setTitlePos(const glm::vec2& value);
    void	setSize(const glm::vec2& value);
    void	setVisible(bool value, bool animate);

    const cb::string& getId() const;
    bool	isVisible() const;
    bool	isAnimating() const;
    MenuState getState() const;

  private:
    using menuitems_t = std::vector<menuitemptr_t>;
    using screenptr_t = std::shared_ptr<Screen>;
    using elementptr_t = std::shared_ptr<Element>;
    using textelementptr_t = std::shared_ptr<TextElement>;
    using stackelementptr_t = std::shared_ptr<StackElement>;

    const cb::string	id;
    core::FontInfo fontInfo;

    screenptr_t screen;
    textelementptr_t titleElement;
    stackelementptr_t itemsElement;

    menuitems_t items;
    glm::vec2	size;

    menuitemptr_t selectedItem;

    core::ValueTransformer<float> scroll;
    MenuState state = MenuState::HIDDEN;
  };
}