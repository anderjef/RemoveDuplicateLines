# Remove Duplicate Lines

## Table of Contents
- [Remove Duplicate Lines](#remove-duplicate-lines)
  - [Table of Contents](#table-of-contents)
  - [Description](#description)
      - [Programming language(s):](#programming-languages)
    - [Folder structure](#folder-structure)
  - [Installation](#installation)
  - [Usage](#usage)
    - [Invokation](#invokation)
    - [Operation](#operation)
    - [Parameters](#parameters)
  - [Contributions](#contributions)

## Description
Parses some number of (ASCII) files for duplicate lines and create numbered output files with the duplicate lines removed. Program developed for a Windows system. <br>
Start date: July 14, 2020 <br>
Current version: 0.1.0 <br>
#### Programming language(s):
- C++
	- [RemoveDuplicateLines/RemoveDuplicateLines.cpp](RemoveDuplicateLines/RemoveDuplicateLines.cpp)&mdash;main program entrance containing all pertinent code and structures

### Folder structure
| Tree with Root Having Local Path | Generation Method | Description |
| -------------------------------- | ----------------- | ----------- |
| [https://github.com/anderjef/RemoveDuplicateLines](https://github.com/anderjef/RemoveDuplicateLines) | auto-generated by Visual Studio IDE | <!-- --> |
| [.github/](.github/) | <!-- --> | holds GitHub-specific documents |

## Installation
- Install [Visual Studio IDE](https://visualstudio.microsoft.com/downloads/) or some other IDE that supports C++14.
- Download [ZIP](https://github.com/anderjef/RemoveDuplicateLines/archive/main.zip). Extract to a folder named RemoveDuplicateLines.
- Import the project from RemoveDuplicateLines into the IDE then build the project from within the IDE.

## Usage
`<executable> [options/flags] [fileName] [options/flags] [additionalFileNames] [options/flags]`
### Invokation
- See [Installation](#installation).
- Run the corresponding executable.

### Operation
After handling any command-line arguments, the program will prompt the user for an additional line of arguments through the standard input. If no additional arguments are desired, the simple blank line will do to terminate the program.

### Parameters
| Option/Flag | Explanation |
| ----------- | ----------- |
| [-h] | Display <u>h</u>elp |
| [-i] | Treat the next argument as if it were a file name (rather than as any option flag) |
| [-n] | Make the line delimiter in subsequent files the <u>n</u>ewline character (which is the default behavior) |
| [-r] | Change the line delimiter in subsequent files to carriage <u>r</u>eturn (instead of the default newline character) |
| [-v] | Toggle whether to additionally print (to standard output) the duplicate lines found for subsequent files and how many of each were removed (verbose mode is off by default) |

## Contributions
- Jeffrey Andersen&mdash;developer

For copyright, license, and warranty, see [LICENSE.md](LICENSE.md).