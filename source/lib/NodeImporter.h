#pragma once

#include <unordered_map>
#include <string>

#include <filesystem>
#include <fstream>
#include <iostream>

namespace ArborMaster
{

//Forward declaration
class TreeNode;

class NodeImporter
{
private:
  std::string m_path;
  inline const static std::string NODE_FLAG_STRING = "//[ArborMaster]";
  inline const static std::string NODE_DELIMITER_STRING = "|";

public:
  inline std::string& getPath()
  {
    return m_path;
  }
  void inline setPath(const std::string& path)
  {
    m_path = path;
  }
  void importAll(std::unordered_map<std::string, TreeNode>& importedNodes);

private:
	//Creates a node from a string data defined in schema format
  //Example format:
  //[ArborMaster]NodeName|childCap|blackboardkey1|...|blackboardkeyN|
  TreeNode importNode(const std::string& source);
};
}

