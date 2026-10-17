; SPDX-License-Identifier: GPL-3.0-or-later
; Flux Download Manager — Inno Setup 6 Windows 11 x64 Installer Script (v0.3.0)
; Installs flux.exe (WIN32 GUI), flux-nmh.exe (CUI Native Messaging Host),
; N_m3u8DL-RE.exe, ffmpeg.exe, browser extension, and registers Native Messaging
; manifests for Chrome, Edge, Brave, and Firefox with automatic path rewriting.

#define MyAppName "Flux Download Manager"
#define MyAppShortName "Flux"
#define MyAppVersion "0.3.0"
#define MyAppPublisher "Flux Contributors"
#define MyAppURL "https://github.com/flux-download-manager/flux"
#define MyAppExeName "flux.exe"
#define MyAppNmhName "flux-nmh.exe"

[Setup]
AppId={{4D9C5F31-6B2E-4D73-8B2E-1F3C3D4E5891}
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
OutputBaseFilename=Flux-Setup-x64-{#MyAppVersion}
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
CloseApplications=yes
AppMutex=Local\Flux_SingleInstance,Local\OpenIDM_SingleInstance

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
; Primary application binaries
Source: "..\build\win-x64\Release\flux.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build\win-x64\Release\flux-nmh.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\tools\win-x64\N_m3u8DL-RE.exe"; DestDir: "{app}\tools"; Flags: ignoreversion
Source: "..\tools\win-x64\ffmpeg.exe"; DestDir: "{app}\tools"; Flags: ignoreversion
Source: "..\extension\*"; DestDir: "{app}\extension"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\README.md"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\LICENSE_REVIEW.md"; DestDir: "{app}"; Flags: ignoreversion
Source: "manifests\com.flux.downloadmanager.chromium.json"; DestDir: "{app}"; Flags: ignoreversion
Source: "manifests\com.flux.downloadmanager.firefox.json"; DestDir: "{app}"; Flags: ignoreversion
Source: "manifests\org.openidm.native.chromium.json"; DestDir: "{app}"; Flags: ignoreversion
Source: "manifests\org.openidm.native.firefox.json"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "autostart"; Description: "Start Flux minimized in system tray when Windows starts"; GroupDescription: "Startup:"

[Registry]
; Primary Flux Native Messaging Registration
Root: HKCU; Subkey: "Software\Google\Chrome\NativeMessagingHosts\com.flux.downloadmanager"; ValueType: string; ValueName: ""; ValueData: "{app}\com.flux.downloadmanager.chromium.json"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Microsoft\Edge\NativeMessagingHosts\com.flux.downloadmanager"; ValueType: string; ValueName: ""; ValueData: "{app}\com.flux.downloadmanager.chromium.json"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\BraveSoftware\Brave-Browser\NativeMessagingHosts\com.flux.downloadmanager"; ValueType: string; ValueName: ""; ValueData: "{app}\com.flux.downloadmanager.chromium.json"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Mozilla\NativeMessagingHosts\com.flux.downloadmanager"; ValueType: string; ValueName: ""; ValueData: "{app}\com.flux.downloadmanager.firefox.json"; Flags: uninsdeletekey

; Backwards-Compatibility Native Messaging Registration (for OpenIDM extensions)
Root: HKCU; Subkey: "Software\Google\Chrome\NativeMessagingHosts\org.openidm.native"; ValueType: string; ValueName: ""; ValueData: "{app}\org.openidm.native.chromium.json"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Microsoft\Edge\NativeMessagingHosts\org.openidm.native"; ValueType: string; ValueName: ""; ValueData: "{app}\org.openidm.native.chromium.json"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\BraveSoftware\Brave-Browser\NativeMessagingHosts\org.openidm.native"; ValueType: string; ValueName: ""; ValueData: "{app}\org.openidm.native.chromium.json"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Mozilla\NativeMessagingHosts\org.openidm.native"; ValueType: string; ValueName: ""; ValueData: "{app}\org.openidm.native.firefox.json"; Flags: uninsdeletekey

; Optional startup registration
Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; ValueType: string; ValueName: "Flux"; ValueData: """{app}\{#MyAppExeName}"" --daemon"; Tasks: autostart; Flags: uninsdeletevalue

[Code]
procedure WriteNativeMessagingManifests();
var
  NmhPathEscaped: String;
  FluxChromiumJson: String;
  FluxFirefoxJson: String;
  LegacyChromiumJson: String;
  LegacyFirefoxJson: String;
begin
  NmhPathEscaped := ExpandConstant('{app}\{#MyAppNmhName}');
  StringChangeEx(NmhPathEscaped, '\', '\\', True);

  FluxChromiumJson :=
    '{' + #13#10 +
    '  "name": "com.flux.downloadmanager",' + #13#10 +
    '  "description": "Flux Download Manager Native Messaging Host",' + #13#10 +
    '  "path": "' + NmhPathEscaped + '",' + #13#10 +
    '  "type": "stdio",' + #13#10 +
    '  "allowed_origins": [' + #13#10 +
    '    "chrome-extension://aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa/",' + #13#10 +
    '    "chrome-extension://ngpampappnmepgilojfohadhhmbhlaek/",' + #13#10 +
    '    "chrome-extension://llbjbkhnmlidjebalopleeepgdfgcpec/"' + #13#10 +
    '  ]' + #13#10 +
    '}' + #13#10;
  SaveStringToFile(ExpandConstant('{app}\com.flux.downloadmanager.chromium.json'), FluxChromiumJson, False);

  FluxFirefoxJson :=
    '{' + #13#10 +
    '  "name": "com.flux.downloadmanager",' + #13#10 +
    '  "description": "Flux Download Manager Native Messaging Host",' + #13#10 +
    '  "path": "' + NmhPathEscaped + '",' + #13#10 +
    '  "type": "stdio",' + #13#10 +
    '  "allowed_extensions": [' + #13#10 +
    '    "flux-extension@fluxdownloadmanager.org"' + #13#10 +
    '  ]' + #13#10 +
    '}' + #13#10;
  SaveStringToFile(ExpandConstant('{app}\com.flux.downloadmanager.firefox.json'), FluxFirefoxJson, False);

  LegacyChromiumJson :=
    '{' + #13#10 +
    '  "name": "org.openidm.native",' + #13#10 +
    '  "description": "Flux Download Manager Legacy Bridge",' + #13#10 +
    '  "path": "' + NmhPathEscaped + '",' + #13#10 +
    '  "type": "stdio",' + #13#10 +
    '  "allowed_origins": [' + #13#10 +
    '    "chrome-extension://aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa/",' + #13#10 +
    '    "chrome-extension://ngpampappnmepgilojfohadhhmbhlaek/",' + #13#10 +
    '    "chrome-extension://llbjbkhnmlidjebalopleeepgdfgcpec/"' + #13#10 +
    '  ]' + #13#10 +
    '}' + #13#10;
  SaveStringToFile(ExpandConstant('{app}\org.openidm.native.chromium.json'), LegacyChromiumJson, False);

  LegacyFirefoxJson :=
    '{' + #13#10 +
    '  "name": "org.openidm.native",' + #13#10 +
    '  "description": "Flux Download Manager Legacy Bridge",' + #13#10 +
    '  "path": "' + NmhPathEscaped + '",' + #13#10 +
    '  "type": "stdio",' + #13#10 +
    '  "allowed_extensions": [' + #13#10 +
    '    "extension@openidm.org"' + #13#10 +
    '  ]' + #13#10 +
    '}' + #13#10;
  SaveStringToFile(ExpandConstant('{app}\org.openidm.native.firefox.json'), LegacyFirefoxJson, False);
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
  begin
    WriteNativeMessagingManifests();
  end;
end;
