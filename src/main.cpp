#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 320, 480, OF_WINDOW); // changed this to match iPhone resolution
	ofRunApp(new testApp());
}
