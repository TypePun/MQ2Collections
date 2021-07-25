# Build Notes

MQ2Collections is provided as a Visual Studio 2019 Solution. Project configurations are
provided for both Debug and Release mode builds.

## Requirements

1. A current copy of the [Macroquest2](https://macroquest2.com/main.php?p=download) source must
be installed on your computer.
2. The environment variable **MQ2** must be set to point to the root of the current Macroquest2 folder. The root folder is the folder that contains the Macroquest2 solution file (Macroquest2.sln) and the MQ2Main folder. The **MQ2** environment variable must be in the environment of your build process. Typically, it is set as a system environment variable (see: [Setting an environment variable](https://www.computerhope.com/issues/ch000549.htm)), or look up how to set an environment variable in your favorite search engine. If it is not set it will default to '..\..', which presumes that the MQ2Collections folder was cloned under the MQ2 folder. This is not an unreasonable assumption for plugins and will work if you build plugins in that manner.
3. The **MQ2Collections** builds assume that MQ2 Release mode binaries are in $(MQ2)\Release and Debug mode binaries are in $(MQ2)\Debug. The stock MQ2 project files only generate output into the $(MQ2)\Release folder, regardless of the selected configuration. Changing the stock MQ2 settings to write output into configuration-specific folders is beyond the scope of this note.
4. Clone this git repo to your local machine. The URL is: https://github.com/TypePun/MQ2Collections.git.

## Environments

There are four sets of project configurations: Debug_Live, Debug, Release and Release_Live. The Debug and Release configurations are build with TEST defined. The Debug_Live and Release_Live configurations do no define TEST.

Make sure you build the corresponding version of MQ2Main if you redefine the target for MQ2Collections or the two will be out of synch and results will be unpredicatable.

## Building

The usual way to build MQ2Collections is to load the Solution file (MQ2Collections.sln in the MQ2Collections folder) and build the desired configuration, or configurations. The solution file can also
be used to build from the command line. To do that you will have to:

1. Open a CMD Window;
2. Run vcvars32.bat (see: [Seting environment variables for Visual Studio command line builds](https://docs.microsoft.com/en-us/cpp/build/setting-the-path-and-environment-variables-for-command-line-builds?view=vs-2017));
3. Change to the MQ2Collections directory;
4. Build the solution in Release mode:
  * MSBuild MQ2Collections.sln /t:Build /p:Configuration=Release;Platform=Win32

Following the build, copy the .\Release\MQ2Collections.dll file to your MQ2 executable directory.
 
