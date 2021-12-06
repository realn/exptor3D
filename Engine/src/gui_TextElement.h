#pragma once

#include <CBCore/Defines.h>

#include "core_FontInfo.h"
#include "gui_Element.h"
#include "gui_ValueSync.h"

namespace gui {
  class TextElement : public Element, public IValueHolder {
  protected:
    core::FontInfo fontInfo;
    std::string		text;

  public:
    TextElement(const core::FontInfo& info);
    ~TextElement() override;

    virtual void	render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) override;
    virtual void	update(const float timeDelta) override;

    void	setText(const std::string& value);

    glm::vec2		getSize() const override;
    std::string	getText() const;

    void setSyncValue(const std::string& value) override;
    const std::string getSyncValue() const override;
  };

}
