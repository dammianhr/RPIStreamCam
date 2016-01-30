
#include "ofMain.h"
#include "ofxHTTP.h"
#include "RPiVideoGrabber.h"
#include "ofAppEGLWindow.h"

class ofApp : public ofBaseApp{
    ofx::HTTP::SimpleIPVideoServer server;
    RPiVideoGrabber vidGrabber;
    int camWidth;
    int camHeight;
    OMXCameraSettings omxCameraSettings;
    
public:
    //--------------------------------------------------------------
    void setup(){
        ofSetLogLevel(OF_LOG_VERBOSE);
        ofSetLogLevel("ofThread", OF_LOG_ERROR);

        camWidth = 320;
        camHeight = 240;
        vidGrabber.setDesiredFrameRate(15);
        vidGrabber.initGrabber(camWidth, camHeight);
        
        ofx::HTTP::SimpleIPVideoServerSettings settings;
        
        settings.setPort(7890);
        settings.ipVideoRouteSettings.setMaxClientConnections(1);
        server.setup(settings);
        
        server.start();
    }
    
    //--------------------------------------------------------------
    void update(){
        vidGrabber.update();
        if (vidGrabber.isFrameNew()){
            ofPixels& pixels = vidGrabber.getPixels();
            server.send(pixels);
        }
        
    }
    
    //--------------------------------------------------------------
    void draw(){
        vidGrabber.draw(0, 0);
        stringstream info;
        info << "App FPS: " << ofGetFrameRate() << "\n";
        info << "URL: " << server.getURL() << "\n";
        info << "Camera Resolution: " << vidGrabber.getWidth() << "x" << vidGrabber.getHeight()	<< " @ "<< vidGrabber.getFrameRate() <<"FPS"<< "\n";
        
        ofDrawBitmapStringHighlight(info.str(), 0, vidGrabber.getHeight(), ofColor::black, ofColor::yellow);
    }
};

#if (OF_VERSION_MINOR != 9) && defined(TARGET_OPENGLES)
#include "ofGLProgrammableRenderer.h"
#endif

#define FORCE_PROGRAMMMABLE 1

#ifdef FORCE_PROGRAMMMABLE
#include "ofGLProgrammableRenderer.h"
#endif
int main()
{
    ofSetLogLevel(OF_LOG_VERBOSE);
#if defined(TARGET_OPENGLES)
#if (OF_VERSION_MINOR == 9)
    ofGLESWindowSettings settings;
    settings.width = 480;
    settings.height = 320;
    settings.setGLESVersion(2);
    ofCreateWindow(settings);
#else
    ofSetLogLevel("ofThread", OF_LOG_ERROR);
    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    ofSetupOpenGL(480, 320, OF_WINDOW);
#endif
#else
#ifdef FORCE_PROGRAMMMABLE
    ofGLWindowSettings glWindowSettings;
    glWindowSettings.width = 480;
    glWindowSettings.height = 320;
    glWindowSettings.setGLVersion(3, 2);
    ofCreateWindow(glWindowSettings);
#else
    ofSetLogLevel("ofThread", OF_LOG_ERROR);
    ofSetupOpenGL(480, 320, OF_WINDOW);
#endif
    
#endif
    ofRunApp( new ofApp());
}

