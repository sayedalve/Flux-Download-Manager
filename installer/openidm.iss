; SPDX-License-Identifier: GPL-3.0-or-later
; OpenIDM — Inno Setup 6 Windows 11 x64 Installer Script (v0.2.0)
; Enforces strict presence of all Windows x64 binaries (zero skipifsourcedoesntexist flags),
; rewrites Native Messaging JSON manifests with the exact installed {app}\openidm-nmh.exe path,
; registers HKCU NativeMessagingHosts keys for Chrome, Edge, Brave, and Firefox, and cleans up on uninstall.

#define MyAppName "OpenIDM"
#define MyAppVersion "0.2.0"
#define MyAppPublisher "OpenIDM Contributors"
#define MyAppURL "https://github.com/openidm/openidm"
#define MyAppExeName "openidm.exe"
#define MyAppNmhName "openidm-nmh.exe"

[Setup]
AppId={{8F9D4A30-5B1E-4C62-9A1D-0E2B2C3D4790}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
LicenseFile=..\LICENSE_REVIEW.md
OutputDir=..\dist
OutputBaseFilename=OpenIDM-Setup-x64-{#MyAppVersion}
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
CloseApplications=yes
AppMutex=Local\OpenIDM_SingleInstance

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
; Strict dependency enforcement: compilation fails immediately if any x64 binary is missing
Source: "..\build\win-x64\openidm.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build\win-x64\openidm-nmh.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\tools\win-x64\N_m3u8DL-RE.exe"; DestDir: "{app}\tools"; Flags: ignoreversion
Source: "..\tools\win-x64\ffmpeg.exe"; DestDir: "{app}\tools"; Flags: ignoreversion
Source: "..\extension\*"; DestDir: "{app}\extension"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\README.md"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\LICENSE_REVIEW.md"; DestDir: "{app}"; Flags: ignoreversion
Source: "manifests\org.openidm.native.chromium.json"; DestDir: "{app}"; Flags: ignoreversion
Source: "manifests\org.openidm.native.firefox.json"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "autostart"; Description: "Start OpenIDM minimized in system tray when Windows starts"; GroupDescription: "Startup:"

[Registry]
; Register Native Messaging Host for Chrome, Edge, Brave, and Firefox
Root: HKCU; Subkey: "Software\Google\Chrome\NativeMessagingHosts\org.openidm.native"; ValueType: string; ValueName: ""; ValueData: "{app}\org.openidm.native.chromium.json"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Microsoft\Edge\NativeMessagingHosts\org.openidm.native"; ValueType: string; ValueName: ""; ValueData: "{app}\org.openidm.native.chromium.json"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\BraveSoftware\Brave-Browser\NativeMessagingHosts\org.openidm.native"; ValueType: string; ValueName: ""; ValueData: "{app}\org.openidm.native.chromium.json"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Mozilla\NativeMessagingHosts\org.openidm.native"; ValueType: string; ValueName: ""; ValueData: "{app}\org.openidm.native.firefox.json"; Flags: uninsdeletekey
; Optional startup registration
Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; ValueType: string; ValueName: "OpenIDM"; ValueData: """{app}\{#MyAppExeName}"" --daemon"; Tasks: autostart; Flags: uninsdeletevalue

[Code]
procedure WriteNativeMessagingManifests();
var
  NmhPathEscaped: String;
  ChromiumJson: String;
  FirefoxJson: String;
begin
  NmhPathEscaped := ExpandConstant('{app}\{#MyAppNmhName}');
  StringChangeEx(NmhPathEscaped, '\', '\\', True);

  ChromiumJson :=
    '{' + #13#10 +
    '  "name": "org.openidm.native",' + #13#10 +
    '  "description": "OpenIDM Native Messaging Host",' + #13#10 +
    '  "path": "' + NmhPathEscaped + '",' + #13#10 +
    '  "type": "stdio",' + #13#10 +
    '  "allowed_origins": [' + #13#10 +
    '    "chrome-extension://openidmchromiumextensionid000000/"' + #13#10 +
    '  ]' + #13#10 +
    '}' + #13#10;
  SaveStringToFile(ExpandConstant('{app}\org.openidm.native.chromium.json'), ChromiumJson, False);

  FirefoxJson :=
    '{' + #13#10 +
    '  "name": "org.openidm.native",' + #13#10 +
    '  "description": "OpenIDM Native Messaging Host",' + #13#10 +
    '  "path": "' + NmhPathEscaped + '",' + #13#10 +
    '  "type": "stdio",' + #13#10 +
    '  "allowed_extensions": [' + #13#10 +
    '    "extension@openidm.org"' + #13#10 +
    '  ]' + #13#10 +
    '}' + #13#10;
  SaveStringToFile(ExpandConstant('{app}\org.openidm.native.firefox.json'), FirefoxJson, False);
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
  begin
    WriteNativeMessagingManifests();
  end;
end;
