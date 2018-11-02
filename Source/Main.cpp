/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

//==============================================================================
#if JUCE_WINDOWS || JUCE_LINUX || JUCE_MAC
//TODO FIGURE OUT WHY ICON ISN'T SHOWING UP
//TODO MAKE THE 3d IMAGE ROTATABLE -> THEN MOVABLE IN SPACE https://www.codeproject.com/Articles/35139/Interactive-Techniques-in-Three-dimensional-Scenes
//TODO MAKE CAMERA CLASS
// Just add a simple icon to the Window system tray area or Mac menu bar..
struct ClingTaskbarComponent : public SystemTrayIconComponent,
							   private Timer
{
	ClingTaskbarComponent()
	{
		setIconImage(getImageFromUI("cling_icon.png"));
		setIconTooltip("CLING");
	}

	void mouseDown(const MouseEvent&) override
	{
		// On OSX, there can be problems launching a menu when we're not the foreground
		// process, so just in case, we'll first make our process active, and then use a
		// timer to wait a moment before opening our menu, which gives the OS some time to
		// get its act together and bring our windows to the front.

		Process::makeForegroundProcess();
		startTimer(50);
	}

	// This is invoked when the menu is clicked or dismissed
	static void menuInvocationCallback(int chosenItemID, ClingTaskbarComponent*)
	{
		if (chosenItemID == 1)
			JUCEApplication::getInstance()->systemRequestedQuit();
	}

	void timerCallback() override
	{
		stopTimer();

		PopupMenu m;
		m.addItem(1, "Quit");

		// It's always better to open menus asynchronously when possible.
		m.showMenuAsync(PopupMenu::Options(),
			ModalCallbackFunction::forComponent(menuInvocationCallback, this));
	}
};
#endif

//==============================================================================
class ClingApplication  : public JUCEApplication
{
public:
    //==============================================================================
    ClingApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..

        mainWindow = new MainWindow (getApplicationName());
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    Desktop::getInstance().getDefaultLookAndFeel()
                                                                          .findColour (ResizableWindow::backgroundColourId),
                                                    DocumentWindow::allButtons)
        {
			setResizable(true, false);
			
            setContentOwned (new MainComponent(), true);
			setVisible(true);
			
            centreWithSize (getWidth(), getHeight());

			#if JUCE_WINDOWS || JUCE_LINUX || JUCE_MAC
				taskbarIcon = new ClingTaskbarComponent();
			#endif
           
        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
			//TODO add prompt to close
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

	private:
		ScopedPointer<ClingTaskbarComponent> taskbarIcon;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    ScopedPointer<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (ClingApplication)
