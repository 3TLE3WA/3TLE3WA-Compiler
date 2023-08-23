#include "3tle3wa/backend/scheduler/SchedMachine.hh"

#include "3tle3wa/backend/scheduler/SchedItem.hh"
#include "3tle3wa/backend/scheduler/SchedPolicy.hh"

/**
 * 读写问题
 * 1. 对于一个单元，当仅有写者正在写入
 *       - 此时进入一个读者，读者的前驱为写者，并加入读者队列
 *       - 此时进入一个写者，写者的前驱为上一个写者，并更新写者为自己
 * 2. 对于一个单元，当仅有读者正在读取
 *       - 此时进入一个读者，加入读者队列
 *       - 此时进入一个写者，写者前驱为当前所有读者，更新写者为自己，并清空读者队列
 * 3. 对于一个单元，当有写者写入，也有读者读取
 *       - 保证读者队列的前驱为目前的写者
 *       - 此时进入一个读者，读者的前驱为写者，并加入读者队列
 *       - 此时进入一个写者，写者前驱为上一个写者和当前所有读者，更新写者为自己，并清空读者队列
 * 4. 对于一个 fence 属性指令，目前为跳转
 *       - 保证它是目前所有操作的后继，是后来所有操作的前驱
 */

SchedMachine::SchedMachine(std::unique_ptr<SchedPolicy> policy) : policy_(std::move(policy)) {}

void SchedMachine::Push(std::unique_ptr<SchedItem> item) {
    policy_->Push(item.get());
    items_.push_back(std::move(item));
}

void SchedMachine::Sched() { policy_->Sched(); }

SchedPolicy *SchedMachine::Policy() { return policy_.get(); }