#include "AIActor.h"

namespace ArborMaster
{
const BehaviourTree& AIActor::getTree() const
{
  return *m_tree;
}
const Blackboard& AIActor::getBlackboard() const
{
  return m_bb;
}
}