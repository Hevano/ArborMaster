#pragma once

#include <unordered_map>
#include <string>

namespace ArborMaster
{

//Forward declaration
class TreeNode;

class NodeImporter
{
private:
  std::string m_path;

public:
  void setPath(const std::string& path);
  const std::string& getPath() const;
  void importAll(std::unordered_map<std::string, TreeNode>& importedNodes);

private:
  TreeNode importNode(const std::string& source);
};
}

