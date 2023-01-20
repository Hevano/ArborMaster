#include "AIActor.h"

namespace ArborMaster
{
AIActor::AIActor(BehaviourTree& t, const std::string& l)
    : m_tree(&t)
    , label(l)
{
  setBlackboard(t.getBlackboard());
}
const BehaviourTree& AIActor::getTree() const
{
  return *m_tree;
}
const std::unordered_map<std::string, std::any>& AIActor::getBlackboard() const
{
  return m_blackboardData;
}
const std::unordered_set<std::string>& AIActor::getBlackboardKeys() const
{
  std::unordered_set<std::string> output;
  return output;
}
void AIActor::setBlackboard(const Blackboard& bb) {
    
}
}