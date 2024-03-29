#include "DebugManager.h"

#include <iostream>
#include <filesystem>

DebugManager* DebugManager::createInstance()
{
  DebugManager* dm = nullptr;
  try {
    dm = new DebugManager;
    //create segment and the flat maps inside of it
    dm->m_segment = ipc::managed_shared_memory(ipc::open_only, "DebuggerSharedMemory");
    dm->m_blackBoardMap.reset(dm->m_segment.find_or_construct<bb_map_type>("BlackboardMap")(dm->m_segment.get_segment_manager()));
    dm->m_actorIdMap.reset(dm->m_segment.find_or_construct<actorid_map_type>("ActorIdMap")(dm->m_segment.get_segment_manager()));
    dm->m_actorIdMessageQueue.reset(new ipc::message_queue(ipc::open_or_create, "ActorSelectMessageQueue", 100, sizeof(unsigned int)));
    dm->m_nodeUpdateMessageQueue.reset(new ipc::message_queue(ipc::open_or_create, "NodeUpdateMessageQueue", 100, sizeof(unsigned int) * 3));

    //Loop through the paths found in ActorIdMap and ensure that those files exist, both as json and ini

    auto x = dm->m_segment.get_free_memory();
    std::cout << x << std::endl;

    for (auto it = dm->m_actorIdMap->begin(); it != dm->m_actorIdMap->end(); it++) {
      auto f = it->first;
      auto s = it->second;
      std::filesystem::path pathDesignJson(std::string(s.begin(), s.end()));
      std::filesystem::path pathDesignIni(std::string(s.begin(), s.end()));
      pathDesignIni.replace_extension(".ini");

      if (pathDesignJson.extension() != ".json") throw std::exception();
      if(!std::filesystem::exists(pathDesignJson)) throw std::exception();
      if (!std::filesystem::exists(pathDesignIni)) throw std::exception();

      //Paths should be absolute
      if (!pathDesignJson.is_absolute()) throw std::exception();
      if (!pathDesignJson.is_absolute()) throw std::exception();
    }

    return dm;
  }
  catch (ipc::interprocess_exception& ex) {
    delete dm;
    return nullptr;
  }
  catch (std::exception& ex) {
    delete dm;
    return nullptr;
  }
  
}



bool DebugManager::getNodeUpdates(unsigned int& nodeId, unsigned int& actorId, unsigned int& status)
{
  try {
    size_t recv_size;
    unsigned int priority;
    if (m_nodeUpdateMessageQueue->get_num_msg() > 0) {
      m_nodeUpdateMessageQueue->receive(m_nodeUpdateBuffer, sizeof(m_nodeUpdateBuffer), recv_size, priority);
      if (recv_size == 12) { //always expect 3 items
        nodeId = m_nodeUpdateBuffer[0];
        actorId = m_nodeUpdateBuffer[1];
        status = m_nodeUpdateBuffer[2];
        return true;
      }
    }
    return false;
  }
  catch (ipc::interprocess_exception& ex) {
    std::cout << "Error: " << ex.what() << std::endl;
  }
	return false;
}

std::unordered_map<unsigned int, std::string> DebugManager::getAllActors() const
{
  std::unordered_map<unsigned int, std::string> map;
  auto size = m_actorIdMap->size();
  for (auto it = m_actorIdMap->begin(); it != m_actorIdMap->end(); it++) {
    map.emplace(it->first, std::string(it->second.begin(), it->second.end()));
  }
  return map;
}

bool DebugManager::selectActor(unsigned int actorId)
{
  try {
    m_actorIdMessageQueue->send(&actorId, sizeof(actorId), 0);
    m_currentActorId = actorId;
    return true;
  }
  catch (ipc::interprocess_exception& ex) {
    std::cout << "Error: " << ex.what() << std::endl;
  }
  return false;
}

std::unordered_map<std::string, std::string> DebugManager::getBlackboard()
{
  std::unordered_map<std::string, std::string> map;
  for (auto it = m_blackBoardMap->begin(); it != m_blackBoardMap->end(); it++) {
    map.emplace(std::string(it->first.begin(), it->first.end()), std::string(it->second.begin(), it->second.end()));
  }
  return map;
}

DebugManager::~DebugManager()
{
  //given that we do not own the flat map's memory, we shouldn't free them when we deconstruct. TODO: custom deleter
  m_actorIdMap.release();
  m_blackBoardMap.release();
}


