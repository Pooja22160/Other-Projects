Aim:

This project is a simulation of GO back N protocol on Omnet++. 

Requirements:
 1. Omnet++
 2. C++ Programming Language

What is GO Back N?

In Go-Back-N ARQ, N is the sender's window size. Suppose we say that Go-Back-3, which means that the three frames can be sent at a time before expecting the acknowledgment from the receiver. It uses the principle of protocol pipelining in which the multiple frames can be sent before receiving the acknowledgment of the first frame. If we have five frames and the concept is Go-Back-3, which means that the three frames can be sent, i.e., frame no 1, frame no 2, frame no 3 can be sent before expecting the acknowledgment of frame no 1. In Go-Back-N ARQ, the frames are numbered sequentially as it sends the multiple frames at a time that requires the numbering approach to distinguish one frame from another. If the acknowledgment of a frame is not received within an agreed-upon time period, then all the frames available in the current window will be retransmitted. Suppose we have sent the frame no 5, but we didn't receive the acknowledgment of frame no 5, and the current window is holding three frames, then these three frames will be retransmitted.

What is Omnet++?

OMNeT++ is an extensible, modular, and component-based C++ simulation library and framework primarily designed for building network simulators. The term "network" is used broadly to encompass wired and wireless communication networks, on-chip networks, queueing networks, and more. Domain-specific functionality, such as support for sensor networks, wireless ad-hoc networks, Internet protocols, performance modeling, and photonic networks, is provided by model frameworks developed as independent projects. OMNeT++ offers a component architecture for models. Components, or modules, are programmed in C++ and then assembled into larger components and models using a high-level language called NED. This setup ensures the reusability of models. OMNeT++ features extensive GUI support, and thanks to its modular architecture, the simulation kernel and models can be easily embedded into various applications.

Additionally, OMNeT++ provides an Eclipse-based integrated development environment (IDE), a graphical runtime environment, and a range of other tools. There are extensions available for real-time simulation, network emulation, database integration, SystemC integration, and several other functions, making OMNeT++ a versatile and powerful tool for network simulation.
