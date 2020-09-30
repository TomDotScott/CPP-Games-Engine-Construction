/*
	HAPI Start
	----------
	This solution contains an already set up HAPI project and this main file

	The directory structure and main files are:

	HAPI_Start - contains the Visual Studio solution file (.sln)
		HAPI_Start - contains the Visual Studio HAPI_APP project file (.vcxproj) and source code
			HAPI - the directory with all the HAPI library files
			Data - a place to put your data files with a few sample ones provided

	Additionally in the top directory there is a batch file for creating a redistributable Demo folder

	For help using HAPI please see the Reference folder in the HAPI sub-directory
*/

// Include the HAPI header to get access to all of HAPIs interfaces
#include <HAPI_lib.h>

// HAPI itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

void setPixelColour(BYTE* _screen, int _width, int _x, int _y, int colour) {
	_screen[_width * _x + _y] = colour;
}

void SetScreenColour(BYTE* _screen, int _width, int _height, int _x, int _y, int colour) {
	for (int i = 0; i < (_width - 1) * 4; i++) {
		for (int j = 0; j < (_height - 1) * 4; j++) {
			setPixelColour(_screen, _width, i, j, colour);
		}
	}
}

bool withinRange(int val) {
	return false;
}

bool ChangeScreenOrPixel() {
	std::cout << "1 to change screen colour, 2 to change pixel colour..." << std::endl;
	int choice;
	choice = HAPI.GetEnteredText();
	if (choice != 0) {
		if (choice == 1) {
			return true;
		} else if(choice == 2){
			return false;
		}
		
	}

}


// Every HAPI program has a HAPI_Main as an entry point
// When this function exits the program will close down
void HAPI_Main() {
	int width{ 1000 };
	int height{ 1000 };
	int col = 255;

	if (HAPI.Initialise(width, height, "Colour Changing Window!")) {
		BYTE* screen = HAPI.GetScreenPointer();
		HAPI.SetShowFPS(true);
		// Keeps track of whether wer'e changing the Screen or the Pixel
		bool changeScreen{ true };
		while (HAPI.Update()) {
			changeScreen = ChangeScreenOrPixel();
		}
	}


}

