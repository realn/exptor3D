#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#include "GUITextPrinter.h"
#include "GUIRenderContext.h"

#include "gui_Screen.h"
#include "gui_TextElement.h"
#include "gui_StackElement.h"
#include "gui_MenuItem.h"
#include "gui_Menu.h"

namespace gui {
  Menu::Menu(const std::string& id, const core::FontInfo& fontInfo) : id(id), fontInfo(fontInfo) {

    scroll.setRange(0.0f, 1.0f);
    scroll.setStep(2.0f);
    scroll.setReverse(true);
    scroll.setEnabled(true);

    screen = std::make_shared<Screen>();

    titleElement = std::make_shared<TextElement>(fontInfo);
    titleElement->setScale({ 2.0f, 2.0f });
    itemsElement = std::make_shared<StackElement>();
    itemsElement->setAlign(HAlign::Left, VAlign::Top);
    itemsElement->setItemPadding({ 10.0f, 10.0f });
    itemsElement->setItemSpace(10.0f);

    {
      auto layout = std::make_shared<StackElement>();
      layout->setAlign(HAlign::Left, VAlign::Top);
      layout->setMargin({ 20.0f, 20.0f });
      layout->setItemSpace(20.0);
      layout->addElement(titleElement);
      layout->addElement(itemsElement);
      screen->addElement(layout);
    }
  }

  Menu::~Menu() = default;

  void	Menu::update(const float timeDelta) {
    scroll.update(timeDelta);

    if (scroll.isFinished()) {
      state = MenuState::HIDDEN;
    }
    else if (scroll.isOnStart()) {
      state = MenuState::VISIBLE;
    }
    else if (scroll.isReversed()) {
      state = MenuState::REVEALING;
    }
    else {
      state = MenuState::HIDING;
    }

    if (state == MenuState::VISIBLE) {
      screen->update(timeDelta);
    }
  }

  RenderContext Menu::makeRender(gui::TextPrinter& printer) const {
    auto ctx = RenderContext();
    ctx.setProjectionMatrix(glm::ortho(0.0f, size.x, size.y, 0.0f));

    if (state != MenuState::HIDDEN) {
      ctx.pushMatrix();
      if (isAnimating()) {
        ctx.translate({ scroll.getValue() * glm::vec2(0.0f, size.y), 0.0f });
      }
      screen->render(ctx, printer, size);
      ctx.popMatrix();
    }

    return ctx;
  }

  void Menu::eventMouseMove(const glm::vec2& pos) {
    auto screenPos = pos * size;

    selectedItem = menuitemptr_t();
    auto it = std::find_if(items.begin(), items.end(), [&](menuitemptr_t item) {return screen->elementContainsPoint(item, screenPos, size); });
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
    itemsElement->addElement(item);
  }

  void	Menu::setTitle(const std::string& value) {
    titleElement->setText(value);
  }

  void	Menu::setTitlePos(const glm::vec2& value) {
    titleElement->setMargin(value);
  }

  void	Menu::setSize(const glm::vec2& value) {
    size = value;
  }

  void	Menu::setVisible(bool value, bool animate) {
    scroll.setReverse(value);
    if (animate) {
      if (scroll.isReversed()) {
        state = MenuState::REVEALING;
        scroll.finish();
      }
      else {
        state = MenuState::HIDING;
        scroll.reset();
      }
    }
    else {
      if (scroll.isReversed()) {
        state = MenuState::VISIBLE;
        scroll.reset();
      }
      else {
        state = MenuState::HIDDEN;
        scroll.finish();
      }
    }
  }

  const std::string& Menu::getId() const {
    return id;
  }

  bool	Menu::isVisible() const {
    return state == MenuState::VISIBLE;
  }

  bool	Menu::isAnimating() const {
    return state == MenuState::HIDING || state == MenuState::REVEALING;
  }

  MenuState Menu::getState() const {
    return state;
  }
}