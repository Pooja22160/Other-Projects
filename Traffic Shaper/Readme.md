Aim:

This project is a simulation of Token Bucket as a Traffic Shaper algorithm on Omnet++.

Requirements:
 1. Omnet++
 2. C++ Programming Language

What is Token Bucket?

The Token Bucket algorithm is a popular and simple method used in computer networking and telecommunications for traffic shaping and rate limiting. It is designed to control the amount of data that a system can send or receive over a period, ensuring that the traffic conforms to a specified rate. The algorithm allows bursty traffic at a regulated maximum rate and enables idle hosts to accumulate credit for future use in the form of tokens. The system removes one token for every unit of data sent. For each tick of the clock, the system sends 'n' tokens to the bucket. For example, if 'n' is 100 and the host is idle for 100 ticks, the bucket collects 10,000 tokens, allowing the host to consume all these tokens at a rate of 10 units of data per tick.

The Token Bucket can be easily implemented with a counter. Initially, the token count is set to zero. Each time a token is added, the counter increments by one. Each time a unit of data is sent, the counter decrements by one. When the counter reaches zero, the host cannot send data until more tokens are added. This mechanism effectively manages traffic rates and allows for efficient and flexible control over data transmission.

What is Omnet++?

OMNeT++ is an extensible, modular, and component-based C++ simulation library and framework primarily designed for building network simulators. The term "network" is used broadly to encompass wired and wireless communication networks, on-chip networks, queueing networks, and more. Domain-specific functionality, such as support for sensor networks, wireless ad-hoc networks, Internet protocols, performance modeling, and photonic networks, is provided by model frameworks developed as independent projects. OMNeT++ offers a component architecture for models. Components, or modules, are programmed in C++ and then assembled into larger components and models using a high-level language called NED. This setup ensures the reusability of models. OMNeT++ features extensive GUI support, and thanks to its modular architecture, the simulation kernel and models can be easily embedded into various applications.

Additionally, OMNeT++ provides an Eclipse-based integrated development environment (IDE), a graphical runtime environment, and a range of other tools. There are extensions available for real-time simulation, network emulation, database integration, SystemC integration, and several other functions, making OMNeT++ a versatile and powerful tool for network simulation.
