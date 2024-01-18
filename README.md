<a name="readme-top"></a>
[![License: LGPL v3](https://img.shields.io/badge/License-LGPL_v3-blue.svg)](https://github.com/Magic146W/Qt_FileExplorer/blob/master/LICENSE)

<br />
<div align="center">
  <h1 align="center">Qt based File Explorer</h1>

  <p align="center">
    Qt_FileExplorer, windows desktop application built with Qt, offers an intuitive interface for file navigation and management.
    <br />
    It efficiently saves your layout preferences and simplifies file previews, enhancing usability.
    <br />
  </p> 
</div>
<details>
  <summary><h2>Table of Contents</h2></summary>
  <ol>
    <li>
      <a href="#changes">Changes</a>
    </li>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#getting-project-to-run-on-visual-studio">Getting project to run on Visual Studio</a></li>
        <li><a href="#getting-project-to-run-on-qt-creator">Getting project to run on Qt Creator</a></li>
      </ul>
    </li>
    <li>
     <a href="#usage">Usage</a>
      <ul>
        <li><a href="#presentation">Presentation</a></li>
      </ul>
    </li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
  </ol>
</details>

## Changes

Currently version v1.0, link to the prototype version's branch [![Prototype](https://img.shields.io/badge/Prototype-blue)](https://github.com/Magic146W/Qt_FileExplorer/tree/Qt_FIleExplorer_prototype)

 - Upgraded Qt version from 6.6.0 to 6.6.1, introducing critical improvements and new features to enhance the overall functionality.
 - Changed the underlying build system from MinGW 64-bit to MSVC2019 64bit, leveraging the benefits of the Microsoft Visual Studio Compiler for optimized performance and compatibility.
 - Implemented a transition from the QMake build system to a more robust CMake-based system for improved project organization and build efficiency.
 - Introduced new modular classes to enhance code structure and better distribute responsibilities, resulting in a cleaner and more maintainable codebase.
 - Conducted a thorough code review, removing redundant sections and optimizing existing code for improved performance and readability.
 - Streamlined the data management approach by consolidating the file list model into a single, unified file system model, eliminating redundancy and enhancing overall project coherence.
 - Implemented a user-friendly feature allowing users to rename items in the list view by simply holding the left mouse button for over a second on listView item (see Presentation section).
 - Enhanced user interface flexibility by introducing the capability to hide the side buttons panel for a more immersive and customizable user experience (see Presentation section).

  <p align="left">
   <br/>
  <i>Use index.html inside Doxygen_Documentation in order to open documentation.</i>
  </p>

## About The Project

 Qt_FileExplorer is a hands-on study project designed to bring theoretical concepts of the Qt framework into practical implementation. By exploring this project, you'll gain practical insights into various fundamental concepts:

 * Qt Signals and Slots - Witness the power of signal-slot connections by enabling interactive features like file selection, deletion, and layout changes.
 * Customizing UI Elements - Learn how to extend and customize Qt's built-in classes like QListview to modify file display and interaction behaviors.
 * Event Handling - Experience event-driven programming by handling user interactions such as clicks or keypresses to trigger specific actions within the file explorer.
 * Dialog Windows and Modalities - Implement dialog windows to facilitate user inputs, providing pop-ups for file operations or preferences customization.
 * QObject and Object-Oriented Design - Explore QObject's role in Qt, understanding its significance in enabling hierarchical object organization and management.

 By examining Qt_FileExplorer's code, you'll see how signals, UI customization, event handling, dialogs, and object-oriented design are practically used in a file explorer application. I would greatly appreciate any feedback on the code!

### Built With

[![Qt](https://img.shields.io/badge/Qt-%23217346.svg?style=for-the-badge&logo=Qt&logoColor=white)](https://www.qt.io/download-open-source) Used as the primary Integrated Development Environment (IDE) for coding the Qt_FileExplorer project. Version 6.6.1.
<br>
<a href="https://www.doxygen.nl/">
  Doxygen
</a>: Employed for generating project documentation, aiding in code readability and comprehension.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Getting Started

For the project to function properly, user must have either Qt Creator or Visual Studio IDE 2019 (or newer) installed on their system. These Integrated Development Environments (IDEs) provide the necessary tools and environment to compile, run, and work with the Qt_FileExplorer application seamlessly.

### Prerequisites

**QT**<br>
You will need Qt version 6.6.1 or newer, along with the Developer and Designer Tools. If you're unsure whether these tools are installed or if they are not installed, open the Qt Maintenance Tool by navigating to:
"Tools > Qt Maintenance Tool > Start Maintenance Tool" (select the necessary options in the 'Select Components' tab)
<br>

**Visual Studio**<br>
If you prefer using the Visual Studio IDE, ensure that you have the necessary components installed. Access the Visual Studio Installer, modify your Visual Studio IDE, and search for the 'C++ CMake tools for Windows' in the 'Individual Components' tab. Install it if needed. Alternatively, you can install the C++ Windows package, ensuring to select CMake if you haven't installed C++ yet.
    
### Getting project to run on Visual Studio

1. Navigate "File > Clone Repository..." and select folder for repository <a href="https://github.com/Magic146W/Qt_FileExplorer.git">https://github.com/Magic146W/Qt_FileExplorer.git</a>.

2. Setting Up CMake for Visual Studio:
- Open Visual Studio and go to “File &gt; Open &gt; CMake…” and select CMakeLists.txt inside your working directory<br>
  - If you don’t find the CMake option, please go through prerequisites for VS<br>
  -  The first time you open this option, an overview of CMake in Visual Studio will appear. Once done, repeat the process of opening CMake from "File &gt; Open" and select your working directory, then choose CMakeLists.txt.<br>
-  Wait for the project to build.
3. To run the project, either click the green arrow with "Qt Application" written next to it or press F5.


### Getting project to run on Qt Creator

1. Clone the repository to your local machine:
   ```sh
   git clone https://github.com/Magic146W/Qt_FileExplorer.git
   ```
2. Open the project by clicking on CMakeLists.txt inside of the cloned directory.
3. When opening the project for the first time or if not previously configured, select the kit "Desktop Qt 6.6.1 MSVC2019 64bit" or a newer version and click on "Configure Project."
4. Wait for the initial project setup and build to finish.
5. Run the project by clicking the green arrow or using the shortcut Ctrl+R.

<p align="right">(<a href="#readme-top">back to top</a>)</p>
## Usage

 File Navigation: 
 * Click on directories in the tree view
 * Double click folders in the list view
 * Click directory button (to the left of path above tree view)
 * Press copy-path button (to the right of path above tree view) to copy selected directory

 File Operations (Use the toolbar buttons or context menu options for file operations):
 * Create folder
 * Rename file or folder
 * Delete file or folder

 File Preview:
 * Double click on files to open images or text files for preview

 File Rename (from listView):
 * Hold left mouse button pressed on selected list view item for +1 seconds

 Layout Preferences:
 * Switch between grid and list view using the layout checkbox
 * Hide or show buttons side panel with highlighted vertical button

 Light/Dark Mode:
 * Toggle between light and dark mode using the dedicated checkbox

 List view file preferences:
 * Hide or show folders at selected path

### Presentation

<br>

 ![QtFileExplorer](https://github.com/Magic146W/Qt_FileExplorer/assets/88307484/dc0e7d39-8fc3-4c71-87da-4e584785799d)]
 
<br>
<br>

 ![egzample 2](https://github.com/Magic146W/Qt_FileExplorer/assets/88307484/8dbff956-b800-48e1-ae37-87abb2d2841b)]
 
<br>
<br>

NEW! - Renaming from listView
<br>
![NEW! - Renaming](https://github.com/Magic146W/Qt_FileExplorer/assets/88307484/c08ab6f9-fff2-44f7-9731-edac50389868)]

<br>
<br>

NEW! - Hide/Show buttons Panel
<br>
![NEW - Hide/Show buttons panel](https://github.com/Magic146W/Qt_FileExplorer/assets/88307484/206421e3-6b05-4ca7-87e8-03eda7bf0567)]

<br>
<br>
<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Contributing

Contributions make the open-source community a fantastic space to learn, inspire, and create. Your contributions are **very much appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/YourFeature`)
3. Commit your Changes (`git commit -m 'Add my YourFeature'`)
4. Push to the Branch (`git push origin feature/YourFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## License

Distributed under the LGPL v3 License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>
