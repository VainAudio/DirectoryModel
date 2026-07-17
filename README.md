# Vain Directory Model

[![Run Tests](https://github.com/VainAudio/DirectoryModel/actions/workflows/RunTests.yml/badge.svg)](https://github.com/VainAudio/DirectoryModel/actions/workflows/RunTests.yml)

A couple JUCE modules that are useful for representing and interacting with files and directories through `juce::ValueTree`.
It allows displaying a directory's contents in a tree view and allowing the user to make selections and open/close directories with their mouse or arrow keys.
All of this library's functionality is based on `juce::ValueTree` properties.
This means you can receive listener callbacks whenever a new item is selected or a directory is opened/closed (or anything else you feel like).

![Demo App Screenshot](/screenshot.png)

This code was originally written for the preset browser in [PFT](https://vainaudio.com/product/pft/).
I added some minor improvements and expansions mostly aimed at making the library more flexible.
That being said it's not quite flexible enough for super general use cases.
The classes in this library are written to work with each other to display disk contents as a `juce::ValueTree`.

## Included:

- `DirectoryModel`: A class that models files and directories using `juce::ValueTree`.
- `DirectoryModel::IUpdateHandler`: Choose which data to populate the tree with.
- `DirectoryModelSync`: Update a `DirectoryModel` when files on the disk change.
- `SelectionModel`: Custom selection logic for the tree.
- `TreeView`: Simple tree view class that optionally integrates with `DirectoryModel`.
- `JuceTreeView`: Some classes that can be used to display a `DirectoryModel` in an existing `juce::TreeView`.

## DirectoryModel / DirectoryModelSync

The main purpose of this library is to maintain a `juce::ValueTree` that models a directory on the disk and updates it as disk contents change.
Generally speaking, it is structured with building blocks so that pieces of functionality can be added and removed as needed.
Create a `DirectoryModel` to model the directory and a `DirectoryModelSync` to keep it up to date.
Add `IUpdateHandlers` to the DirectoryModel to populate `juce::ValueTrees` with desired properties.

It's probably best to create a model class that encapsulates all of this:

```c++
class MyDirectoryModel
{
public:
    MyDirectoryModel()
    {
        model.addUpdateHandler(nameUpdateHandler);

        model.initialize('/root/directory/');
        sync.syncModel(model);

        juce::ValueTree rootTree = model.getValueTree();
    }
    
private:
    vdm::DirectoryModel model;
    vdm::DirectoryModelSync sync;
    vdm::NameUpdateHandler nameUpdateHandler; // writes the file name to the ValueTree
};
```

## SelectionModel

The library supports selecting trees within the `DirectoryModel`.
To do so, you need to add a `SelectionModel` and at least one `ISelectionHandler`.
The `SelectionModel` responds to selection requests and passes them on to the `ISelectionHandler` so that custom selection logic can be implemented.

```c++
class SelectionDirectoryModel
{
public:
    MyDirectoryModel()
    {
        model.initialize('/root/directory/');
        sync.syncModel(model);

        selectionModel.addSelectionHandler("default", selector);
        selectionModel.setSelectionMode("default");
        selectionModel.setValueTree(model.getValueTree());
    }
    
private:
    vdm::DirectoryModel model;
    
    vdm::SelectionModel selection;
    vdm::SingleSelectionHandler selector;
};
```

## TreeView

The `vdm_ui` module has a `TreeView` base class.
The motivation for creating it is that I don't like `juce::TreeView`.
I think it does too much.
The `vdm::TreeView` only handles creating and laying out child components.
The return value from `vdm::TreeView::createTreeViewItem` is null checked but, the class isn't really intended to be used that way so the function should return a valid `juce::Component`.
Keep in mind this isn't a very efficient tree view.
Its main purpose is displaying a handful of files.
Each tree view item component is created on demand, but they aren't trimmed away once the directory is closed or reused.

Another thing to note about `TreeView` is that it uses some of the `DirectoryModel` keys for resizing itself.
You can use it with generic value trees, but you need to add the appropriate keys from `DirectoryModel::Keys`.

```c++
class MyTreeView : public vdm::TreeView
{
private:
    std::unique_ptr<juce::Component> createTreeViewItem(juce::ValueTree tree) override;
};

vdm::DirectoryModel model;
model.initialize('/root/directory/');
juce::ValueTree rootTree = model.getValueTree();

MyTreeView treeView;
treeView.setValueTree(rootTree);
```

# Adding it to Your Project

DirectoryModel is organized as a set of JUCE modules.
To use it in your project include the `Modules` directory in `CMakeLists.txt`:

```cmake
add_subdirectory("DirectoryModel/Modules")
```

Link to the following targets:

```cmake
target_link_libraries("Project"
    PRIVATE
    vdm::vdm
    vdm::vdm_directory
    vdm::vdm_ui
)
```
