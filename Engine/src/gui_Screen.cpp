
#include <CBCore/StringConvert.h>

#include "StrEx.h"
#include "core_FileParser.h"

#include "gui_StackElement.h"
#include "gui_Element.h"
#include "gui_TextElement.h"
#include "gui_ButtonElement.h"
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

  void Screen::eventPointerMove(const glm::vec2& pointerPos, const glm::vec2& screenSize) {
    for (auto element : elements) {
      element->setFocus(element->containsPoint(pointerPos, screenSize));
      element->eventPointerMove(pointerPos, screenSize);
    }
  }

  void Screen::eventProcess(GuiEventType type) {
    for (auto element : elements) {
      element->eventProcess(type);
    }
  }

  void	Screen::setMargin(const glm::vec2& value) {
    margin = value;
  }

  void Screen::addElement(std::shared_ptr<Element> element) {
    elements.push_back(element);
  }

  class ScreenParserContext {
  public:
    using elementptr_t = std::shared_ptr<Element>;
    using containers_t = std::vector<std::shared_ptr<ContainerElement>>;

    ScreenParserContext(Screen& scr) : screen(scr) {}

    template<class _Type, class... _Args>
    void addElement(_Args... args) {
      auto elem = std::make_shared<_Type>(args...);
      add(elem);
    }

    template<class _Type, class... _Args>
    void addContainer(_Args... args) {
      auto elem = std::make_shared<_Type>(args...);
      add(elem);
      containers.push_back(elem);
    }

    void add(elementptr_t elem) {
      element = elem;
      if (containers.empty()) {
        screen.addElement(element);
      }
      else {
        containers.back()->addElement(element);
      }
    }

    template<class _Type>
    std::shared_ptr<_Type> get() {
      return std::dynamic_pointer_cast<_Type>(element);
    }

    template<class _Type>
    std::shared_ptr<_Type> getCont() {
      if (containers.empty())
        return nullptr;
      return std::dynamic_pointer_cast<_Type>(containers.back());
    }


    bool valid() const {
      return element != nullptr;
    }

    bool validContainer() {
      return !containers.empty();
    }

    void popContainer() {
      if (containers.empty())
        return;

      containers.pop_back();
      if (!containers.empty())
        element = containers.back();
    }

    Screen& screen;
    elementptr_t element;
    containers_t containers;
  };

  bool Screen::load(const cb::string& filename, const core::FontInfo& fontInfo) {
    if (filename.empty())
      return false;

    auto parser = core::FileParser(filename);
    ScreenParserContext ctx(*this);

    while (parser.readLine()) {

      auto cmd = parser.getCmd();
 
      if (cmd == L"TEXT") {
        ctx.addElement<TextElement>(fontInfo);
        ctx.get<TextElement>()->setText(parser.getArg(0));
      }
      else if (cmd == L"BUTTON") {
        ctx.addElement<ButtonElement>(fontInfo);
        ctx.get<ButtonElement>()->setText(parser.getArg(0));
      }
      else if (cmd == L"STACK") {
        ctx.addContainer<StackElement>();
      }
      else if (cmd == L"END") {
        ctx.popContainer();
      }
      else if (cmd == L"ITEMPADDING" && ctx.validContainer() ) {
        ctx.getCont<ContainerElement>()->setItemPadding(parser.getVec2FromArgs(0));
      }
      else if (cmd == L"ITEMSPACE" && ctx.validContainer()) {
        ctx.getCont<ContainerElement>()->setItemSpace(parser.getFloat(0));
      }
      else if (cmd == L"ALIGN" && ctx.valid()) {
        ctx.get<Element>()->setAlign(getElementAlignH(parser.getArg(0)), getElementAlignV(parser.getArg(1)));
      }
      else if (cmd == L"MARGIN" && ctx.valid()) {
        ctx.get<Element>()->setMargin(parser.getVec2FromArgs(0));
      }
      else if (cmd == L"SCALE" && ctx.valid()) {
        ctx.get<TextElement>()->setScale(parser.getVec2FromArgs(0, glm::vec2(1.0f)));
      }
      else if (cmd == L"COLOR" && ctx.valid()) {
        ctx.get<Element>()->setColor(parser.getVec4FromArgs(0, glm::vec4(1.0f)));
      }
      else if (cmd == L"SYNCVALUE") {
        auto holder = ctx.get<IValueHolder>();
        if (holder != nullptr) {
          values.addValueSync(holder, parser.getArg(0), parser.getArg(1), parser.getArg(2));
        }
      }
    }

    return true;
  }


}
