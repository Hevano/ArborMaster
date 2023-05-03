#include "Debugger.h"

namespace ArborMasterAdapter {
  //Initiates IPC with the designer, returning false if it fails
  bool Debugger::init()
  {
    //clears existing message queues
    ipc::message_queue::remove("NodeUpdateMessageQueue");
    ipc::message_queue::remove("ActorSelectMessageQueue");
    // Deallocate the shared memory segment, if it already exists
    ipc::shared_memory_object::remove("DebuggerSharedMemory");

    //create segment and the flat maps inside of it
    m_segment = ipc::managed_shared_memory(ipc::create_only, "DebuggerSharedMemory", MAX_BYTES);
    m_blackBoardMap.reset(m_segment.find_or_construct<bb_map_type>("BlackboardMap")(m_segment.get_segment_manager()));
    m_actorIdMap.reset(m_segment.find_or_construct<actorid_map_type>("ActorIdMap")(m_segment.get_segment_manager()));

    m_charAllocator = std::make_unique<char_allocator>(char_allocator(m_segment.get_segment_manager()));
    m_actorIdMessageQueue.reset(new ipc::message_queue(ipc::open_or_create, "ActorSelectMessageQueue", 100, sizeof(unsigned int)));
    m_nodeUpdateMessageQueue.reset(new ipc::message_queue(ipc::open_or_create, "NodeUpdateMessageQueue", 100, sizeof(ActorUpdate)));

    m_active = true;

    return true;
  }

  //returns true if actor has changed, in which case blackboard should be updated
  bool Debugger::tick()
  {
    ipc::message_queue::size_type recv_size;
    unsigned int priority;
    unsigned int id = 0;
    try {
      if (m_actorIdMessageQueue->get_num_msg() > 0) {
        m_actorIdMessageQueue->receive(&id, sizeof(id), recv_size, priority);
        if (recv_size == sizeof(id)) {
          m_currentActorId = id;
          return true;
        }
      }
    }
    catch (std::exception& ex) {
      std::cout << "Error: " << ex.what() << std::endl;
    }
    return false;
  }

  Debugger::~Debugger()
  {
    // Deallocate the object in the shared memory segment
    if (m_active && m_segment.find<actorid_map_type>("ActorIdMap").first) {
      m_segment.destroy<actorid_map_type>("ActorIdMap");
    }
    if (m_active && m_segment.find<bb_map_type>("BlackboardMap").first) {
      m_segment.destroy<bb_map_type>("BlackboardMap");
    }

    //Release the flat map pointers rather than allow unique_ptr to free them. TODO: add custom deleter to handle this properly
    m_actorIdMap.release();
    m_blackBoardMap.release();

    // Deallocate the shared memory segment
    ipc::shared_memory_object::remove("DebuggerSharedMemory");
  }

  //Updates the running status of a node
  void Debugger::updateNodeStatus(
    unsigned int nodeId,
    unsigned int actorId,
    unsigned int status)
  {
    //Only update the message queue if we are currently watching that actor
    if (actorId != m_currentActorId) return;

    ActorUpdate update(nodeId, actorId, status);
    try {
      if (m_nodeUpdateMessageQueue->get_num_msg() < m_nodeUpdateMessageQueue->get_max_msg()) {
        m_nodeUpdateMessageQueue->send(&update, sizeof(ActorUpdate), 0);
      }
    }
    catch (ipc::interprocess_exception& ex) {
      std::cout << "Error: " << ex.what() << std::endl;
    }
  };

  //Updates the blackboard value of an actor
  void Debugger::updateDebugBlackboard(
    unsigned int actorId,
    std::string key,
    std::string value)
  {
    if (key.empty()) {
      return;
    }
    else if (value.empty()) {
      m_blackBoardMap->erase(char_string(key.begin(), key.end(), *m_charAllocator));
    }
    else {
      auto keyBoostString = char_string(key.begin(), key.end(), *m_charAllocator);
      if (m_blackBoardMap->contains(keyBoostString))
      {
        m_blackBoardMap->at(keyBoostString) = char_string(value.begin(), value.end(), *m_charAllocator);
      }
      else {
        m_blackBoardMap->insert({ keyBoostString, char_string(value.begin(), value.end(), *m_charAllocator) });
      }
    }
  };

  void Debugger::updateDebugBlackboard(
    unsigned int actorId,
    std::pair<std::string, std::string> keyValue)
  {
    updateDebugBlackboard(actorId, keyValue.first, keyValue.second);
  };

  //declares that there is an actor who uses a tree at the path
  void Debugger::createDebugActor(unsigned int actorId, std::string treePath)
  {
    m_actorIdMap->insert({ actorId, char_string(treePath.begin(), treePath.end(), *m_charAllocator) });
  };

  void Debugger::removeDebugActor(unsigned int actorId)
  {
    m_actorIdMap->erase(actorId);
    auto size = m_actorIdMap->size();
  };

  void Debugger::resetDebugBlackboard(const std::unordered_map<std::string, std::string>& newBlackboard)
  {
    m_blackBoardMap->clear();
    for (auto& [key, value] : newBlackboard) {
      m_blackBoardMap->insert({ char_string(key.begin(), key.end(), *m_charAllocator), char_string(value.begin(), value.end(), *m_charAllocator) });
    }
  }
}