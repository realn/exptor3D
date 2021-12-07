#pragma once

#include <CBCore/Defines.h>

#include "core_FontInfo.h"
#include "gui_Element.h"
#include "gui_ValueSync.h"

namespace gui {
  class TextElement : public Element, public IValueHolder {
  public:
    TextElement(const core::FontInfo& info);
    ~TextElement() override;

    virtual void	render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize) const override;
    virtual void	update(const float timeDelta) override;

    void	setText(const std::string& value);
    void	setScale(const glm::vec2& value);

    glm::vec2		getSize() const override;
    std::string	getText() const;
    glm::vec2	getScale() const;

    void setSyncValue(const std::string& value) override;
    const std::string getSyncValue() const override;

  protected:
    core::FontInfo fontInfo;
    std::string		text;
    glm::vec2	scale = glm::vec2(1.0f);
  };

}
