#pragma once
#include <unordered_map>
#include <string>
#include <any>
namespace ArborMaster
{

//There may be a conceptual difference between blackboards at runtime and blackboards at design time
class Blackboard
{
public:
  std::unordered_map<std::string, std::any> data;

public:
  bool hasKey(const std::string & keyName) const;
};
}
	

