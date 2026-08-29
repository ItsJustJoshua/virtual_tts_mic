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
ISpObjectToken* GetCableInputToken() {
	ISpObjectTokenCategory* pCategory = nullptr;
	CoCreateInstance(CLSID_SpObjectTokenCategory, nullptr, CLSCTX_ALL, IID_ISpObjectTokenCategory, (void**)&pCategory);
	pCategory->SetId(SPCAT_AUDIOOUT, FALSE);

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

		if (desc) CoTaskMemFree(desc);
		pToken->Release();
	}
	pEnum->Release();

	return found;
}

int start_up()
{
	log("Setting up voice!");

	CoInitialize(nullptr);

	pVoice = nullptr;
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
	if (FAILED(hr)) {
		log("Failed to create SAPI voice.");
		return 0;
	}
	
	log("Voice initialized successfully!");

	log("Setting up voice to use the virtual microphone!");

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

void main_loop()
{
	std::cout << "Waiting for input!" << std::endl;
	std::string input;
	std::getline(std::cin, input); // get input from stdin

	log("Input received: " + input);

	std::wstring wline(input.begin(), input.end()); // convert the input into needed format

	pVoice->Speak(wline.c_str(), SPF_DEFAULT, nullptr); // make it speak the input (will speak it through the virtual microphone)
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

	while (true)
	{
		main_loop();
	}

}
