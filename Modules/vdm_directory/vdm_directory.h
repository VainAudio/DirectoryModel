/**
    BEGIN_JUCE_MODULE_DECLARATION

    ID:                 vdm_directory_model
    vendor:             VainAudio
    version:            1.0.1
    name:               Vain Directory Model
    description:        Tools for representing directories from the disk as a juce::ValueTree
    website:            VainAudio.com
    license:            MIT
    minimumCppStandard: 17
    dependencies:       vdm juce_core juce_data_structures

    END_JUCE_MODULE_DECLARATION
 */

#pragma once

#include "DirectoryModel/vdm_DirectoryModel.h"
#include "DirectoryModel/vdm_DirectoryModelSync.h"
#include "DirectoryModel/vdm_DirectoryTree.h"
#include "DirectoryModel/vdm_DirectoryModelUpdateHandlers.h"

#include "SelectionModel/vdm_SelectionModel.h"
#include "SelectionModel/vdm_SelectionModelListener.h"
#include "SelectionModel/vdm_SelectionHandlers.h"
#include "SelectionModel/vdm_TreeViewSelector.h"

#include "Cursor/vdm_Cursor.h"
#include "Cursor/vdm_TreeViewCursor.h"
