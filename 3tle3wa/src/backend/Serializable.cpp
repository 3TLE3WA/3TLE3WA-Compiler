#include "3tle3wa/backend/Interface.hh"

void Serializable::MakeString() {
    if (buf_) {
        free(buf_);
        buf_ = nullptr;
        buflen_ = 0;
    }

    fp_ = open_memstream(&buf_, &buflen_);
    Assert(fp_, "open memstream fail");

    formatString(fp_);

    fflush(fp_);
    fclose(fp_);
    fp_ = nullptr;
}

std::string Serializable::ToString() {
    if (buf_ == nullptr) {
        MakeString();
    }
    Assert(buf_, "MakeString not implemented well");
    return std::string(buf_);
}

std::string_view Serializable::ToStringView() {
    if (buf_ == nullptr) {
        MakeString();
    }
    Assert(buf_, "MakeString not implemented well");
    return std::string_view(buf_, buflen_);
}

const char *Serializable::CString() {
    if (buf_ == nullptr) {
        MakeString();
    }
    Assert(buf_, "MakeString not implemented well");
    return buf_;
}

Serializable::~Serializable() {
    if (buf_) {
        free(buf_);
        buf_ = nullptr;
        buflen_ = 0;
    }
}