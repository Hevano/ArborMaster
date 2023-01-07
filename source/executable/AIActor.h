#pragma once
#include "Blackboard.h"

#include <string>
#include <memory>

namespace ArborMaster
{
class AIActor
{
public:
  std::string label;

private:
  std::shared_ptr<BehaviourTree> m_tree;
  Blackboard m_bb;

public:
  const BehaviourTree& getTree() const;
  const Blackboard& getBlackboard() const;
};
}

