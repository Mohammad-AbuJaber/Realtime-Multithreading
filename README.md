# Ant Searching for Food Simulation

Welcome to the Ant Searching for Food Simulation project! This repository contains a multi-threading application that simulates the behavior of a group of ants searching for food. The simulation aims to provide a simplified representation of ant behavior while considering key aspects such as movement, pheromones, food consumption, and simulation termination.

## Table of Contents

- [Description](#description)
- [Features](#features)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Configuration](#configuration)
- [Dependencies](#dependencies)
- [Compilation](#compilation)

## Description

This simulation replicates the behavior of ants in search of food within a controlled environment. The ants move randomly in various directions and speeds while responding to the presence of food. Pheromones play a crucial role in guiding ants towards the food sources, and once gathered around a food piece, they consume it until it's depleted.

## Features

- Simulates ant movement, random directions, and speeds.
- Handles ant behavior at simulation window boundaries.
- Introduces pheromones to trigger social responses in neighboring ants.
- Ants change direction based on the strength of pheromone smell.
- Allows ants to prioritize between multiple food sources.
- Enables controlled food consumption by ants.
- Terminates the simulation after a user-defined time.

## Getting Started

To get started with the simulation, follow these steps:

1. Clone this repository to your local machine.
2. Compile the source code using the instructions in the [Compilation](#compilation) section.
3. Configure the simulation parameters as needed.
4. Run the compiled executable.
5. Observe the simulation and its graphical representation.

## Usage

After compiling and running the executable, you will witness the ant simulation unfold in a graphical environment. Ants will move randomly across the screen, and food pieces will appear periodically. The simulation will proceed according to the rules outlined in the project description.

## Configuration

The behavior of the simulation can be configured using a text file that contains various parameters. This approach avoids hard-coding values in the source code, making the simulation more adaptable. Modify the configuration file as needed before running the program.

## Dependencies

This project utilizes the following libraries:

- OpenGL: Provides graphical elements for visualization.

Please ensure you have the necessary libraries installed on your system before compiling and running the simulation.

## Compilation

Compile the source code using the following command:

```bash
make
```
```bash
make run
```
