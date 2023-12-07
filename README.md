<a name="readme-top"></a>
[![License: LGPL v3](https://img.shields.io/badge/License-LGPL_v3-blue.svg)](https://github.com/Magic146W/Qt_FileExplorer/blob/master/LICENSE)

<br />
<div align="center">
  <h1 align="center">Qt based File Explorer</h1>

  <p align="center">
    Qt_FileExplorer, windows desktop application built with Qt, offers an intuitive interface for file navigation and management.
    <br />
    It efficiently saves your layout preferences and simplifies file previews, enhancing usability.
  </p>
</div>

<details>
  <summary><h2>Table of Contents</h2></summary>
  <ol>
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

## About The Project

 Qt_FileExplorer is a hands-on study project designed to bring theoretical concepts of the Qt framework into practical implementation. By exploring this project, you'll gain practical insights into various fundamental concepts:

 * Qt Signals and Slots - Witness the power of signal-slot connections by enabling interactive features like file selection, deletion, and layout changes.
 * Customizing UI Elements - Learn how to extend and customize Qt's built-in classes like QListview to modify file display and interaction behaviors.
 * Event Handling - Experience event-driven programming by handling user interactions such as clicks or keypresses to trigger specific actions within the file explorer.
 * Dialog Windows and Modalities - Implement dialog windows to facilitate user inputs, providing pop-ups for file operations or preferences customization.
 * QObject and Object-Oriented Design - Explore QObject's role in Qt, understanding its significance in enabling hierarchical object organization and management.

 By examining Qt_FileExplorer's code, you'll see how signals, UI customization, event handling, dialogs, and object-oriented design are practically used in a file explorer application. I would greatly appreciate any feedback on the code!

### Built With

[![Qt](https://img.shields.io/badge/Qt-%23217346.svg?style=for-the-badge&logo=Qt&logoColor=white)](https://www.qt.io/download-open-source) Used as the primary Integrated Development Environment (IDE) for coding the Qt_FileExplorer project. Version 6.6.0.
<br>
<a href="https://www.doxygen.nl/">
  Doxygen
</a>: Employed for generating project documentation, aiding in code readability and comprehension.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Getting Started

For the project to function properly, user must have either Qt Creator or Visual Studio IDE 2019 (or newer) installed on their system. These Integrated Development Environments (IDEs) provide the necessary tools and environment to compile, run, and work with the Qt_FileExplorer application seamlessly.

### Prerequisites

**QT**<br>
You will need Qt version 6.6.0 or newer, along with the Developer and Designer Tools. If you're unsure whether these tools are installed or if they are not installed, open the Qt Maintenance Tool by navigating to:
"Tools > Qt Maintenance Tool > Start Maintenance Tool" (select the necessary options in the 'Select Components' tab)
<br>

**Visual Studio**<br>
If you prefer using the Visual Studio IDE, you will need the 'Qt Visual Studio Tools' extension. If you encounter any issues with the extension, consider downloading and using older version '
<a href="https://download.qt.io/development_releases/vsaddin/2.8.0/">
  Qt Visual Studio Tools 2.8.0
</a>
' as an attempt to resolve any problems. 
<br>Also you must add the Qt version in the Options. To do this, select "Tools > Options..." and search for 'Qt' and select Version. Then, choose 'Add' and specify the path, 
for example: 'C:\Qt\6.6.0\msvc2019_64\bin' and select the 'qmake.exe' file. Avoid using MinGW path as Visual Studio does not support it, which may prevent the addition of the Qt version.
    
### Getting project to run on Visual Studio

 1. Navigate "File > Clone Repository..." and select folder for repository (https://github.com/Magic146W/Qt_FileExplorer.git).

2. After the project loads, navigate to "Extensions > Qt VS Tools > Open Qt Project File (.pro)...
 - If you encounter an error stating "qmake has generated a .vcproj file, but it needs to be converted...", you might need to manually open "Qt_FileExplorer.vcxproj" from the project directory. Also, ensure to install any necessary packages highlighted by Visual Studio.

3. Select "File > Open > Project/Solution..." and open "Qt_FileExplorer.vcxproj" inside repository folder.
4. Project is ready to run by pressing on "►Local Windows Debugger".

### Getting project to run on Qt Creator

1. Clone the repository to your local machine:
   ```sh
   git clone https://github.com/Magic146W/Qt_FileExplorer.git
   ```
2. Open the project by clicking on Qt_FileExplorer.pro from the cloned directory.
3. When opening the project for the first time or if not previously configured, select the kit "Desktop Qt 6.6.0 MinGW 64-bit" or a newer version and click on "Configure Project."
4. Wait for the initial project setup and build to finish.
5. Run the project by clicking the green arrow or using the shortcut Ctrl+R.
<br>This updated version highlights that the project should be opened by clicking on the Qt_FileExplorer.pro file if it was previously configured, emphasizing its importance for starting the project in Qt Creator..

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
 * Felete file or folder

 File Preview:
 * Double click on files to open images or text files for preview

 Layout Preferences:
 * Switch between grid and list view using the layout checkbox

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
