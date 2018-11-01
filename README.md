# ecsproto
C++ ECS hybrid system for games

# Files

ecsproto.cpp - Main example program.

ecs.hpp - Main ECS header.
ecs.cpp - Global data and function implementations.
ecsentity.hpp - ECS entity.
ecscomponent.hpp - ECS component base.
ecspool.hpp - Template class to allocate objects.

# Classes

## Entity 
- entity class which can contain components

## Component 
- base class that components inherit from

## Pool
- memory pool to allocate from, ensures allocations are all contiguous and cache friendly
- multiple ways to iterate through allocated objects: forEach or begin/end iterator

## Face (example
- example component
- derived from Component
- uses Pool to allocate from so all Face objects allocated contiguously
