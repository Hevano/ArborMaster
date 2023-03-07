#pragma once

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/flat_map.hpp>

#include <memory>

namespace ipc = boost::interprocess;

class DebugManager {
private:
	unsigned int m_nodeUpdateBuffer[3];

  typedef ipc::allocator<std::pair<const std::string, std::string>, ipc::managed_shared_memory::segment_manager> bb_allocator_type;
  typedef ipc::flat_map<std::string, std::string, std::less<std::string>, bb_allocator_type> bb_map_type;

  typedef ipc::allocator<std::pair<unsigned int, std::string>, ipc::managed_shared_memory::segment_manager> actorid_allocator_type;
  typedef ipc::flat_map<unsigned int, std::string, std::less<unsigned int>, actorid_allocator_type> actorid_map_type;

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