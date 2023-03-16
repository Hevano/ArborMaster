#include "TreeExporter.h"
#include "EditorNode.h"

#include <nlohmann/json.hpp>
#include "imnodes.h"

#include <fstream>
#include <algorithm>
#include <filesystem>

namespace ArborMaster
{
void TreeExporter::exportTree(const EditorTree& tree) const
{
  json jTree;

  /*if (tree.getPath().empty()) {
    std::filesystem::path designPath(m_path);
    designPath.replace_filename(designPath.filename().string() + "Design");

  }*/

  std::filesystem::path designPath(tree.getPath());

  jTree["debugPath"] = json(std::filesystem::absolute(designPath).c_str());

  for (auto childId : tree.m_adjList.at(1))
  {
    jTree["root"] = traverseTree(childId, tree.m_adjList, tree.m_editorNodes);
  }

  std::ofstream stream(m_path);
  stream << std::setw(4) << jTree << std::endl;
  stream.close();
}

void to_json(json& j, const EditorLink& link)
{
  j = json{ {"id", link.id}, {"startId", link.startId}, {"endId", link.endId} };
}

void from_json(const json& j, EditorLink& link)
{
  j.at("id").get_to(link.id);
  j.at("startId").get_to(link.startId);
  j.at("endId").get_to(link.endId);
}

void to_json(json& j, const EditorNode& node)
{
  //We treat TreeNode ptrs as int for design serialization, because they may have change when we reload a design
  j = json{ {"id", node.id}, {"treeNode", reinterpret_cast<intptr_t>(node.treeNode)} };
}

void from_json(const json& j, EditorNode& node)
{
  j.at("id").get_to(node.id);
  node.treeNode = reinterpret_cast<const TreeNode*>(j.at("treeNode").get<intptr_t>());
}

void TreeExporter::saveDesign(const EditorTree& t, int currentId) const
{
  std::filesystem::path pathObj = t.getPath();
  std::filesystem::path iniPathObj = t.getPath();
  iniPathObj.replace_extension(".ini");

  ImNodes::SaveCurrentEditorStateToIniFile(iniPathObj.string().c_str());

  json j;

  //We need to create a table to translate the pointers to the treeNodes used with their names
  std::unordered_map<intptr_t, std::string> nodeDefs;
  for (auto& [id, node] : t.m_editorNodes) {
    nodeDefs.emplace(std::make_pair(reinterpret_cast<intptr_t>(node.treeNode), node.treeNode->name));
  }


  j["editorNodes"] = json(t.m_editorNodes);
  j["nodeDefs"] = json(nodeDefs);
  j["editorLinks"] = json(t.m_editorLinks);
  j["adjList"] = json(t.m_adjList);
  j["freeNodes"] = json(t.m_freeNodes);
  j["currentId"] = json(currentId);

  std::ofstream stream(pathObj);
  stream << std::setw(4) << j << std::endl;
  stream.close();
}
bool TreeExporter::loadDesign(EditorTree& t, const NodeFactory& nodeCache, const std::string& path)
{
  std::filesystem::path pathObj = t.getPath();
  std::filesystem::path iniPathObj = t.getPath();
  iniPathObj.replace_extension(".ini");

  if (!std::filesystem::exists(pathObj) || !std::filesystem::exists(iniPathObj)) {
    return false;
  }

  ImNodes::LoadCurrentEditorStateFromIniFile(iniPathObj.string().c_str());

  std::ifstream stream(path);
  json data;
  stream >> data;

  stream.close();

  data["editorNodes"].get_to(t.m_editorNodes);
  data["editorLinks"].get_to(t.m_editorLinks);
  data["adjList"].get_to(t.m_adjList);
  data["freeNodes"].get_to(t.m_freeNodes);
  data["currentId"].get_to(t.m_editorId);

  std::unordered_map<intptr_t, std::string> nodeDefs;
  data["nodeDefs"].get_to(nodeDefs);

  for (auto& [id, node] : t.m_editorNodes) {
    intptr_t tn = reinterpret_cast<intptr_t>(node.treeNode);
    if (nodeDefs.contains(tn) && nodeCache.getNodes().contains(nodeDefs[tn])) {
      node.treeNode = &nodeCache.getNodes().at(nodeDefs[tn]);
    }
    else {
      return false; //If we cannot find the translations, loading has failed
    }
  }

  t.m_designPath = path;

  return true;
}
json TreeExporter::traverseTree(
    int id, 
    const std::unordered_map<int, std::vector<int>>& adjList,
    const std::unordered_map<int, EditorNode>& nodes) const
{
  json subtree;

  subtree["id"] = id;
  subtree["name"] = nodes.at(id).treeNode->name;

  if (adjList.contains(id))
  {
    for (auto childId : adjList.at(id)) {
    subtree["children"].push_back(traverseTree(childId, adjList, nodes));
  }
  }
  

  return subtree;
}
}  // namespace ArborMaster