# Audbile-Altimeter

![C++](https://img.shields.io/badge/c++-black.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-black?style=for-the-badge&logo=cmake&logoColor=white)
![Python](https://img.shields.io/badge/python-black?style=for-the-badge&logo=python&logoColor=white)
![Raspberry Pi Logo](https://img.shields.io/badge/Raspberry%20Pi-black?style=for-the-badge&logo=Raspberry%20Pi&logoColor=white)

![Clang Format](https://github.com/khurd21/Audible-Altimeter/actions/workflows/clang-format.yml/badge.svg)
![Build and Test](https://github.com/khurd21/Audible-Altimeter/actions/workflows/general-build.yml/badge.svg)
![Pico Test](https://github.com/khurd21/Audible-Altimeter/actions/workflows/pico-build.yml/badge.svg)

Audible Altimeter is a speaking altimeter. It acts as a mechanism for skydivers to maintain altitude
awareness while skydiving. Designed by a skydiver, it aims to match the simplicity and reliability of most
modern audible devices, yet still be cost-effective compared to the typical audible altimeters on the market.
The audible reports essential altitudes between 15,000 feet to 0 feet. It can be turned on with a simple flick
of a switch and is dimensionally designed to fit into most skydiving helmets.

The audible can be assembled at home! Feel free to view the [wiki](https://github.com/khurd21/Audible-Altimeter/wiki)
to see how it can be assembled.

## Tools Used

This project depends on the following tools. Assuming you have a modern C++ compiler and access to GitHub,
the VSCode extensions and cmake tools should install all of these for you. It is recommended to install
the recommended extensions in this project, although not strictly necessary in order to develop.

- [Pico SDK - v2.0.0](https://github.com/raspberrypi/pico-sdk/releases/tag/2.0.0)
- [GTest - v1.15.2](https://github.com/google/googletest/releases/tag/v1.15.2)
- [Clang Format](https://clang.llvm.org/docs/ClangFormat.html)
- C++ 20

## Setup Environment

1. Clone the repository and install the recommended extensions in VSCode.
2. In the CMake settings, select the Pico kit for compiling for the Tiny2350, or a compiler of your choice for the general build.
`NOTE:` The general build will not produce a flashable executable and is only intended to allow tests to be run.
3. Run `CMake: build` in the Command Palette.

