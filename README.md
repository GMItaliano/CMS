# KUKA 7-DOF Industrial Robot Simulation

A robotics simulation environment implementing a KUKA 7-degree-of-freedom manipulator with computer vision capabilities for automated pick-and-place operations in industrial applications.

## Technical Overview

This project implements forward/inverse kinematics algorithms, computer vision-based object detection, and autonomous manipulation strategies for a KUKA robot mounted on a mobile platform. The system integrates machine learning models for object classification and real-time path planning algorithms.

**Core Features:**
- Forward & inverse kinematics solver for 7-DOF manipulator
- Vision-based object detection and classification using TensorFlow
- Automated stock management system
- Real-time trajectory planning and control
- CoppeliaSim integration for physics-based simulation

## Project Purpose & Methodology

### Why This Project Exists
This simulation demonstrates Industry 4.0 concepts by integrating robotics, computer vision, and intelligent automation in a retail/warehouse environment. It serves as a proof-of-concept for autonomous robotic systems that can:
- Identify and classify objects using machine learning
- Make intelligent decisions based on visual feedback
- Execute precise manipulation tasks without human intervention
- Manage inventory in real-time

### How It Works
1. **Vision Pipeline**: Camera captures workspace images → Python ML model classifies objects (CanTypeA/CanTypeB) → Coordinates sent to MATLAB
2. **Motion Planning**: MATLAB calculates inverse kinematics → Generates collision-free trajectories → Sends joint commands via Remote API
3. **Execution Loop**: Robot moves to target → Grasps object → Places in designated location → Updates inventory database
4. **Feedback Control**: Real-time position feedback ensures precision and error correction

### What It Demonstrates
- **Autonomous Decision Making**: Robot selects appropriate actions based on object classification
- **Human-Robot Collaboration**: Safe operation in shared workspaces with predictable behavior
- **Scalable Architecture**: Modular design allows easy integration of new object types and behaviors
- **Industry 4.0 Integration**: Combines IoT sensors, ML algorithms, and robotic automation

## Tech Stack

| Component | Technology |
|-----------|------------|
| **Simulation Engine** | CoppeliaSim (`.ttt` scene files) |
| **Control System** | MATLAB/Simulink + Robotics Toolbox |
| **Computer Vision** | Python + TensorFlow/Keras |
| **Image Processing** | OpenCV, NumPy |
| **API Interface** | Remote API (remApi) |
| **Kinematics** | Custom MATLAB implementation |

## Quick Start

```bash
# Prerequisites: MATLAB R2020b+, CoppeliaSim, Python 3.8+
1. Open CoppeliaSim and load: CaseStudy2_SmartRetail_Kuka7dof.ttt
2. Run MATLAB: main_program.m
3. For vision training: cd Vision_ML && python datasetgeneration.py
```

## System Architecture

```
├── main_program.m          # Main control loop
├── arm_Kinematics.m        # Kinematics calculations
├── vision.m                # Vision processing interface
├── pick_n_place.m          # Manipulation algorithms
├── Stock_Manager.m         # Inventory management
└── Vision_ML/              # ML models & training scripts
    ├── sai.py              # TensorFlow classification model
    └── inference.py        # Real-time inference
```
