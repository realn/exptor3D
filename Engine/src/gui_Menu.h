#pragma once

#include <glm/vec2.hpp>

#include "core_FontInfo.h"
#include "GUIRenderContext.h"

namespace gui {
  class RenderContext;
  class TextPrinter;
  class MenuItem;
  class Screen;
  class Element;
  class TextElement;
  class StackElement;

  class Menu {
  public:
    using menuitemptr_t = std::shared_ptr<MenuItem>;

    Menu(const std::string& id, const core::FontInfo& fontInfo);
    ~Menu();

    void	update(const float fTD);
    RenderContext	makeRender(gui::TextPrinter& printer) const;

    void	eventMouseMove(const glm::vec2& pos);
    void	eventMoveUp();
    void	eventMoveDown();
    bool	eventEnter(std::string& outScript);

    void	addMenuItem(menuitemptr_t item);

    void	setTitle(const std::string& value);
    void	setTitlePos(const glm::vec2& value);
    void	setSize(const glm::vec2& value);
    void	setVisible(bool value, bool animate);

    const std::string& getId() const;
    bool	isVisible() const;
    bool	isAnimating() const;

  private:
    using menuitems_t = std::vector<menuitemptr_t>;
    using screenptr_t = std::shared_ptr<Screen>;
    using elementptr_t = std::shared_ptr<Element>;
    using textelementptr_t = std::shared_ptr<TextElement>;
    using stackelementptr_t = std::shared_ptr<StackElement>;

    const std::string	id;
    core::FontInfo fontInfo;

    screenptr_t screen;
    textelementptr_t titleElement;
    stackelementptr_t itemsElement;

    menuitems_t items;
    glm::vec2	size;

    menuitemptr_t selectedItem;

    bool	visible = true;
    float	scroll = 0.0f;
  };
}