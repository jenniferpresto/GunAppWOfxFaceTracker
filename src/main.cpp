#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 640, 480, OF_WINDOW); // changed this, too
	ofRunApp(new testApp());
}
