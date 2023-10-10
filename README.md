# Extend QML  using Shimmer++ libraries

The repo is used for demonstration purposes on how to define QML types using C++ and specifically the [Shimmer++ Libraries](https://eddytheco.github.io/Shimmerpp/about/).
The custom type takes care of monitor the NFT outputs on certain address.

## How to reuse
The CMake project on this repo define a target called `nftMonitor` that defines the `NFTMonitor` and `CPPMonitor` QML elements.

This QML types can be reused in your CMake  project by using CMake Packages. 

* Using [find_package](https://cmake.org/cmake/help/latest/command/find_package.html) `Config mode` after installing the library on the system.

Like:
```
find_package(nftMonitor REQUIRED COMPONENTS library CONFIG)
```

* Using [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html)

Like:
```
FetchContent_Declare(
	nftMonitor
	GIT_REPOSITORY https://github.com/EddyTheCo/NFTQMLShimmerpp.git
	GIT_TAG main
	)
FetchContent_MakeAvailable(nftMonitor)
```

Then you need to link the backing target to your target like 
```
target_link_libraries(yourTarget PRIVATE nftMonitor)
```

For the QML engine to find the module one needs to add the path of the module, in this case `/esterVtech.com/imports`.

Like:
```
// Add `/esterVtech.com/imports` to the QML Import Path https://doc.qt.io/qt-6/qtqml-syntax-imports.html
engine.addImportPath("qrc:/esterVtech.com/imports");
```

## Example of using the library
An example on how to use the QML types in an application is present on this repo. 

The example use two NFTMonitor element to create a [simple NFT game](https://eddytheco.github.io/NFTQMLShimmerpp/)

The game consists of comparing addresses based on the NFTs they hold.
Each address is assigned an attack coefficient based on one of the NFTs they hold.
You can mint your own NFT and the game will assign an attack coefficient.
Also, the name on the metadata field of the NFT Output will be shown.
So you can brag about the NFTs you have by only sharing your address.
The NFTs issued by estervtech will have the maximum attack coefficient.
This is a very simple game just to show the use of the library.
