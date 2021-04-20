===========================
Proposal for SekirOgre
===========================


Basic information
=================

Recently, I enjoy playing Sekiro: Shadows Die Twice. So I think it might be
a good and interesting practice to build my own game engine specially for this
kind of action adventure game.

I will use Ogre enigne (Object-Oriented Graphics Rendering Engine, a open 3D
rendering engine), and manage other game objects. Physical engine and collision
detection are essential.

github repo: https://github.com/xyhuang7215/Sekirogre

Problem to solve
================
   
1. Manage the characters, items and scenes.
2. Physics engine.
3. Interaction between rendering system and game objects.

Perspective users
=================

Those who want to build a action adventure game but lack of programming skill.

System architecture
===================

python scripts to make character/scene/event, and define their behaviors.
C++ scripts to control rendering and implement behaviors.


API description
===============
addCharacter
Character.appendAction()


Engineering infrastructure
==========================
Ogre engine
Collision 
physical engine


Schedule
========

| 04/19 ~ 04/25 (1 week) : setup environment / learn basic Ogre engine
| 04/26 ~ 05/16 (3 week) : physic engine / collision detection
| 05/17 ~ 05/23 (1 week) : game objects management
| 05/24 ~ 06/07 (2 week) : debug / demo

References
==========
https://www.ogre3d.org/
https://en.wikipedia.org/wiki/Sekiro:_Shadows_Die_Twice



