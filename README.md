# boss
Base objects for service solutions

Cross-platform component framework in C++11

**Boss gives you:**
- Ability of building loosely-coupled systems
- Minimal effort for creating new components
- Support for several operating systems
- Advantages of C++11
- Availability for open and proprietary software

Large projects written in C++ sometimes require system decomposition. A system under development can evolve from one executable
 to the complex multi-component large system. Furthermore, a system can be initially planned as a large software
 product. One can easily face a problem "What to use for system decomposition? How to organize components interaction?".
All these questions can be addressed by employing the component framework Boss.

Boss is aimed at providing tools for building a system of multiple components with a minimal effort.
 The Boss framework was started with an idea in mind "Minimalism everywhere where it is possible": minimal implementation,
 minimal amount of steps for creating and using components within the developing system.

At a stage when the system should get distributed Boss provides relatively smooth transition to a multi-process
 application, both within one computer or distributed in a cluster. The existent code should not be modified to archive
 such goals. All you need to do is to implement a Proxy/Stub module for each component. The spirit of the minimalism
 presents in the Proxy/Stub implementation as well. Finally, you modify the configuration of your system and you are done.

Boss does not provide specific implementations for auxiliary services and utilities. Instead, a developer should use ad
-hoc libraries. Boss is the way to efficiently glue your components into one software system.
