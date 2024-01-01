# Line Segment Light Visibility System (LSLVS)

## What is this project?
A demonstration of a light/visibility system done via line segment intersection.

## Tools
- C++
- SFML
- CMake

---

## Dev Log

### Entry #1 [Nov 14th -> Nov 26th]:
- Basic 2D geometry added to the world.
- Basic player controls.
- Ability to push and move objects in the world.
- Introduction of entity memory pooling.

I did not take screenshots during this period.

---

### Entry #2 [Nov 27th -> Dec 30th]:
Not a huge amount of time was spent working on the project during this period due to work and with the Christmas holidays.
However, I did start a separate branch on the project for building my own line segment intersection algorithm.

The goal with this was to add a visibility system to the game so we could determine what is visible to a given entity.

This went through many changes as a result of the wrong strategy being used. The initial implementation worked for
a set of rays in a radial pattern around the player entity. However, when joining these rays with a TriangleFan there 
was a lot of jagged edges, flickering and clipping over the corners of other world entities. Eventually, I managed to
succeed in getting this to work.

---

#### Screenshots

![Sat 30 Dec 2023 16:57:20 GMT](https://github.com/Ticketedmoon/line-segment-light-visibility-system/assets/21260839/13a49eac-5329-405d-a8b1-42d4cab983c4)
---
![Sat 30 Dec 2023 16:50:20 GMT](https://github.com/Ticketedmoon/line-segment-light-visibility-system/assets/21260839/875e6331-0756-4a0f-80a4-6ddcfcf30e9e)
---


- The pinky block represents our player and light source. 
- The yellow area represents the region the player can see.

---

---

### Credit
- https://ncase.me/sight-and-light/
