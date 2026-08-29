# virtual_tts_mic

![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![SAPI](https://img.shields.io/badge/Speech%20API-SAPI-blue?style=for-the-badge)

A lightweight C++ application for Windows that turns any text you type into speech, routed through a virtual microphone so you can "speak" into voice calls, games, or streams just by typing.

## How it works

The app uses [VB-Audio Virtual Cable](https://vb-audio.com/Cable/) to create a virtual microphone device. When you type text into the console and hit Enter, the app converts it to speech and plays it through that virtual mic, so any application listening for microphone input (Discord, Zoom, games, etc.) hears it as if you spoke.

## Technology & Dependencies

- **Language:** C++
- **Text-to-Speech:** [Microsoft Speech API (SAPI)](https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee125077(v=vs.85)) - converts typed text into synthesized speech.
- **COM libraries:** `ole32.lib`, `oleaut32.lib` - required to initialize and interact with SAPI's COM-based interfaces.
- **Audio routing:** [VB-Audio Virtual Cable](https://vb-audio.com/Cable/) - SAPI plays audio through the default output device, so setting your default playback device to the VB-Cable input routes the speech into the virtual microphone.

Linked libraries:
```cpp
#pragma comment(lib, "sapi.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
```

## Prerequisites

- Windows OS
- [VB-Audio Virtual Cable](https://vb-audio.com/Cable/) installed and configured
- A C++ compiler / build tools (e.g. Visual Studio) to build from source

## Installation

1. Clone or download this repository.
2. Install [VB-Audio Virtual Cable](https://vb-audio.com/Cable/) if you haven't already.
3. Compile the project.
   - Build in **Debug** mode if you run into issues - it provides extra diagnostic output.

## Usage

1. Launch the application - a console window will appear.
2. In your target app (Discord, Zoom, a game, etc.), set your microphone/input device to the **VB-Audio virtual cable** created by the installer.
3. In the console window, type the text you want spoken and press **Enter**.
4. The app converts your text to speech and plays it through the virtual microphone.
5. Type 'exit' to close out of the program.

## Troubleshooting

- **No audio output / other apps can't hear it:** Double-check that your target application's input device is set to the VB-Audio virtual cable, not your physical mic. Also confirm your Windows default playback device is set to VB-Cable.
- **Does not work:** Recompile the source in debug mode and look for errors in the console.

## Notes

### Voice

#### Known limitations
- The voice can have issues with pronouncing words.
- The voice does not sound the best.

#### Possible improvements

- Instead of SAPI use something like [ElevenLabs](https://elevenlabs.io/) for the voice it should pronounce things better and sound less robotic.

