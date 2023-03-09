#pragma once

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/flat_map.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include <memory>

namespace ipc = boost::interprocess;

class DebugManager {
private:
  unsigned int m_nodeUpdateBuffer[3] = { 0,0,0 };

  typedef ipc::allocator<char, ipc::managed_shared_memory::segment_manager> char_allocator_type;
  typedef ipc::basic_string<char, std::char_traits<char>, char_allocator_type> char_string_type;

  typedef ipc::allocator<std::pair<const std::string, std::string>, ipc::managed_shared_memory::segment_manager> bb_allocator_type;
  typedef ipc::flat_map<std::string, std::string, std::less<std::string>, bb_allocator_type> bb_map_type;

  //typedef ipc::allocator<std::pair<unsigned int, char_string_type>, ipc::managed_shared_memory::segment_manager> actorid_allocator_type;
  //typedef ipc::flat_map<unsigned int, char_string_type, std::less<unsigned int>, actorid_allocator_type> actorid_map_type;

  typedef ipc::allocator<std::pair<const unsigned int, ipc::basic_string<char>>, ipc::managed_shared_memory::segment_manager> actorid_allocator_type;
  typedef ipc::flat_map<unsigned int, ipc::basic_string<char>, std::less<unsigned int>, actorid_allocator_type> actorid_map_type;

  ipc::managed_shared_memory m_segment;
  std::unique_ptr<bb_map_type> m_blackBoardMap;
  std::unique_ptr<actorid_map_type> m_actorIdMap;

private:
  //Only construct using createInstance
  DebugManager() = default;

public:
  static DebugManager* createInstance();
	bool getNodeUpdates(unsigned int& nodeId, unsigned int& actorId, unsigned int& statusId);

  std::unordered_map<unsigned int, std::string> getAllActors() const;

  bool selectActor(unsigned int actorId);

};