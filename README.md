# WatchWinders

## STM32F9 Microcontroller Programmed to Wind a Watch

### Project Overview
The WatchWinders project demonstrates the use of an STM32F9 microcontroller for automating the winding of a mechanical watch. The system dynamically controls the winding speed (RPM) using PWM signals derived from ADC inputs, calculates the time remaining based on rotations, and displays this information on an OLED and 7-segment display.  If more interested download the demo video in the repository for a full understanding of the embedded controller and surrounding circuitry.

---

### Watch Winder in Action!
![WatchWinders Demo](demo.gif)

---

## Features
- **Variable RPM Control**: ADC readings are converted into motor speed using PWM.
- **Real-Time Display**: Shows current RPM and remaining time on both an OLED and 7-segment display.
- **Precision Motor Control**: PWM duty cycle dynamically adjusts motor speed.
- **Boxcar Averaging**: Smoothens ADC readings for stable RPM calculations.
- **DMA Utilization**: Efficient data transfer for display updates using SPI communication.

---

## Components
- **STM32F9 Microcontroller**: Used for ADC sampling, PWM generation, and display updates.
- **OLED Display**: Displays detailed RPM and time-left calculations via SPI.
- **7-Segment Display**: Provides simplified time-left information for quick viewing.
- **ADC**: Converts voltage input into digital values for RPM calculation.
- **PWM Motor Control**: Adjusts motor speed based on normalized ADC input.

---

## PlatformIO Setup
1. Install [PlatformIO](https://platformio.org/) for your preferred IDE (e.g., VS Code).
2. Initialize a new PlatformIO project:
   ```bash
   platformio init --board stm32f9
