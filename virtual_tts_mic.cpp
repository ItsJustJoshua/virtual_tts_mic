#include <windows.h>
#include <sapi.h>
#include <sperror.h>
#include <iostream>
#include <string>

// libs needed for sapi and virtual cable
#pragma comment(lib, "sapi.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")

// global voice poiner so i can acess it anywhere here
ISpVoice* pVoice;

// log function that works if debug is enabled
void log(const std::string& message)
{
#if _DEBUG
	std::cout << "[tts_mic] " << message << std::endl;
#endif
}

// get the virtual cable input token so we can make the voice speak through it
ISpObjectToken* GetCableInputToken() 
{
	// create a category for audio output tokens
	ISpObjectTokenCategory* pCategory = nullptr;
	CoCreateInstance(CLSID_SpObjectTokenCategory, nullptr, CLSCTX_ALL, IID_ISpObjectTokenCategory, (void**)&pCategory);
	pCategory->SetId(SPCAT_AUDIOOUT, FALSE);

	// enumerate the tokens in the category
	IEnumSpObjectTokens* pEnum = nullptr;
	pCategory->EnumTokens(nullptr, nullptr, &pEnum);
	pCategory->Release();

	ISpObjectToken* pToken = nullptr;
	ISpObjectToken* found = nullptr;

	// loop through until we find the virtual cable input token
	while (pEnum->Next(1, &pToken, nullptr) == S_OK) {
		LPWSTR desc = nullptr;
		pToken->GetStringValue(nullptr, &desc);

		if (desc && wcsstr(desc, L"CABLE Input") != nullptr) {
			found = pToken;
			found->AddRef();
		}
		// free the description string and release the token
		if (desc) CoTaskMemFree(desc);
		pToken->Release();
	}
	// release the enumerator
	pEnum->Release();

	return found;
}

int start_up()
{
	log("Setting up voice!");

	// initialize COM library
	CoInitialize(nullptr);

	// create the SAPI voice
	pVoice = nullptr;
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
	if (FAILED(hr)) {
		log("Failed to create SAPI voice.");
		return 0;
	}
	
	log("Voice initialized successfully!");

	log("Setting up voice to use the virtual microphone!");

	// get the virtual cable input token and set it as the output for the voice
	ISpObjectToken* pCable = GetCableInputToken();
	if (pCable) {
		pVoice->SetOutput(pCable, FALSE);
		pCable->Release();
	}
	else {
		log("Failed to find CABLE Input device.");
		return 0;
	}

	return 1;
}

bool main_loop()
{
	std::cout << "Waiting for input! Type 'exit' to quit." << std::endl;

	// create a string to hold the input from stdin
	std::string input;

	// get input from stdin
	std::getline(std::cin, input);

	// log input for debugging purposes
	log("Input received: " + input);
	if (input == "exit") {
		return true;
	}

	// convert the input into needed format
	std::wstring wline(input.begin(), input.end()); 

	// make it speak the input (will speak it through the virtual microphone)
	pVoice->Speak(wline.c_str(), SPF_DEFAULT, nullptr);
	return false;
}

void cleanup()
{
	// clean up the voice and COM library
	if (pVoice) {
		pVoice->Release();
		pVoice = nullptr;
	}

	CoUninitialize();
}

int main()
{
	log("Starting virtual TTS microphone");

	// if can create everything needed then move onto main loop if not then exit with error code 1
	if (start_up() == 0)	
	{
		log("Failed to start up virtual TTS microphone");
		return 1;
	}

	bool exit = false;
	while (!exit)
	{
		exit = main_loop();
	}

	cleanup();
}
