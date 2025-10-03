# Project Title

A brief description of what this project does and who it's for.

## Table of Contents

- [Introduction](#introduction)
- [Installation](#installation)
- [Usage](#usage)
- [Technology Stack](#technology-stack)

## Introduction
The goal of this project is to develop hardware to facilitate a new learning environment for use in ECE3430 (Introduction to Embedded Computer Systems). The board will support the use of two widely used development boards, the STM32 development board and the TI MSPM0 development board, and contain a number of auxiliary I/O that can be used to develop learning activities to learn how to interface to digital I/O, analog I/O, and serial I/O. This includes a complete hardware design and software solutions for specific implementations of learning activities using specific I/O 
## Usage
- **STM32**
  - For each of the provided software solutions: Open STM32CubeIDE and imp# Bill of Materials (BOM)

## Overview

A comprehensive list of all the components required for the project, including specifications and sourcing information.

## Table of Contents

- [Components](#components)
- [Tools Required](#tools-required)
- [Notes](#notes)

## Components

| Item No | Component              | Quantity | Description                   | Supplier/Source         | Part Number       | Price      |
|---------|-----------------------|----------|-------------------------------|-------------------------|-------------------|------------|
| 1       | Resistor              | 10       | 220 Ohm, 1/4 Watt, 5% Tolerance | [Digi-Key](https://www.digikey.com/) | 220R-1/4W-5%       | $0.10     |
| 2       | Capacitor             | 5        | 100 µF, 16V Electrolytic     | [Mouser](https://www.mouser.com/) | 100uF-16V        | $0.25     |
| 3       | Microcontroller       | 1        | ATmega328P                    | [SparkFun](https://www.sparkfun.com/) | ATMEGA328P         | $2.50     |
| 4       | PCB                   | 1        | Custom PCB                    | [JLCPCB](https://www.jlcpcb.com/) | CustomPCB-XYZ      | $3.00     |
| 5       | KiCad Software        | 1        | PCB Design Software           | [KiCad](https://www.kicad.org/) | N/A                 | Free       |

## Tools Required

- Soldering Iron
- Wire Cutters
- Multimeter
- Screwdriver Set

## Notes

- Ensure all components meet the specified requirements.
- Check for component availability and potential substitutions.
ort file as new project. Flash to STM32 development board, and connect correct IO to hardware pins as documented in solution.

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/MilesMayhew/Pedagogical-PCB.git
## Technology Stack

- **Languages**: 
  - C
- **Boards**
  - STM32 nucleo
  - TI LP-MSPMOG3507
- **Tools / Libraries**:
  - Git
  - STM32CubeIDE 1.16.0
  - Code Composer Studio
- **Hardware Design**:
  - KiCad


