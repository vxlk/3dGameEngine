/*
  ==============================================================================

    FileBrowser.h
    Created: 8 Nov 2018 2:26:33pm
    Author:  Tyler

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class FileBrowser : public FilePreviewComponent,
						   FileBrowserComponent,
						   FileBrowserListener,
						   FileDragAndDropTarget
{
public:

	//cant inherit and have filefilter in the constructor ... where to get fileFilter?

	FileBrowser(int flags, const File &initialFileOrDirectory, const WildcardFileFilter *fileFilter, FilePreviewComponent *previewComp);

	~FileBrowser() {  }


	//fileBrowserListener Methods
		void selectionChanged();
		//Callback when the user selects a different file in the browser.More...

		void fileClicked(const File &file, const MouseEvent &e);
		//Callback when the user clicks on a file in the browser.More...

		void fileDoubleClicked(const File &file);
		//Callback when the user double - clicks on a file in the browser.More...

		void browserRootChanged(const File &newRoot);


	//FileDragAndDrop Methods
		bool isInterestedInFileDrag(const StringArray &files);
		//Callback to check whether this target is interested in the set of files being offered.More...

		void fileDragEnter(const StringArray &files, int x, int y);
		//Callback to indicate that some files are being dragged over this component.More...

		void fileDragMove(const StringArray &files, int x, int y);
		//Callback to indicate that the user is dragging some files over this component.More...

		void fileDragExit(const StringArray &files);
		//Callback to indicate that the mouse has moved away from this component.More...

		void filesDropped(const StringArray &files, int x, int y);
		//Callback to indicate that the user has dropped the files onto this component.More...

private: 
	/*
	Stuff used for component initialization
	*/
	File initialFile;
	//FileFilter fileFilter;
	//FilePreviewComponent preview;


	//FileListComponent fileList;
	ScopedPointer<FileBrowserComponent> fb;
};
