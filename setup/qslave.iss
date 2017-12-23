#define Name        "QSlave"
#define Version       "1.0.2"
#define Publisher     "maisvendoo"
#define URL           "https://github.com/maisvendoo/qslave.git"
#define ExeName       "qslave-gui.exe"
#define BinDir        "..\..\bin"
#define SrcDir        "..\"
#define ResourceDir   "..\qslave-gui\resources\"
#define ConfigDir     "..\cfg\"

[Setup]
AppId={{EFD32EA4-BAB0-409D-90C6-DABF0FF365BC}}

AppName={#Name}
AppVersion={#Version}
AppPublisher={#Publisher}
AppSupportURL={#URL}
AppUpdatesURL={#URL}

DefaultDirName={pf}\{#Name}
DefaultGroupName={#Name}

OutputDir=..\..\bin-setup
OutputBaseFilename=qslave-v{#Version}-setup

SetupIconFile={#ResourceDir}\img\logo.ico

Compression=lzma
SolidCompression=yes

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]

; Исполняемый файл
Source: "{#BinDir}\{#ExeName}"; DestDir: "{app}"; Flags: ignoreversion

; Прилагающиеся ресурсы
Source: "{#BinDir}\*.*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#ResourceDir}\img\logo.ico"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#ConfigDir}\example\*.*"; DestDir: "{app}\example"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\{#Name}"; Filename: "{app}\{#ExeName}"; IconFilename: "{app}\logo.ico"                            
Name: "{commondesktop}\{#Name}"; Filename: "{app}\{#ExeName}"; IconFilename: "{app}\logo.ico"; Tasks: desktopicon