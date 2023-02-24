#include "TreeExporter.h"
#include "EditorNode.h"

#include <nlohmann/json.hpp>

#include <unordered_set>

#include <fstream>

namespace ArborMaster
{
const std::string& TreeExporter::getPath() const
{
  return m_path;
}
void TreeExporter::setPath(const std::string& path) {
  m_path = path;
}
void TreeExporter::exportTree(
    const std::unordered_map<int, EditorLink>& links,
    const std::unordered_map<int, EditorNode>& nodes) const
{
  using json = nlohmann::json;

  json j;

  for (const auto& [id, link] : links) {
    json node;
    int parentId = link.startId >> 16;
    int childId = link.endId >> 8;
    node["id"] = childId;
    node["parentId"] = parentId;
    node["name"] = nodes.at(childId).treeNode.name;
    j[std::to_string(childId)] = node;
  }

  std::ofstream stream(m_path);
  stream << std::setw(4) << j << std::endl;
  stream.close();
}
void TreeExporter::saveDesign(const BehaviourTree& bt, const std::string& path) const
{
}
BehaviourTree TreeExporter::loadDesign(const std::string& path)
{
  return BehaviourTree();
}
}  // namespace ArborMaster