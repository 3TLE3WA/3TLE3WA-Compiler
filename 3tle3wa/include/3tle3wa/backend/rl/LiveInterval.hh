#pragma once

#include <cstddef>
#include <memory>
#include <set>

class VirtualRegister;

class Segment {
    size_t bgn_;
    size_t end_;

    VirtualRegister *belong_to_;

   public:
    Segment(size_t bgn, size_t end, VirtualRegister *belong_to);

    bool operator<(const Segment &other) const;

    size_t GetBegin() const;
    size_t GetEnd() const;
    VirtualRegister *GetOwner() const;

    bool Contain(size_t idx) const;

    void ExtendEnd(size_t end);
};

class LiveInterval {
    std::set<Segment> segs_;
    size_t live_size_;

   public:
    LiveInterval();

    void MakeSeg(size_t bgn, size_t end, VirtualRegister *belong_to);
    void ClearAll();

    size_t LiveSize();
    size_t SegNum();

    bool LiveAt(size_t idx);

    std::set<Segment> &Segs();
};