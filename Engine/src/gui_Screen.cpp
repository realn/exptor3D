
#include <CBCore/StringConvert.h>

#include "StrEx.h"
#include "FileParser.h"

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

  void Screen::render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) {
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

  const bool	Screen::load(const std::string& filename, const core::FontInfo& fontInfo) {
    if (filename.empty())
      return false;

    auto parser = core::FileParser(cb::fromUtf8(filename));

    std::shared_ptr<Element> element;

    while (parser.readLine()) {

      auto cmd = parser.getCmd();
 
      if (cmd == L"TEXT") {
        auto ele = std::make_shared<TextElement>(fontInfo);
        ele->setText(cb::toUtf8(parser.getArg(0)));
        element = ele;
        addElement(element);
      }
      else if (cmd == L"ALIGN") {
        if (element != nullptr)
          element->setAlign(getElementAlignH(parser.getArg(0)), getElementAlignV(parser.getArg(1)));
      }
      else if (cmd == L"MARGIN") {
        if (element != nullptr)
          element->setMargin(parser.getVec2FromArgs(0));
      }
      else if (cmd == L"SCALE") {
        if (element != nullptr)
          element->setScale(parser.getVec2FromArgs(0, glm::vec2(1.0f)));
      }
      else if (cmd == L"COLOR") {
        if (element != nullptr) {
          element->setColor(parser.getVec4FromArgs(0, glm::vec4(1.0f)));
        }
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
