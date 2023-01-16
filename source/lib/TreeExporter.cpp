#include "TreeExporter.h"

namespace ArborMaster
{
void TreeExporter::exportTree(const BehaviourTree& bt) const {}
void TreeExporter::saveDesign(const BehaviourTree& bt,
                              const std::string& path) const
{
}
BehaviourTree TreeExporter::loadDesign(const std::string& path)
{
  return BehaviourTree();
}
}  // namespace ArborMaster