#include "NodeImporter.h"
#include "TreeNode.h"

namespace ArborMaster
{
void NodeImporter::setPath(const std::string& path) {}
const std::string& NodeImporter::getPath() const
{
  return m_path;
}
void NodeImporter::importAll(
    std::unordered_map<std::string, TreeNode>& importedNodes)
{
}
TreeNode NodeImporter::importNode(const std::string& source)
{
  return TreeNode();
}
}