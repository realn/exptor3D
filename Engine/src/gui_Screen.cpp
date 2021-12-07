
#include <CBCore/StringConvert.h>

#include "StrEx.h"
#include "FileParser.h"

#include "gui_StackElement.h"
#include "gui_Element.h"
#include "gui_TextElement.h"
#include "gui_Screen.h"

namespace gui {
  namespace {
    const HAlign	getElementAlignH(const cb::string& align) {
      auto al = core::trim(align);
      if (al == L"LEFT")
        return HAlign::Left;
      if (al == L"CENTER")
        return HAlign::Center;
      if (al == L"RIGHT")
        return HAlign::Right;

      return HAlign::Center;
    }

    const VAlign	getElementAlignV(const cb::string& align) {
      auto al = core::trim(align);
      if (al == L"TOP")
        return VAlign::Top;
      if (al == L"CENTER")
        return VAlign::Middle;
      if (al == L"BOTTOM")
        return VAlign::Bottom;

      return VAlign::Middle;
    }
  }

  Screen::~Screen() = default;

  void Screen::render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) const {
    for (auto& item : elements) {
      item->render(ctx, printer, screenSize);
    }
  }

  void	Screen::update(const float timeDelta) {
    for (auto& item : elements) {
      item->update(timeDelta);
    }
  }

  void	Screen::setMargin(const glm::vec2& value) {
    margin = value;
  }

  void Screen::addElement(std::shared_ptr<Element> element) {
    elements.push_back(element);
  }

  bool Screen::elementContainsPoint(elementptr_t element, const glm::vec2& point, const glm::vec2& screenSize, bool searchDown) const {
    for (auto item : elements) {
      if (item == element) {
        return item->containsPoint(point, screenSize);
      }

      if (searchDown) {
        auto cont = std::dynamic_pointer_cast<ContainerElement>(item);
        if (cont && cont->elementContainsPoint(element, point, screenSize, searchDown))
          return true;
      }
    }
    return false;
  }

  const bool	Screen::load(const std::string& filename, const core::FontInfo& fontInfo) {
    if (filename.empty())
      return false;

    auto parser = core::FileParser(cb::fromUtf8(filename));

    using containers_t = std::vector<std::shared_ptr<ContainerElement>>;

    std::shared_ptr<Element> element;
    containers_t containers;

    while (parser.readLine()) {

      auto cmd = parser.getCmd();
 
      if (cmd == L"TEXT") {
        auto ele = std::make_shared<TextElement>(fontInfo);
        ele->setText(cb::toUtf8(parser.getArg(0)));
        element = ele;
        if (containers.empty()) {
          addElement(element);
        }
        else {
          containers.back()->addElement(element);
        }
      }
      else if (cmd == L"STACK") {
        auto cont = std::make_shared<StackElement>();
        element = cont;
        if (containers.empty())
          addElement(element);
        else
          containers.back()->addElement(element);
        containers.push_back(cont);
      }
      else if (cmd == L"END" && !containers.empty()) {
        containers.pop_back();
        if(!containers.empty())
          element = containers.back();
      }
      else if (cmd == L"ITEMPADDING" && !containers.empty() ) {
        auto cont = containers.back();
        cont->setItemPadding(parser.getVec2FromArgs(0));
      }
      else if (cmd == L"ITEMSPACE" && !containers.empty()) {
        auto cont = containers.back();
        cont->setItemSpace(parser.getFloat(0));
      }
      else if (cmd == L"ALIGN" && element) {
        element->setAlign(getElementAlignH(parser.getArg(0)), getElementAlignV(parser.getArg(1)));
      }
      else if (cmd == L"MARGIN" && element) {
        element->setMargin(parser.getVec2FromArgs(0));
      }
      else if (cmd == L"SCALE" && element) {
        auto text = std::dynamic_pointer_cast<TextElement>(element);
        if (text) {
          text->setScale(parser.getVec2FromArgs(0, glm::vec2(1.0f)));
        }
      }
      else if (cmd == L"COLOR" && element) {
        element->setColor(parser.getVec4FromArgs(0, glm::vec4(1.0f)));
      }
      else if (cmd == L"SYNCVALUE") {
        auto holder = std::dynamic_pointer_cast<IValueHolder>(element);
        if (holder != nullptr) {
          values.addValueSync(holder, parser.getArgUtf8(0), parser.getArgUtf8(1), parser.getArgUtf8(2));
        }
      }
    }

    return true;
  }


}
