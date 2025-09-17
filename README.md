# Communication and Monitoring System for People with Alzheimer and Dementia

## Table of Contents
- [Problem Statement](#problem-statement)
- [Goals](#goals)
- [Overview](#overview)
- [Key Features](#key-features)
- [Technical Overview](#technical-overview)
- [Tech Stack](#tech-stack)

## 1. Problem Statement<a name="problem-statement"></a>

The project addresses the safety and autonomy of individuals with Alzheimer’s and Dementia by implementing an advanced, real-time security and monitoring system. The system is designed to promptly alert caregivers to unusual or potentially dangerous events, especially when individuals are alone. This solution aims to provide peace of mind and flexibility for caregivers, while empowering those under care to maintain a sense of independence within a safe environment.

For a more detailed explanation, please refer to the Project Report.

## 2. Goals<a name="goals"></a>

The overarching goals of this project include:
- Apply knowledge gained from prior curriculum units
- Reinforce understanding of embedded systems specialization
- Explore new concepts within embedded technology
- Ensure the development of a reliable, fault-tolerant system
- Build a compact and discrete system to minimize its impact on home aesthetics
- Address a market niche overlooked by mainstream solutions

## 3. Overview<a name="overview"></a>

This project is a comprehensive embedded system integrating multiple hardware and software components to monitor, detect, and report events in a residential environment. The system leverages sensors (motion, magnetic, button, relay, LED) interfaced via custom device drivers, and a central daemon for data aggregation and event handling. The architecture is modular, supporting deployment on both Raspberry Pi 4B and standard Linux environments, and is designed for extensibility and maintainability. The solution emphasizes robustness, low-latency alerting, and ease of integration with existing home infrastructure.

## 4. Key Features<a name="key-features"></a>

- **Real-time Alerts:** Immediate notifications for abnormal or hazardous events.
- **Caregiver Flexibility:** Enables remote monitoring and flexible scheduling for caregivers.
- **User Independence:** Supports autonomy for individuals with cognitive impairments.
- **Robustness:** Designed for high reliability and fault tolerance.
- **Compact, Discrete Design:** Minimal impact on home interior and daily life.
- **Modular Architecture:** Easily extendable for additional sensors or features.

## 5. Technical Overview<a name="technical-overview"></a>

The system consists of the following main components:
- **Embedded Device Drivers:** Custom Linux kernel modules for interfacing with motion, magnetic, button, relay, and LED hardware.
- **Central Daemon:** A C++ service responsible for sensor data collection, event processing, and alert dispatching.
- **Database Integration:** Local or remote database support for event logging and historical analysis.
- **Python Integration:** Python scripts for database management and potential AI/ML extensions.
- **Build System:** CMake-based build configuration supporting both Raspberry Pi 4B and generic Linux targets.

The architecture ensures separation of concerns, with clear interfaces between hardware abstraction, core logic, and external integrations.

## 6. Tech Stack<a name="tech-stack"></a>

**Hardware:**
- Raspberry Pi 4B (primary target)
- Standard Linux PC (development/testing)
- Motion, magnetic, button, relay, and LED sensors

**Software:**
- C++ (core logic, daemon)
- C (Linux kernel modules/device drivers)
- Python (database management, scripting)
- Bash (build and deployment scripts)

**Frameworks & Tools:**
- Linux Kernel (module development)
- CMake (build system)
- GCC/G++ (compilation)
- Firebase (database, configurable)
- Git (version control)

**Deployment:**
- Supports both Raspberry Pi OS and standard Linux distributions
- Modular build options in `CMakeLists.txt` for target selection

For a more comprehensive understanding, please refer to the Project Report. Feel free to explore the report for a detailed analysis of the project's development and implementation.
