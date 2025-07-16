# Sublime for Unreal Engine

## How to install
### Tutorial
You can watch a detailed video tutorial here: https://www.youtube.com/watch?v=eTVvhbQyGts

### Step by step
- Download Sublime Text.
- You need to download LLVM, you can find it looking for "Unreal Engine (your version) Release Notes" and look for "Platform Sdk Upgrades" and in Windows the preferred LLVM must appear, then search: "LLVM (the version that appears)", install it and add to the path.
- Download this repository.
- In your game or in a temporary game, create a folder called "Plugins", within that folder creates another called "Developer" and put there inside the folder the "SublimeSourceCodeAccess" folder of the repository.
- Now you need to generate the Visual Studio files.
- Now open the .sln in Visual Studio and compile the entire solution.
- Now copy the compilated plugin to the folder: `C:\Program Files\Epic Games\Your Version\Engine\Plugins\Developer`
- Now open your project and in "Editor Settings" > "Source Code" you change it to "Sublime" and restart the editor.
- Now press in "Tools" > "Open sublime" and should work, but you don't have the autocomplete, so close the editor, and put this command in the cmd: `"C:\Program Files\Epic Games\Your Version\Engine\Build\BatchFiles\Build.bat" -mode=GenerateClangDatabase -project="C:\Your\Game\file.uproject" -game -engine -dotnet yourgameEditor Development Win64` be sure to add "Editor" at the end of your game name.
- Copy the `compile_commands.json` file of the directory that the output says to the folder of your project.
- Open your project and click in "Open Sublime" and press Ctrl + Shift + P and put "install package control" and press enter, after it is installed restart Sublime Text.
- And now install these packages: "LSP" and "LSP-Clangd" and then wait for the files to load.
- Now go to the LSP package settings and then put this code:
```json
{
  "semantic_highlighting": true,
}
```
- And you have it! Now you can customize the editor however you want!
