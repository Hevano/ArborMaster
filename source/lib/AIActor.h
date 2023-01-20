#pragma once
#include "Blackboard.h"
#include "BehaviourTree.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <any>

namespace ArborMaster
{
class AIActor
{
public:
  std::string label;

private:
  std::shared_ptr<BehaviourTree> m_tree;
  std::unordered_map<std::string, std::any> m_blackboardData;

public:
  AIActor() = default;
  AIActor(BehaviourTree& t, const std::string& l);
  const BehaviourTree& getTree() const;
  const std::unordered_map<std::string, std::any>& getBlackboard() const;
  const std::unordered_set<std::string>& getBlackboardKeys() const;
  void setBlackboard(const Blackboard& bb);
};
}

