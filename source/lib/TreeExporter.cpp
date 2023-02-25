#include "TreeExporter.h"
#include "EditorNode.h"

#include <nlohmann/json.hpp>

#include <fstream>
#include <algorithm>

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
    const std::unordered_map<int, std::vector<int>>& adjList,
    const std::unordered_map<int, EditorNode>& nodes) const
{
  
  json tree;

  for (auto childId : adjList.at(1))
  {
    tree["root"] = traverseTree(childId, adjList, nodes);
  }

  std::ofstream stream(m_path);
  stream << std::setw(4) << tree << std::endl;
  stream.close();
}
void TreeExporter::saveDesign(const BehaviourTree& bt, const std::string& path) const
{
}
BehaviourTree TreeExporter::loadDesign(const std::string& path)
{
  return BehaviourTree();
}
json TreeExporter::traverseTree(
    int id, 
    const std::unordered_map<int, std::vector<int>>& adjList,
    const std::unordered_map<int, EditorNode>& nodes) const
{
  json subtree;

  subtree["id"] = id;
  subtree["name"] = nodes.at(id).treeNode.name;

  if (adjList.contains(id))
  {
    for (auto childId : adjList.at(id)) {
    subtree["children"].push_back(traverseTree(childId, adjList, nodes));
  }
  }
  

  return subtree;
}
}  // namespace ArborMaster