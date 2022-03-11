# Build Notes

MQ2Collections is provided as a Visual Studio 2019 Solution. Project configurations are
provided for both Debug and Release mode builds.

## Requirements

1. A current copy of the [MQNext](https://gitlab.com/macroquest/next/mqnext) source must
be installed on your computer.
2. Clone this repo into MQNext's plugin folder as a submodule. From the command prompt:
```
    cd mqnext\plugins
    git add submodule https://github.com/TypePun/MQ2Collections.git
```
3. Open Visual Studio 2019 and load the MQNext solution file. The solution file is found at:
```
    mqnext\src\Macroquest.sln
```
4. Add the MQ2Collections Visual Studio 2019 project file to the *plugins* folder. The MQ2Collections project file is found at:
```
    mqnext\plugins\MQ2Collections\plugin\MQ2Collection.vcxproj
```
5. Change the project dependencies of MQ2Collections such that MQ2Collections is dependent on MQ2Main. In other words, MQ2Main must build BEFORE MQ2Collections (see: [Project Dependencies](https://docs.microsoft.com/en-us/visualstudio/ide/how-to-create-and-remove-project-dependencies?view=vs-2019#:~:text=To%20assign%20dependencies%20to%20projects%201%20In%20Solution,that%20must%20build%20before%20this%20project%20does.%20))

## Building

Build MQNext. MQCollections will build in the selected build modes.
