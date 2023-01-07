#pragma once
namespace ArborMaster
{
class NodeImporter
{
private:
  std::string m_path;

public:
  void setPath(const std::string& path);
  const std::string& getPath() const;
};
}

