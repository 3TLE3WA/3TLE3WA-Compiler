#include "3tle3wa/backend/scheduler/FIFO.hh"

SchedFIFO::SchedFIFO() {}

void SchedFIFO::Push(SchedItem *item) { view_.push_back(item); }

void SchedFIFO::Sched() {}