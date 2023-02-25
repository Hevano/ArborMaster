#pragma once
#include "BehaviourTree.h"

#include <nlohmann/json.hpp>

#include <string>
#include <unordered_map>

namespace ArborMaster {

using json = nlohmann::json;

class EditorLink;
class EditorNode;

class TreeExporter
{
private:
  std::string m_path;

public:
  const std::string& getPath() const;
  void setPath(const std::string& path);
  //Exports the tree to m_path for use in client code
  void exportTree(const std::unordered_map<int, std::vector<int>>& adjList, const std::unordered_map<int, EditorNode>& nodes) const;

  //saves the design of the tree to path in serialized json format
  void saveDesign(const BehaviourTree& bt, const std::string& path) const;

  //Loads the design of the tree from path
  BehaviourTree loadDesign(const std::string& path);

  private:
  json traverseTree(int id, const std::unordered_map<int, std::vector<int>>& adjList, const std::unordered_map<int, EditorNode>& nodes) const;


};
}

