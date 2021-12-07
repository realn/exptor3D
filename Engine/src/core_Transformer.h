#pragma once

#include <chrono>
#include <vector>
#include <memory>

#include <glm/common.hpp>

namespace core {
  class Transformer {
  public:
    virtual ~Transformer();

    virtual void update(float timeDelta) = 0;

    virtual void reset() = 0;
    virtual void finish() = 0;

    virtual void setDuration(std::chrono::duration<float> seconds) = 0;

    void setEnabled(bool val);
    void setReverse(bool val);

    bool isEnabled() const;
    bool isReversed() const;

  protected:
    bool enabled = false;
    bool reversed = false;
  };

  class TransformerList {
  public:
    using transformerptr_t = std::shared_ptr<Transformer>;

    ~TransformerList();

    void update(float timeDelta);

    void add(transformerptr_t item);

  private:
    using transformers_t = std::vector<transformerptr_t>;

    transformers_t transformers;
  };

  template<class _Type>
  class ValueTransformer : public Transformer {
  public:
    ~ValueTransformer() override = default;

    void update(float timeDelta) override {
      if (!reversed) {
        value += step * _Type(timeDelta);
      }
      else {
        value -= step * _Type(timeDelta);
      }
      value = glm::clamp(value, min, max);
    }

    void reset() {
      value = min;
    }

    void finish() {
      value = max;
    }

    _Type getValue() const {
      return value;
    }

    void setStep(_Type val) {
      step = val;
    }

    void setDuration(std::chrono::duration<float> seconds) override {
      step = (max - min) / _Type(seconds.count());
    }

    void setRange(_Type minVal, _Type maxVal) {
      min = minVal;
      max = maxVal;
    }

  private:
    _Type value;
    _Type min;
    _Type max;
    _Type step;
  };
}
