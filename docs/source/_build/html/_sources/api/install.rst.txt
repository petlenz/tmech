Install
=======

Although tmech is a header-only library, we provide standardized methods to install it with cmake.

Besides the tmech headers, all these methods place the ``cmake`` project
configuration file in the right location so that third-party projects can use
cmake's ``find_package`` to locate tmech headers.


From source with cmake
----------------------

You can also install ``tmech`` from source with cmake. On Unix platforms, from the
source directory:

.. code::

    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=path_to_prefix ..
    make install

On Windows platforms, from the source directory:

.. code::

    mkdir build
    cd build
    cmake -G "NMake Makefiles" -DCMAKE_INSTALL_PREFIX=path_to_prefix ..
    nmake
    nmake install

``path_to_prefix`` is the absolute path to the folder where cmake searches for
dependencies and installs libraries. ``tmech`` installation from cmake assumes
this folder contains ``include`` and ``lib`` subfolders.


Including tmech in your project
---------------------------------

After installing the library with cmake, you can add ``tmech`` to your project using cmake:

.. code::

    find_package(tmech REQUIRED)
    target_include_directories(your_target PUBLIC ${tmech_INCLUDE_DIR})
    target_link_libraries(your_target PUBLIC tmech)
