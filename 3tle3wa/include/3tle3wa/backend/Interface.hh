#pragma once

#include <string>
#include <string_view>

#include "3tle3wa/utils/logs.hh"

class InternalTranslation;
struct InternalTranslationContext;

class Serializable {
   private:
    char *buf_{nullptr};
    size_t buflen_{0};
    FILE *fp_{nullptr};

   protected:
    virtual void formatString(FILE *fp) = 0;

   public:
    virtual std::string ToString() final;

    virtual std::string_view ToStringView() final;

    virtual const char *CString() final;

    virtual void MakeString() final;

    ~Serializable();
};

class Translatable {
   public:
    virtual void TranslateTo(InternalTranslation &itx, InternalTranslationContext &ctx);
};