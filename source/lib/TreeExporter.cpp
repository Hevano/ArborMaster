#include "TreeExporter.h"

namespace ArborMaster
{
const std::string& TreeExporter::getPath() const
{
  return m_path;
}
void TreeExporter::setPath(const std::string& path) {
  m_path = path;
}
void TreeExporter::exportTree(const BehaviourTree& bt) const {}
void TreeExporter::saveDesign(const BehaviourTree& bt, const std::string& path) const
{
}
BehaviourTree TreeExporter::loadDesign(const std::string& path)
{
  return BehaviourTree();
}
}  // namespace ArborMaster