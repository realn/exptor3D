#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#include "GUITextPrinter.h"
#include "GUIRenderContext.h"

#include "gui_MenuItem.h"
#include "gui_Menu.h"

namespace gui {
  Menu::Menu(const std::string& id) : id(id) {}

  Menu::~Menu() = default;

  void	Menu::update(const float timeDelta, const core::FontInfo& fontInfo) {
    if (visible) {
      if (scroll > 0.0f)
        scroll -= timeDelta * 2.0f;
      else {
        scroll = 0.0f;
        for (auto& item : items) {
          item->update(timeDelta, fontInfo);
        }
      }
    }
    else {
      if (scroll < 1.0f)
        scroll += timeDelta * 2.0f;
      else
        scroll = 1.0f;
    }
  }

  RenderContext Menu::makeRender(gui::TextPrinter& printer) const {
    auto ctx = RenderContext();
    ctx.setProjectionMatrix(glm::ortho(0.0f, size.x, size.y, 0.0f));

    ctx.pushMatrix();
    ctx.setColor({ 1.0f, 1.0f, 1.0f, 1.0f });

    ctx.translate({ scroll * size.x, 0.0f, 0.0f });

    ctx.pushMatrix();
    ctx.scale({ 2.5f, 2.5f, 1.0f });
    printer.print(ctx, titlePos, title);
    ctx.popMatrix();

    for (auto& item : items) {
      item->render(ctx, printer);
    }

    ctx.popMatrix();

    return ctx;
  }

  void Menu::eventMouseMove(const glm::vec2& pos) {
    auto screenPos = pos * size;

    selectedItem = menuitemptr_t();
    auto it = std::find_if(items.begin(), items.end(), [&](menuitemptr_t item) {return item->contains(screenPos); });
    if (it != items.end()) {
      selectedItem = *it;
    }

    for (auto& item : items) {
      item->setHighlight(item == selectedItem);
    }
  }

  bool	Menu::eventEnter(std::string& outScript) {
    if (selectedItem) {
      outScript = selectedItem->getScript();
      return true;
    }
    return false;
  }

  void	Menu::eventMoveUp() {
    auto it = std::find(items.rbegin(), items.rend(), selectedItem);
    if (it != items.rend()) {
      it++;
    }
    if (it == items.rend()) {
      it = items.rbegin();
    }

    selectedItem = it != items.rend() ? *it : menuitemptr_t();

    for (auto& item : items) {
      item->setHighlight(item == selectedItem);
    }
  }

  void	Menu::eventMoveDown() {
    auto it = std::find(items.begin(), items.end(), selectedItem);
    if (it != items.end()) {
      it++;
    }
    if (it == items.end()) {
      it = items.begin();
    }

    selectedItem = it != items.end() ? *it : menuitemptr_t();

    for (auto& item : items) {
      item->setHighlight(item == selectedItem);
    }
  }

  void Menu::addMenuItem(menuitemptr_t item) {
    items.push_back(item);
  }

  void	Menu::setTitle(const std::string& value) {
    title = value;
  }

  void	Menu::setTitlePos(const glm::vec2& value) {
    titlePos = value;
  }

  void	Menu::setSize(const glm::vec2& value) {
    size = value;
  }

  void	Menu::setVisible(bool value, bool animate) {
    if (value) {
      visible = true;
      if (!animate)
        scroll = 0.0f;
    }
    else {
      visible = false;
      if (!animate)
        scroll = 1.0f;
    }
  }

  const std::string& Menu::getId() const {
    return id;
  }

  bool	Menu::isVisible() const {
    return visible;
  }

  bool	Menu::isAnimating() const {
    if (visible)
      return scroll > 0.0f;
    else
      return scroll < 1.0f;
  }
}