#include "3tle3wa/backend/rl/LiveInterval.hh"

Segment::Segment(size_t bgn, size_t end, VirtualRegister *belong_to) : bgn_(bgn), end_(end), belong_to_(belong_to) {}

bool Segment::operator<(const Segment &other) const { return this->bgn_ < other.bgn_; }

size_t Segment::GetBegin() const { return bgn_; }

size_t Segment::GetEnd() const { return end_; }

VirtualRegister *Segment::GetOwner() const { return belong_to_; }

bool Segment::Contain(size_t idx) const { return idx >= bgn_ and idx < end_; }

void Segment::ExtendEnd(size_t end) { end_ = end; }

LiveInterval::LiveInterval() : segs_(), live_size_(0) {}

void LiveInterval::MakeSeg(size_t bgn, size_t end, VirtualRegister *belong_to) {
    if (bgn == end) {
        return;
    }

    Segment seg(bgn, end, belong_to);
    live_size_ += end - bgn;

    if (segs_.empty()) {
        segs_.insert(seg);
        return;
    }

    auto front = --segs_.lower_bound(seg);

    if (front->GetEnd() == seg.GetBegin()) {
        Segment nwseg(front->GetBegin(), seg.GetEnd(), belong_to);
        segs_.erase(front);
        segs_.insert(nwseg);
    } else {
        segs_.insert(seg);
    }
}

void LiveInterval::ClearAll() { segs_.clear(); }

size_t LiveInterval::LiveSize() { return live_size_; }

size_t LiveInterval::SegNum() { return segs_.size(); }

bool LiveInterval::LiveAt(size_t idx) {
    for (auto &&seg : segs_) {
        if (seg.Contain(idx)) {
            return true;
        }
    }
    return false;
}

std::set<Segment> &LiveInterval::Segs() { return segs_; }