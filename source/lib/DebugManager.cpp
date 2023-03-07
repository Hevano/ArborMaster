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

    //Loop through the paths found in ActorIdMap and ensure that those files exist, both as json and ini

    for (auto it = dm->m_actorIdMap->begin(); it != dm->m_actorIdMap->end(); it++) {
      std::filesystem::path pathDesignJson(it->second);
      std::filesystem::path pathDesignIni(it->second);
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



bool DebugManager::getNodeUpdates(unsigned int& nodeId, unsigned int& actorId, unsigned int& statusId)
{
  ipc::message_queue msgQueue(ipc::open_or_create, "NodeUpdateMessageQueue", 100, sizeof(unsigned int));
  try {
    size_t recv_size;
    unsigned int priority;
    if (msgQueue.get_num_msg() < msgQueue.get_max_msg()) {
      msgQueue.receive(m_nodeUpdateBuffer, sizeof(m_nodeUpdateBuffer), recv_size, priority);
      if (recv_size == 3) { //always expect 3 items
        nodeId = m_nodeUpdateBuffer[0];
        actorId = m_nodeUpdateBuffer[1];
        statusId = m_nodeUpdateBuffer[2];
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
  for (auto it = m_actorIdMap->begin(); it != m_actorIdMap->end(); it++) {
    map.emplace(*it);
  }
  return map;
}

bool DebugManager::selectActor(unsigned int actorId)
{
  ipc::message_queue mq(ipc::open_or_create, "ActorSelectMessageQueue", 100, sizeof(unsigned int));
  try {
    mq.send(&actorId, sizeof(unsigned int), 0);
    return true;
  }
  catch (ipc::interprocess_exception& ex) {
    //throws error if queue is empty
  }
  return false;
}


