#pragma once

#include <functional>

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
    using menuitemweakptr_t = std::weak_ptr<MenuItem>;
    using func_t = std::function<void(menuitemptr_t)>;

    Menu(const cb::string& id, const core::FontInfo& fontInfo);
    ~Menu();

    void update(float timeDelta);
    void render(RenderContext& ctx, gui::TextPrinter& printer, glm::vec2 screenSize) const;

    void	eventMouseMove(const glm::vec2& pos, const glm::vec2& screenSize);
    void	eventMoveUp();
    void	eventMoveDown();
    void	eventEnter();

    void	addMenuItem(menuitemptr_t item);
    menuitemptr_t addMenuItem(cb::string id, cb::string title = cb::string(), cb::string script = cb::string());
    menuitemptr_t addMenuItemPush(cb::string id, cb::string title, cb::string menuId);
    menuitemptr_t addMenuItemPop(cb::string id, cb::string title);

    void clearItems();

    void setTitle(const cb::string& value);
    void setTitlePos(const glm::vec2& value);
    void setVisible(bool value, bool animate);
    void setEnterFunction(func_t func);

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

    void invokeEnter(menuitemptr_t item);

    const cb::string	id;
    core::FontInfo fontInfo;

    screenptr_t screen;
    textelementptr_t titleElement;
    stackelementptr_t itemsElement;

    menuitems_t items;

    menuitemptr_t selectedItem;

    core::ValueTransformer<float> scroll;
    MenuState state = MenuState::HIDDEN;

    func_t enterFunction;
  };
}