# ArborMaster

ArborMaster is a tool to help game developers implement, test, and iterate on behaviour trees. Firstly, the node-based visual designer allows developers to craft behaviour trees from nodes they have implemented with a drag and drop interface. 



https://user-images.githubusercontent.com/25439882/235259455-0bb016d8-4224-4133-b72e-4213b51d448f.mp4


In addition to the tool’s designing capability, the debugging feature links into the game at runtime to visually display the status of behaviour trees on selected AI actors, as well as the state of data within the behaviour tree’s blackboard. This tool is implementation-agnostic, supporting C++ projects that do not use a preexisting engine or behaviour tree implementation library.


https://user-images.githubusercontent.com/25439882/235268671-5f3db77e-014f-4f49-830c-c3b5b541e804.mp4


# Example
This project was developed alongside a demo game, Sapling Tactics, which can be found [here](https://github.com/Hevano/SaplingTactics).

# Integration

If you wish to integrate ArborMaster with your project, follow the steps below.
1. Include ArborMaster_adapter.lib, and all headers in source/adapter as source files in your build
2. In your behaviour tree implementation, nodes and actors should each have associated unique ids represented by unsigned ints
3. In your tree implementation, include ArborMasterAdapter::Debugger, create a Debugger object, and call the relevant methods:
    - updateNodeStatus, changes the status of a node in a specific actor's tree
    - updateDebugBlackboard, when a blackboard value is added or changed
    - createDebugActor, when an actor is created
    - removeDebugActor, when an actor is removed
    - resetDebugBlackboard, when the blackboard must be cleared
    - tick, to check if new actor has been selected in the editor
 
4. Flag node implementations in source code files.

    - Use the format [ArborMaster]NodeName|Max Children|BlackboardKey1|...|BlackboardKeyN|

3. Import node definitions into the editor using File->Import or Ctrl+I
4. Design a tree, save the design, and export it to a json file using File->Export
5. Include ArborMasterAdapter::Adapter to load and construct trees at runtime
    - Create an Adapter Object
    - Create a TreeDesignNode tree using Adapter::loadTree()
    - Traverse the TreeDesignNode tree to construct your behaviour tree instance
7. Edit and debug tree designs
 
# Building and installing

See the [BUILDING](BUILDING.md) document.

# Licensing

<!--
Please go to https://choosealicense.com/licenses/ and choose a license that
fits your needs. The recommended license for a project of this type is the
GNU AGPLv3.
-->
